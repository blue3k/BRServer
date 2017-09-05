////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameInstance Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameInstance
		{
 			// C2S: Game instance deletion
			const MessageID DeleteGameC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 0);
			Result DeleteGameC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );


			Proc_End:

				return hr;

			}; // Result DeleteGameC2SEvt::ParseMessage( MessageData* pIMsg )


			Result DeleteGameC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) DeleteGameC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result DeleteGameC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* DeleteGameC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::DeleteGameC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* DeleteGameC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext )

			Result DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result DeleteGameC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				DeleteGameC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "DeleteGame:{0}:{1} , RouteContext:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext()); 
				return ResultCode::SUCCESS;
			}; // Result DeleteGameC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Join Game
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 1);
			Result JoinGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestedRole, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::ParseMessage( MessageData* pIMsg )


			Result JoinGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) JoinGameCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const uint8_t &InRequestedRole )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerInformation)
					+ sizeof(AuthTicket)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::JoinGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InRequestedRole, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerInformation &InPlayer, const AuthTicket &InTicket, const uint8_t &InRequestedRole )

			Result JoinGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result JoinGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				JoinGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Player:{4}, Ticket:{5}, RequestedRole:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayer(), parser.GetTicket(), (int)parser.GetRequestedRole()); 
				return ResultCode::SUCCESS;
			}; // Result JoinGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 1);
			Result JoinGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofChatHistoryData = 0; uint8_t* pChatHistoryData = nullptr;
				uint16_t numberofGameLogData = 0; uint8_t* pGameLogData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameInsSvr, pCur, iMsgSize, sizeof(NetAddress) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsNewJoin, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(uint8_t)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(uint8_t)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);


			Proc_End:

				return hr;

			}; // Result JoinGameRes::ParseMessage( MessageData* pIMsg )


			Result JoinGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) JoinGameRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result JoinGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* JoinGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(NetAddress)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)*InChatHistoryData.size() + sizeof(uint16_t)
					+ sizeof(uint8_t)*InGameLogData.size() + sizeof(uint16_t));


				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.size(); 
				uint16_t numberOfInGameLogData = (uint16_t)InGameLogData.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::JoinGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InGameInsSvr, sizeof(NetAddress));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InIsNewJoin, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(uint8_t)*InChatHistoryData.size())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(uint8_t)*InGameLogData.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* JoinGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )

			Result JoinGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result JoinGameRes::OverrideRouteContextDestination( EntityUID to )


			Result JoinGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				JoinGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, GameInsSvr:{5}, TimeStamp:{6}, GameState:{7}, Day:{8}, MaxPlayer:{9}, PlayerIndex:{10}, PlayerCharacter:{11}, Role:{12}, Dead:{13}, IsNewJoin:{14}, ChatHistoryData:{15,30}, GameLogData:{16,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetGameInsSvr(), parser.GetTimeStamp(), (int)parser.GetGameState(), parser.GetDay(), parser.GetMaxPlayer(), parser.GetPlayerIndex(), parser.GetPlayerCharacter(), (int)parser.GetRole(), parser.GetDead(), parser.GetIsNewJoin(), parser.GetChatHistoryData(), parser.GetGameLogData()); 
				return ResultCode::SUCCESS;
			}; // Result JoinGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player Joined
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 2);
			Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerRole, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerDead, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerIndex, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerCharacter, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerJoinedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerJoinedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerInformation)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerDead, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerIndex, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerCharacter, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerJoinedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )

			Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerJoinedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerJoined:{0}:{1} , RouteContext:{2}, JoinedPlayer:{3}, JoinedPlayerRole:{4}, JoinedPlayerDead:{5}, JoinedPlayerIndex:{6}, JoinedPlayerCharacter:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetJoinedPlayer(), (int)parser.GetJoinedPlayerRole(), parser.GetJoinedPlayerDead(), parser.GetJoinedPlayerIndex(), parser.GetJoinedPlayerCharacter()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Change configue preset
			const MessageID SetConfigPresetC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 3);
			Result SetConfigPresetC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PresetID, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result SetConfigPresetC2SEvt::ParseMessage( MessageData* pIMsg )


			Result SetConfigPresetC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) SetConfigPresetC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result SetConfigPresetC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* SetConfigPresetC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint32_t &InPresetID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::SetConfigPresetC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPresetID, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* SetConfigPresetC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint32_t &InPresetID )

			Result SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result SetConfigPresetC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				SetConfigPresetC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "SetConfigPreset:{0}:{1} , RouteContext:{2}, PresetID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPresetID()); 
				return ResultCode::SUCCESS;
			}; // Result SetConfigPresetC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Leave Game
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 4);
			Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )


			Result LeaveGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) LeaveGameCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result LeaveGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* LeaveGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::LeaveGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* LeaveGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result LeaveGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				LeaveGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 4);
			Result LeaveGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::ParseMessage( MessageData* pIMsg )


			Result LeaveGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) LeaveGameRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result LeaveGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* LeaveGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::LeaveGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* LeaveGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result LeaveGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::OverrideRouteContextDestination( EntityUID to )


			Result LeaveGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				LeaveGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player left
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 5);
			Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerLeftS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerLeftS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerLeftS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )

			Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerLeftS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerLeftS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerLeft:{0}:{1} , RouteContext:{2}, LeftPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetLeftPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerLeftS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 6);
			Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result KickPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) KickPlayerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result KickPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* KickPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* KickPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )

			Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				KickPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, PlayerToKick:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetPlayerToKick()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 6);
			Result KickPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::ParseMessage( MessageData* pIMsg )


			Result KickPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) KickPlayerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result KickPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* KickPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* KickPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				KickPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 7);
			Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerKickedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerKickedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerKickedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )

			Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKickedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerKickedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKicked:{0}:{1} , RouteContext:{2}, KickedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKickedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKickedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Assign role
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 8);
			Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )


			Result AssignRoleCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AssignRoleCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AssignRoleCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* AssignRoleCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::AssignRoleCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AssignRoleCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )


			Result AssignRoleCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				AssignRoleCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AssignRole:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result AssignRoleCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 8);
			Result AssignRoleRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::ParseMessage( MessageData* pIMsg )


			Result AssignRoleRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AssignRoleRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AssignRoleRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* AssignRoleRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::AssignRoleRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AssignRoleRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result AssignRoleRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::OverrideRouteContextDestination( EntityUID to )


			Result AssignRoleRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				AssignRoleRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AssignRole:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AssignRoleRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Assign role
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 9);
			Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result RoleAssignedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) RoleAssignedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result RoleAssignedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* RoleAssignedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint8_t &InRole )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::RoleAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* RoleAssignedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint8_t &InRole )

			Result RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result RoleAssignedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				RoleAssignedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RoleAssigned:{0}:{1} , RouteContext:{2}, Role:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), (int)parser.GetRole()); 
				return ResultCode::SUCCESS;
			}; // Result RoleAssignedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// C2S: Chatting message
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 10);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) ChatMessageC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* ChatMessageC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInChatMessageLength = InChatMessage ? (uint16_t)(strlen(InChatMessage)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInChatMessageLength 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::ChatMessageC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* ChatMessageC2SEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, Role:{4}, ChatMessage:{5,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), (int)parser.GetRole(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 11);
			Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )


			Result AdvanceGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AdvanceGameCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AdvanceGameCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* AdvanceGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::AdvanceGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AdvanceGameCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result AdvanceGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				AdvanceGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AdvanceGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result AdvanceGameCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 11);
			Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )


			Result AdvanceGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) AdvanceGameRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result AdvanceGameRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* AdvanceGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::AdvanceGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* AdvanceGameRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )


			Result AdvanceGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				AdvanceGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AdvanceGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AdvanceGameRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 12);
			Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameAdvancedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameAdvancedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameAdvancedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GameAdvancedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GameAdvancedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GameAdvancedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )

			Result GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameAdvancedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GameAdvancedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameAdvanced:{0}:{1} , RouteContext:{2}, TimeStamp:{3}, GameState:{4}, Day:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTimeStamp(), (int)parser.GetGameState(), parser.GetDay()); 
				return ResultCode::SUCCESS;
			}; // Result GameAdvancedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 13);
			Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Winner, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedExp, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedGameMoney, pCur, iMsgSize, sizeof(uint32_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayedRole, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsWon, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameEndedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameEndedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameEndedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GameEndedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(uint8_t)
					+ sizeof(uint32_t)
					+ sizeof(uint32_t)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GameEndedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InWinner, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InGainedExp, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InGainedGameMoney, sizeof(uint32_t));
				Protocol::PackParamCopy( pMsgData, &InPlayedRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InIsWon, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GameEndedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )

			Result GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameEndedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GameEndedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameEnded:{0}:{1} , RouteContext:{2}, Winner:{3}, GainedExp:{4}, GainedGameMoney:{5}, PlayedRole:{6}, IsWon:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), (int)parser.GetWinner(), parser.GetGainedExp(), parser.GetGainedGameMoney(), (int)parser.GetPlayedRole(), parser.GetIsWon()); 
				return ResultCode::SUCCESS;
			}; // Result GameEndedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: *Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 14);
			Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )


			Result VoteGameAdvanceCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VoteGameAdvanceCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VoteGameAdvanceCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VoteGameAdvanceCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VoteGameAdvanceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VoteGameAdvanceCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )


			Result VoteGameAdvanceCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VoteGameAdvanceCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteGameAdvance:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result VoteGameAdvanceCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 14);
			Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )


			Result VoteGameAdvanceRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VoteGameAdvanceRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VoteGameAdvanceRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VoteGameAdvanceRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VoteGameAdvanceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VoteGameAdvanceRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )


			Result VoteGameAdvanceRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VoteGameAdvanceRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteGameAdvance:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result VoteGameAdvanceRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: *GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 15);
			Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameAdvanceVotedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameAdvanceVotedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GameAdvanceVotedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InVoter )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GameAdvanceVotedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InVoter )

			Result GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameAdvanceVotedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GameAdvanceVotedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameAdvanceVoted:{0}:{1} , RouteContext:{2}, Voter:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoter()); 
				return ResultCode::SUCCESS;
			}; // Result GameAdvanceVotedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Assign role
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 16);
			Result VoteCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteTarget, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ActionSerial, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result VoteCmd::ParseMessage( MessageData* pIMsg )


			Result VoteCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VoteCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VoteCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VoteCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VoteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVoteTarget, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InActionSerial, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VoteCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )

			Result VoteCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VoteCmd::OverrideRouteContextDestination( EntityUID to )


			Result VoteCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VoteCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Vote:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, VoteTarget:{5}, ActionSerial:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetVoteTarget(), parser.GetActionSerial()); 
				return ResultCode::SUCCESS;
			}; // Result VoteCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 16);
			Result VoteRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteRes::ParseMessage( MessageData* pIMsg )


			Result VoteRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VoteRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VoteRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VoteRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VoteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VoteRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result VoteRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VoteRes::OverrideRouteContextDestination( EntityUID to )


			Result VoteRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VoteRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Vote:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result VoteRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 17);
			Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedTarget, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result VotedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VotedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VotedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VotedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVotedTarget, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VotedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )

			Result VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result VotedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VotedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Voted:{0}:{1} , RouteContext:{2}, Voter:{3}, VotedTarget:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoter(), parser.GetVotedTarget()); 
				return ResultCode::SUCCESS;
			}; // Result VotedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 18);
			Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofVoted = 0; PlayerID* pVoted = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofVoted, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pVoted, pCur, iMsgSize, sizeof(PlayerID)*numberofVoted ) );
				m_Voted.SetLinkedBuffer(numberofVoted, numberofVoted, pVoted);


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )


			Result VoteEndS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) VoteEndS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result VoteEndS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* VoteEndS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)*InVoted.size() + sizeof(uint16_t));


				uint16_t numberOfInVoted = (uint16_t)InVoted.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::VoteEndS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &numberOfInVoted, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InVoted.data(), (INT)(sizeof(PlayerID)*InVoted.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* VoteEndS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )

			Result VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result VoteEndS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				VoteEndS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteEnd:{0}:{1} , RouteContext:{2}, Voted:{3,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoted()); 
				return ResultCode::SUCCESS;
			}; // Result VoteEndS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 19);
			Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(uint8_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerRevealedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerRevealedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerRevealedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint8_t)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRevealedPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(uint8_t));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerRevealedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )

			Result PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerRevealedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerRevealedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerRevealed:{0}:{1} , RouteContext:{2}, RevealedPlayerID:{3}, Role:{4}, Reason:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRevealedPlayerID(), (int)parser.GetRole(), (int)parser.GetReason()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerRevealedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 20);
			Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KilledPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(uint8_t) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerKilledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerKilledS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerKilledS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerKilledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint8_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::PlayerKilledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InKilledPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(uint8_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerKilledS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )

			Result PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKilledS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				PlayerKilledS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKilled:{0}:{1} , RouteContext:{2}, KilledPlayer:{3}, Reason:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKilledPlayer(), (int)parser.GetReason()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKilledS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 21);
			Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(uint64_t) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayAgainCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayAgainCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayAgainCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayAgainCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(uint64_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayAgainCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )

			Result GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayAgainCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TransactionID:{3}, LeadPlayer:{4}, PartyUID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetLeadPlayer(), parser.GetPartyUID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 21);
			Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ReplayMemberCount, pCur, iMsgSize, sizeof(uint32_t) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayAgainRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayAgainRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayAgainRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(uint32_t));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayAgainRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InReplayMemberCount, sizeof(uint32_t));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayAgainRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )

			Result GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayAgainRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, ReplayMemberCount:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetReplayMemberCount()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 22);
			Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(uint64_t) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayAgainS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayAgainS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID)
					+ sizeof(uint64_t)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(uint64_t));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayAgainS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )

			Result GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayAgainS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TargetPlayer:{3}, PartyUID:{4}, LeadPlayer:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTargetPlayer(), parser.GetPartyUID(), parser.GetLeadPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Player. revive himself
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 23);
			Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result GameRevealPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameRevealPlayerCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameRevealPlayerCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GameRevealPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID)*InTargetPlayerID.size() + sizeof(uint16_t));


				uint16_t numberOfInTargetPlayerID = (uint16_t)InTargetPlayerID.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GameRevealPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GameRevealPlayerCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )

			Result GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result GameRevealPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GameRevealPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameRevealPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, TargetPlayerID:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTargetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GameRevealPlayerCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 23);
			Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t numberofRevealedPlayerID = 0; PlayerID* pRevealedPlayerID = nullptr;
				uint16_t numberofRevealedRole = 0; uint8_t* pRevealedRole = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedPlayerID, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofRevealedPlayerID ) );
				m_RevealedPlayerID.SetLinkedBuffer(numberofRevealedPlayerID, numberofRevealedPlayerID, pRevealedPlayerID);
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedRole, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedRole, pCur, iMsgSize, sizeof(uint8_t)*numberofRevealedRole ) );
				m_RevealedRole.SetLinkedBuffer(numberofRevealedRole, numberofRevealedRole, pRevealedRole);


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )


			Result GameRevealPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GameRevealPlayerRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GameRevealPlayerRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GameRevealPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result)
					+ sizeof(PlayerID)*InRevealedPlayerID.size() + sizeof(uint16_t)
					+ sizeof(uint8_t)*InRevealedRole.size() + sizeof(uint16_t));


				uint16_t numberOfInRevealedPlayerID = (uint16_t)InRevealedPlayerID.size(); 
				uint16_t numberOfInRevealedRole = (uint16_t)InRevealedRole.size(); 
				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GameRevealPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedPlayerID, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRevealedPlayerID.data(), (INT)(sizeof(PlayerID)*InRevealedPlayerID.size())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedRole, sizeof(uint16_t)); 
				Protocol::PackParamCopy( pMsgData, InRevealedRole.data(), (INT)(sizeof(uint8_t)*InRevealedRole.size())); 


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GameRevealPlayerRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )

			Result GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result GameRevealPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GameRevealPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameRevealPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, RevealedPlayerID:{5,30}, RevealedRole:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRevealedPlayerID(), parser.GetRevealedRole()); 
				return ResultCode::SUCCESS;
			}; // Result GameRevealPlayerRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 24);
			Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )


			Result GamePlayerReviveCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayerReviveCmd(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayerReviveCmd::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayerReviveCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayerReviveCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayerReviveCmd::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const PlayerID &InPlayerID )

			Result GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerReviveCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayerReviveCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevive:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerReviveCmd::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 24);
			Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TransactionID, pCur, iMsgSize, sizeof(TransactionID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )


			Result GamePlayerReviveRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayerReviveRes(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayerReviveRes::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayerReviveRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(TransactionID)
					+ sizeof(Result));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayerReviveRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InTransactionID, sizeof(TransactionID));
				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayerReviveRes::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult )

			Result GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerReviveRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayerReviveRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevive:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerReviveRes::TraceOut(const char* prefix, MessageDataPtr& pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 25);
			Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RouteContext, pCur, iMsgSize, sizeof(RouteContext) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevivedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GamePlayerRevivedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) GamePlayerRevivedS2CEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageDataPtr& pIMsg, MessageBase* &pMessageBase )

			MessageData* GamePlayerRevivedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(RouteContext)
					+ sizeof(PlayerID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameInstance::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRouteContext, sizeof(RouteContext));
				Protocol::PackParamCopy( pMsgData, &InRevivedPlayerID, sizeof(PlayerID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* GamePlayerRevivedS2CEvt::Create( IMemoryManager& memoryManager, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )

			Result GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				Assert( iMsgSize >= (INT)sizeof(RouteContext) );
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerRevivedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)
			{
 				GamePlayerRevivedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevived:{0}:{1} , RouteContext:{2}, RevivedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRevivedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerRevivedS2CEvt::TraceOut(const char* prefix, MessageDataPtr& pMsg)



		}; // namespace GameInstance
	}; // namespace Message
}; // namespace SF


