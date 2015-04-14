////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server Transactions
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "ServerSystem/Transaction.h"
#include "Common/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"



namespace BR {
namespace EntityServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//




	//// Register User Entity
	//class TransRegisterUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::EntityServer::RegisterUserEntityCmd, TransRegisterUserEntity>
	//{
	//private:
	//	// Authentication 
	//	AuthTicket		m_AuthTicket;

	//public:
	//	TransRegisterUserEntity( Message::MessageData* &pIMsg );
	//	virtual ~TransRegisterUserEntity();

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual HRESULT CloseTransaction( HRESULT hrRes );
	//};



	//// Unregister User Entity
	//class TransUnregisterUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::EntityServer::UnregisterUserEntityCmd, TransUnregisterUserEntity>
	//{
	//private:

	//public:
	//	TransUnregisterUserEntity( Message::MessageData* &pIMsg );
	//	virtual ~TransUnregisterUserEntity();

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual HRESULT CloseTransaction( HRESULT hrRes );
	//};

	//

	//// Unregister User Entity
	//class TransUpdateUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::EntityServer::UpdateUserEntityCmd, TransUpdateUserEntity>
	//{
	//private:

	//public:
	//	TransUpdateUserEntity( Message::MessageData* &pIMsg );
	//	virtual ~TransUpdateUserEntity();

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual HRESULT CloseTransaction( HRESULT hrRes );
	//};



	//// Find User Entity
	//class TransFindUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::ISvrPolicyEntityServer, Message::EntityServer::FindUserEntityCmd, TransFindUserEntity>
	//{
	//private:
	//	// UID for find User
	//	EntityUID		m_EntUID;

	//	// Authentication 
	//	AuthTicket		m_AuthTicket;

	//public:
	//	TransFindUserEntity( Message::MessageData* &pIMsg );
	//	virtual ~TransFindUserEntity();

	//	// Start Transaction
	//	virtual HRESULT StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual HRESULT CloseTransaction( HRESULT hrRes );
	//};


};// namespace EntityServer 
};// namespace BR 

