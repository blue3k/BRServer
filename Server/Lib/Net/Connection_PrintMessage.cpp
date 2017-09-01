////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net Connection implementation
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Thread/Thread.h"
#include "SFAssert.h"
#include "Common/Utility.h"
#include "ResultCode/SFResultCodeNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Net/NetTrace.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetUtil.h"
#include "Util/TimeUtil.h"
#include "Common/SvrPolicyID.h"

#include "Protocol/ProtocolVer.h"
#include "Net/Connection.h"

#include "Protocol/Message/LoginMsgDebug.h"
#include "Protocol/Message/LoginServerMsgDebug.h"
#include "Protocol/Message/ServerMsgDebug.h"
#include "Protocol/Message/EntityServerMsgDebug.h"
#include "Protocol/Message/GameMasterServerMsgDebug.h"
#include "Protocol/Message/GameServerMsgDebug.h"
#include "Protocol/Message/GameMsgDebug.h"
#include "Protocol/Message/GamePartyMsgDebug.h"
#include "Protocol/Message/ClusterServerMsgDebug.h"
#include "Protocol/Message/PartyMatchingMsgDebug.h"
#include "Protocol/Message/PartyMatchingQueueMsgDebug.h"
#include "Protocol/Message/GameInstanceMsgDebug.h"
#include "Protocol/Message/GameInstanceManagerMsgDebug.h"
#include "Protocol/Message/GamePartyManagerMsgDebug.h"
#include "Protocol/Message/RankingServerMsgDebug.h"
#include "Protocol/Message/MonitoringMsgDebug.h"



namespace BR {
namespace Net {

	void RegisterConnectionDebugMessage()
	{
		Message::Debug::RegisterDebugTraceLogin( );
		Message::Debug::RegisterDebugTraceLoginServer( );
		Message::Debug::RegisterDebugTraceGame( );
		Message::Debug::RegisterDebugTraceServer( );
		Message::Debug::RegisterDebugTraceGameServer( );
		Message::Debug::RegisterDebugTraceGameParty( );
		Message::Debug::RegisterDebugTraceGamePartyManager();
		Message::Debug::RegisterDebugTraceGameMasterServer( );
		Message::Debug::RegisterDebugTraceEntityServer( );
		Message::Debug::RegisterDebugTraceClusterServer( );
		Message::Debug::RegisterDebugTracePartyMatching( );
		Message::Debug::RegisterDebugTracePartyMatchingQueue( );
		Message::Debug::RegisterDebugTraceGameInstance();
		Message::Debug::RegisterDebugTraceGameInstanceManager();
		Message::Debug::RegisterDebugTraceRankingServer();
		Message::Debug::RegisterDebugTraceMonitoring();
	}

	void Connection::PrintDebugMessage( const char* strPrefix, Message::MessageData *pMsg )
	{
		if( pMsg == nullptr )
			return;

		// check network message trace flag
		if( !netTraceModule.CheckTrace( TRC_MSG ) )
			return;

		switch( pMsg->GetMessageHeader()->msgID.IDs.Policy )
		{
		case PROTOCOLID_LOGIN:
		case PROTOCOLID_SVR_LOGIN:
			Message::Debug::DebugOutLogin( strPrefix, pMsg );
			break;
		case PROTOCOLID_LOGINSERVER:
		case PROTOCOLID_SVR_LOGINSERVER:
			Message::Debug::DebugOutLoginServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAME:
		case PROTOCOLID_SVR_GAME:
			Message::Debug::DebugOutGame( strPrefix, pMsg );
			break;
		case PROTOCOLID_SERVER:
		case PROTOCOLID_SVR_SERVER:
			Message::Debug::DebugOutServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMESERVER:
		case PROTOCOLID_SVR_GAMESERVER:
			Message::Debug::DebugOutGameServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMEPARTY:
		case PROTOCOLID_SVR_GAMEPARTY:
			Message::Debug::DebugOutGameParty( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMEMASTERSERVER:
		case PROTOCOLID_SVR_GAMEMASTERSERVER:
			Message::Debug::DebugOutGameMasterServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_ENTITYSERVER:
		case PROTOCOLID_SVR_ENTITYSERVER:
			Message::Debug::DebugOutEntityServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_CLUSTERSERVER:
		case PROTOCOLID_SVR_CLUSTERSERVER:
			Message::Debug::DebugOutClusterServer( strPrefix, pMsg );
			break;
		case PROTOCOLID_PARTYMATCHING:
		case PROTOCOLID_SVR_PARTYMATCHING:
			Message::Debug::DebugOutPartyMatching( strPrefix, pMsg );
			break;
		case PROTOCOLID_PARTYMATCHINGQUEUE:
		case PROTOCOLID_SVR_PARTYMATCHINGQUEUE:
			Message::Debug::DebugOutPartyMatchingQueue( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMEINSTANCE:
		case PROTOCOLID_SVR_GAMEINSTANCE:
			Message::Debug::DebugOutGameInstance( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMEINSTANCEMANAGER:
		case PROTOCOLID_SVR_GAMEINSTANCEMANAGER:
			Message::Debug::DebugOutGameInstanceManager( strPrefix, pMsg );
			break;
		case PROTOCOLID_GAMEPARTYMANAGER:
		case PROTOCOLID_SVR_GAMEPARTYMANAGER:
			Message::Debug::DebugOutGamePartyManager(strPrefix, pMsg);
			break;
		case PROTOCOLID_RANKINGSERVER:
		case PROTOCOLID_SVR_RANKINGSERVER:
			Message::Debug::DebugOutRankingServer(strPrefix, pMsg);
			break;
		case PROTOCOLID_MONITORING:
		case PROTOCOLID_SVR_MONITORING:
			Message::Debug::DebugOutMonitoring(strPrefix, pMsg);
			break;
		default:
			netTrace( TRC_NETCTRL, "Invalid MsgID for Debug {0}", pMsg->GetMessageHeader()->msgID );
			break;
		}
	}
	

} // namespace Net
} // namespace BR


