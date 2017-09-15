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


#include "stdafx.h"
#include "String/StrUtil.h"
#include "Util/TimeUtil.h"
#include "ResultCode/SFResultCodeNet.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "SvrConst.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "Transaction/ParallelTransaction.h"
#include "Server/BrServer.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityTable.h"
#include "Server/BrServerUtil.h"
#include "Service/ServerService.h"


namespace SF {
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
		: m_QuerieQueue(querieQueue)
		, m_bExitRequested(false)
	{
	}

	void ParallelTransactionManager::TaskWorker::Run()
	{
		const DurationMS desiredLoopInterval = DurationMS(500);
		
		while( 1 )
		{
			ParallelTransaction* pTransaction = nullptr;
			auto loopInterval = UpdateInterval( desiredLoopInterval );

			auto bExitRequested = m_bExitRequested.load(std::memory_order_relaxed);
			if (bExitRequested || CheckKillEvent(loopInterval))
			{
				// Kill Event signaled
				break;
			}

			if( (m_QuerieQueue.DequeueMT( pTransaction, desiredLoopInterval )) )
			{
				Result hr = pTransaction->StartTransaction();
				if( !(hr) )
				{
					svrTrace( Error, "Transaction Failed hr = 0x{0:X8}, {1}", hr, typeid(*pTransaction).name() );
				}

				if( pTransaction->IsClosed() )
					pTransaction->FlushTransaction();
				else
				{
					svrTrace( Error, "Transaction must be closed hr = 0x{0:X8}, {1}", hr, typeid(*pTransaction).name() );
					pTransaction->CloseTransaction(ResultCode::SUCCESS);
					pTransaction->FlushTransaction();
				}

				if (pTransaction->GetParentTransID() != 0)
				{
					TransactionResult* pRes = pTransaction;
					if ((Service::EntityTable->RouteTransactionResult(pRes)))
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
		: SimpleEntity(0,0)
		, m_PendingQueries(GetMemoryManager())
	{
	}

	ParallelTransactionManager::~ParallelTransactionManager()
	{
	}

	Result ParallelTransactionManager::EnqueueTransaction(ParallelTransaction* pTrans)
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(m_PendingQueries.Enqueue(pTrans));

	Proc_End:

		return hr;
	}

	// Initialize Manager
	Result ParallelTransactionManager::InitializeComponent()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(Service::EntityManager->AddEntity(EntityFaculty::Service, this));


		for( int iworker = 0; iworker < 20; iworker++ )
		{
			TaskWorker *pWorker = new(GetMemoryManager()) TaskWorker(m_PendingQueries);
			pWorker->Start();
			m_QueryWorker.push_back(pWorker);
		}

	Proc_End:


		return hr;
	}

	// Terminate Manager
	void ParallelTransactionManager::TerminateComponent()
	{
		// set exit flag for all
		for (size_t iworker = 0; iworker < m_QueryWorker.size(); iworker++)
		{
			TaskWorker *pWorker = m_QueryWorker[iworker];
			// Just kill the thread
			pWorker->SetExitRequested();
		}

		// Poke all worker at least once
		for (size_t iworker = 0; iworker < m_QueryWorker.size(); iworker++)
		{
			ParallelTransaction* pTransNOP = new(GetMemoryManager()) ParallelTransactionNop(GetMemoryManager());
			m_PendingQueries.Enqueue(pTransNOP);
		}

		// request thread stop
		for( size_t iworker = 0; iworker < m_QueryWorker.size(); iworker++ )
		{
			TaskWorker *pWorker = m_QueryWorker[iworker];
			// Just kill the thread
			pWorker->Stop(false);
		}

		m_QueryWorker.clear();
	}








}; // namespace Svr
}; // namespace SF



