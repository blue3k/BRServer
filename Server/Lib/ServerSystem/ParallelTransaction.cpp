////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : External task class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/HRESNet.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "ServerSystem/SvrConstDefault.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ParallelTransaction.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/EntityManager.h"


namespace BR {
namespace Svr
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Entity base class
	//


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	External task class
	//


	ParallelTransactionManager::TaskWorker::TaskWorker(PageQueue<ParallelTransaction*> &querieQueue)
		:m_QuerieQueue(querieQueue)
	{
	}

	void ParallelTransactionManager::TaskWorker::Run()
	{
		const DurationMS desiredLoopInterval = DurationMS(500);
		
		while( 1 )
		{
			ParallelTransaction* pTransaction = nullptr;
			auto loopInterval = UpdateInterval( desiredLoopInterval );

			if (CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			if( SUCCEEDED(m_QuerieQueue.DequeueMT( pTransaction, desiredLoopInterval )) )
			{
				HRESULT hr = pTransaction->StartTransaction();
				if( FAILED(hr) )
				{
					svrTrace( Trace::TRC_ERROR, "Transaction Failed hr = 0x{0:X8}, %1%", hr, typeid(*pTransaction).name() );
				}

				if( pTransaction->IsClosed() )
					pTransaction->FlushTransaction();
				else
				{
					svrTrace( Trace::TRC_ERROR, "Transaction must be closed hr = 0x{0:X8}, %1%", hr, typeid(*pTransaction).name() );
					pTransaction->CloseTransaction(S_OK);
					pTransaction->FlushTransaction();
				}

				if (pTransaction->GetParentTransID() != 0)
				{
					TransactionResult* pRes = pTransaction;
					if (SUCCEEDED(GetEntityTable().RouteTransactionResult(pRes)))
					{
						pTransaction = nullptr;
					}
				}

				if (pTransaction != nullptr)
				{
					SharedPointer(pTransaction);
				}
			}

		}
	}



	ParallelTransactionManager::ParallelTransactionManager()
		:SimpleEntity(0,0)
		,IServerComponent(ComponentID)
	{
	}

	ParallelTransactionManager::~ParallelTransactionManager()
	{
	}

	HRESULT ParallelTransactionManager::EnqueueTransaction(ParallelTransaction* pTrans)
	{
		HRESULT hr = S_OK;

		svrChk(m_PendingQueries.Enqueue(pTrans));

	Proc_End:

		return hr;
	}

	// Initialize Manager
	HRESULT ParallelTransactionManager::InitializeComponent()
	{
		HRESULT hr = S_OK;

		if (GetServerComponent<EntityManager>() != nullptr)
		{
			svrChk(GetServerComponent<EntityManager>()->AddEntity(EntityFaculty::Service, this));
		}


		for( int iworker = 0; iworker < 20; iworker++ )
		{
			TaskWorker *pWorker = new TaskWorker(m_PendingQueries);
			pWorker->Start();
			m_QueryWorker.push_back(pWorker);
		}

	Proc_End:


		return hr;
	}

	// Terminate Manager
	void ParallelTransactionManager::TerminateComponent()
	{
		for( size_t iworker = 0; iworker < m_QueryWorker.size(); iworker++ )
		{
			TaskWorker *pWorker = m_QueryWorker[iworker];
			// Just kill the thread
			pWorker->Stop(false);
		}

		m_QueryWorker.clear();
	}








}; // namespace Svr
}; // namespace BR



