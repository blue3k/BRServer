////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game play system
//	
////////////////////////////////////////////////////////////////////////////////




#include "ServerSystemPCH.h"

#include "Util/SFTimeUtil.h"
#include "Util/SFRandom.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"

#include "SvrConst.h"
#include "SvrTrace.h"
#include "Server/BrServerUtil.h"

#include "Protocol/Policy/GameInstanceNetPolicy.h"
#include "Protocol/Policy/GameNetPolicy.h"

#include "ServiceEntity/Game/Subsystem/UserNotificationSystem.h"

#include "ServiceEntity/Game/GamePlayerEntity.h"





namespace SF {
	constexpr int UserNotifySystem_Notification::MAX_NOTIFICATION_TEXT;


namespace Svr {

	constexpr StringCrc32 UserNotificationSystem::ComponentID;
	constexpr int UserNotificationSystem::MAX_NOTIFICATION;


	
	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlay state class
	//

	// Constructor 
	UserNotificationSystem::UserNotificationSystem( GamePlayerEntity* pEntity )
		: GameSystem(pEntity)
		, m_Notifications(pEntity->GetHeap())
	{
	}

	UserNotificationSystem::~UserNotificationSystem()
	{
	}

	// Initialize system
	Result UserNotificationSystem::InitializeComponent()
	{
		m_Notifications.Clear();
		return ResultCode::SUCCESS;
	}

	// Terminate component
	void UserNotificationSystem::TerminateComponent()
	{
		m_Notifications.Clear();
	}


	////////////////////////////////////////////////////////////////////
	//
	//	Operations
	//

	
	// Clear Notification list
	void UserNotificationSystem::ClearNotificationList()
	{
		m_Notifications.Clear();
	}

	// Get Notification information
	UserNotificationSystem::Notification* UserNotificationSystem::GetNotification( uint notificationID )
	{
		for( unsigned int index = 0; index < m_Notifications.size(); index++ )
		{
			if( m_Notifications[index].NotificationID == notificationID )
				return &m_Notifications[index];
		}

		return nullptr;
	}

	// Find
	INT UserNotificationSystem::FindNotification( uint notificationID )
	{
		for( int index = 0; index < (INT)m_Notifications.size(); index++ )
		{
			if( m_Notifications[index].NotificationID == notificationID )
				return index;
		}

		return -1;
	}

	// Add a Notification
	Result UserNotificationSystem::AddNotification( const Notification& info )
	{
		if( GetNotification(info.NotificationID) )
			return ResultCode::SUCCESS_FALSE;

		return m_Notifications.insert( 1, info );
	}

	Result UserNotificationSystem::AddNotification( uint NotificationID, NotificationType MessageID, int64_t MessageParam0, int64_t MessageParam1, const char* MessageText, uint8_t IsRead, int64_t timeStamp )
	{
		if( MessageText == nullptr)
			return ResultCode::INVALID_ARG;

		Notification info;
		memset( &info, 0, sizeof(info) );

		info.NotificationID = NotificationID;
		info.MessageID = MessageID;
		info.MessageParam0 = MessageParam0;
		info.MessageParam1 = MessageParam1;
		StrUtil::StringCopy(info.MessageText, MessageText);
		info.IsRead = IsRead;
		info.TimeStamp = timeStamp;

		return AddNotification(info);
	}


	// Remove a Notification
	Result UserNotificationSystem::RemoveNotification( uint notificationID )
	{
		INT index = FindNotification(notificationID);
		if( index < 0 )
			return ResultCode::SUCCESS_FALSE;

		return m_Notifications.RemoveAt( index );
	}


	Result UserNotificationSystem::ForeachNotification( std::function<Result(const UserNotificationSystem::Notification&)> functor )
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







