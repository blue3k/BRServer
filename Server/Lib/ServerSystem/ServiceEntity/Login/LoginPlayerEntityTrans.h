////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"
#include "Common/Memento.h"
#include "Common/ArrayUtil.h"
#include "Common/BrBaseTypes.h"
#include "Common/Message.h"

#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginIPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Policy/LoginServerIPolicy.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServerEntity.h"

#include "ServerSystem/ServiceEntity/Login/LoginPlayerEntity.h"




namespace BR {
namespace Svr {


	template<class MessageClass, class TransactionClass>
	class LoginPlayerTransLoginBase : public Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, MessageClass, TransactionClass, sizeof(Svr::TransactionMessageHandlerType)*6 >
	{
	private:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, MessageClass, TransactionClass, sizeof(Svr::TransactionMessageHandlerType) * 6 > super;

	protected:

		NetAddress m_GameServerAddr;
		NetAddress m_GameServerAddrIPV4;
		//UINT m_RegisterTryCount;
		EntityUID m_GameEntityUID;

		UINT m_CreateRequestCount;

	public:
		LoginPlayerTransLoginBase( Message::MessageData* &pIMsg );
		virtual ~LoginPlayerTransLoginBase() {}

		Result OnLogin( Result hrRes, AccountID accountID, FacebookUID FBUserID, INT shardID );
		Result OnSessionRegistered( Svr::TransactionResult* &pRes );
		Result OnRegisterPlayerToJoinGameServer( Svr::TransactionResult* &pRes );
		Result OnConnectToGameServerRes(Svr::TransactionResult* &pRes);

		Result RegisterNewPlayerToJoinGameServer();

		virtual Result OnGenericError(Svr::TransactionResult* &pRes) override;

		// Start Transaction
		virtual Result StartTransaction() override;

		// dummies for template chainning
		virtual Result CloseTransaction(Result hrRes) override { return super::CloseTransaction(hrRes); }
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	class LoginPlayerTransLogin : public LoginPlayerTransLoginBase<Message::Login::LoginCmd, LoginPlayerTransLogin>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::LoginCmd, LoginPlayerTransLogin> super;

	public:
		LoginPlayerTransLogin( Message::MessageData* &pIMsg );
		virtual ~LoginPlayerTransLogin() {}

		Result OnLogin( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(LoginRes,m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	class LoginPlayerTransLoginByFacebook : public LoginPlayerTransLoginBase<Message::Login::LoginByFacebookCmd, LoginPlayerTransLoginByFacebook>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::LoginByFacebookCmd, LoginPlayerTransLoginByFacebook> super;

	public:
		LoginPlayerTransLoginByFacebook( Message::MessageData* &pIMsg );
		virtual ~LoginPlayerTransLoginByFacebook() {}

		Result OnUserCreated( Svr::TransactionResult* &pRes );
		Result OnLoginFacebook( Svr::TransactionResult* &pRes );


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(LoginByFacebookRes,m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Login
	//

	class LoginPlayerTransCreateRandomUser : public LoginPlayerTransLoginBase<Message::Login::CreateRandomUserCmd, LoginPlayerTransCreateRandomUser>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::CreateRandomUserCmd, LoginPlayerTransCreateRandomUser> super;

	public:
		LoginPlayerTransCreateRandomUser(Message::MessageData* &pIMsg);
		virtual ~LoginPlayerTransCreateRandomUser() {}

		Result OnCreated(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(CreateRandomUserRes, m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};

	

	// Close zone instance
	class LoginPlayerTransCloseInstance : public Svr::TransactionT<LoginPlayerEntity, LoginPlayerTransCloseInstance,sizeof(Svr::TransactionMessageHandlerType)*2>
	{
	public:
		typedef Svr::TransactionT<LoginPlayerEntity, LoginPlayerTransCloseInstance, sizeof(Svr::TransactionMessageHandlerType) * 2> super;

	public:
		LoginPlayerTransCloseInstance();
		virtual ~LoginPlayerTransCloseInstance() {}

		Result OnDeleteLoginSessionRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		virtual Result OnCloseTransaction( Result hrRes ) override;

	};





	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Login Server service
	//

	class LoginPlayerKickPlayerTrans : public Svr::UserTransactionS2SCmd< LoginPlayerEntity, Policy::ISvrPolicyLoginServer, Message::LoginServer::KickPlayerCmd, LoginPlayerKickPlayerTrans, sizeof(Svr::TransactionMessageHandlerType)*1>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< LoginPlayerEntity, Policy::ISvrPolicyLoginServer, Message::LoginServer::KickPlayerCmd, LoginPlayerKickPlayerTrans, sizeof(Svr::TransactionMessageHandlerType) * 1> super;

	public:

		LoginPlayerKickPlayerTrans( Message::MessageData* &pIMsg );// : UserTransactionS2SCmd(pIMsg) {}
		virtual ~LoginPlayerKickPlayerTrans() {}

		Result OnDeleteSession( Svr::TransactionResult *pRes );

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(KickPlayerRes,GetRouteContext().GetSwaped());
	};


	class LoginPlayerJoinedToGameServerTrans : public Svr::UserTransactionS2SCmd< LoginPlayerEntity, Policy::ISvrPolicyLoginServer, Message::LoginServer::PlayerJoinedToGameServerCmd, LoginPlayerJoinedToGameServerTrans, sizeof(Svr::TransactionMessageHandlerType)*1>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< LoginPlayerEntity, Policy::ISvrPolicyLoginServer, Message::LoginServer::PlayerJoinedToGameServerCmd, LoginPlayerJoinedToGameServerTrans, sizeof(Svr::TransactionMessageHandlerType) * 1> super;

	private:
		GameInsUID m_GameUID;
		PartyUID m_PartyUID;
		MatchingQueueTicket m_MatchingTicket;

	public:

		LoginPlayerJoinedToGameServerTrans( Message::MessageData* &pIMsg );
		virtual ~LoginPlayerJoinedToGameServerTrans() {}

		Result OnConnectToGameServerRes( Svr::TransactionResult* &pRes );
		Result OnValidateGameServerSessionRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(PlayerJoinedToGameServerRes,GetRouteContext().GetSwaped());
	};





	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ranking handling
	//

	class RankingUpdateScoreTrans : public Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, Message::Login::UpdateMyScoreCmd, RankingUpdateScoreTrans, sizeof(Svr::TransactionMessageHandlerType) * 1 >
	{
	public:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, Message::Login::UpdateMyScoreCmd, RankingUpdateScoreTrans, sizeof(Svr::TransactionMessageHandlerType) * 1 > super;

	private:

		StaticArray<TotalRankingPlayerInformation, 20> m_RankingList;

	public:
		RankingUpdateScoreTrans(Message::MessageData* &pIMsg);
		virtual ~RankingUpdateScoreTrans() {}

		Result OnScoreUpdated(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(UpdateMyScoreRes, m_RankingList);
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ranking handling
	//

	class LoginUserDataTestTrans : public Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, Message::Login::DataTestCmd, LoginUserDataTestTrans, 1 >
	{
	public:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Policy::ISvrPolicyLogin, Message::Login::DataTestCmd, LoginUserDataTestTrans, 1 > super;

	private:

		static std::atomic<uint32_t> stm_TestCount;
		StaticArray<uint8_t, 30 * 1024> m_Data;

	public:
		LoginUserDataTestTrans(Message::MessageData* &pIMsg);
		virtual ~LoginUserDataTestTrans() {}


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(DataTestRes, m_Data);
	};



} // namespace Svr 
} // namespace BR 

