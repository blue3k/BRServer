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
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "Memory/MemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Util/TimeUtil.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/ClusterManagerServiceEntity.h"

#include "ServerSystem/ServerService/RankingServerService.h"
#include "ServerSystem/ServerService/GameServerService.h"
#include "Protocol/Message/RankingServerMsgClass.h"


#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"
#include "Protocol/Message/GameServerMsgClass.h"
#include "Protocol/Policy/GameServerNetPolicy.h"

#include "ServerSystem/ServiceEntity/Login/LoginPlayerEntityTrans.h"
#include "ServiceEntity/Login/LoginPlayerEntity.h"

#include "Server/BrServer.h"

#include "DB/AccountDB.h"
#include "DB/AccountQuery.h"
#include "DB/LoginSessionDB.h"
#include "DB/LoginSessionQuery.h"


SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerTransLogin);

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerTransLoginByFacebook);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerTransCreateRandomUser);

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerTransCloseInstance);

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerJoinedToGameServerTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginPlayerKickPlayerTrans);

SF_MEMORYPOOL_IMPLEMENT(SF::Svr::RankingUpdateScoreTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginUserDataTestTrans);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::LoginUserDebugPrintALLRankingTrans);


	
namespace SF {
namespace Svr {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	template<class MessageClass, class TransactionClass>
	LoginPlayerTransLoginBase<MessageClass,TransactionClass>::LoginPlayerTransLoginBase( MessageDataPtr &pIMsg )
		: super( pIMsg )
		, m_CreateRequestCount(0)
	{
		super::SetExclusive(true);
		//BR_TRANS_MESSAGE( DB::QueryLoginCmd, { return OnLogin(pRes); });
		super::template BR_TRANS_MESSAGE( DB::QueryRegisterAuthTicketCmd, { return OnSessionRegistered(pRes); });
		//BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnSessionDeleted(pRes); });
		//BR_TRANS_MESSAGE( DB::QueryReplaceLoginSessionCmd, { return OnSessionReplaced(pRes); });
		//BR_TRANS_MESSAGE( Message::LoginServer::KickPlayerRes, { return OnKickedPlyaer(pRes); });
		super::template BR_TRANS_MESSAGE( Message::GameServer::RegisterPlayerToJoinGameServerRes, { return OnRegisterPlayerToJoinGameServer(pRes); });
		super::template BR_TRANS_MESSAGE(DB::QueryConnectedToGameServerCmd, { return OnConnectToGameServerRes(pRes); });

		m_UserID[0] = '\0';
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass, TransactionClass>::OnGenericError(Svr::TransactionResult* &pRes)
	{
		if (pRes->GetResult() == Result(ResultCode::E_INVALID_PLAYERID) || pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			if (super::GetMyOwner()->GetPlayerID() != 0 && m_CreateRequestCount == 0)
			{
				m_CreateRequestCount++;
				// Garbage login session information will lead process to here. Ignore it and create new one
				svrTrace(Svr::TRC_ENTITY, "Garbage login session information will lead process to here. Ignore it and create new one UID:{0} ticket:{1}",
					super::GetMyOwner()->GetPlayerID(), super::GetMyOwner()->GetAuthTicket());
				if (!(RegisterNewPlayerToJoinGameServer()))
				{
					return super::OnGenericError(pRes);
				}
			}
			else
			{
				return super::OnGenericError(pRes);
			}
		}
		else
		{
			return super::OnGenericError(pRes);
		}

		return ResultCode::SUCCESS;
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnLogin( Result hrRes, AccountID accountID, FacebookUID FBUserID, INT shardID )
	{
		Result hr = ResultCode::SUCCESS;
		AuthTicket newTicket = 0;

		svrChkClose(hrRes);

		super::GetMyOwner()->HeartBit();

		super::GetMyOwner()->SetIsTicketOwner(false);

		// succeeded to login
		super::GetMyOwner()->SetAccountID(accountID);
		super::GetMyOwner()->SetFacebookUID(FBUserID);
		super::GetMyOwner()->SetShardID(shardID);

		// Generate new authenticate ID
		newTicket = super::GetMyOwner()->GetAuthTicketGenerator().NewUID();
		svrTrace(Svr::TRC_DBGINFO, "New Player Ticket Ticket:{0}", newTicket);
		super::GetMyOwner()->SetAuthTicket(newTicket);

		// register new ticket
		svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->RegisterAuthTicket(super::GetTransID(), super::GetMyOwner()->GetPlayerID(), newTicket, super::GetMyOwner()->GetEntityUID() ) );

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnSessionRegistered( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryRegisterAuthTicketCmd* pDBRes = (DB::QueryRegisterAuthTicketCmd*)pRes;
		Policy::IPolicyGameServer *pGameServerPolicy = nullptr;

		svrChk(pRes->GetResult());

		super::GetMyOwner()->SetIsTicketOwner(pDBRes->Result != 0);
		m_GameEntityUID = pDBRes->GameEntityUID;

		super::GetMyOwner()->HeartBit();

		// if someone already logged in
		if (pDBRes->Result != 0)
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE);
		}
		else
		{
			Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
			// To accomodate login only services, allow login without game server
			if (!Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity((ClusterID)((uint)ClusterID::Game + (uint)super::GetGameID()), pServiceEntity)
				|| pServiceEntity->GetNumberOfNonWatcherServices() == 0)
			{
				// Login is succeeded, but there is no game cluster for the game or no available game service
				CloseTransaction(hr);
				goto Proc_End;
			}

			if( m_GameEntityUID == 0 )
			{
				svrTrace(Svr::TRC_ENTITY, "No login session");
				svrChk( RegisterNewPlayerToJoinGameServer() );
			}
			else
			{
				pGameServerPolicy = Svr::GetServerComponent<Svr::ServerEntityManager>()->GetServerPolicy<Policy::IPolicyGameServer>(m_GameEntityUID.GetServerID());
				if (pGameServerPolicy == nullptr
					|| !(pGameServerPolicy->RegisterPlayerToJoinGameServerCmd(RouteContext(super::GetOwnerEntityUID(), m_GameEntityUID), super::GetTransID(),
						super::GetMyOwner()->GetPlayerID(), super::GetMyOwner()->GetAuthTicket(), super::GetMyOwner()->GetFacebookUID(), super::GetMyOwner()->GetShardID())))
				{
					svrChk(RegisterNewPlayerToJoinGameServer());
				}
			}
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass,TransactionClass>::RegisterNewPlayerToJoinGameServer()
	{
		Result hr = ResultCode::SUCCESS;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pService = nullptr;

		// Find new game server for this player
		svrChk( Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity( (ClusterID)((uint)ClusterID::Game + (uint)super::GetGameID()), pServiceEntity ) );
		hr = pServiceEntity->FindRandomService( pService );
		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find cluster service entity for game:{0} PlayerID:{1}", super::GetGameID(), super::GetMyOwner()->GetPlayerID());
			goto Proc_End;
		}

		super::GetMyOwner()->HeartBit();

		svrTrace(Svr::TRC_ENTITY, "Creating new Entity for PID:{0}, on svr:{1}", super::GetMyOwner()->GetPlayerID(), pService->GetEntityUID());

		svrChk( pService->GetService<Svr::GameServerService>()->RegisterPlayerToJoinGameServerCmd(super::GetTransID(),
			super::GetMyOwner()->GetPlayerID(), super::GetMyOwner()->GetAuthTicket(), super::GetMyOwner()->GetFacebookUID(), super::GetMyOwner()->GetShardID()));
	Proc_End:

		return hr; 
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass,TransactionClass>::OnRegisterPlayerToJoinGameServer( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::GameServer::RegisterPlayerToJoinGameServerRes res;

		if( pRes->GetResult() == Result(ResultCode::E_INVALID_PLAYERID) || pRes->GetResult() == Result(ResultCode::SVR_INVALID_ENTITYUID))
		{
			if (super::GetMyOwner()->GetPlayerID() == 0)
			{
				hr = pRes->GetResult();
			}
			else
			{
				// Garbage login session information will lead process to here. Ignore it and create new one
				svrTrace(Svr::TRC_ENTITY, "Garbage login session information will lead process to here. Ignore it and create new one UID:{0} ticket:{1}", 
					super::GetMyOwner()->GetPlayerID(), super::GetMyOwner()->GetAuthTicket());
				svrChkSilent( RegisterNewPlayerToJoinGameServer() );
			}
			goto Proc_End;
		}

		svrChk(pRes->GetResult());
		svrChk( res.ParseMessage( pMsgRes->GetMessage() ) );

		super::GetMyOwner()->HeartBit();

		if (!StrUtil::IsNullOrEmpty(res.GetPublicAddressV6()))
		{
			svrChk(Net::SetNetAddress(m_GameServerAddr, res.GetPublicAddressV6(), (USHORT)res.GetPort()));
		}
		svrChk(Net::SetNetAddress(m_GameServerAddrIPV4, res.GetPublicAddress(), (USHORT)res.GetPort()));
		m_GameEntityUID = res.GetRouteContext().GetFrom();

		svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ConnectedToGameServer(super::GetTransID(), super::GetMyOwner()->GetPlayerID(), super::GetMyOwner()->GetAuthTicket(), super::GetOwnerEntityUID(), m_GameEntityUID));


	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass, TransactionClass>::OnConnectToGameServerRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryConnectedToGameServerCmd* pDBRes = (DB::QueryConnectedToGameServerCmd*)pRes;

		svrChk(pRes->GetResult());

		super::GetMyOwner()->HeartBit();

		// succeeded to create
		if (pDBRes->Result == 0)
		{
			super::GetMyOwner()->SetIsTicketOwner(false);
		}
		else
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		CloseTransaction(hr);

		return ResultCode::SUCCESS;
	}

	// Start Transaction
	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass,TransactionClass>::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		//m_RegisterTryCount = 0;
		m_GameEntityUID = 0;

		svrChk( super::StartTransaction() );

		super::GetMyOwner()->HeartBit();

		if(super::GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN);
		}

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}

