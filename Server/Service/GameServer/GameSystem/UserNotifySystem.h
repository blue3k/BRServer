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
#include "Common/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "ServerSystem/GameSystem.h"
#include "Common/MemoryBufferUtil.h"
#include "Common/GameConst.h"
#include "UserSystemComponentIDs.h"


namespace BR {
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
			UINT NotificationID;
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
		Notification* GetNotification( UINT notificationID );

		// FInd
		INT FindNotification( UINT notificationID );

		// Add a Notification
		Result AddNotification( const Notification& info );
		Result AddNotification( UINT NotificationID, NotificationType MessageID, int64_t MessageParam0, int64_t MessageParam1, const char* MessageText, uint8_t IsRead, int64_t timeStamp );

		// Remove a Notification
		Result RemoveNotification( UINT notificationID ); 

		//// Read Notification
		//Result ReadNotification( UINT notificationID ); 

		// iterate all Notification list
		Result ForeachNotification( std::function<Result(const Notification&)> functor );
	};


}; // namespace GameServer
}; // namespace BR







