////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves
// 
// Author : KyungKun Ko
//
// Description : game cluster entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Game/GameClusterServiceTrans.h"
#include "ServiceEntity/Game/GameClusterServiceEntity.h"




//SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameServerTransRegisterPlayerToJoinGameServer);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameServerTransGamePlayerEntityCreatedS2CEvt);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::GameServerTransGamePlayerEntityDeletedS2CEvt);


namespace SF {
namespace Svr {


	//GameServerTransRegisterPlayerToJoinGameServer::GameServerTransRegisterPlayerToJoinGameServer(MessageDataPtr &pIMsg)
	//	: ServerEntityMessageTransaction(pIMsg)
	//	, m_PublicAddress(nullptr)
	//	, m_PublicAddressIPV6(nullptr)
	//	, m_Port(0)
	//{
	//	SetWorkOnServerEntity(true);

	//	BR_TRANS_MESSAGE(Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes, { return OnPlayerRegisteredRes(pRes); });
	//}

	//Result GameServerTransRegisterPlayerToJoinGameServer::OnPlayerRegisteredRes(Svr::TransactionResult* &pRes)
	//{
	//	Result hr = ResultCode::SUCCESS;

	//	Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
	//	Message::GameServer::RegisterPlayerToJoinGameServerOnPlayerEntityRes res;

	//	svrChk(pRes->GetResult());
	//	svrChk(res.ParseMessage(pMsgRes->GetMessage()));

	//	m_PublicAddress = GetMyOwner()->GetPublicNetConfig()->IPV4.c_str();
	//	m_PublicAddressIPV6 = GetMyOwner()->GetPublicNetConfig()->IPV6.c_str();
	//	m_Port = GetMyOwner()->GetPublicNetConfig()->Port;

	//Proc_End:

	//	CloseTransaction(hr);

	//	return ResultCode::SUCCESS;
	//}

	//// Start Transaction
	//Result GameServerTransRegisterPlayerToJoinGameServer::StartTransaction()
	//{
	//	Result hr = ResultCode::SUCCESS;
	//	SharedPointerT<Svr::Entity> pEntity;
	//	GamePlayerEntity *pPlayerEntity = nullptr;
	//	Policy::NetPolicyGameServer *pTargetPolicy = nullptr;

	//	m_PlayerUID = 0;

	//	svrChk(super::StartTransaction());

	//	if (GetPlayerID() == 0)
	//	{
	//		svrErr(ResultCode::INVALID_PLAYERID);
	//	}

	//	if ((Service::EntityTable->find(GetRouteContext().GetTo(), pEntity))
	//		&& pEntity->GetEntityID().GetFacultyID() == (uint)EntityFaculty::User)
	//	{
	//		svrChkPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>((Svr::Entity*)pEntity));

	//		// If a login server has invalid login session information from the DB. the player ID will not be match
	//		if (pPlayerEntity->GetPlayerID() != GetPlayerID())
	//		{
	//			svrErrClose(ResultCode::INVALID_PLAYERID);
	//		}

	//		svrTrace(SVR_ENTITY, "Reinitialize Player Entity UID:{0}", GetPlayerID());
	//		Assert(pPlayerEntity->GetShardID() == GetShardID());
	//	}
	//	else
	//	{
	//		Svr::Entity* pEntity = nullptr;
	//		svrTrace(SVR_ENTITY, "Create new Player Entity UID:{0}", GetPlayerID());

	//		svrChk(Svr::GetServerComponent<GameEntityManager>()->CreateEntity(ClusterID::Game_Conspiracy, EntityFaculty::User, pEntity));
	//		svrChkPtr(pPlayerEntity = dynamic_cast<GamePlayerEntity*>(pEntity));
	//		svrChk(Svr::GetServerComponent<GameEntityManager>()->AddEntity(EntityFaculty::User, pEntity));

	//		// Add Entity will Initialize entity so that AccountID is erased.
	//		// SetAccountID need to be set after entity is added
	//		pPlayerEntity->SetAccountID(GetPlayerID());
	//	}

	//	pPlayerEntity->SetShardID(GetShardID());
	//	m_PlayerUID = pPlayerEntity->GetEntityUID();

	//	// it's local player send message to local loopback entity
	//	svrChkPtr(pTargetPolicy = GetMyServer()->GetLoopbackServerEntity()->GetInterface<Policy::NetPolicyGameServer>());

	//	svrChk(pTargetPolicy->RegisterPlayerToJoinGameServerOnPlayerEntityCmd(
	//		RouteContext(GetOwnerEntityUID(), m_PlayerUID), GetTransID(),
	//		GetPlayerID(), GetTicket(), GetFBUserID()));


	//Proc_End:

	//	if (!(hr))
	//		CloseTransaction(hr);

	//	return hr;
	//}



	// Start Transaction
	Result GameServerTransGamePlayerEntityCreatedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		ServerEntity *pServerEntity = nullptr;

		svrChk( super::StartTransaction() );

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity(EntityUID(GetPlayerUID()).GetServerID(), pServerEntity ) );

		svrChk( GetMyOwner()->CreatePlayer( GetPlayerID(), GetPlayerUID(), pServerEntity ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	Result GameServerTransGamePlayerEntityDeletedS2CEvt::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->DeletePlayer( GetPlayerID(), GetPlayerUID() ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



};// namespace Svr 
};// namespace SF 

