////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : login player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "LoginServer.h"
#include "LoginServerClass.h"

#include "Common/HRESCommon.h"
#include "Common/HRESGame.h"
#include "Common/HRESLogin.h"
#include "Common/MemoryPool.h"
#include "Common/BRBaseTypes.h"
#include "Common/GameConst.h"

#include "Common/TimeUtil.h"

#include "LoginServerClass.h"
#include "ServerSystem/BRServerUtil.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/ServerEntityManager.h"
#include "ServerSystem/EntityManager.h"
#include "ServerSystem/ServiceEntity/LoginClusterServiceEntity.h"
#include "ServerSystem/ServiceEntity/ClusterManagerServiceEntity.h"
#include "ServerSystem/ServerService/LoginServerService.h"
#include "ServerSystem/ServerService/GameServerService.h"


#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Policy/LoginServerIPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerIPolicy.h"

#include "LoginPlayerEntityTrans.h"
#include "Entity/LoginPlayerEntity.h"

#include "ServerSystem/BrServer.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginPlayerTransLogin);

BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginPlayerTransLoginByFacebook);
BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginPlayerTransCloseInstance);

BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginPlayerJoinedToGameServerTrans);
BR_MEMORYPOOL_IMPLEMENT(LoginServer::LoginPlayerKickPlayerTrans);


	
namespace BR {
namespace LoginServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	template<class MessageClass, class TransactionClass>
	LoginPlayerTransLoginBase<MessageClass,TransactionClass>::LoginPlayerTransLoginBase( Message::MessageData* &pIMsg )
		:MessageTransaction( pIMsg )
	{
		SetExclusive(true);
		//BR_TRANS_MESSAGE( DB::QueryLoginCmd, { return OnLogin(pRes); });
		BR_TRANS_MESSAGE( DB::QueryRegisterAuthTicketCmd, { return OnSessionRegistered(pRes); });
		//BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnSessionDeleted(pRes); });
		//BR_TRANS_MESSAGE( DB::QueryReplaceLoginSessionCmd, { return OnSessionReplaced(pRes); });
		//BR_TRANS_MESSAGE( Message::LoginServer::KickPlayerRes, { return OnKickedPlyaer(pRes); });
		BR_TRANS_MESSAGE( Message::GameServer::RegisterPlayerToJoinGameServerRes, { return OnRegisterPlayerToJoinGameServer(pRes); });
		BR_TRANS_MESSAGE(DB::QueryConnectedToGameServerCmd, { return OnConnectToGameServerRes(pRes); });
	}

	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnLogin( HRESULT hrRes, AccountID accountID, FacebookUID FBUserID, INT shardID )
	{
		HRESULT hr = S_OK;
		AuthTicket newTicket = 0;

		svrChkClose(hrRes);

		GetMyOwner()->HeartBit();

		GetMyOwner()->SetIsTicketOwner(false);

		// succeeded to login
		GetMyOwner()->SetAccountID(accountID);
		GetMyOwner()->SetFacebookUID(FBUserID);
		GetMyOwner()->SetShardID(shardID);

		// Generate new authenticate ID
		newTicket = GetMyServer()->GetAuthTicketGenerator().NewUID();
		svrTrace(Svr::TRC_ENTITY, "New Player Ticket Ticket:%0%", newTicket);
		GetMyOwner()->SetAuthTicket(newTicket);
		//m_RegisterTryCount++;

		// register new ticket
		svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->RegisterAuthTicket( GetTransID(), GetMyOwner()->GetPlayerID(), newTicket, GetMyOwner()->GetEntityUID() ) );

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnSessionRegistered( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryRegisterAuthTicketCmd* pDBRes = (DB::QueryRegisterAuthTicketCmd*)pRes;
		Svr::ServerServiceInformation *pServiceInfo = nullptr;
		Policy::IPolicyGameServer *pGameServerPolicy = nullptr;

		svrChk(pRes->GetHRESULT());

		GetMyOwner()->SetIsTicketOwner(pDBRes->Result != 0);
		m_GameEntityUID = pDBRes->GameEntityUID;

		GetMyOwner()->HeartBit();

		// if someone already logged in
		if (pDBRes->Result != 0)
		{
			svrErrClose(E_ALREADY_LOGGEDIN_OTHERPLACE);
		}
		else
		{
			if( m_GameEntityUID == 0 )
			{
				svrTrace(Svr::TRC_ENTITY, "No login session");
				svrChk( RegisterNewPlayerToJoinGameServer() );
			}
			else
			{
				pGameServerPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameServer>(m_GameEntityUID.SvrID);
				if (pGameServerPolicy == nullptr
					|| FAILED(pGameServerPolicy->RegisterPlayerToJoinGameServerCmd(GetTransID(), RouteContext(GetOwnerEntityUID(), m_GameEntityUID),
												GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetShardID())))
				{
					svrChk(RegisterNewPlayerToJoinGameServer());
				}
			}
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass,TransactionClass>::RegisterNewPlayerToJoinGameServer()
	{
		HRESULT hr = S_OK;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pService = nullptr;

		// Find new game server for this player
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity( (ClusterID)((UINT)ClusterID::Game + (UINT)GetGameID()), pServiceEntity ) );
		svrChk( pServiceEntity->FindRandomService( pService ) );

		GetMyOwner()->HeartBit();

		svrTrace(Svr::TRC_ENTITY, "Creating new Entity for UID:%0%, on svr:%1%", GetMyOwner()->GetPlayerID(), pService->GetEntityUID());

		svrChk( pService->GetService<Svr::GameServerService>()->RegisterPlayerToJoinGameServerCmd( GetTransID(),
			GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetShardID()));
	Proc_End:

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnRegisterPlayerToJoinGameServer( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerRes res;

		if( pRes->GetHRESULT() == E_INVALID_PLAYERID )
		{
			if (GetMyOwner()->GetPlayerID() == 0)
			{
				hr = pRes->GetHRESULT();
			}
			else
			{
				// Garbage login session information will lead process to here. Ignore it and create new one
				svrTrace(Svr::TRC_ENTITY, "Garbage login session information will lead process to here. Ignore it and create new one UID:%0% ticket:%1%", GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket());
				svrChk( RegisterNewPlayerToJoinGameServer() );
			}
			goto Proc_End;
		}

		svrChk(pRes->GetHRESULT());
		svrChk( res.ParseIMsg( pMsgRes->GetMessage() ) );

		GetMyOwner()->HeartBit();

		m_GameServerAddr = res.GetPublicAddress();
		m_GameEntityUID = res.GetRouteContext().From;

		svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ConnectedToGameServer(GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), GetOwnerEntityUID(), m_GameEntityUID));


	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass, TransactionClass>::OnConnectToGameServerRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		DB::QueryConnectedToGameServerCmd* pDBRes = (DB::QueryConnectedToGameServerCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		GetMyOwner()->HeartBit();

		// succeeded to create
		if (pDBRes->Result == 0)
		{
			GetMyOwner()->SetIsTicketOwner(false);
		}
		else
		{
			svrErrClose(E_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	template<class MessageClass, class TransactionClass>
	HRESULT LoginPlayerTransLoginBase<MessageClass,TransactionClass>::StartTransaction()
	{
		HRESULT hr = S_OK;

		//m_RegisterTryCount = 0;
		m_GameEntityUID = 0;

		svrChk( __super::StartTransaction() );

		GetMyOwner()->HeartBit();

		if( GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(E_ALREADY_LOGGEDIN);
		}

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}



	LoginPlayerTransLogin::LoginPlayerTransLogin( Message::MessageData* &pIMsg )
		:LoginPlayerTransLoginBase( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryLoginCmd, { return OnLogin(pRes); });
	}

	HRESULT LoginPlayerTransLogin::OnLogin( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryLoginCmd* pDBRes = (DB::QueryLoginCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		GetMyOwner()->HeartBit();

		// succeeded to login
		if( pDBRes->Result == 0 )
		{
			svrChk( __super::OnLogin( pRes->GetHRESULT(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID ) );
		}
		else
		{
			// Login failed
			if( pDBRes->Result == -1 )
			{
				CloseTransaction(E_INVALID_USERNAME);
			}
			else
			{
				CloseTransaction(E_INVALID_PASSWORD);
			}
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}


	// Start Transaction
	HRESULT LoginPlayerTransLogin::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(E_ALREADY_LOGGEDIN);
		}

		GetMyOwner()->HeartBit();

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->LogIn( GetTransID(), GetID(), GetPassword() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	LoginPlayerTransLoginByFacebook::LoginPlayerTransLoginByFacebook( Message::MessageData* &pIMsg )
		:LoginPlayerTransLoginBase( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryFacebookCreateUserCmd, { return OnUserCreated(pRes); });
		BR_TRANS_MESSAGE( DB::QueryFacebookLoginCmd, { return OnLoginFacebook(pRes); });
	}
	
	HRESULT LoginPlayerTransLoginByFacebook::OnUserCreated( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryFacebookCreateUserCmd* pDBRes = (DB::QueryFacebookCreateUserCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		GetMyOwner()->HeartBit();

		// succeeded to create
		if( pDBRes->Result == 0 )
		{
			svrChk(OnLogin(pDBRes->GetHRESULT(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID));
		}
		else
		{
			svrErr(E_UNEXPECTED);
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}

	HRESULT LoginPlayerTransLoginByFacebook::OnLoginFacebook( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryFacebookLoginCmd* pDBRes = (DB::QueryFacebookLoginCmd*)pRes;
		char email[GameConst::MAX_EMAIL];

		svrChk(pRes->GetHRESULT());

		svrChk(StrUtil::StringLwr(email, GetEMail()));

		GetMyOwner()->HeartBit();

		// succeeded to login
		if( pDBRes->Result == 0 )
		{
			if (StrUtil::StringCmp(email, pDBRes->EMail) != 0)
			{
				svrChk(Svr::GetServerComponent<DB::AccountDB>()->UpdateUserEMail(0, pDBRes->AccountID, email));
			}

			svrChk( OnLogin( pDBRes->GetHRESULT(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID ) );
		}
		else
		{
			// try to create new one
			svrChk( Svr::GetServerComponent<DB::AccountDB>()->FacebookCreateUser( GetTransID(), GetUID(), email ) );
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr; 
	}


	// Start Transaction
	HRESULT LoginPlayerTransLoginByFacebook::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		if( GetUID() == 0 )
		{
			svrErr(E_INVALID_ACCOUNTID);
		}

		if( GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(E_ALREADY_LOGGEDIN);
		}

		GetMyOwner()->HeartBit();

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FacebookLogIn( GetTransID(), GetUID() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	LoginPlayerTransCloseInstance::LoginPlayerTransCloseInstance()
		: TransactionT( TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnDeleteLoginSessionRes(pRes); });
	}

	HRESULT LoginPlayerTransCloseInstance::OnDeleteLoginSessionRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryDeleteLoginSessionCmd* pDBRes = (DB::QueryDeleteLoginSessionCmd*)pRes;

		svrChk(pRes->GetHRESULT());

	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}

	HRESULT LoginPlayerTransCloseInstance::StartTransaction()
	{
		HRESULT hr = S_OK;
		LoginPlayerEntity* pOwner = (LoginPlayerEntity*)GetOwnerEntity();

		svrChk( __super::StartTransaction() );

		if (GetMyOwner()->GetAuthTicket() != 0 && GetMyOwner()->GetIsTicketOwner() )
		{
			svrTrace(Svr::TRC_ENTITY, "Login entity closed without gameserver joining. Delete login session for UID:%0%, ticket:%1%", GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket());
			svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession( GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket() ) );
		}
		else
		{
			svrChk( CloseTransaction(hr) );
		}

	Proc_End:

		if( FAILED(hr) )
			CloseTransaction(hr);

		return hr;
	}

	HRESULT LoginPlayerTransCloseInstance::OnCloseTransaction( HRESULT hrRes )
	{
		// This must be called prior to clear authTicket otherwise the connection must be released already
		//Assert( GetMyOwner()->GetConnection() == nullptr );
		GetMyOwner()->ReleaseConnection();

		Svr::GetServerComponent<Svr::EntityManager>()->RemoveEntity( GetMyOwner() );

		return S_OK;
	}

	


	
	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Login Server service
	//

	LoginPlayerKickPlayerTrans::LoginPlayerKickPlayerTrans( Message::MessageData* &pIMsg )
		: UserTransactionS2SCmd(pIMsg)
	{
		BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnDeleteSession(pRes); } );
	}

	HRESULT LoginPlayerKickPlayerTrans::OnDeleteSession( Svr::TransactionResult *pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryDeleteLoginSessionCmd* pDBRes = (DB::QueryDeleteLoginSessionCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to create
		if( pDBRes->Result >= 0 )
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
			GetMyOwner()->PendingCloseTransaction();
		}
		else
		{
			svrErr(E_UNEXPECTED);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr; 
	}

	// Start Transaction
	HRESULT LoginPlayerKickPlayerTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		// TODO: need this?
		svrChk(E_NOTIMPL);
		//if( GetMyOwner()->GetAccountID() != 0 || GetMyOwner()->GetAccountID() != GetPlayerID() )
		//{
		//	svrErrClose(E_INVALID_PLAYERID);
		//}

		//if( GetMyOwner()->GetAuthTicket() != 0 || GetMyOwner()->GetAuthTicket() != GetAuthTicket() )
		//{
		//	svrErrClose(E_INVALID_TICKET);
		//}

		//svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession( GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket() ) );

	Proc_End:

		if( FAILED(hr) )
		{
			GetMyOwner()->PendingCloseTransaction();
			CloseTransaction( hr );
		}

		return hr;
	}



	

	LoginPlayerJoinedToGameServerTrans::LoginPlayerJoinedToGameServerTrans( Message::MessageData* &pIMsg )
		:UserTransactionS2SCmd( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryConnectedToGameServerCmd, { return OnConnectToGameServerRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryValidateGameServerSessionCmd, { return OnValidateGameServerSessionRes(pRes); });
	}
	
	HRESULT LoginPlayerJoinedToGameServerTrans::OnConnectToGameServerRes( Svr::TransactionResult* &pRes )
	{
		HRESULT hr = S_OK;
		DB::QueryConnectedToGameServerCmd* pDBRes = (DB::QueryConnectedToGameServerCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to create
		if( pDBRes->Result >= 0 )
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
		}
		else
		{
			svrErrClose(E_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr; 
	}

	HRESULT LoginPlayerJoinedToGameServerTrans::OnValidateGameServerSessionRes(Svr::TransactionResult* &pRes)
	{
		HRESULT hr = S_OK;
		DB::QueryValidateGameServerSessionCmd* pDBRes = (DB::QueryValidateGameServerSessionCmd*)pRes;

		svrChk(pRes->GetHRESULT());

		// succeeded to create
		if (pDBRes->Result >= 0)
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
		}
		else
		{
			svrErrClose(E_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	HRESULT LoginPlayerJoinedToGameServerTrans::StartTransaction()
	{
		HRESULT hr = S_OK;

		svrChk( __super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAccountID() != GetPlayerID() )
		{
			svrErrClose(E_INVALID_PLAYERID);
		}

		if( GetMyOwner()->GetAuthTicket() == 0 || GetMyOwner()->GetAuthTicket() != GetAuthTicket() )
		{
			svrErrClose(E_INVALID_TICKET);
		}

		if (GetMyOwner()->GetIsTicketOwner())
		{
			svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ConnectedToGameServer(GetTransID(), GetPlayerID(), GetAuthTicket(), GetOwnerEntityUID(), GetRouteContext().From));
		}
		else
		{
			// just check the ticket
			svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ValidateGameServerSession(GetTransID(), GetPlayerID(), GetAuthTicket(), GetRouteContext().From));
		}

	Proc_End:

		if( FAILED(hr) )
		{
			GetMyOwner()->PendingCloseTransaction();
			CloseTransaction( hr );
		}

		return hr;
	}

	


};// namespace LoginServer 
};// namespace BR 

