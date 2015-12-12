////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Performance counter
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/StrUtil.h"
#include "Common/ClassUtil.h"
#include "Common/TimeUtil.h"




namespace BR {
namespace Svr {



	//////////////////////////////////////////////////////////////////////////
	//
	//	performance counter class
	//


	class PerformanceCounter
	{
	public:

		enum {
			MAX_COUNTERNAME = 128,
		};

		enum class DataTypes : UINT8
		{
			Int32,
			UInt32,
			Int64,
			UInt64,
		};

		enum class CountingTypes : UINT8
		{
			Raw,
			AveragePerSec,
			TickPerSec,
		};

	private:
		char m_CounterName[MAX_COUNTERNAME];
		BRCLASS_ATTRIBUTE_READONLY(DataTypes, DataType);

		BRCLASS_ATTRIBUTE_READONLY(CountingTypes, CountingType);

		std::atomic<UINT> m_SyncSerial;

	protected:

		void IncSyncSerial()								{ m_SyncSerial.fetch_add(1, std::memory_order_relaxed); }

	public:
		PerformanceCounter(const char* counterName, DataTypes dataType, CountingTypes countingType);
		PerformanceCounter(DataTypes dataType, CountingTypes countingType);
		virtual ~PerformanceCounter();

		void SetCounterName(const char* strName);
		const char* GetCounterName()						{ return m_CounterName; }
		UINT GetSyncSerial()								{ return m_SyncSerial.load(std::memory_order_relaxed); }

		virtual INT64 GetRawValue() = 0;

		virtual HRESULT CopyTo(UINT bufferSize, BYTE* pBuffer) { return S_OK; }
		virtual HRESULT CopyFrom(UINT bufferSize, BYTE* pBuffer) { return S_OK; }

	};




	// Data type convertion templates
	template<class DataType> inline PerformanceCounter::DataTypes PerformanceCounterRaw_GetDataType() { return PerformanceCounter::DataTypes::Int32; }
	template<> inline PerformanceCounter::DataTypes PerformanceCounterRaw_GetDataType<INT32>() { return PerformanceCounter::DataTypes::Int32; }
	template<> inline PerformanceCounter::DataTypes PerformanceCounterRaw_GetDataType<UINT32>() { return PerformanceCounter::DataTypes::UInt32; }
	template<> inline PerformanceCounter::DataTypes PerformanceCounterRaw_GetDataType<INT64>() { return PerformanceCounter::DataTypes::Int64; }
	template<> inline PerformanceCounter::DataTypes PerformanceCounterRaw_GetDataType<UINT64>() { return PerformanceCounter::DataTypes::UInt64; }


	template<class DataType>
	class PerformanceCounterRaw : public PerformanceCounter
	{
	private:
		std::atomic<DataType> m_RawValue;

	protected:
		PerformanceCounterRaw(const char* counterName, CountingTypes countingType)
			: PerformanceCounter(counterName, PerformanceCounterRaw_GetDataType<DataType>(), countingType)
			, m_RawValue(DataType(0))
		{
		}

		PerformanceCounterRaw(CountingTypes countingType)
			: PerformanceCounter(PerformanceCounterRaw_GetDataType<DataType>(), countingType)
			, m_RawValue(DataType(0))
		{
		}


	public:
		PerformanceCounterRaw(const char* counterName)
			: PerformanceCounter(counterName, PerformanceCounterRaw_GetDataType<DataType>(), CountingTypes::Raw)
			, m_RawValue(DataType(0))
		{
		}

		PerformanceCounterRaw()
			: PerformanceCounter(PerformanceCounterRaw_GetDataType<DataType>(), CountingTypes::Raw)
			, m_RawValue(DataType(0))
		{
		}



		virtual HRESULT CopyTo(UINT bufferSize, BYTE* pBuffer) override
		{
			Assert(bufferSize >= sizeof(DataType));
			if (bufferSize < sizeof(DataType))
				return E_FAIL;

			DataType temp = m_RawValue.load(std::memory_order_relaxed);

			memcpy(pBuffer, &temp, sizeof(DataType));

			return S_OK;
		}

		virtual HRESULT CopyFrom(UINT bufferSize, BYTE* pBuffer) override
		{
			Assert(bufferSize >= sizeof(DataType));
			if (bufferSize < sizeof(DataType))
				return E_FAIL;

			DataType temp;
			memcpy(&temp, pBuffer, sizeof(DataType));

			m_RawValue.store(temp, std::memory_order_relaxed);

			IncSyncSerial();

			return S_OK;
		}

		virtual INT64 GetRawValue() override						{ return (INT64)m_RawValue.load(std::memory_order_relaxed); }

		void SetRawValue(const DataType& value)		{ m_RawValue.store(value, std::memory_order_release); IncSyncSerial(); }

		PerformanceCounterRaw<DataType>& operator = (const DataType& value)
		{
			m_RawValue.store(value, std::memory_order_release);
			IncSyncSerial();
			return *this;
		}

		PerformanceCounterRaw<DataType>& operator += (const DataType& value)
		{
			m_RawValue.fetch_add(value, std::memory_order_release);
			IncSyncSerial();
			return *this;
		}

		PerformanceCounterRaw<DataType>& operator -= (const DataType& value)
		{
			m_RawValue.fetch_sub(value, std::memory_order_release);
			IncSyncSerial();
			return *this;
		}

		DataType Inc ()
		{
			auto prevVal = m_RawValue.fetch_add(1, std::memory_order_release);
			IncSyncSerial();
			return prevVal + 1;
		}