	template<class MessageClass, class TransactionClass>
	Result LoginPlayerTransLoginBase<MessageClass, TransactionClass>::OnCloseTransaction(Result hrRes)
	{
		if (!super::IsClosed())
		{
			svrTrace(TRC_INFO, "LoginRequest result of {0}, hr={1}", m_UserID, hrRes);
		}

		Result result = super::OnCloseTransaction(hrRes);

		return result;
	}



	LoginPlayerTransLogin::LoginPlayerTransLogin( MessageDataPtr &pIMsg )
		:LoginPlayerTransLoginBase( pIMsg )
	{
		BR_TRANS_MESSAGE( DB::QueryLoginCmd, { return OnLogin(pRes); });
	}

	Result LoginPlayerTransLogin::OnLogin( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryLoginCmd* pDBRes = (DB::QueryLoginCmd*)pRes;

		svrChk(pRes->GetResult());

		GetMyOwner()->HeartBit();

		GetMyOwner()->SetUserName(GetID());

		// succeeded to login
		if( pDBRes->Result == 0 )
		{
			svrChk( super::OnLogin( pRes->GetResult(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID ) );
		}
		else
		{
			// Login failed
			if( pDBRes->Result == -1 )
			{
				CloseTransaction(ResultCode::E_LOGIN_INVALID_USERNAME);
			}
			else
			{
				CloseTransaction(ResultCode::E_LOGIN_INVALID_PASSWORD);
			}
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}


	// Start Transaction
	Result LoginPlayerTransLogin::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN);
		}

