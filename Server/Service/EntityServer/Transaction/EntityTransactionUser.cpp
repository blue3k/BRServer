////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "EntityServer.h"
#include "ServerSystem/SvrTrace.h"
#include "EntityServerClass.h"
#include "Memory/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"


#include "ServerSystem/ServerEntity.h"
//#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"



#include "EntityTransactionServer.h"
#include "EntityTransactionUser.h"




//SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransRegisterUserEntity);
//SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransUpdateUserEntity);
//SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransUnregisterUserEntity);
//SF_MEMORYPOOL_IMPLEMENT(BR::EntityServer::TransFindUserEntity);
//

namespace BR {
namespace EntityServer {

	/////////////////////////////////////////////////////////////////////////////
	//
	//	Login command transaction
	//
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	TransRegisterUserEntity::TransRegisterUserEntity( Message::MessageData* &pIMsg )
//		:MessageTransaction( pIMsg )
//	{
//	}
//
//	TransRegisterUserEntity::~TransRegisterUserEntity()
//	{
//	}
//
//	// Start Transaction
//	Result TransRegisterUserEntity::StartTransaction()
//	{
//		Result hr = ResultCode::SUCCESS;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( super::StartTransaction() );
//
//		svrChkPtr( pInfoTbl = GetEntityInfoTable() );
//
//		svrChk( pInfoTbl->RegisterUserEntity( EntityUID( GetMyOwner()->GetEntityUID().SvrID, GetLocalEntID()),
//			GetEntName(), GetAccID(), m_AuthTicket, pEntInfo ) );
//
//
//	Proc_End:
//
//		CloseTransaction( hr );
//
//		return hr;
//	}
//
//	// Close transaction and notify to parent
//	// process abnormal termination of transaction
//	Result TransRegisterUserEntity::CloseTransaction( Result hrRes )
//	{
//		Result hr = ResultCode::SUCCESS;
//
//		if( IsClosed() )
//			return ResultCode::SUCCESS;
//
//		svrChkPtr( GetPolicy() );
//
//		svrChk( GetPolicy()->RegisterUserEntityRes( GetContext(), hrRes, GetMyOwner()->GetEntityUID(), m_AuthTicket ) );
//
//Proc_End:
//
//		SetClosed();
//
//		return hr;
//	}
//	
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	TransUnregisterUserEntity::TransUnregisterUserEntity( Message::MessageData* &pIMsg )
//		:MessageTransaction( pIMsg )
//	{
//	}
//
//	TransUnregisterUserEntity::~TransUnregisterUserEntity()
//	{
//	}
//
//	// Start Transaction
//	Result TransUnregisterUserEntity::StartTransaction()
//	{
//		Result hr = ResultCode::SUCCESS;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( super::StartTransaction() );
//
//		svrChkPtr( pInfoTbl = GetEntityInfoTable() );
//
//		svrChk( pInfoTbl->UnregisterUserEntity( GetAccID(), GetMyOwner()->GetServerID(), GetTicket() ) );
//
//	Proc_End:
//
//		CloseTransaction( hr );
//
//		return hr;
//	}
//
//
//
//	// Close transaction and notify to parent
//	// process abnormal termination of transaction
//	Result TransUnregisterUserEntity::CloseTransaction( Result hrRes )
//	{
//		Result hr = ResultCode::SUCCESS;
//
//		if( IsClosed() )
//			return ResultCode::SUCCESS;
//
//		svrChkPtr( GetPolicy() );
//
//		svrChk( GetPolicy()->UnregisterUserEntityRes( GetContext(), hrRes ) );
//
//Proc_End:
//
//		SetClosed();
//
//		return hr;
//	}
//
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	TransUpdateUserEntity::TransUpdateUserEntity( Message::MessageData* &pIMsg )
//		:MessageTransaction( pIMsg )
//	{
//	}
//
//	TransUpdateUserEntity::~TransUpdateUserEntity()
//	{
//	}
//
//	// Start Transaction
//	Result TransUpdateUserEntity::StartTransaction()
//	{
//		Result hr = ResultCode::SUCCESS;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( super::StartTransaction() );
//
//		svrChkPtr( pInfoTbl = GetEntityInfoTable() );
//
//		svrChk( pInfoTbl->UpdateUserEntity( GetAccID(), GetTicket(), GetMyOwner()->GetServerID() ) );
//
//	Proc_End:
//
//		CloseTransaction( hr );
//
//		return hr;
//	}
//
//
//	// Close transaction and notify to parent
//	// process abnormal termination of transaction
//	Result TransUpdateUserEntity::CloseTransaction( Result hrRes )
//	{
//		Result hr = ResultCode::SUCCESS;
//
//		if( IsClosed() )
//			return ResultCode::SUCCESS;
//
//		svrChkPtr( GetPolicy() );
//
//		svrChk( GetPolicy()->UpdateUserEntityRes( GetContext(), hrRes ) );
//
//	Proc_End:
//
//		SetClosed();
//
//		return hr;
//	}
//	
//
//	/////////////////////////////////////////////////////////////////////////////////////////////
//
//	TransFindUserEntity::TransFindUserEntity( Message::MessageData* &pIMsg )
//		:MessageTransaction( pIMsg )
//	{
//	}
//
//	TransFindUserEntity::~TransFindUserEntity()
//	{
//	}
//
//	// Start Transaction
//	Result TransFindUserEntity::StartTransaction()
//	{
//		Result hr = ResultCode::SUCCESS;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//		EntityInfoTable::AccountIDMap::iterator iterAcc;
//
//		svrChk( super::StartTransaction() );
//
//		svrChkPtr( pInfoTbl = GetEntityInfoTable() );
//
//		svrChk( pInfoTbl->FindUserEntity( GetAccID(), iterAcc ) );
//
//		m_EntUID = iterAcc->GetUID();
//		m_AuthTicket = iterAcc->GetAuthTicket();
//
//	Proc_End:
//
//		CloseTransaction( hr );
//
//		return hr;
//	}
//
//
//	// Close transaction and notify to parent
//	// process abnormal termination of transaction
//	Result TransFindUserEntity::CloseTransaction( Result hrRes )
//	{
//		Result hr = ResultCode::SUCCESS;
//
//		if( IsClosed() )
//			return ResultCode::SUCCESS;
//
//		svrChkPtr( GetPolicy() );
//
//		svrChk( GetPolicy()->FindUserEntityRes( GetContext(), hrRes, m_EntUID, m_AuthTicket ) );
//
//Proc_End:
//
//		SetClosed();
//
//		return hr;
//	}
//

};// namespace EntityServer 
};// namespace BR 

