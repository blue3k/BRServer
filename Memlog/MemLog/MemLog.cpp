

// Precompiled Header
#include "stdafx.h"

#include <unordered_map>
#include <unordered_set>



#define MEMLOG_FUNC __declspec(dllexport)

#include "MemLog.h"




#include "util/callstack.h"





namespace MemLog
{
	struct MemoryInfo
	{
		void* pPtr;
		CallStack* pCallStack;
		size_t AllocateSize;
		UINT32 uiUser1;
		UINT32 uiUser2;
		UINT32 uiUser3;

	};

	class MemLogger : public IMemLogger
	{
	private:

		bool m_bIsInitialized;
		bool m_bIsDiffCheck;

		std::unordered_map<void*,MemoryInfo*> m_dbgLeakDetectData;
		std::unordered_map<void*,MemoryInfo*> m_dbgLeakDetectDiffData;
		std::unordered_set<std::string> m_dbgIgnoreList;

		UINT m_uiDetectMode;

		Logging m_logMask;

		CRITICAL_SECTION m_LogListLock;

	public:
		MemLogger()
			:m_bIsInitialized(false),
			m_bIsDiffCheck(false),
			m_uiDetectMode(0)
		{
			InitializeCriticalSection( &m_LogListLock );
		}

		~MemLogger()
		{
			Terminate();

			DeleteCriticalSection( &m_LogListLock );
		}

		void Release()
		{
			delete this;
		}


		void Initialize( Logging logPlace, UINT uiLogMask )
		{
			if( !m_bIsInitialized )
			{
				m_uiDetectMode = uiLogMask;
				CallStackSystem::Initialize();
				m_bIsInitialized = true;
			}
		}


		void Terminate()
		{
			if( m_bIsInitialized )
			{
				ReportLeak();

				EnterCriticalSection( &m_LogListLock );

				auto iter = m_dbgLeakDetectData.begin();
				for( ; iter != m_dbgLeakDetectData.end(); iter++ )
				{
					delete iter->second->pCallStack;
					delete iter->second;
				}
				m_dbgLeakDetectData.clear();

				m_dbgLeakDetectDiffData.clear();

				LeaveCriticalSection( &m_LogListLock );

				CallStackSystem::Terminate();
				m_bIsInitialized = false;
			}
		}

		virtual UINT32 GetLogMask()
		{
			return m_uiDetectMode;
		}

		virtual void SetLogMask( UINT32 uiLogMask )
		{
			m_uiDetectMode = uiLogMask;
		}

		void EnableLog( UINT uiLogMask, bool bIsEnable )
		{
			if( bIsEnable )
				m_uiDetectMode |= uiLogMask;
			else
				m_uiDetectMode &= ~uiLogMask;
		}


		void AddToLog( UINT32 uiLogMask, void* pObj, size_t szSize, UINT32 uiUser1, UINT32 uiUser2, UINT32 uiUser3 )
		{
			if( m_bIsInitialized && (m_uiDetectMode & uiLogMask) && pObj )
			{
				MemoryInfo *pInfo = new MemoryInfo;
				CallStack* pCallStack = new SafeCallStack;
				pCallStack->getstacktrace( 0xffffffff, NULL );

				pInfo->pPtr = pObj;
				pInfo->pCallStack = pCallStack;
				pInfo->AllocateSize = szSize;
				pInfo->uiUser1 = uiUser1;
				pInfo->uiUser2 = uiUser2;
				pInfo->uiUser3 = uiUser3;

				EnterCriticalSection( &m_LogListLock );
				m_dbgLeakDetectData.insert( std::make_pair( pObj, pInfo ) );
				if( m_bIsDiffCheck )
					m_dbgLeakDetectDiffData.insert( std::make_pair( pObj, pInfo ) );
				LeaveCriticalSection( &m_LogListLock );
			}
		}


		void RemoveFromLog( void* pObj )
		{
			if( m_bIsInitialized && pObj )
			{
				EnterCriticalSection( &m_LogListLock );
				auto found = m_dbgLeakDetectData.find(pObj);
				//_ASSERT_EXPR( found != m_dbgLeakDetectData.end(), L"Invalid Object remove" );
				if( found != m_dbgLeakDetectData.end() )
				{
					delete found->second->pCallStack;
					delete found->second;
					m_dbgLeakDetectData.erase( found );
				}
				found = m_dbgLeakDetectDiffData.find(pObj);
				if( found != m_dbgLeakDetectDiffData.end() )
					m_dbgLeakDetectDiffData.erase( found );
				LeaveCriticalSection( &m_LogListLock );
			}
		}


		virtual void TrackAddress( void* pTrackAddress )
		{
			intptr_t ChkValue = (intptr_t)pTrackAddress;
			auto itCur = m_dbgLeakDetectData.begin();
			for( ; itCur != m_dbgLeakDetectData.end(); itCur++ )
			{
				intptr_t Start = (intptr_t)itCur->second->pPtr;
				if( Start <= ChkValue && (Start+(intptr_t)itCur->second->AllocateSize) >= ChkValue )
				{
					OutputDebugStringA( "\n\tTracked Object!\n" );
					itCur->second->pCallStack->dump();
					return;
				}
			}
		}


		void ReportLeak()
		{
			if( m_bIsInitialized )
			{
				EnterCriticalSection( &m_LogListLock );
				if( m_dbgLeakDetectData.size() > 0 )
				{
					auto iter = m_dbgLeakDetectData.begin();
					for( ; iter != m_dbgLeakDetectData.end(); iter++ )
					{
						iter->second->pCallStack->dump();
					}
				}
				else
				{
					OutputDebugStringA( "No Memory Leak\n" );
				}
				LeaveCriticalSection( &m_LogListLock );
			}
		}


		// Difference check
		void StartDifferenceCheck()
		{
			if( !m_bIsInitialized ) return;

			m_bIsDiffCheck = true;
			m_dbgLeakDetectDiffData.clear();
		}

		void ReportDifference()
		{
			if( !m_bIsInitialized ) return;

			EnterCriticalSection( &m_LogListLock );

			if( m_dbgLeakDetectDiffData.size() > 0 )
			{
				auto iter = m_dbgLeakDetectDiffData.begin();
				for( ; iter != m_dbgLeakDetectDiffData.end(); iter++ )
					iter->second->pCallStack->dump();
			}
			else
			{
				OutputDebugStringA( "No Memory Leak\n" );
			}

			LeaveCriticalSection( &m_LogListLock );
		}

		void StopDifferenceCheck()
		{
			if( !m_bIsInitialized ) return;

			m_bIsDiffCheck = false;
		}

	};


};



MEMLOG_FUNC HRESULT APIENTRY QueryMemLogger( MemLog::IMemLogger* &pLogger )
{
	pLogger = new MemLog::MemLogger;

	return pLogger == NULL ? E_OUTOFMEMORY : S_OK;
}




