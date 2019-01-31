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

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerNetPolicy.h"



namespace SF {
namespace EntityServer {


	/////////////////////////////////////////////////////////////////////////////
	//
	//	Entity command transaction
	//




	//// Register User Entity
	//class TransRegisterUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::EntityServer::RegisterUserEntityCmd, TransRegisterUserEntity>
	//{
	//private:
	//	// Authentication 
	//	AuthTicket		m_AuthTicket;

	//public:
	//	TransRegisterUserEntity( MessageDataPtr &pIMsg );
	//	virtual ~TransRegisterUserEntity();

	//	// Start Transaction
	//	virtual Result StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual Result CloseTransaction( Result hrRes );
	//};



	//// Unregister User Entity
	//class TransUnregisterUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::EntityServer::UnregisterUserEntityCmd, TransUnregisterUserEntity>
	//{
	//private:

	//public:
	//	TransUnregisterUserEntity( MessageDataPtr &pIMsg );
	//	virtual ~TransUnregisterUserEntity();

	//	// Start Transaction
	//	virtual Result StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual Result CloseTransaction( Result hrRes );
	//};

	//

	//// Unregister User Entity
	//class TransUpdateUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::EntityServer::UpdateUserEntityCmd, TransUpdateUserEntity>
	//{
	//private:

	//public:
	//	TransUpdateUserEntity( MessageDataPtr &pIMsg );
	//	virtual ~TransUpdateUserEntity();

	//	// Start Transaction
	//	virtual Result StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual Result CloseTransaction( Result hrRes );
	//};



	//// Find User Entity
	//class TransFindUserEntity : public Svr::MessageTransaction< Svr::ServerEntity, Policy::NetSvrPolicyEntityServer, Message::EntityServer::FindUserEntityCmd, TransFindUserEntity>
	//{
	//private:
	//	// UID for find User
	//	EntityUID		m_EntUID;

	//	// Authentication 
	//	AuthTicket		m_AuthTicket;

	//public:
	//	TransFindUserEntity( MessageDataPtr &pIMsg );
	//	virtual ~TransFindUserEntity();

	//	// Start Transaction
	//	virtual Result StartTransaction();

	//	// Close transaction and notify to parent
	//	// process abnormal termination of transaction
	//	virtual Result CloseTransaction( Result hrRes );
	//};


};// namespace EntityServer 
};// namespace SF 

