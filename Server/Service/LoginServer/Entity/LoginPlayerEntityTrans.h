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

#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginIPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Policy/LoginServerIPolicy.h"

#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/ServerEntity.h"

#include "LoginServerClass.h"

#include "Entity/LoginPlayerEntity.h"
#include "Common/Message.h"



namespace BR {
namespace LoginServer {


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

	public:
		LoginPlayerTransLoginBase( Message::MessageData* &pIMsg );
		virtual ~LoginPlayerTransLoginBase() {}

		HRESULT OnLogin( HRESULT hrRes, AccountID accountID, FacebookUID FBUserID, INT shardID );
		HRESULT OnSessionRegistered( Svr::TransactionResult* &pRes );
		HRESULT OnRegisterPlayerToJoinGameServer( Svr::TransactionResult* &pRes );
		HRESULT OnConnectToGameServerRes(Svr::TransactionResult* &pRes);

		HRESULT RegisterNewPlayerToJoinGameServer();



		// Start Transaction
		virtual HRESULT StartTransaction() override;

		// dummies for template chainning
		virtual HRESULT CloseTransaction(HRESULT hrRes) override { return super::CloseTransaction(hrRes); }

		//BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(LoginRes,m_GameServerAddr,GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID(), GetMyOwner()->GetGameNick());
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

		HRESULT OnLogin( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

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

		HRESULT OnUserCreated( Svr::TransactionResult* &pRes );
		HRESULT OnLoginFacebook( Svr::TransactionResult* &pRes );


		// Start Transaction
		virtual HRESULT StartTransaction() override;

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

		HRESULT OnCreated(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

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

		HRESULT OnDeleteLoginSessionRes( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual HRESULT StartTransaction() override;

		virtual HRESULT OnCloseTransaction( HRESULT hrRes ) override;

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

		HRESULT OnDeleteSession( Svr::TransactionResult *pRes );

		// Start Transaction
		virtual HRESULT StartTransaction();

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

		HRESULT OnConnectToGameServerRes( Svr::TransactionResult* &pRes );
		HRESULT OnValidateGameServerSessionRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual HRESULT StartTransaction();

		BR_SVR_MSGTRANS_CLOSE(PlayerJoinedToGameServerRes,GetRouteContext().GetSwaped());
	};


} // namespace LoginServer 
} // namespace BR 

