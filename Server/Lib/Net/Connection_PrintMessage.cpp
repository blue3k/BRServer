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

#include "StdAfx.h"
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
		BR::Message::Debug::RegisterDebugTraceLogin( );
		BR::Message::Debug::RegisterDebugTraceLoginServer( );
		BR::Message::Debug::RegisterDebugTraceGame( );
		BR::Message::Debug::RegisterDebugTraceServer( );
		BR::Message::Debug::RegisterDebugTraceGameServer( );
		BR::Message::Debug::RegisterDebugTraceGameParty( );
		BR::Message::Debug::RegisterDebugTraceGamePartyManager();
		BR::Message::Debug::RegisterDebugTraceGameMasterServer( );
		BR::Message::Debug::RegisterDebugTraceEntityServer( );
		BR::Message::Debug::RegisterDebugTraceClusterServer( );
		BR::Message::Debug::RegisterDebugTracePartyMatching( );
		BR::Message::Debug::RegisterDebugTracePartyMatchingQueue( );
		BR::Message::Debug::RegisterDebugTraceGameInstance();
		BR::Message::Debug::RegisterDebugTraceGameInstanceManager();
		BR::Message::Debug::RegisterDebugTraceRanking();
		BR::Message::Debug::RegisterDebugTraceMonitoring();
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
		case BR::POLICY_LOGIN:
		case BR::POLICY_SVR_LOGIN:
			BR::Message::Debug::DebugOutLogin( strPrefix, pMsg );
			break;
		case BR::POLICY_LOGINSERVER:
		case BR::POLICY_SVR_LOGINSERVER:
			BR::Message::Debug::DebugOutLoginServer( strPrefix, pMsg );
			break;
		case BR::POLICY_GAME:
		case BR::POLICY_SVR_GAME:
			BR::Message::Debug::DebugOutGame( strPrefix, pMsg );
			break;
		case BR::POLICY_SERVER:
		case BR::POLICY_SVR_SERVER:
			BR::Message::Debug::DebugOutServer( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMESERVER:
		case BR::POLICY_SVR_GAMESERVER:
			BR::Message::Debug::DebugOutGameServer( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMEPARTY:
		case BR::POLICY_SVR_GAMEPARTY:
			BR::Message::Debug::DebugOutGameParty( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMEMASTERSERVER:
		case BR::POLICY_SVR_GAMEMASTERSERVER:
			BR::Message::Debug::DebugOutGameMasterServer( strPrefix, pMsg );
			break;
		case BR::POLICY_ENTITYSERVER:
		case BR::POLICY_SVR_ENTITYSERVER:
			BR::Message::Debug::DebugOutEntityServer( strPrefix, pMsg );
			break;
		case BR::POLICY_CLUSTERSERVER:
		case BR::POLICY_SVR_CLUSTERSERVER:
			BR::Message::Debug::DebugOutClusterServer( strPrefix, pMsg );
			break;
		case BR::POLICY_PARTYMATCHING:
		case BR::POLICY_SVR_PARTYMATCHING:
			BR::Message::Debug::DebugOutPartyMatching( strPrefix, pMsg );
			break;
		case BR::POLICY_PARTYMATCHINGQUEUE:
		case BR::POLICY_SVR_PARTYMATCHINGQUEUE:
			BR::Message::Debug::DebugOutPartyMatchingQueue( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMEINSTANCE:
		case BR::POLICY_SVR_GAMEINSTANCE:
			BR::Message::Debug::DebugOutGameInstance( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMEINSTANCEMANAGER:
		case BR::POLICY_SVR_GAMEINSTANCEMANAGER:
			BR::Message::Debug::DebugOutGameInstanceManager( strPrefix, pMsg );
			break;
		case BR::POLICY_GAMEPARTYMANAGER:
		case BR::POLICY_SVR_GAMEPARTYMANAGER:
			BR::Message::Debug::DebugOutGamePartyManager(strPrefix, pMsg);
			break;
		case BR::POLICY_RANKING:
		case BR::POLICY_SVR_RANKING:
			BR::Message::Debug::DebugOutRanking(strPrefix, pMsg);
			break;
		case BR::POLICY_MONITORING:
		case BR::POLICY_SVR_MONITORING:
			BR::Message::Debug::DebugOutMonitoring(strPrefix, pMsg);
			break;
		default:
			netTrace( TRC_NETCTRL, "Invalid MsgID for Debug %0%", pMsg->GetMessageHeader()->msgID );
			break;
		}
	}
	

} // namespace Net
} // namespace BR


