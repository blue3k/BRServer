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
#include "Common/Thread.h"
#include "Common/BrAssert.h"
#include "Common/Utility.h"
#include "Common/HRESNet.h"
#include "Net/NetConst.h"
#include "Net/NetSystem.h"
#include "Net/NetTrace.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetCtrl.h"
#include "Net/NetUtil.h"
#include "Common/TimeUtil.h"
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
#include "Protocol/Message/RankingMsgDebug.h"
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
		Message::Debug::RegisterDebugTraceRanking();
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
		case POLICY_LOGIN:
		case POLICY_SVR_LOGIN:
			Message::Debug::DebugOutLogin( strPrefix, pMsg );
			break;
		case POLICY_LOGINSERVER:
		case POLICY_SVR_LOGINSERVER:
			Message::Debug::DebugOutLoginServer( strPrefix, pMsg );
			break;
		case POLICY_GAME:
		case POLICY_SVR_GAME:
			Message::Debug::DebugOutGame( strPrefix, pMsg );
			break;
		case POLICY_SERVER:
		case POLICY_SVR_SERVER:
			Message::Debug::DebugOutServer( strPrefix, pMsg );
			break;
		case POLICY_GAMESERVER:
		case POLICY_SVR_GAMESERVER:
			Message::Debug::DebugOutGameServer( strPrefix, pMsg );
			break;
		case POLICY_GAMEPARTY:
		case POLICY_SVR_GAMEPARTY:
			Message::Debug::DebugOutGameParty( strPrefix, pMsg );
			break;
		case POLICY_GAMEMASTERSERVER:
		case POLICY_SVR_GAMEMASTERSERVER:
			Message::Debug::DebugOutGameMasterServer( strPrefix, pMsg );
			break;
		case POLICY_ENTITYSERVER:
		case POLICY_SVR_ENTITYSERVER:
			Message::Debug::DebugOutEntityServer( strPrefix, pMsg );
			break;
		case POLICY_CLUSTERSERVER:
		case POLICY_SVR_CLUSTERSERVER:
			Message::Debug::DebugOutClusterServer( strPrefix, pMsg );
			break;
		case POLICY_PARTYMATCHING:
		case POLICY_SVR_PARTYMATCHING:
			Message::Debug::DebugOutPartyMatching( strPrefix, pMsg );
			break;
		case POLICY_PARTYMATCHINGQUEUE:
		case POLICY_SVR_PARTYMATCHINGQUEUE:
			Message::Debug::DebugOutPartyMatchingQueue( strPrefix, pMsg );
			break;
		case POLICY_GAMEINSTANCE:
		case POLICY_SVR_GAMEINSTANCE:
			Message::Debug::DebugOutGameInstance( strPrefix, pMsg );
			break;
		case POLICY_GAMEINSTANCEMANAGER:
		case POLICY_SVR_GAMEINSTANCEMANAGER:
			Message::Debug::DebugOutGameInstanceManager( strPrefix, pMsg );
			break;
		case POLICY_GAMEPARTYMANAGER:
		case POLICY_SVR_GAMEPARTYMANAGER:
			Message::Debug::DebugOutGamePartyManager(strPrefix, pMsg);
			break;
		case POLICY_RANKING:
		case POLICY_SVR_RANKING:
			Message::Debug::DebugOutRanking(strPrefix, pMsg);
			break;
		case POLICY_MONITORING:
		case POLICY_SVR_MONITORING:
			Message::Debug::DebugOutMonitoring(strPrefix, pMsg);
			break;
		default:
			netTrace( TRC_NETCTRL, "Invalid MsgID for Debug {0}", pMsg->GetMessageHeader()->msgID );
			break;
		}
	}
	

} // namespace Net
} // namespace BR


