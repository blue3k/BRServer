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
#include "Memory/MemoryPool.h"

#include "ServiceEntity/Game/GameSystem.h"
#include "Common/MemoryBufferUtil.h"
#include "GameConst.h"
#include "UserSystemComponentIDs.h"


namespace SF {
namespace GameServer {

	class GamePlayerEntity;

	//////////////////////////////////////////////////////////////////////////
	//
	//	GamePlaySystem class
	//

	class UserNotifySystem : public Svr::GameSystem<GamePlayerEntity,UserNotifySystem>
	{
	public:

		enum {
			ComponentID				= UserSystemComponentID_Notification,

			MAX_NOTIFICATION		= 128,
			MAX_NOTIFICATION_TEXT	= 512,
		};

		struct Notification
		{
			uint NotificationID;
			NotificationType MessageID;
			int64_t MessageParam0;
			int64_t MessageParam1;
			char MessageText[MAX_NOTIFICATION_TEXT];
			uint8_t IsRead;
			int64_t TimeStamp;

			bool operator == ( const Notification& src )
			{
				return NotificationID == src.NotificationID && MessageID == src.MessageID;
			}
		};


	private:

		MemoryBuffer<Notification,sizeof(Notification)*MAX_NOTIFICATION>	m_Notifications;


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







