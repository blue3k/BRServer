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
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"
#include "Common/ResultCode/BRResultCodeLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BrSvrTypes.h"

#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceTrans.h"
#include "ServerSystem/ServiceEntity/Game/GameClusterServiceEntity.h"




//BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransRegisterPlayerToJoinGameServer);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityCreatedS2CEvt);
BR_MEMORYPOOL_IMPLEMENT(Svr::GameServerTransGamePlayerEntityDeletedS2CEvt);


namespace BR {
namespace Svr {


	//GameServerTransRegisterPlayerToJoinGameServer::GameServerTransRegisterPlayerToJoinGameServer(Message::MessageData* &pIMsg)
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
	//	svrChk(res.ParseIMsg(pMsgRes->GetMessage()));

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
	//	Policy::IPolicyGameServer *pTargetPolicy = nullptr;

	//	m_PlayerUID = 0;

	//	svrChk(super::StartTransaction());

	//	if (GetPlayerID() == 0)
	//	{
	//		svrErr(ResultCode::E_INVALID_PLAYERID);
	//	}

	//	if ((Svr::GetServerComponent<Svr::EntityManager>()->FindEntity(GetRouteContext().GetTo(), pEntity))
	//		&& pEntity->GetEntityID().GetFacultyID() == (UINT)EntityFaculty::User)
	//	{
	//		svrChkPtr(pPlayerEntity = BR_DYNAMIC_CAST(GamePlayerEntity*, (Svr::Entity*)pEntity));

	//		// If a login server has invalid login session information from the DB. the player ID will not be match
	//		if (pPlayerEntity->GetPlayerID() != GetPlayerID())
	//		{
	//			svrErrClose(ResultCode::E_INVALID_PLAYERID);
	//		}

	//		svrTrace(Svr::TRC_ENTITY, "Reinitialize Player Entity UID:{0}", GetPlayerID());
	//		Assert(pPlayerEntity->GetShardID() == GetShardID());
	//	}
	//	else
	//	{
	//		Svr::Entity* pEntity = nullptr;
	//		svrTrace(Svr::TRC_ENTITY, "Create new Player Entity UID:{0}", GetPlayerID());

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
	//	svrChkPtr(pTargetPolicy = GetMyServer()->GetLoopbackServerEntity()->GetPolicy<Policy::IPolicyGameServer>());

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

		svrChk( GetServerComponent<ServerEntityManager>()->GetServerEntity( GetPlayerUID().GetServerID(), pServerEntity ) );

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
};// namespace BR 

