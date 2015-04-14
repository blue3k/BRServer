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
#include "Common/MemoryPool.h"

#include "Protocol/Message/EntityServerMsgClass.h"
#include "Protocol/Policy/EntityServerIPolicy.h"


#include "ServerSystem/ServerEntity.h"
#include "ServerEntity/GameServerEntity.h"

#include "Protocol/Policy/EntityServerIPolicy.h"
#include "Protocol/Policy/GameServerIPolicy.h"



#include "EntityTransactionServer.h"
#include "EntityTransactionUser.h"




//BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransRegisterUserEntity);
//BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransUpdateUserEntity);
//BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransUnregisterUserEntity);
//BR_MEMORYPOOL_IMPLEMENT(EntityServer::TransFindUserEntity);
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
//	HRESULT TransRegisterUserEntity::StartTransaction()
//	{
//		HRESULT hr = S_OK;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( __super::StartTransaction() );
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
//	HRESULT TransRegisterUserEntity::CloseTransaction( HRESULT hrRes )
//	{
//		HRESULT hr = S_OK;
//
//		if( IsClosed() )
//			return S_OK;
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
//	HRESULT TransUnregisterUserEntity::StartTransaction()
//	{
//		HRESULT hr = S_OK;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( __super::StartTransaction() );
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
//	HRESULT TransUnregisterUserEntity::CloseTransaction( HRESULT hrRes )
//	{
//		HRESULT hr = S_OK;
//
//		if( IsClosed() )
//			return S_OK;
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
//	HRESULT TransUpdateUserEntity::StartTransaction()
//	{
//		HRESULT hr = S_OK;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//
//		svrChk( __super::StartTransaction() );
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
//	HRESULT TransUpdateUserEntity::CloseTransaction( HRESULT hrRes )
//	{
//		HRESULT hr = S_OK;
//
//		if( IsClosed() )
//			return S_OK;
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
//	HRESULT TransFindUserEntity::StartTransaction()
//	{
//		HRESULT hr = S_OK;
//		EntityInfoTable *pInfoTbl = nullptr;
//		EntityInformation *pEntInfo = nullptr;
//		EntityInfoTable::AccountIDMap::iterator iterAcc;
//
//		svrChk( __super::StartTransaction() );
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
//	HRESULT TransFindUserEntity::CloseTransaction( HRESULT hrRes )
//	{
//		HRESULT hr = S_OK;
//
//		if( IsClosed() )
//			return S_OK;
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

