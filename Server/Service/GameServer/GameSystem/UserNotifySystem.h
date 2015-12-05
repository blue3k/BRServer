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

#include "Common/Typedefs.h"
#include "Common/BrGameTypes.h"
#include "Common/MemoryPool.h"
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
			INT64 MessageParam0;
			INT64 MessageParam1;
			char MessageText[MAX_NOTIFICATION_TEXT];
			BYTE IsRead;
			INT64 TimeStamp;

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
		virtual HRESULT InitializeComponent();

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
		HRESULT AddNotification( const Notification& info );
		HRESULT AddNotification( UINT NotificationID, NotificationType MessageID, INT64 MessageParam0, INT64 MessageParam1, const char* MessageText, BYTE IsRead, INT64 timeStamp );

		// Remove a Notification
		HRESULT RemoveNotification( UINT notificationID ); 

		//// Read Notification
		//HRESULT ReadNotification( UINT notificationID ); 

		// iterate all Notification list
		HRESULT ForeachNotification( std::function<HRESULT(const Notification&)> functor );
	};


}; // namespace GameServer
}; // namespace BR







