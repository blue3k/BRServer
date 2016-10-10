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
	Result UserNotifySystem::InitializeComponent()
	{
		m_Notifications.Clear();
		return ResultCode::SUCCESS;
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
	Result UserNotifySystem::AddNotification( const Notification& info )
	{
		if( GetNotification(info.NotificationID) )
			return ResultCode::SUCCESS_FALSE;

		return m_Notifications.Append( 1, &info );
	}

	Result UserNotifySystem::AddNotification( UINT NotificationID, NotificationType MessageID, INT64 MessageParam0, INT64 MessageParam1, const char* MessageText, BYTE IsRead, INT64 timeStamp )
	{
		if( MessageText == nullptr)
			return ResultCode::INVALID_ARG;

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
	Result UserNotifySystem::RemoveNotification( UINT notificationID )
	{
		INT index = FindNotification(notificationID);
		if( index < 0 )
			return ResultCode::SUCCESS_FALSE;

		return m_Notifications.RemoveAt( index );
	}


	Result UserNotifySystem::ForeachNotification( std::function<Result(const UserNotifySystem::Notification&)> functor )
	{
		for( unsigned int index = 0; index < m_Notifications.GetSize(); index++ )
		{
			Result hr = functor( m_Notifications[index] );
			if( FAILED(hr) )
				return hr;
		}

		return ResultCode::SUCCESS;
	}

}; // namespace GameServer
}; // namespace BR







