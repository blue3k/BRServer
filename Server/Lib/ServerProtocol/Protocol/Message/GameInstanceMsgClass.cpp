////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/GameInstanceMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameInstance
		{
 			// C2S: Nitify that a game instance is deleted. Game instance send this message to manager before it destroy itself.
			const MessageID DeleteGameC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 0);
			Result DeleteGameC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));

				return hr;

			}; // Result DeleteGameC2SEvt::ParseMessage( MessageData* pIMsg )


			Result DeleteGameC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) DeleteGameC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result DeleteGameC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* DeleteGameC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::DeleteGameC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));

				return hr;
			}; // MessageData* DeleteGameC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext )

			Result DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result DeleteGameC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result DeleteGameC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				DeleteGameC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "DeleteGame:{0}:{1} , RouteContext:{2}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext()); 
				return ResultCode::SUCCESS;
			}; // Result DeleteGameC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Join to a game instance. You can call multiple times, but it would be waste
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 1);
			Result JoinGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Player));
				protocolCheck(input->Read(m_RequestedRole));

				return hr;

			}; // Result JoinGameCmd::ParseMessage( MessageData* pIMsg )


			Result JoinGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinGameCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayer)
					+ SerializedSizeOf(InRequestedRole)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::JoinGameCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayer));
				protocolCheck(output->Write(InRequestedRole));

				return hr;
			}; // MessageData* JoinGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerInformation &InPlayer, const uint8_t &InRequestedRole )

			Result JoinGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result JoinGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result JoinGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Player:{4}, RequestedRole:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayer(), (int)parser.GetRequestedRole()); 
				return ResultCode::SUCCESS;
			}; // Result JoinGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 1);
			Result JoinGameRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));
				protocolCheck(input->Read(m_GameInsSvr));
				protocolCheck(input->Read(m_TimeStamp));
				protocolCheck(input->Read(m_GameState));
				protocolCheck(input->Read(m_Day));
				protocolCheck(input->Read(m_MaxPlayer));
				protocolCheck(input->Read(m_PlayerIndex));
				protocolCheck(input->Read(m_PlayerCharacter));
				protocolCheck(input->Read(m_Role));
				protocolCheck(input->Read(m_Dead));
				protocolCheck(input->Read(m_IsNewJoin));
				protocolCheck(input->Read(ArrayLen));
				uint8_t* ChatHistoryDataPtr = nullptr;
				protocolCheck(input->ReadLink(ChatHistoryDataPtr, ArrayLen));
				m_ChatHistoryData.SetLinkedBuffer(ArrayLen, ChatHistoryDataPtr);
				protocolCheck(input->Read(ArrayLen));
				uint8_t* GameLogDataPtr = nullptr;
				protocolCheck(input->ReadLink(GameLogDataPtr, ArrayLen));
				m_GameLogData.SetLinkedBuffer(ArrayLen, GameLogDataPtr);

				return hr;

			}; // Result JoinGameRes::ParseMessage( MessageData* pIMsg )


			Result JoinGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) JoinGameRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result JoinGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* JoinGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInChatHistoryData = (uint16_t)InChatHistoryData.size(); 
				uint16_t numberOfInGameLogData = (uint16_t)InGameLogData.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InGameInsSvr)
					+ SerializedSizeOf(InTimeStamp)
					+ SerializedSizeOf(InGameState)
					+ SerializedSizeOf(InDay)
					+ SerializedSizeOf(InMaxPlayer)
					+ SerializedSizeOf(InPlayerIndex)
					+ SerializedSizeOf(InPlayerCharacter)
					+ SerializedSizeOf(InRole)
					+ SerializedSizeOf(InDead)
					+ SerializedSizeOf(InIsNewJoin)
					+ SerializedSizeOf(InChatHistoryData)
					+ SerializedSizeOf(InGameLogData)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::JoinGameRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));
				protocolCheck(output->Write(InGameInsSvr));
				protocolCheck(output->Write(InTimeStamp));
				protocolCheck(output->Write(InGameState));
				protocolCheck(output->Write(InDay));
				protocolCheck(output->Write(InMaxPlayer));
				protocolCheck(output->Write(InPlayerIndex));
				protocolCheck(output->Write(InPlayerCharacter));
				protocolCheck(output->Write(InRole));
				protocolCheck(output->Write(InDead));
				protocolCheck(output->Write(InIsNewJoin));
				protocolCheck(output->Write(InChatHistoryData));
				protocolCheck(output->Write(InGameLogData));

				return hr;
			}; // MessageData* JoinGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const NetAddress &InGameInsSvr, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay, const uint8_t &InMaxPlayer, const uint8_t &InPlayerIndex, const uint8_t &InPlayerCharacter, const uint8_t &InRole, const uint8_t &InDead, const uint8_t &InIsNewJoin, const Array<uint8_t>& InChatHistoryData, const Array<uint8_t>& InGameLogData )

			Result JoinGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result JoinGameRes::OverrideRouteContextDestination( EntityUID to )


			Result JoinGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				JoinGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "JoinGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, GameInsSvr:{5}, TimeStamp:{6}, GameState:{7}, Day:{8}, MaxPlayer:{9}, PlayerIndex:{10}, PlayerCharacter:{11}, Role:{12}, Dead:{13}, IsNewJoin:{14}, ChatHistoryData:{15,30}, GameLogData:{16,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetGameInsSvr(), parser.GetTimeStamp(), (int)parser.GetGameState(), parser.GetDay(), parser.GetMaxPlayer(), parser.GetPlayerIndex(), parser.GetPlayerCharacter(), (int)parser.GetRole(), parser.GetDead(), parser.GetIsNewJoin(), parser.GetChatHistoryData(), parser.GetGameLogData()); 
				return ResultCode::SUCCESS;
			}; // Result JoinGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player joined event. This event is brocasted when a player joined
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 2);
			Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_JoinedPlayer));
				protocolCheck(input->Read(m_JoinedPlayerRole));
				protocolCheck(input->Read(m_JoinedPlayerDead));
				protocolCheck(input->Read(m_JoinedPlayerIndex));
				protocolCheck(input->Read(m_JoinedPlayerCharacter));

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerJoinedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InJoinedPlayer)
					+ SerializedSizeOf(InJoinedPlayerRole)
					+ SerializedSizeOf(InJoinedPlayerDead)
					+ SerializedSizeOf(InJoinedPlayerIndex)
					+ SerializedSizeOf(InJoinedPlayerCharacter)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InJoinedPlayer));
				protocolCheck(output->Write(InJoinedPlayerRole));
				protocolCheck(output->Write(InJoinedPlayerDead));
				protocolCheck(output->Write(InJoinedPlayerIndex));
				protocolCheck(output->Write(InJoinedPlayerCharacter));

				return hr;
			}; // MessageData* PlayerJoinedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerInformation &InJoinedPlayer, const uint8_t &InJoinedPlayerRole, const uint8_t &InJoinedPlayerDead, const uint8_t &InJoinedPlayerIndex, const uint8_t &InJoinedPlayerCharacter )

			Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PlayerJoinedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerJoinedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerJoined:{0}:{1} , RouteContext:{2}, JoinedPlayer:{3}, JoinedPlayerRole:{4}, JoinedPlayerDead:{5}, JoinedPlayerIndex:{6}, JoinedPlayerCharacter:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetJoinedPlayer(), (int)parser.GetJoinedPlayerRole(), parser.GetJoinedPlayerDead(), parser.GetJoinedPlayerIndex(), parser.GetJoinedPlayerCharacter()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerJoinedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: For debug purpose, change configue preset. There is a game setting table. you can switch between those setting value.
			const MessageID SetConfigPresetC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 3);
			Result SetConfigPresetC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_PresetID));

				return hr;

			}; // Result SetConfigPresetC2SEvt::ParseMessage( MessageData* pIMsg )


			Result SetConfigPresetC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) SetConfigPresetC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result SetConfigPresetC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* SetConfigPresetC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InPresetID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InPresetID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::SetConfigPresetC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InPresetID));

				return hr;
			}; // MessageData* SetConfigPresetC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InPresetID )

			Result SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result SetConfigPresetC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result SetConfigPresetC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				SetConfigPresetC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "SetConfigPreset:{0}:{1} , RouteContext:{2}, PresetID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPresetID()); 
				return ResultCode::SUCCESS;
			}; // Result SetConfigPresetC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Leave game instance.
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 4);
			Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));

				return hr;

			}; // Result LeaveGameCmd::ParseMessage( MessageData* pIMsg )


			Result LeaveGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeaveGameCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeaveGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeaveGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::LeaveGameCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));

				return hr;
			}; // MessageData* LeaveGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result LeaveGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result LeaveGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeaveGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 4);
			Result LeaveGameRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result LeaveGameRes::ParseMessage( MessageData* pIMsg )


			Result LeaveGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) LeaveGameRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result LeaveGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* LeaveGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::LeaveGameRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* LeaveGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result LeaveGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result LeaveGameRes::OverrideRouteContextDestination( EntityUID to )


			Result LeaveGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				LeaveGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "LeaveGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result LeaveGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player left event.
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 5);
			Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_LeftPlayerID));

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerLeftS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InLeftPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::PlayerLeftS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InLeftPlayerID));

				return hr;
			}; // MessageData* PlayerLeftS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InLeftPlayerID )

			Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PlayerLeftS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerLeftS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerLeftS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerLeft:{0}:{1} , RouteContext:{2}, LeftPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetLeftPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerLeftS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Kick player with given ID
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 6);
			Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(m_PlayerToKick));

				return hr;

			}; // Result KickPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InPlayerToKick)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::KickPlayerCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InPlayerToKick));

				return hr;
			}; // MessageData* KickPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )

			Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result KickPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				KickPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, PlayerToKick:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetPlayerToKick()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 6);
			Result KickPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result KickPlayerRes::ParseMessage( MessageData* pIMsg )


			Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) KickPlayerRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result KickPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::KickPlayerRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* KickPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result KickPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result KickPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				KickPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "KickPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result KickPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player kicked event. this event will be brocasted when a player kicked.
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 7);
			Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_KickedPlayerID));

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerKickedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InKickedPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::PlayerKickedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InKickedPlayerID));

				return hr;
			}; // MessageData* PlayerKickedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKickedPlayerID )

			Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PlayerKickedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKickedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerKickedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKicked:{0}:{1} , RouteContext:{2}, KickedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKickedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKickedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Assign new roles to all players.
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 8);
			Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));

				return hr;

			}; // Result AssignRoleCmd::ParseMessage( MessageData* pIMsg )


			Result AssignRoleCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AssignRoleCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AssignRoleCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* AssignRoleCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::AssignRoleCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));

				return hr;
			}; // MessageData* AssignRoleCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result AssignRoleCmd::OverrideRouteContextDestination( EntityUID to )


			Result AssignRoleCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AssignRoleCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AssignRole:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result AssignRoleCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 8);
			Result AssignRoleRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result AssignRoleRes::ParseMessage( MessageData* pIMsg )


			Result AssignRoleRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AssignRoleRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AssignRoleRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* AssignRoleRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::AssignRoleRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AssignRoleRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AssignRoleRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result AssignRoleRes::OverrideRouteContextDestination( EntityUID to )


			Result AssignRoleRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AssignRoleRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AssignRole:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AssignRoleRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event for role assigned
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 9);
			Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_Role));

				return hr;

			}; // Result RoleAssignedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result RoleAssignedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RoleAssignedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RoleAssignedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* RoleAssignedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint8_t &InRole )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InRole)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::RoleAssignedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InRole));

				return hr;
			}; // MessageData* RoleAssignedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint8_t &InRole )

			Result RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result RoleAssignedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result RoleAssignedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RoleAssignedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RoleAssigned:{0}:{1} , RouteContext:{2}, Role:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), (int)parser.GetRole()); 
				return ResultCode::SUCCESS;
			}; // Result RoleAssignedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// C2S: in-game chatting message.
			const MessageID ChatMessageC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 10);
			Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(m_Role));
				protocolCheck(input->Read(ArrayLen));
				protocolCheck(input->ReadLink(m_ChatMessage, ArrayLen));

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessage( MessageData* pIMsg )


			Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) ChatMessageC2SEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result ChatMessageC2SEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InRole)
					+ SerializedSizeOf(InChatMessage)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::ChatMessageC2SEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InRole));
				protocolCheck(output->Write(InChatMessage));

				return hr;
			}; // MessageData* ChatMessageC2SEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InPlayerID, const uint8_t &InRole, const char* InChatMessage )

			Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result ChatMessageC2SEvt::OverrideRouteContextDestination( EntityUID to )


			Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				ChatMessageC2SEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "ChatMessage:{0}:{1} , RouteContext:{2}, PlayerID:{3}, Role:{4}, ChatMessage:{5,60}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetPlayerID(), (int)parser.GetRole(), parser.GetChatMessage()); 
				return ResultCode::SUCCESS;
			}; // Result ChatMessageC2SEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 11);
			Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));

				return hr;

			}; // Result AdvanceGameCmd::ParseMessage( MessageData* pIMsg )


			Result AdvanceGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AdvanceGameCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AdvanceGameCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* AdvanceGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::AdvanceGameCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));

				return hr;
			}; // MessageData* AdvanceGameCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result AdvanceGameCmd::OverrideRouteContextDestination( EntityUID to )


			Result AdvanceGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AdvanceGameCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AdvanceGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result AdvanceGameCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 11);
			Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result AdvanceGameRes::ParseMessage( MessageData* pIMsg )


			Result AdvanceGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AdvanceGameRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AdvanceGameRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* AdvanceGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::AdvanceGameRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AdvanceGameRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result AdvanceGameRes::OverrideRouteContextDestination( EntityUID to )


			Result AdvanceGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AdvanceGameRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AdvanceGame:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AdvanceGameRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 12);
			Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TimeStamp));
				protocolCheck(input->Read(m_GameState));
				protocolCheck(input->Read(m_Day));

				return hr;

			}; // Result GameAdvancedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameAdvancedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GameAdvancedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GameAdvancedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GameAdvancedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTimeStamp)
					+ SerializedSizeOf(InGameState)
					+ SerializedSizeOf(InDay)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GameAdvancedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTimeStamp));
				protocolCheck(output->Write(InGameState));
				protocolCheck(output->Write(InDay));

				return hr;
			}; // MessageData* GameAdvancedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint32_t &InTimeStamp, const uint8_t &InGameState, const uint8_t &InDay )

			Result GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GameAdvancedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameAdvancedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GameAdvancedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameAdvanced:{0}:{1} , RouteContext:{2}, TimeStamp:{3}, GameState:{4}, Day:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTimeStamp(), (int)parser.GetGameState(), parser.GetDay()); 
				return ResultCode::SUCCESS;
			}; // Result GameAdvancedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 13);
			Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_Winner));
				protocolCheck(input->Read(m_GainedExp));
				protocolCheck(input->Read(m_GainedGameMoney));
				protocolCheck(input->Read(m_PlayedRole));
				protocolCheck(input->Read(m_IsWon));

				return hr;

			}; // Result GameEndedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameEndedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GameEndedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GameEndedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GameEndedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InWinner)
					+ SerializedSizeOf(InGainedExp)
					+ SerializedSizeOf(InGainedGameMoney)
					+ SerializedSizeOf(InPlayedRole)
					+ SerializedSizeOf(InIsWon)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GameEndedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InWinner));
				protocolCheck(output->Write(InGainedExp));
				protocolCheck(output->Write(InGainedGameMoney));
				protocolCheck(output->Write(InPlayedRole));
				protocolCheck(output->Write(InIsWon));

				return hr;
			}; // MessageData* GameEndedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint8_t &InWinner, const uint32_t &InGainedExp, const uint32_t &InGainedGameMoney, const uint8_t &InPlayedRole, const uint8_t &InIsWon )

			Result GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GameEndedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameEndedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GameEndedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameEnded:{0}:{1} , RouteContext:{2}, Winner:{3}, GainedExp:{4}, GainedGameMoney:{5}, PlayedRole:{6}, IsWon:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), (int)parser.GetWinner(), parser.GetGainedExp(), parser.GetGainedGameMoney(), (int)parser.GetPlayedRole(), parser.GetIsWon()); 
				return ResultCode::SUCCESS;
			}; // Result GameEndedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: *Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 14);
			Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));

				return hr;

			}; // Result VoteGameAdvanceCmd::ParseMessage( MessageData* pIMsg )


			Result VoteGameAdvanceCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VoteGameAdvanceCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VoteGameAdvanceCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VoteGameAdvanceCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VoteGameAdvanceCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));

				return hr;
			}; // MessageData* VoteGameAdvanceCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VoteGameAdvanceCmd::OverrideRouteContextDestination( EntityUID to )


			Result VoteGameAdvanceCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VoteGameAdvanceCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteGameAdvance:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result VoteGameAdvanceCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 14);
			Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result VoteGameAdvanceRes::ParseMessage( MessageData* pIMsg )


			Result VoteGameAdvanceRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VoteGameAdvanceRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VoteGameAdvanceRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VoteGameAdvanceRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VoteGameAdvanceRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* VoteGameAdvanceRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VoteGameAdvanceRes::OverrideRouteContextDestination( EntityUID to )


			Result VoteGameAdvanceRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VoteGameAdvanceRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteGameAdvance:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result VoteGameAdvanceRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: *GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 15);
			Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_Voter));

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GameAdvanceVotedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GameAdvanceVotedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GameAdvanceVotedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InVoter )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InVoter)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InVoter));

				return hr;
			}; // MessageData* GameAdvanceVotedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InVoter )

			Result GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GameAdvanceVotedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GameAdvanceVotedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameAdvanceVoted:{0}:{1} , RouteContext:{2}, Voter:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoter()); 
				return ResultCode::SUCCESS;
			}; // Result GameAdvanceVotedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Assign role
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 16);
			Result VoteCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(m_VoteTarget));
				protocolCheck(input->Read(m_ActionSerial));

				return hr;

			}; // Result VoteCmd::ParseMessage( MessageData* pIMsg )


			Result VoteCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VoteCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VoteCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VoteCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InVoteTarget)
					+ SerializedSizeOf(InActionSerial)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VoteCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InVoteTarget));
				protocolCheck(output->Write(InActionSerial));

				return hr;
			}; // MessageData* VoteCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const PlayerID &InVoteTarget, const uint32_t &InActionSerial )

			Result VoteCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VoteCmd::OverrideRouteContextDestination( EntityUID to )


			Result VoteCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VoteCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Vote:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, VoteTarget:{5}, ActionSerial:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetVoteTarget(), parser.GetActionSerial()); 
				return ResultCode::SUCCESS;
			}; // Result VoteCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 16);
			Result VoteRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result VoteRes::ParseMessage( MessageData* pIMsg )


			Result VoteRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VoteRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VoteRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VoteRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VoteRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* VoteRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result VoteRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VoteRes::OverrideRouteContextDestination( EntityUID to )


			Result VoteRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VoteRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Vote:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result VoteRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 17);
			Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_Voter));
				protocolCheck(input->Read(m_VotedTarget));

				return hr;

			}; // Result VotedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result VotedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VotedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VotedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VotedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InVoter)
					+ SerializedSizeOf(InVotedTarget)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VotedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InVoter));
				protocolCheck(output->Write(InVotedTarget));

				return hr;
			}; // MessageData* VotedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InVoter, const PlayerID &InVotedTarget )

			Result VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VotedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result VotedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VotedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "Voted:{0}:{1} , RouteContext:{2}, Voter:{3}, VotedTarget:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoter(), parser.GetVotedTarget()); 
				return ResultCode::SUCCESS;
			}; // Result VotedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 18);
			Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(ArrayLen));
				PlayerID* VotedPtr = nullptr;
				protocolCheck(input->ReadLink(VotedPtr, ArrayLen));
				m_Voted.SetLinkedBuffer(ArrayLen, VotedPtr);

				return hr;

			}; // Result VoteEndS2CEvt::ParseMessage( MessageData* pIMsg )


			Result VoteEndS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) VoteEndS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result VoteEndS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* VoteEndS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInVoted = (uint16_t)InVoted.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InVoted)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::VoteEndS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InVoted));

				return hr;
			}; // MessageData* VoteEndS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const Array<PlayerID>& InVoted )

			Result VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result VoteEndS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result VoteEndS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				VoteEndS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "VoteEnd:{0}:{1} , RouteContext:{2}, Voted:{3,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetVoted()); 
				return ResultCode::SUCCESS;
			}; // Result VoteEndS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 19);
			Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_RevealedPlayerID));
				protocolCheck(input->Read(m_Role));
				protocolCheck(input->Read(m_Reason));

				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerRevealedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerRevealedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerRevealedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InRevealedPlayerID)
					+ SerializedSizeOf(InRole)
					+ SerializedSizeOf(InReason)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InRevealedPlayerID));
				protocolCheck(output->Write(InRole));
				protocolCheck(output->Write(InReason));

				return hr;
			}; // MessageData* PlayerRevealedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InRevealedPlayerID, const uint8_t &InRole, const uint8_t &InReason )

			Result PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PlayerRevealedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerRevealedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerRevealedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerRevealed:{0}:{1} , RouteContext:{2}, RevealedPlayerID:{3}, Role:{4}, Reason:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRevealedPlayerID(), (int)parser.GetRole(), (int)parser.GetReason()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerRevealedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 20);
			Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_KilledPlayer));
				protocolCheck(input->Read(m_Reason));

				return hr;

			}; // Result PlayerKilledS2CEvt::ParseMessage( MessageData* pIMsg )


			Result PlayerKilledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerKilledS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerKilledS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerKilledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InKilledPlayer)
					+ SerializedSizeOf(InReason)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::PlayerKilledS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InKilledPlayer));
				protocolCheck(output->Write(InReason));

				return hr;
			}; // MessageData* PlayerKilledS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InKilledPlayer, const uint8_t &InReason )

			Result PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result PlayerKilledS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result PlayerKilledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerKilledS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "PlayerKilled:{0}:{1} , RouteContext:{2}, KilledPlayer:{3}, Reason:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetKilledPlayer(), (int)parser.GetReason()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerKilledS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 21);
			Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_LeadPlayer));
				protocolCheck(input->Read(m_PartyUID));

				return hr;

			}; // Result GamePlayAgainCmd::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayAgainCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayAgainCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayAgainCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InLeadPlayer)
					+ SerializedSizeOf(InPartyUID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayAgainCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InLeadPlayer));
				protocolCheck(output->Write(InPartyUID));

				return hr;
			}; // MessageData* GamePlayAgainCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InLeadPlayer, const uint64_t &InPartyUID )

			Result GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayAgainCmd::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayAgainCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TransactionID:{3}, LeadPlayer:{4}, PartyUID:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetLeadPlayer(), parser.GetPartyUID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 21);
			Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));
				protocolCheck(input->Read(m_ReplayMemberCount));

				return hr;

			}; // Result GamePlayAgainRes::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayAgainRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayAgainRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayAgainRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InReplayMemberCount)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayAgainRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));
				protocolCheck(output->Write(InReplayMemberCount));

				return hr;
			}; // MessageData* GamePlayAgainRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint32_t &InReplayMemberCount )

			Result GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayAgainRes::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayAgainRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, ReplayMemberCount:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetReplayMemberCount()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 22);
			Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TargetPlayer));
				protocolCheck(input->Read(m_PartyUID));
				protocolCheck(input->Read(m_LeadPlayer));

				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GamePlayAgainS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayAgainS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayAgainS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTargetPlayer)
					+ SerializedSizeOf(InPartyUID)
					+ SerializedSizeOf(InLeadPlayer)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTargetPlayer));
				protocolCheck(output->Write(InPartyUID));
				protocolCheck(output->Write(InLeadPlayer));

				return hr;
			}; // MessageData* GamePlayAgainS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InTargetPlayer, const uint64_t &InPartyUID, const PlayerID &InLeadPlayer )

			Result GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayAgainS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayAgainS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayAgainS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayAgain:{0}:{1} , RouteContext:{2}, TargetPlayer:{3}, PartyUID:{4}, LeadPlayer:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTargetPlayer(), parser.GetPartyUID(), parser.GetLeadPlayer()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayAgainS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Player. revive himself
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 23);
			Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));
				protocolCheck(input->Read(ArrayLen));
				PlayerID* TargetPlayerIDPtr = nullptr;
				protocolCheck(input->ReadLink(TargetPlayerIDPtr, ArrayLen));
				m_TargetPlayerID.SetLinkedBuffer(ArrayLen, TargetPlayerIDPtr);

				return hr;

			}; // Result GameRevealPlayerCmd::ParseMessage( MessageData* pIMsg )


			Result GameRevealPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GameRevealPlayerCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GameRevealPlayerCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GameRevealPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInTargetPlayerID = (uint16_t)InTargetPlayerID.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
					+ SerializedSizeOf(InTargetPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GameRevealPlayerCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InTargetPlayerID));

				return hr;
			}; // MessageData* GameRevealPlayerCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const Array<PlayerID>& InTargetPlayerID )

			Result GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GameRevealPlayerCmd::OverrideRouteContextDestination( EntityUID to )


			Result GameRevealPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GameRevealPlayerCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameRevealPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, TargetPlayerID:{5,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetTargetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GameRevealPlayerCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 23);
			Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));
				protocolCheck(input->Read(ArrayLen));
				PlayerID* RevealedPlayerIDPtr = nullptr;
				protocolCheck(input->ReadLink(RevealedPlayerIDPtr, ArrayLen));
				m_RevealedPlayerID.SetLinkedBuffer(ArrayLen, RevealedPlayerIDPtr);
				protocolCheck(input->Read(ArrayLen));
				uint8_t* RevealedRolePtr = nullptr;
				protocolCheck(input->ReadLink(RevealedRolePtr, ArrayLen));
				m_RevealedRole.SetLinkedBuffer(ArrayLen, RevealedRolePtr);

				return hr;

			}; // Result GameRevealPlayerRes::ParseMessage( MessageData* pIMsg )


			Result GameRevealPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GameRevealPlayerRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GameRevealPlayerRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GameRevealPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				uint16_t numberOfInRevealedPlayerID = (uint16_t)InRevealedPlayerID.size(); 
				uint16_t numberOfInRevealedRole = (uint16_t)InRevealedRole.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
					+ SerializedSizeOf(InRevealedPlayerID)
					+ SerializedSizeOf(InRevealedRole)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GameRevealPlayerRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));
				protocolCheck(output->Write(InRevealedPlayerID));
				protocolCheck(output->Write(InRevealedRole));

				return hr;
			}; // MessageData* GameRevealPlayerRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<uint8_t>& InRevealedRole )

			Result GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GameRevealPlayerRes::OverrideRouteContextDestination( EntityUID to )


			Result GameRevealPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GameRevealPlayerRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GameRevealPlayer:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, RevealedPlayerID:{5,30}, RevealedRole:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetRevealedPlayerID(), parser.GetRevealedRole()); 
				return ResultCode::SUCCESS;
			}; // Result GameRevealPlayerRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 24);
			Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_PlayerID));

				return hr;

			}; // Result GamePlayerReviveCmd::ParseMessage( MessageData* pIMsg )


			Result GamePlayerReviveCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayerReviveCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayerReviveCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayerReviveCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayerReviveCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));

				return hr;
			}; // MessageData* GamePlayerReviveCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID )

			Result GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayerReviveCmd::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerReviveCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayerReviveCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevive:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerReviveCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 24);
			Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_TransactionID));
				protocolCheck(input->Read(m_Result));

				return hr;

			}; // Result GamePlayerReviveRes::ParseMessage( MessageData* pIMsg )


			Result GamePlayerReviveRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayerReviveRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayerReviveRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayerReviveRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InTransactionID)
					+ SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayerReviveRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* GamePlayerReviveRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayerReviveRes::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerReviveRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayerReviveRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevive:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerReviveRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEINSTANCE, 25);
			Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )
			{
 				FunctionContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;

				protocolCheck(input->Read(m_RouteContext));
				protocolCheck(input->Read(m_RevivedPlayerID));

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseMessage( MessageData* pIMsg )


			Result GamePlayerRevivedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GamePlayerRevivedS2CEvt(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )


			MessageData* GamePlayerRevivedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				FunctionContext hr([pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						delete pNewMsg;
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InRevivedPlayerID)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, GameInstance::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InRevivedPlayerID));

				return hr;
			}; // MessageData* GamePlayerRevivedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const PlayerID &InRevivedPlayerID )

			Result GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )
			{
 				FunctionContext hr;

				MessageData* pIMsg = GetMessage();
				RouteContext routeContext;

				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = (int)((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;
				uint8_t* pCur = nullptr;

				pCur = input->GetBufferPtr() + input->GetPosition();
				Assert(input->GetRemainSize() >= sizeof(RouteContext));
				memcpy( &routeContext, pCur, sizeof(RouteContext) );
				routeContext.Components.To = to;
				memcpy( pCur, &routeContext, sizeof(RouteContext) );

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::OverrideRouteContextDestination( EntityUID to )


			Result GamePlayerRevivedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GamePlayerRevivedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GamePlayerRevived:{0}:{1} , RouteContext:{2}, RevivedPlayerID:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRevivedPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result GamePlayerRevivedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace GameInstance
	}; // namespace Message
}; // namespace SF


