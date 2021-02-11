////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/SFMemoryPool.h"
#include "Util/Memento.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "Net/SFMessage.h"

#include "Protocol/Message/LoginMsgClass.h"
#include "Protocol/Policy/LoginNetPolicy.h"
#include "Protocol/Message/LoginServerMsgClass.h"
#include "Protocol/Policy/LoginServerNetPolicy.h"

#include "Transaction/MessageRoute.h"
#include "ServerEntity/ServerEntity.h"

#include "ServiceEntity/Login/LoginPlayerEntity.h"




namespace SF {


	template<class MessageClass>
	class LoginPlayerTransLoginBase : public Svr::MessageTransaction< LoginPlayerEntity, MessageClass >
	{
	private:
		typedef Svr::MessageTransaction< LoginPlayerEntity, MessageClass> super;

	protected:

		NetAddress m_GameServerAddr;
		NetAddress m_GameServerAddrIPV4;
		//uint m_RegisterTryCount;
		EntityUID m_GameEntityUID;

		uint m_CreateRequestCount;

		char m_UserID[128];


	public:
		LoginPlayerTransLoginBase(IHeap& memMgr, MessageDataPtr &pIMsg );
		virtual ~LoginPlayerTransLoginBase() {}

		Result OnLogin( Result hrRes, AccountID accountID, FacebookUID FBUserID, INT shardID );
		Result OnSessionRegistered( Svr::TransactionResult* &pRes );
		//Result OnRegisterPlayerToJoinGameServer( Svr::TransactionResult* &pRes );
		//Result OnConnectToGameServerRes(Svr::TransactionResult* &pRes);

		//Result RegisterNewPlayerToJoinGameServer();

		virtual Result OnGenericError(Svr::TransactionResult* pRes) override;

		// Start Transaction
		virtual Result StartTransaction() override;

		// dummies for template chaining
		virtual Result CloseTransaction(Result hrRes) override { return super::CloseTransaction(hrRes); }

		virtual Result OnCloseTransaction(Result hrRes) override;
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	class LoginPlayerTransLogin : public LoginPlayerTransLoginBase<Message::Login::LoginCmd>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::LoginCmd> super;

	public:
		LoginPlayerTransLogin(IHeap& memMgr, MessageDataPtr &pIMsg );
		virtual ~LoginPlayerTransLogin() {}

		Result OnLogin( Svr::TransactionResult* &pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyLogin, LoginRes, m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Game command transaction
	//

	class LoginPlayerTransLoginByFacebook : public LoginPlayerTransLoginBase<Message::Login::LoginByFacebookCmd>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::LoginByFacebookCmd> super;

	public:
		LoginPlayerTransLoginByFacebook(IHeap& memMgr, MessageDataPtr &pIMsg );
		virtual ~LoginPlayerTransLoginByFacebook() {}

		Result OnUserCreated( Svr::TransactionResult* &pRes );
		Result OnLoginFacebook( Svr::TransactionResult* &pRes );


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyLogin, LoginByFacebookRes,m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Login
	//

	class LoginPlayerTransCreateRandomUser : public LoginPlayerTransLoginBase<Message::Login::CreateRandomUserCmd>
	{
	public:
		typedef LoginPlayerTransLoginBase<Message::Login::CreateRandomUserCmd> super;

	public:
		LoginPlayerTransCreateRandomUser(IHeap& memMgr, MessageDataPtr &pIMsg);
		virtual ~LoginPlayerTransCreateRandomUser() {}

		Result OnCreated(Svr::TransactionResult* pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyLogin, CreateRandomUserRes, m_GameServerAddr, m_GameServerAddrIPV4, GetMyOwner()->GetPlayerID(), GetMyOwner()->GetAuthTicket(), (Context)GetMyOwner()->GetEntityUID());
	};

	

	// Close zone instance
	class LoginPlayerTransCloseInstance : public Svr::TransactionT<LoginPlayerEntity>
	{
	public:
		typedef Svr::TransactionT<LoginPlayerEntity> super;

	public:
		LoginPlayerTransCloseInstance(IHeap& memMgr);
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

	class LoginPlayerKickPlayerTrans : public Svr::UserTransactionS2SCmd< LoginPlayerEntity, Message::LoginServer::KickPlayerCmd>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< LoginPlayerEntity, Message::LoginServer::KickPlayerCmd> super;

	public:

		LoginPlayerKickPlayerTrans(IHeap& memMgr, MessageDataPtr &pIMsg );// : UserTransactionS2SCmd(pIMsg) {}
		virtual ~LoginPlayerKickPlayerTrans() {}

		Result OnDeleteSession( Svr::TransactionResult *pRes );

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyLoginServer, KickPlayerRes,GetRouteContext().GetSwaped());
	};


