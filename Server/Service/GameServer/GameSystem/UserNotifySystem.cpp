////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "stdafx.h"

#include "Common/TimeUtil.h"
#include "Common/BrRandom.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/ResultCode/BRResultCodeCommon.h"
#include "Common/ResultCode/BRResultCodeGame.h"

#include "ServerSystem/SvrConst.h"
#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServerUtil.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/GameInstanceIPolicy.h"
#include "GameServerClass.h"
#include "Protocol/Policy/GameIPolicy.h"

#include "GameSystem/UserNotifySystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "GameInstance/GamePlayerEntity.h"

#include "Table/conspiracy/LevelTbl.h"


BR_MEMORYPOOL_IMPLEMENT(GameServer::UserNotifySystem);


namespace BR {
namespace GameServer {


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state calss
	//

	// Constructor 
	UserNotifySystem::UserNotifySystem( GamePlayerEntity* pEntity )
		:GameSystem(pEntity)
	{
	}

	UserNotifySystem::~UserNotifySystem()
	{
	}

	// Initialzie system
	HRESULT UserNotifySystem::InitializeComponent()
	{
		m_Notifications.Clear();
		return S_OK;
	}

	// Terminate component
	void UserNotifySystem::TerminateComponent()
	{
		m_Notifications.Clear();
	}


	////////////////////////////////////////////////////////////////////
	//
	//	Operations
	//

	
	// Clear Notification list
	void UserNotifySystem::ClearNotificationList()
	{
		m_Notifications.Clear();
	}

	// Get Notification information
	UserNotifySystem::Notification* UserNotifySystem::GetNotification( UINT notificationID )
	{
		for( unsigned int index = 0; index < m_Notifications.GetSize(); index++ )
		{
			if( m_Notifications[index].NotificationID == notificationID )
				return &m_Notifications[index];
		}

		return nullptr;
	}

	// Find
	INT UserNotifySystem::FindNotification( UINT notificationID )
	{
		for( int index = 0; index < (INT)m_Notifications.GetSize(); index++ )
		{
			if( m_Notifications[index].NotificationID == notificationID )
				return index;
		}

		return -1;
	}

	// Add a Notification
	HRESULT UserNotifySystem::AddNotification( const Notification& info )
	{
		if( GetNotification(info.NotificationID) )
			return S_SYSTEM_FALSE;

		return m_Notifications.Append( 1, &info );
	}

	HRESULT UserNotifySystem::AddNotification( UINT NotificationID, NotificationType MessageID, INT64 MessageParam0, INT64 MessageParam1, const char* MessageText, BYTE IsRead, INT64 timeStamp )
	{
		if( MessageText == nullptr)
			return E_SYSTEM_INVALIDARG;

		Notification info;
		memset( &info, 0, sizeof(info) );

		info.NotificationID = NotificationID;
		info.MessageID = MessageID;
		info.MessageParam0 = MessageParam0;
		info.MessageParam1 = MessageParam1;
		StrUtil::StringCpy(info.MessageText, MessageText);
		info.IsRead = IsRead;
		info.TimeStamp = timeStamp;

		return AddNotification(info);
	}


	// Remove a Notification
	HRESULT UserNotifySystem::RemoveNotification( UINT notificationID )
	{
		INT index = FindNotification(notificationID);
		if( index < 0 )
			return S_SYSTEM_FALSE;

		return m_Notifications.RemoveAt( index );
	}


	HRESULT UserNotifySystem::ForeachNotification( std::function<HRESULT(const UserNotifySystem::Notification&)> functor )
	{
		for( unsigned int index = 0; index < m_Notifications.GetSize(); index++ )
		{
			HRESULT hr = functor( m_Notifications[index] );
			if( FAILED(hr) )
				return hr;
		}

		return S_OK;
	}

}; // namespace GameServer
}; // namespace BR







