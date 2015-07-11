////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameInstance Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/PolicyID.h"
#include "Common/ToStringSvr.h"
#include "Common/ToStringGame.h"
#include "Common/ArrayUtil.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/ProtocolTrace.h"
#include "Common/SvrPolicyID.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameInstance
		{
 			// C2S: Game instance deletion
			const MessageID DeleteGameC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 0);
			HRESULT DeleteGameC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT DeleteGameC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT DeleteGameC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::DeleteGameC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT DeleteGameC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext )

			HRESULT DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )


			VOID DeleteGameC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:DeleteGameC2SEvt:%1%:%2% , RouteContext:%3%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext); 
			}; // VOID DeleteGameC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join Game
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 1);
			HRESULT JoinGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestedRole, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerInformation)
					+ sizeof(AuthTicket)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::JoinGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InRequestedRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const PlayerRole &InRequestedRole )

			HRESULT JoinGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameCmd::OverrideRouteContextDestination( EntityUID to )


			VOID JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameCmd:%1%:%2% , Context:%3%, RouteContext:%4%, Player:%5%, Ticket:%6%, RequestedRole:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_Player, m_Ticket, m_RequestedRole); 
			}; // VOID JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 1);
			HRESULT JoinGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameInsSvr, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsNewJoin, pCur, iMsgSize, sizeof(BYTE) ) );
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				UINT16 numberofGameLogData = 0; BYTE* pGameLogData = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(BYTE)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);


			Proc_End:

				return hr;

			}; // HRESULT JoinGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(NetAddress)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(BYTE)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(BYTE)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16)
					+ sizeof(BYTE)*InGameLogData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::JoinGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InGameInsSvr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InIsNewJoin, sizeof(BYTE));
				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 
				UINT16 numberOfInGameLogData = (UINT16)InGameLogData.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(BYTE)*InGameLogData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const NetAddress &InGameInsSvr, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const BYTE &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const BYTE &InIsNewJoin, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )

			HRESULT JoinGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT JoinGameRes::OverrideRouteContextDestination( EntityUID to )


			VOID JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:JoinGameRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%, GameInsSvr:%6%, TimeStamp:%7%, GameState:%8%, Day:%9%, MaxPlayer:%10%, PlayerIndex:%11%, PlayerCharacter:%12%, Role:%13%, Dead:%14%, IsNewJoin:%15%, ChatHistoryData:%16%, GameLogData:%17%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext, m_GameInsSvr, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_IsNewJoin, ArgArray<BYTE>(m_ChatHistoryData), ArgArray<BYTE>(m_GameLogData)); 
			}; // VOID JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 2);
			HRESULT PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerRole, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerDead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerInformation)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerCharacter, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )

			HRESULT PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerJoinedS2CEvt:%1%:%2% , RouteContext:%3%, JoinedPlayer:%4%, JoinedPlayerRole:%5%, JoinedPlayerDead:%6%, JoinedPlayerIndex:%7%, JoinedPlayerCharacter:%8%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_JoinedPlayer, m_JoinedPlayerRole, m_JoinedPlayerDead, m_JoinedPlayerIndex, m_JoinedPlayerCharacter); 
			}; // VOID PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Change configue preset
			const MessageID SetConfigPresetC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 3);
			HRESULT SetConfigPresetC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PresetID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT SetConfigPresetC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT SetConfigPresetC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InPresetID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::SetConfigPresetC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPresetID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT SetConfigPresetC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InPresetID )

			HRESULT SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )


			VOID SetConfigPresetC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:SetConfigPresetC2SEvt:%1%:%2% , RouteContext:%3%, PresetID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_PresetID); 
			}; // VOID SetConfigPresetC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave Game
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 4);
			HRESULT LeaveGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::LeaveGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )

			HRESULT LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )


			VOID LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeaveGameCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID); 
			}; // VOID LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 4);
			HRESULT LeaveGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::LeaveGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT LeaveGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT LeaveGameRes::OverrideRouteContextDestination( EntityUID to )


			VOID LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:LeaveGameRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player left
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 5);
			HRESULT PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )

			HRESULT PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerLeftS2CEvt:%1%:%2% , RouteContext:%3%, LeftPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_LeftPlayerID); 
			}; // VOID PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 6);
			HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )

			HRESULT KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			VOID KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:KickPlayerCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%, PlayerToKick:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID, m_PlayerToKick); 
			}; // VOID KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 6);
			HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			VOID KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:KickPlayerRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 7);
			HRESULT PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )

			HRESULT PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerKickedS2CEvt:%1%:%2% , RouteContext:%3%, KickedPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_KickedPlayerID); 
			}; // VOID PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Assign role
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 8);
			HRESULT AssignRoleCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::AssignRoleCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )

			HRESULT AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )


			VOID AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AssignRoleCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID); 
			}; // VOID AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 8);
			HRESULT AssignRoleRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::AssignRoleRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT AssignRoleRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT AssignRoleRes::OverrideRouteContextDestination( EntityUID to )


			VOID AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AssignRoleRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Assign role
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 9);
			HRESULT RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // HRESULT RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerRole &InRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::RoleAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerRole &InRole )

			HRESULT RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:RoleAssignedS2CEvt:%1%:%2% , RouteContext:%3%, Role:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Role); 
			}; // VOID RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// C2S: Chatting message
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 10);
			HRESULT ChatMessageC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				UINT16 uiSizeOfChatMessage = 0;
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageC2SEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT ChatMessageC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) +  + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::ChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageC2SEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerRole &InRole, const char* InChatMessage )

			HRESULT ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			VOID ChatMessageC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:ChatMessageC2SEvt:%1%:%2% , RouteContext:%3%, PlayerID:%4%, Role:%5%, ChatMessage:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_PlayerID, m_Role, m_ChatMessage); 
			}; // VOID ChatMessageC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 11);
			HRESULT AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::AdvanceGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )

			HRESULT AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )


			VOID AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AdvanceGameCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID); 
			}; // VOID AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 11);
			HRESULT AdvanceGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameRes::ParseIMsg( MessageData* pIMsg )

			HRESULT AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::AdvanceGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )


			VOID AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:AdvanceGameRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 12);
			HRESULT GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GameAdvancedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )

			HRESULT GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameAdvancedS2CEvt:%1%:%2% , RouteContext:%3%, TimeStamp:%4%, GameState:%5%, Day:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TimeStamp, m_GameState, m_Day); 
			}; // VOID GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 13);
			HRESULT GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Winner, pCur, iMsgSize, sizeof(GameWinner) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedExp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedGameMoney, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayedRole, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsWon, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(GameWinner)
					+ sizeof(UINT32)
					+ sizeof(UINT32)
					+ sizeof(PlayerRole)
					+ sizeof(BYTE));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GameEndedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InWinner, sizeof(GameWinner));
				Protocol::PackParamCopy( pMsgData, &InGainedExp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGainedGameMoney, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InPlayedRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InIsWon, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney, const PlayerRole &InPlayedRole, const BYTE &InIsWon )

			HRESULT GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameEndedS2CEvt:%1%:%2% , RouteContext:%3%, Winner:%4%, GainedExp:%5%, GainedGameMoney:%6%, PlayedRole:%7%, IsWon:%8%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Winner, m_GainedExp, m_GainedGameMoney, m_PlayedRole, m_IsWon); 
			}; // VOID GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: *Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 14);
			HRESULT VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VoteGameAdvanceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )

			HRESULT VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )


			VOID VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteGameAdvanceCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID); 
			}; // VOID VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 14);
			HRESULT VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VoteGameAdvanceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )


			VOID VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteGameAdvanceRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: *GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 15);
			HRESULT GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter )

			HRESULT GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameAdvanceVotedS2CEvt:%1%:%2% , RouteContext:%3%, Voter:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Voter); 
			}; // VOID GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Assign role
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 16);
			HRESULT VoteCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteTarget, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ActionSerial, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VoteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVoteTarget, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InActionSerial, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )

			HRESULT VoteCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VoteCmd::OverrideRouteContextDestination( EntityUID to )


			VOID VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%, VoteTarget:%6%, ActionSerial:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID, m_VoteTarget, m_ActionSerial); 
			}; // VOID VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 16);
			HRESULT VoteRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT VoteRes::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VoteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT VoteRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VoteRes::OverrideRouteContextDestination( EntityUID to )


			VOID VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 17);
			HRESULT VotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedTarget, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT VotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVotedTarget, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )

			HRESULT VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VotedS2CEvt:%1%:%2% , RouteContext:%3%, Voter:%4%, VotedTarget:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_Voter, m_VotedTarget); 
			}; // VOID VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 18);
			HRESULT VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				UINT16 numberofVoted = 0; PlayerID* pVoted = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofVoted, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pVoted, pCur, iMsgSize, sizeof(PlayerID)*numberofVoted ) );
				m_Voted.SetLinkedBuffer(numberofVoted, numberofVoted, pVoted);


			Proc_End:

				return hr;

			}; // HRESULT VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)*InVoted.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::VoteEndS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				UINT16 numberOfInVoted = (UINT16)InVoted.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInVoted, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InVoted.data(), (INT)(sizeof(PlayerID)*InVoted.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )

			HRESULT VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:VoteEndS2CEvt:%1%:%2% , RouteContext:%3%, Voted:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, ArgArray<PlayerID>(m_Voted)); 
			}; // VOID VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 19);
			HRESULT PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerRevealedReason) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole)
					+ sizeof(PlayerRevealedReason));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRevealedPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerRevealedReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )

			HRESULT PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerRevealedS2CEvt:%1%:%2% , RouteContext:%3%, RevealedPlayerID:%4%, Role:%5%, Reason:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RevealedPlayerID, m_Role, m_Reason); 
			}; // VOID PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 20);
			HRESULT PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KilledPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerKilledReason) ) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerKilledReason));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::PlayerKilledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InKilledPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerKilledReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )

			HRESULT PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:PlayerKilledS2CEvt:%1%:%2% , RouteContext:%3%, KilledPlayer:%4%, Reason:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_KilledPlayer, m_Reason); 
			}; // VOID PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 21);
			HRESULT GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayAgainCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InLeadPlayer, const PartyUID &InPartyUID )

			HRESULT GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainCmd:%1%:%2% , Context:%3%, RouteContext:%4%, LeadPlayer:%5%, PartyUID:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_LeadPlayer, m_PartyUID); 
			}; // VOID GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 21);
			HRESULT GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ReplayMemberCount, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(UINT32));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayAgainRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InReplayMemberCount, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const UINT32 &InReplayMemberCount )

			HRESULT GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%, ReplayMemberCount:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext, m_ReplayMemberCount); 
			}; // VOID GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 22);
			HRESULT GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )

			HRESULT GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayAgainS2CEvt:%1%:%2% , RouteContext:%3%, TargetPlayer:%4%, PartyUID:%5%, LeadPlayer:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_TargetPlayer, m_PartyUID, m_LeadPlayer); 
			}; // VOID GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. revive himself
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 23);
			HRESULT GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				UINT16 numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GameRevealPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				UINT16 numberOfInTargetPlayerID = (UINT16)InTargetPlayerID.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )

			HRESULT GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			VOID GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameRevealPlayerCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%, TargetPlayerID:%6%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID, ArgArray<PlayerID>(m_TargetPlayerID)); 
			}; // VOID GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 23);
			HRESULT GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				UINT16 numberofRevealedPlayerID = 0; PlayerID* pRevealedPlayerID = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofRevealedPlayerID ) );
				m_RevealedPlayerID.SetLinkedBuffer(numberofRevealedPlayerID, numberofRevealedPlayerID, pRevealedPlayerID);
				UINT16 numberofRevealedRole = 0; PlayerRole* pRevealedRole = nullptr;
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedRole, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedRole, pCur, iMsgSize, sizeof(PlayerRole)*numberofRevealedRole ) );
				m_RevealedRole.SetLinkedBuffer(numberofRevealedRole, numberofRevealedRole, pRevealedRole);


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)*InRevealedPlayerID.GetSize() + sizeof(UINT16)
					+ sizeof(PlayerRole)*InRevealedRole.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GameRevealPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				UINT16 numberOfInRevealedPlayerID = (UINT16)InRevealedPlayerID.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedPlayerID.data(), (INT)(sizeof(PlayerID)*InRevealedPlayerID.GetSize())); 
				UINT16 numberOfInRevealedRole = (UINT16)InRevealedRole.GetSize(); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedRole, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedRole.data(), (INT)(sizeof(PlayerRole)*InRevealedRole.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole )

			HRESULT GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )


			VOID GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GameRevealPlayerRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%, RevealedPlayerID:%6%, RevealedRole:%7%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext, ArgArray<PlayerID>(m_RevealedPlayerID), ArgArray<PlayerRole>(m_RevealedRole)); 
			}; // VOID GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 24);
			HRESULT GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayerReviveCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const RouteContext &InRouteContext, const PlayerID &InPlayerID )

			HRESULT GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerReviveCmd:%1%:%2% , Context:%3%, RouteContext:%4%, PlayerID:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, m_RouteContext, m_PlayerID); 
			}; // VOID GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 24);
			HRESULT GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(HRESULT) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(Context)
					+ sizeof(HRESULT)
					+ sizeof(RouteContext));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayerReviveRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InContext, sizeof(Context));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(HRESULT));
				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Context &InContext, const HRESULT &InResult, const RouteContext &InRouteContext )

			HRESULT GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				pCur += sizeof(Context); iMsgSize -= sizeof(Context);
				pCur += sizeof(HRESULT); iMsgSize -= sizeof(HRESULT);
				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerReviveRes:%1%:%2% , Context:%3%, Result:%4%, RouteContext:%5%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Context, ArgHex32(m_Result), m_RouteContext); 
			}; // VOID GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, POLICY_GAMEINSTANCE, 25);
			HRESULT GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevivedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )

			HRESULT GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
			{
 				HRESULT hr = S_OK;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = NULL;

				protocolMem( pNewMsg = MessageData::NewMessage( GameInstance::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRevivedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )

			HRESULT GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				HRESULT hr = S_OK;

				INT iMsgSize;
				BYTE* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // HRESULT GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			VOID GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				Prefix;
				protocolTrace(Trace::TRC_DBG1, "%0%:GamePlayerRevivedS2CEvt:%1%:%2% , RouteContext:%3%, RevivedPlayerID:%4%",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RouteContext, m_RevivedPlayerID); 
			}; // VOID GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace GameInstance
	}; // namespace Message
}; // namespace BR