	class LoginPlayerJoinedToGameServerTrans : public Svr::UserTransactionS2SCmd< LoginPlayerEntity, Message::LoginServer::PlayerJoinedToGameServerCmd>
	{
	public:
		typedef Svr::UserTransactionS2SCmd< LoginPlayerEntity, Message::LoginServer::PlayerJoinedToGameServerCmd> super;

	private:
		GameInsUID m_GameUID;
		PartyUID m_PartyUID;
		MatchingQueueTicket m_MatchingTicket;

	public:

		LoginPlayerJoinedToGameServerTrans(IHeap& memMgr, MessageDataPtr &pIMsg );
		virtual ~LoginPlayerJoinedToGameServerTrans() {}

		Result OnConnectToGameServerRes( Svr::TransactionResult* &pRes );
		Result OnValidateGameServerSessionRes(Svr::TransactionResult* &pRes);

		// Start Transaction
		virtual Result StartTransaction() override;

		BR_SVR_MSGTRANS_CLOSE(NetSvrPolicyLoginServer, PlayerJoinedToGameServerRes,GetRouteContext().GetSwaped());
	};





	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ranking handling
	//

	class RankingUpdateScoreTrans : public Svr::MessageTransaction< LoginPlayerEntity, Message::Login::UpdateMyScoreCmd >
	{
	public:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Message::Login::UpdateMyScoreCmd > super;

	private:

		StaticArray<TotalRankingPlayerInformation, 20> m_RankingList;

	public:
		RankingUpdateScoreTrans(IHeap& memMgr, MessageDataPtr &pIMsg);
		virtual ~RankingUpdateScoreTrans() {}

		Result OnScoreUpdated(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyLogin, UpdateMyScoreRes, m_RankingList);
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Ranking handling
	//

	class LoginUserDataTestTrans : public Svr::MessageTransaction< LoginPlayerEntity, Message::Login::DataTestCmd >
	{
	public:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Message::Login::DataTestCmd > super;

	private:

		static std::atomic<uint32_t> stm_TestCount;
		StaticArray<uint8_t, 30 * 1024> m_Data;

	public:
		LoginUserDataTestTrans(IHeap& memMgr, MessageDataPtr &pIMsg);
		virtual ~LoginUserDataTestTrans() {}


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE_ARGS(NetSvrPolicyLogin, DataTestRes, m_Data);
	};

	class LoginUserDebugPrintALLRankingTrans : public Svr::MessageTransaction< LoginPlayerEntity, Message::Login::DebugPrintALLRankingCmd >
	{
	public:
		typedef Svr::MessageTransaction< LoginPlayerEntity, Message::Login::DebugPrintALLRankingCmd > super;

	private:


	public:
		LoginUserDebugPrintALLRankingTrans(IHeap& memMgr, MessageDataPtr &pIMsg);
		virtual ~LoginUserDebugPrintALLRankingTrans() {}

		Result OnPrintAllRankingRes(Svr::TransactionResult* &pRes);


		// Start Transaction
		virtual Result StartTransaction() override;

		BR_IMPLEMENT_USERMSGTRANS_CLOSE(NetSvrPolicyLogin, DebugPrintALLRankingRes);
	};



} // namespace SF 