		DataType operator ++ ()
		{
			auto prevVal = m_RawValue.fetch_add(1, std::memory_order_release);
			IncSyncSerial();
			return prevVal + 1;
		}

		DataType operator -- ()
		{
			auto prevVal = m_RawValue.fetch_sub(1, std::memory_order_release);
			IncSyncSerial();
			return prevVal -1;
		}
	};



	template<class DataType>
	class PerformanceCounterAveragePerSec : public PerformanceCounterRaw<DataType>
	{
	private:
		std::atomic<ULONG> m_TickIndex;
		TimeStampMS m_TickCountStartTime;
		struct
		{
			std::atomic<ULONG> Working;
			std::atomic<ULONG> TotalCount;
			std::atomic<DataType> TotalRawValue;
		} m_Values[2];

	public:
		PerformanceCounterAveragePerSec(const char* counterName)
			: PerformanceCounterRaw<DataType>(counterName, PerformanceCounter::CountingTypes::AveragePerSec)
			, m_TickIndex(0)
		{
			memset(m_Values, 0, sizeof(m_Values));
		}


		PerformanceCounterRaw<DataType>& operator = (const DataType& value)
		{
			auto index = m_TickIndex.load(std::memory_order_relaxed) % countof(m_Values);
			m_Values[index].Working.fetch_add(1, std::memory_order_acquire);

			m_Values[index].TotalCount.fetch_add(1, std::memory_order_release);
			m_Values[index].TotalRawValue.fetch_add(value);

			m_Values[index].Working.fetch_sub(1, std::memory_order_release);
			return *this;
		}

		virtual HRESULT CopyTo(UINT bufferSize, BYTE* pBuffer) override
		{
			auto timeSince = Util::TimeSince(m_TickCountStartTime);
			if (m_TickCountStartTime == TimeStampMS::min() || timeSince > DurationMS(1000))
			{
				auto curTime = Util::Time.GetTimeMs();
				auto nextTickIndex = (m_TickIndex.load(std::memory_order_relaxed) + 1) % countof(m_Values);

				auto indexCalculate = nextTickIndex;
				while (m_Values[indexCalculate].Working.load(std::memory_order_relaxed) > 0)
				{
					// This should be rarely happened.
					ThisThread::SleepFor(DurationMS(0));
				}
				auto total = m_Values[indexCalculate].TotalRawValue.load(std::memory_order_relaxed);
				auto totalCount = m_Values[indexCalculate].TotalCount.load(std::memory_order_relaxed);
				auto newValue = (total * 1000) / totalCount;

				PerformanceCounterRaw<DataType>::SetRawValue(newValue / timeSince.count());


				// clear used values
				m_Values[indexCalculate].TotalRawValue.store(0, std::memory_order_release);
				m_Values[indexCalculate].TotalCount.store(0, std::memory_order_release);
				m_Values[indexCalculate].Working.store(0, std::memory_order_release);

				// move to next
				m_TickIndex.fetch_add(1, std::memory_order_release);
				m_TickCountStartTime = curTime;
			}

			return PerformanceCounterRaw<DataType>::CopyTo(bufferSize, pBuffer);
		}

	};



	template<class DataType>
	class PerformanceCounterTickPerSec : public PerformanceCounterRaw<DataType>
	{
	private:
		std::atomic<ULONG> m_TickIndex;
		TimeStampMS m_TickCountStartTime;
		std::atomic<DataType> m_Values[2];

	public:
		PerformanceCounterTickPerSec(const char* counterName)
			: PerformanceCounterRaw<DataType>(counterName, PerformanceCounter::CountingTypes::TickPerSec)
			, m_TickIndex(0)
			, m_TickCountStartTime(TimeStampMS::min())
		{
			memset(m_Values, 0, sizeof(m_Values));
		}


		virtual HRESULT CopyTo(UINT bufferSize, BYTE* pBuffer) override
		{
			auto timeSince = Util::TimeSince(m_TickCountStartTime);
			if (m_TickCountStartTime == TimeStampMS::min() || timeSince > DurationMS(1000))
			{
				auto nextTickIndex = (m_TickIndex.load(std::memory_order_relaxed) + 1) % countof(m_Values);
				m_Values[nextTickIndex].store(0, std::memory_order_release);

				m_TickIndex.fetch_add(1, std::memory_order_release);
				auto ticks = m_Values[nextTickIndex].load(std::memory_order_relaxed);

				PerformanceCounterRaw<DataType>::SetRawValue((ticks * 1000) / timeSince.count());

				m_TickCountStartTime = Util::Time.GetTimeMs();
			}

			return PerformanceCounterRaw<DataType>::CopyTo(bufferSize, pBuffer);
		}


		PerformanceCounterRaw<DataType>& operator += (const DataType& value)
		{
			auto tickIndex = m_TickIndex.load(std::memory_order_relaxed) % countof(m_Values);
			m_Values[tickIndex].fetch_add(value, std::memory_order_release);

			PerformanceCounterRaw<DataType>::IncSyncSerial();

			return *this;
		}

		DataType operator ++ ()
		{
			auto tickIndex = m_TickIndex.load(std::memory_order_relaxed) % countof(m_Values);
			auto prevVal = m_Values[tickIndex].fetch_add(1, std::memory_order_release);

			PerformanceCounterRaw<DataType>::IncSyncSerial();

			return prevVal + 1;
		}


	};


#include "PerformanceCounter.inl"



}; // namespace BR
}; // namespace Svr





