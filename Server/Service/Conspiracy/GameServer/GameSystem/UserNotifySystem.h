////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game notify system
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "MemoryManager/SFMemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"

#include "GameConst.h"
#include "UserSystemComponentIDs.h"


namespace SF {



	struct UserNotifySystem_Notification
	{
		static constexpr int MAX_NOTIFICATION_TEXT = 512;

		uint NotificationID;
		NotificationType MessageID;
		int64_t MessageParam0;
		int64_t MessageParam1;
		char MessageText[MAX_NOTIFICATION_TEXT];
		uint8_t IsRead;
		int64_t TimeStamp;

		bool operator == (const UserNotifySystem_Notification& src)
		{
			return NotificationID == src.NotificationID && MessageID == src.MessageID;
		}
	};



namespace GameServer {

	class GamePlayerEntity;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class UserNotifySystem : public Svr::GameSystem<GamePlayerEntity,UserNotifySystem>
	{
	public:

		static constexpr int ComponentID = UserSystemComponentID_Notification;
		static constexpr int MAX_NOTIFICATION = 128;

		typedef UserNotifySystem_Notification Notification;

	private:

		StaticArray<Notification,MAX_NOTIFICATION>	m_Notifications;


	public:

		// Constructor 
		UserNotifySystem( GamePlayerEntity* pEntity );
		~UserNotifySystem();


		// Initialize server component
		virtual Result InitializeComponent();

		// Terminate server component
		virtual void TerminateComponent();



		////////////////////////////////////////////////////////////////////
		//
		//	Operations
		//

		// Clear notification list
		void ClearNotificationList();

		// Get Notification
		Notification* GetNotification( uint notificationID );

		// FInd
		INT FindNotification( uint notificationID );

		// Add a Notification
		Result AddNotification( const Notification& info );
		Result AddNotification( uint NotificationID, NotificationType MessageID, int64_t MessageParam0, int64_t MessageParam1, const char* MessageText, uint8_t IsRead, int64_t timeStamp );

		// Remove a Notification
		Result RemoveNotification( uint notificationID ); 

		//// Read Notification
		//Result ReadNotification( uint notificationID ); 

		// iterate all Notification list
		Result ForeachNotification( std::function<Result(const Notification&)> functor );
	};


}; // namespace GameServer
}; // namespace SF







