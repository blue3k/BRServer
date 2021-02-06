////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Remote Entity Manager implementation
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "Util/SFTimeUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "SvrConst.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Transaction/Transaction.h"
//#include "ServerSystem/PlugIn.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Entity/EntityTable.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "Net/SFNetServerPeerTCP.h"
#include "ServiceEntity/ClusteredServiceEntity.h"
#include "Service/ServerService.h"



namespace SF {
	namespace Svr
	{


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	ServerEntityManager class
		//

		constexpr StringCrc64 ServerEntityManager::TypeName;

		ServerEntityManager::ServerEntityManager(uint threadCount)
			: m_Heap("ServerEntityManager", GetSystemHeap())
			, m_ThreadCount(threadCount)
			, m_ServerIDMap(GetHeap())
		{
			//Service::ServerEntityManager = this;
		}

		ServerEntityManager::~ServerEntityManager()
		{
		}

		void ServerEntityManager::Dispose()
		{
			//Service::ServerEntityManager = nullptr;

			Clear();
		}

		void ServerEntityManager::Clear()
		{
			m_ServerIDMap.for_each([this](ServerID, ServerEntity* pSvrEntity)
				{
					Service::EntityManager->RemoveEntity(pSvrEntity);
					return true;
				});
			Service::EntityManager->FlushDeletedEntity();
			m_ServerIDMap.Clear();
		}

		Result ServerEntityManager::GetOrRegisterServer(ServerID serverID, const String& eventRouterAddress, ServerEntity*& pServerEntity)
		{
			Result hr = ResultCode::SUCCESS;
			ServerEntity* pNewServerEntity = nullptr;
			Net::ConnectionPtr pConnection;

			MutexScopeLock localLock(m_ServerTableLock);

			ServerEntity* pSvrEntity = nullptr;
			if ((GetServerEntity(serverID, pSvrEntity)))
			{
				pServerEntity = pSvrEntity;
				return hr;
			}

			svrTrace(SVR_ENTITY, "Registering Server SvrID:{1} route:{2}", serverID, eventRouterAddress);

			svrCheck(BrServer::GetInstance()->CreateServerEntity(pNewServerEntity));

			pNewServerEntity->SetEventRouterAddress(eventRouterAddress);
			pNewServerEntity->SetServerID(serverID);

			pServerEntity = pNewServerEntity;

			svrCheck(AddServerEntity(pNewServerEntity));

			//svrCheck(BrServer::GetInstance()->GetNetPrivate()->RegisterServerConnection(serverID, eventRouterAddress, pConnection));

			svrTrace(SVR_ENTITY, "Registered Server {0} SvrID:{1} {2}, taskGrp:{3}", serverID, eventRouterAddress, pNewServerEntity->GetTaskGroupID());

			return hr;
		}



		// Get remote entity
		Result ServerEntityManager::GetServerEntity(ServerID svrID, ServerEntity*& pServerEntity)
		{
			Result hr = ResultCode::SUCCESS;

			hr = m_ServerIDMap.Find(svrID, pServerEntity);

			Assert(!(hr) || pServerEntity != nullptr);

			return hr;
		}



		Result ServerEntityManager::GetEntityManagerServerEntity(ServerEntity*& pServerEntity)
		{
			Result hr = ResultCode::SUCCESS;

			// Get first non-zero one, This should be the biggest one
			ServerUpTimeList::iterator itEntity = m_EntityManagerServerUpTimeMap.begin();
			for (; itEntity.IsValid(); ++itEntity)
			{
				ServerUpTimeListNodeItem* pNode = (ServerUpTimeListNodeItem*)&*itEntity;
				if (pNode->pServerEntity != nullptr)
				{
					pServerEntity = pNode->pServerEntity;
					break;
				}
			}

			return pServerEntity != nullptr ? hr : ResultCode::FAIL;
		}

		// Add new connection
		Result ServerEntityManager::AddServerEntity(ServerEntity* pServerEntity)
		{
			Result hr = ResultCode::SUCCESS, res;
			svrChkPtr(pServerEntity);

			svrChk(pServerEntity->InitializeEntity(Service::EntityTable->GenEntityID(EntityFaculty::Server)));

			// Add to task list
			svrChk(AddTickTask(pServerEntity));

			svrChk(Service::EntityTable->insert(pServerEntity));

			svrChk(m_ServerIDMap.Insert(pServerEntity->GetServerID(), pServerEntity));

			pServerEntity = nullptr;

		Proc_End:

			if (pServerEntity)
			{
				RemoveTickTask(pServerEntity);
			}

			return hr;
		}

		Result ServerEntityManager::AddOrGetServerEntity(ServerID serverID, ServerEntity* pServerEntity)
		{
			Result hr = ResultCode::SUCCESS;

			MutexScopeLock localLock(m_ServerTableLock);

			ServerEntity* pOldEntity = nullptr;
			if ((GetServerEntity(serverID, pOldEntity)))
			{
				if (pServerEntity != nullptr && pOldEntity != pServerEntity)
				{
					svrTrace(SVR_ENTITY, "Adding Duplicated Server SvrID:{0}", serverID);
					AssertRel(false);
					return ResultCode::UNEXPECTED;
				}

				return hr;
			}

			svrTrace(SVR_ENTITY, "Adding Server SvrID:{0}", serverID);

			if (pServerEntity == nullptr)
			{
				svrCheck(BrServer::GetInstance()->CreateServerEntity(pServerEntity));
				pServerEntity->SetServerID(serverID);
			}

			svrCheck(AddServerEntity(pServerEntity));

			return hr;
		}

	} // namespace Svr
} // namespace SF