		GetMyOwner()->HeartBit();

		StrUtil::StringCpy(m_UserID, GetID());

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->LogIn( GetTransID(), GetID(), GetPassword() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}


	LoginPlayerTransLoginByFacebook::LoginPlayerTransLoginByFacebook( MessageDataPtr &pIMsg )
		:LoginPlayerTransLoginBase( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryFacebookCreateUserCmd, { return OnUserCreated(pRes); });
		BR_TRANS_MESSAGE( DB::QueryFacebookLoginCmd, { return OnLoginFacebook(pRes); });
	}
	
	Result LoginPlayerTransLoginByFacebook::OnUserCreated( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryFacebookCreateUserCmd* pDBRes = (DB::QueryFacebookCreateUserCmd*)pRes;

		svrChk(pRes->GetResult());

		GetMyOwner()->HeartBit();

		// succeeded to create
		if( pDBRes->Result == 0 )
		{
			svrChk(OnLogin(pDBRes->GetResult(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID));
		}
		else
		{
			svrTrace(Trace::TRC_ERROR, "Failed to create player FBUID:{0}, email:{1}, result:{2}", GetUID(), pDBRes->EMail, pDBRes->Result);
			hr = ResultCode::E_INVALID_VALUE;
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return ResultCode::SUCCESS; 
	}

	Result LoginPlayerTransLoginByFacebook::OnLoginFacebook( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryFacebookLoginCmd* pDBRes = (DB::QueryFacebookLoginCmd*)pRes;
		char email[GameConst::MAX_EMAIL];

		svrChk(pRes->GetResult());


		if (GetEMail() == nullptr || GetEMail()[0] == '\0')
		{
			// Generate fake&unique email from UID
			svrChk(StrUtil::Format(email, "{0}@braveplayer.com__", GetUID()));
		}
		else
		{
			svrChk(StrUtil::StringLwr(email, GetEMail()));
		}


		GetMyOwner()->HeartBit();

		// succeeded to login
		if( pDBRes->Result == 0 )
		{
			if (StrUtil::StringCmp(email, pDBRes->EMail) != 0)
			{
				svrChk(Svr::GetServerComponent<DB::AccountDB>()->UpdateUserContactInfo(0, pDBRes->AccountID, email, ""));
			}

			svrChk( OnLogin( pDBRes->GetResult(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID ) );
		}
		else
		{
			// try to create new one
			svrChk(Svr::GetServerComponent<DB::AccountDB>()->FacebookCreateUser(GetTransID(), GetUID(), email, ""));
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return hr; 
	}


	// Start Transaction
	Result LoginPlayerTransLoginByFacebook::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetUID() == 0 )
		{
			svrErr(ResultCode::E_INVALID_ACCOUNTID);
		}

		if( GetMyOwner()->GetAccountID() != 0 )
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN);
		}

		GetMyOwner()->HeartBit();

		StrUtil::Format(m_UserID, "{0}", GetUID());

		svrChk( Svr::GetServerComponent<DB::AccountDB>()->FacebookLogIn( GetTransID(), GetUID() ) );

	Proc_End:

		if( !(hr) )
		{
			CloseTransaction( hr );
		}

		return hr;
	}






	LoginPlayerTransCreateRandomUser::LoginPlayerTransCreateRandomUser(MessageDataPtr &pIMsg)
		:LoginPlayerTransLoginBase(pIMsg)
	{
		BR_TRANS_MESSAGE(DB::QueryCreateRandomUserCmd, { return OnCreated(pRes); });
	}

	Result LoginPlayerTransCreateRandomUser::OnCreated(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		auto* pDBRes = (DB::QueryCreateRandomUserCmd*)pRes;

		svrChk(pRes->GetResult());

		GetMyOwner()->HeartBit();

		GetMyOwner()->SetUserName(GetCellPhone());

		// succeeded to login
		if (pDBRes->Result == 0)
		{
			svrChk(super::OnLogin(pRes->GetResult(), pDBRes->AccountID, pDBRes->FBUserID, pDBRes->ShardID));
		}
		else
		{
			CloseTransaction(ResultCode::E_LOGIN_INVALID_SIGNATURE);
		}

	Proc_End:

		if (!(hr))
			CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result LoginPlayerTransCreateRandomUser::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() != 0)
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN);
		}

		GetMyOwner()->HeartBit();

		if (GetCellPhone() == nullptr || GetCellPhone()[0] == '\0')
		{
			svrErrClose(ResultCode::E_LOGIN_INVALID_SIGNATURE);
		}

		svrChk(StrUtil::Format(m_UserID, "{0}", GetCellPhone()));

		svrChk(Svr::GetServerComponent<DB::AccountDB>()->CreateRandomUser(GetTransID(), m_UserID, GetCellPhone()));

	Proc_End:

		if (!(hr))
		{
			CloseTransaction(hr);
		}

		return hr;
	}




	LoginPlayerTransCloseInstance::LoginPlayerTransCloseInstance()
		: TransactionT( TransactionID() )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnDeleteLoginSessionRes(pRes); });
	}

	Result LoginPlayerTransCloseInstance::OnDeleteLoginSessionRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		//DB::QueryDeleteLoginSessionCmd* pDBRes = (DB::QueryDeleteLoginSessionCmd*)pRes;

		svrChk(pRes->GetResult());

	Proc_End:

		CloseTransaction(hr);

		return hr; 
	}

	Result LoginPlayerTransCloseInstance::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		//LoginPlayerEntity* pOwner = (LoginPlayerEntity*)GetOwnerEntity();

		svrChk( super::StartTransaction() );

		if (GetMyOwner()->GetAuthTicket() != 0 && GetMyOwner()->GetIsTicketOwner() )
		{
			svrTrace(Svr::TRC_ENTITY, "Login entity closed without gameserver joining. Delete login session for UID:{0}, ticket:{1}", GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket());
			svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession( GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket() ) );
		}
		else
		{
			svrChk( CloseTransaction(hr) );
		}

	Proc_End:

		if( !(hr) )
			CloseTransaction(hr);

		return hr;
	}

	Result LoginPlayerTransCloseInstance::OnCloseTransaction( Result hrRes )
	{
		// This must be called prior to clear authTicket otherwise the connection must be released already
		GetMyOwner()->ReleaseConnection();

		Svr::GetServerComponent<Svr::EntityManager>()->RemoveEntity( GetMyOwner() );

		return ResultCode::SUCCESS;
	}

	


	
	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Login Server service
	//

	LoginPlayerKickPlayerTrans::LoginPlayerKickPlayerTrans( MessageDataPtr &pIMsg )
		: UserTransactionS2SCmd(pIMsg)
	{
		BR_TRANS_MESSAGE( DB::QueryDeleteLoginSessionCmd, { return OnDeleteSession(pRes); } );
	}

	Result LoginPlayerKickPlayerTrans::OnDeleteSession( Svr::TransactionResult *pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryDeleteLoginSessionCmd* pDBRes = (DB::QueryDeleteLoginSessionCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to create
		if( pDBRes->Result >= 0 )
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
			GetMyOwner()->PendingCloseTransaction();
		}
		else
		{
			svrErr(ResultCode::UNEXPECTED);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr; 
	}

	// Start Transaction
	Result LoginPlayerKickPlayerTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		// TODO: need this?
		svrChk(ResultCode::NOT_IMPLEMENTED);
		//if( GetMyOwner()->GetAccountID() != 0 || GetMyOwner()->GetAccountID() != GetPlayerID() )
		//{
		//	svrErrClose(ResultCode::E_INVALID_PLAYERID);
		//}

		//if( GetMyOwner()->GetAuthTicket() != 0 || GetMyOwner()->GetAuthTicket() != GetAuthTicket() )
		//{
		//	svrErrClose(ResultCode::E_INVALID_TICKET);
		//}

		//svrChk( Svr::GetServerComponent<DB::LoginSessionDB>()->DeleteLoginSession( GetTransID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket() ) );

	Proc_End:

		if( !(hr) )
		{
			GetMyOwner()->PendingCloseTransaction();
			CloseTransaction( hr );
		}

		return hr;
	}





	LoginPlayerJoinedToGameServerTrans::LoginPlayerJoinedToGameServerTrans( MessageDataPtr &pIMsg )
		:UserTransactionS2SCmd( pIMsg )
	{
		SetExclusive(true);
		BR_TRANS_MESSAGE(DB::QueryConnectedToGameServerCmd, { return OnConnectToGameServerRes(pRes); });
		BR_TRANS_MESSAGE(DB::QueryValidateGameServerSessionCmd, { return OnValidateGameServerSessionRes(pRes); });
	}

	Result LoginPlayerJoinedToGameServerTrans::OnConnectToGameServerRes( Svr::TransactionResult* &pRes )
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryConnectedToGameServerCmd* pDBRes = (DB::QueryConnectedToGameServerCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to create
		if( pDBRes->Result >= 0 )
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
		}
		else
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr; 
	}

	Result LoginPlayerJoinedToGameServerTrans::OnValidateGameServerSessionRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;
		DB::QueryValidateGameServerSessionCmd* pDBRes = (DB::QueryValidateGameServerSessionCmd*)pRes;

		svrChk(pRes->GetResult());

		// succeeded to create
		if (pDBRes->Result >= 0)
		{
			// Clear local ticket value and close entity
			GetMyOwner()->SetAuthTicket(0);
		}
		else
		{
			svrErrClose(ResultCode::E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE);
		}

	Proc_End:

		GetMyOwner()->PendingCloseTransaction();
		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result LoginPlayerJoinedToGameServerTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		if( GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAccountID() != GetPlayerID() )
		{
			svrErrClose(ResultCode::E_INVALID_PLAYERID);
		}

		if( GetMyOwner()->GetAuthTicket() == 0 || GetMyOwner()->GetAuthTicket() != GetAuthTicket() )
		{
			svrErrClose(ResultCode::E_INVALID_TICKET);
		}

		if (GetMyOwner()->GetIsTicketOwner())
		{
			svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ConnectedToGameServer(GetTransID(), GetPlayerID(), GetAuthTicket(), GetOwnerEntityUID(), GetRouteContext().GetFrom()));
		}
		else
		{
			// just check the ticket
			svrChk(Svr::GetServerComponent<DB::LoginSessionDB>()->ValidateGameServerSession(GetTransID(), GetPlayerID(), GetAuthTicket(), GetRouteContext().GetFrom()));
		}

	Proc_End:

		if( !(hr) )
		{
			GetMyOwner()->PendingCloseTransaction();
			CloseTransaction( hr );
		}

		return hr;
	}

	

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ranking handling
	//

	RankingUpdateScoreTrans::RankingUpdateScoreTrans(MessageDataPtr &pIMsg)
		: MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::RankingServer::UpdatePlayerScoreRes, { return OnScoreUpdated(pRes); });
	}

	Result RankingUpdateScoreTrans::OnScoreUpdated(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::RankingServer::UpdatePlayerScoreRes res;

		svrChk(pRes->GetResult());

		svrChk(res.ParseMessage(pMsgRes->GetMessage()));

		super::GetMyOwner()->HeartBit();

		for (auto itRank : res.GetRanking())
		{
			svrChk(m_RankingList.Add(itRank));
		}

		svrTrace(TRC_INFO, "Ranking Score updated {0} : {1}", GetMyOwner()->GetUserName(), GetRankingScore());


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result RankingUpdateScoreTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PlayerInformation playerInfo;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk(super::StartTransaction());

		if (GetMyOwner()->GetAccountID() == 0)
		{
			svrErrClose(ResultCode::E_INVALID_PLAYERID);
		}

		m_RankingList.Clear();

		// TODO: RankingType


		// Find new ranking server
		svrChk(Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity(ClusterID::Ranking, pServiceEntity));
		hr = pServiceEntity->FindRandomService(pService);
		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find ranking service");
			goto Proc_End;
		}

		// update life time of this user entity
		super::GetMyOwner()->HeartBit();

		svrChk(playerInfo.InitPlayerInformation(GetMyOwner()->GetAccountID(), GetMyOwner()->GetFacebookUID(), GetMyOwner()->GetUserName(), 0, 0, Util::Time.GetTimeUTCSec().time_since_epoch().count()));

		svrChk(pService->GetService<Svr::RankingServerService>()->UpdatePlayerScoreCmd(super::GetTransID(), GetRankingScore(), playerInfo, GetCount()));


	Proc_End:

		if (!(hr))
		{
			CloseTransaction(hr);
		}

		return hr;
	}




	std::atomic<uint32_t> LoginUserDataTestTrans::stm_TestCount(0);

	LoginUserDataTestTrans::LoginUserDataTestTrans(MessageDataPtr &pIMsg)
		: MessageTransaction(pIMsg)
	{
	}
	// Start Transaction
	Result LoginUserDataTestTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		uint32_t testCount;

		svrChk(super::StartTransaction());

		// update life time of this user entity
		super::GetMyOwner()->HeartBit();

		m_Data.AddItems(GetTestData().GetSize(), GetTestData().data());
		testCount = stm_TestCount.fetch_add(1, std::memory_order_relaxed);
		if ((testCount % 50) == 0)
		{
			svrTrace(TRC_INFO, "TestData processed count:{0}", testCount);
		}

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}





	LoginUserDebugPrintALLRankingTrans::LoginUserDebugPrintALLRankingTrans(MessageDataPtr &pIMsg)
		: MessageTransaction(pIMsg)
	{
		BR_TRANS_MESSAGE(Message::RankingServer::DebugPrintALLRankingRes, { return OnPrintAllRankingRes(pRes); });
	}

	Result LoginUserDebugPrintALLRankingTrans::OnPrintAllRankingRes(Svr::TransactionResult* &pRes)
	{
		Result hr = ResultCode::SUCCESS;

		Svr::MessageResult *pMsgRes = (Svr::MessageResult*)pRes;
		Message::RankingServer::UpdatePlayerScoreRes res;

		svrChk(pRes->GetResult());


	Proc_End:

		CloseTransaction(hr);

		return hr;
	}

	// Start Transaction
	Result LoginUserDebugPrintALLRankingTrans::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;
		PlayerInformation playerInfo;
		Svr::ClusteredServiceEntity *pServiceEntity = nullptr;
		Svr::ServerServiceInformation *pService = nullptr;

		svrChk(super::StartTransaction());


		// Find new ranking server
		svrChk(Svr::GetServerComponent<Svr::ClusterManagerServiceEntity>()->GetClusterServiceEntity(ClusterID::Ranking, pServiceEntity));
		hr = pServiceEntity->FindRandomService(pService);
		if (!(hr))
		{
			svrTrace(Trace::TRC_ERROR, "Failed to find ranking service");
			goto Proc_End;
		}

		// update life time of this user entity
		super::GetMyOwner()->HeartBit();


		svrChk(pService->GetService<Svr::RankingServerService>()->DebugPrintALLRankingCmd(super::GetTransID(), GetFileName()));


	Proc_End:

		if (!(hr))
		{
			CloseTransaction(hr);
		}

		return hr;
	}



};// namespace Svr 
};// namespace SF 

