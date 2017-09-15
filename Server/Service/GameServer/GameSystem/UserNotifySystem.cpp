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

#include "Util/TimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"
#include "GameSvrConst.h"

#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "GameServerClass.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "GameSystem/UserNotifySystem.h"
#include "GameSystem/UserGamePlayerInfoSystem.h"

#include "GameInstance/GamePlayerEntity.h"



SF_MEMORYPOOL_IMPLEMENT(BR::GameServer::UserNotifySystem);


namespace SF {
	constexpr int UserNotifySystem_Notification::MAX_NOTIFICATION_TEXT;


namespace GameServer {

	constexpr int UserNotifySystem::ComponentID;
	constexpr int UserNotifySystem::MAX_NOTIFICATION;


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state class
	//

	// Constructor 
	UserNotifySystem::UserNotifySystem( GamePlayerEntity* pEntity )
		: GameSystem(pEntity)
		, m_Notifications(pEntity->GetHeap())
	{
	}

	UserNotifySystem::~UserNotifySystem()
	{
	}

	// Initialize system
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
	UserNotifySystem::Notification* UserNotifySystem::GetNotification( uint notificationID )
	{
		for( unsigned int index = 0; index < m_Notifications.size(); index++ )
		{
			if( m_Notifications[index].NotificationID == notificationID )
				return &m_Notifications[index];
		}

		return nullptr;
	}

	// Find
	INT UserNotifySystem::FindNotification( uint notificationID )
	{
		for( int index = 0; index < (INT)m_Notifications.size(); index++ )
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

		return m_Notifications.insert( 1, info );
	}

	Result UserNotifySystem::AddNotification( uint NotificationID, NotificationType MessageID, int64_t MessageParam0, int64_t MessageParam1, const char* MessageText, uint8_t IsRead, int64_t timeStamp )
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
	Result UserNotifySystem::RemoveNotification( uint notificationID )
	{
		INT index = FindNotification(notificationID);
		if( index < 0 )
			return ResultCode::SUCCESS_FALSE;

		return m_Notifications.RemoveAt( index );
	}


	Result UserNotifySystem::ForeachNotification( std::function<Result(const UserNotifySystem::Notification&)> functor )
	{
		for( unsigned int index = 0; index < m_Notifications.size(); index++ )
		{
			Result hr = functor( m_Notifications[index] );
			if( !(hr) )
				return hr;
		}

		return ResultCode::SUCCESS;
	}

}; // namespace GameServer
}; // namespace SF







