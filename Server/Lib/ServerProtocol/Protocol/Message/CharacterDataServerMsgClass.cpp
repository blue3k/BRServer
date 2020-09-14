////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : CharacterDataServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "String/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/Message/CharacterDataServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace CharacterDataServer
		{
 			// Cmd: Add character data to the account
			const MessageID AddCharacterDataCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 0);
			Result AddCharacterDataCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_Attributes));

				return hr;

			}; // Result AddCharacterDataCmd::ParseMessage( MessageData* pIMsg )


			Result AddCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AddCharacterDataCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AddCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AddCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributes)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AddCharacterDataCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributes));

				return hr;
			}; // MessageData* AddCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )

			Result AddCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AddCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )


			Result AddCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AddCharacterDataCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AddCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, Attributes:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributes()); 
				return ResultCode::SUCCESS;
			}; // Result AddCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AddCharacterDataRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 0);
			Result AddCharacterDataRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result AddCharacterDataRes::ParseMessage( MessageData* pIMsg )


			Result AddCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AddCharacterDataRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AddCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AddCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AddCharacterDataRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AddCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AddCharacterDataRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AddCharacterDataRes::OverrideRouteContextDestination( EntityUID to )


			Result AddCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AddCharacterDataRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AddCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AddCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Remove character data
			const MessageID RemoveCharacterDataCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 1);
			Result RemoveCharacterDataCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));

				return hr;

			}; // Result RemoveCharacterDataCmd::ParseMessage( MessageData* pIMsg )


			Result RemoveCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RemoveCharacterDataCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RemoveCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RemoveCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::RemoveCharacterDataCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));

				return hr;
			}; // MessageData* RemoveCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )

			Result RemoveCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemoveCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )


			Result RemoveCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RemoveCharacterDataCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RemoveCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName()); 
				return ResultCode::SUCCESS;
			}; // Result RemoveCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RemoveCharacterDataRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 1);
			Result RemoveCharacterDataRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result RemoveCharacterDataRes::ParseMessage( MessageData* pIMsg )


			Result RemoveCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RemoveCharacterDataRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RemoveCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RemoveCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::RemoveCharacterDataRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* RemoveCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result RemoveCharacterDataRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemoveCharacterDataRes::OverrideRouteContextDestination( EntityUID to )


			Result RemoveCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RemoveCharacterDataRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RemoveCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RemoveCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Get character data
			const MessageID GetCharacterDataCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 2);
			Result GetCharacterDataCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));

				return hr;

			}; // Result GetCharacterDataCmd::ParseMessage( MessageData* pIMsg )


			Result GetCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetCharacterDataCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetCharacterDataCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::GetCharacterDataCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));

				return hr;
			}; // MessageData* GetCharacterDataCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName )

			Result GetCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetCharacterDataCmd::OverrideRouteContextDestination( EntityUID to )


			Result GetCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetCharacterDataCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName()); 
				return ResultCode::SUCCESS;
			}; // Result GetCharacterDataCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID GetCharacterDataRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 2);
			Result GetCharacterDataRes::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_Attributes));

				return hr;

			}; // Result GetCharacterDataRes::ParseMessage( MessageData* pIMsg )


			Result GetCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) GetCharacterDataRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result GetCharacterDataRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* GetCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributes)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::GetCharacterDataRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributes));

				return hr;
			}; // MessageData* GetCharacterDataRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )

			Result GetCharacterDataRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result GetCharacterDataRes::OverrideRouteContextDestination( EntityUID to )


			Result GetCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				GetCharacterDataRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "GetCharacterData:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}, CharacterName:{5}, Attributes:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult(), parser.GetCharacterName(), parser.GetAttributes()); 
				return ResultCode::SUCCESS;
			}; // Result GetCharacterDataRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Set(add or update) attribute value
			const MessageID SetAttributeCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 3);
			Result SetAttributeCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_Attributes));

				return hr;

			}; // Result SetAttributeCmd::ParseMessage( MessageData* pIMsg )


			Result SetAttributeCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) SetAttributeCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result SetAttributeCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* SetAttributeCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributes)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::SetAttributeCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributes));

				return hr;
			}; // MessageData* SetAttributeCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes )

			Result SetAttributeCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result SetAttributeCmd::OverrideRouteContextDestination( EntityUID to )


			Result SetAttributeCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				SetAttributeCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "SetAttribute:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, Attributes:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributes()); 
				return ResultCode::SUCCESS;
			}; // Result SetAttributeCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID SetAttributeRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 3);
			Result SetAttributeRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result SetAttributeRes::ParseMessage( MessageData* pIMsg )


			Result SetAttributeRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) SetAttributeRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result SetAttributeRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* SetAttributeRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::SetAttributeRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* SetAttributeRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result SetAttributeRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result SetAttributeRes::OverrideRouteContextDestination( EntityUID to )


			Result SetAttributeRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				SetAttributeRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "SetAttribute:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result SetAttributeRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Remove an attribute value
			const MessageID RemoveAttributesCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 4);
			Result RemoveAttributesCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(ArrayLen));
				StringCrc32* AttributeNamesPtr = nullptr;
				protocolCheck(input->ReadLink(AttributeNamesPtr, ArrayLen * sizeof(StringCrc32)));
				m_AttributeNames.SetLinkedBuffer(ArrayLen, AttributeNamesPtr);

				return hr;

			}; // Result RemoveAttributesCmd::ParseMessage( MessageData* pIMsg )


			Result RemoveAttributesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RemoveAttributesCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RemoveAttributesCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RemoveAttributesCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )
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

				uint16_t numberOfInAttributeNames = (uint16_t)InAttributeNames.size(); 
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributeNames)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::RemoveAttributesCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributeNames));

				return hr;
			}; // MessageData* RemoveAttributesCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames )

			Result RemoveAttributesCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemoveAttributesCmd::OverrideRouteContextDestination( EntityUID to )


			Result RemoveAttributesCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RemoveAttributesCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RemoveAttributes:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, AttributeNames:{6,30}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributeNames()); 
				return ResultCode::SUCCESS;
			}; // Result RemoveAttributesCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID RemoveAttributesRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 4);
			Result RemoveAttributesRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result RemoveAttributesRes::ParseMessage( MessageData* pIMsg )


			Result RemoveAttributesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) RemoveAttributesRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result RemoveAttributesRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* RemoveAttributesRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::RemoveAttributesRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* RemoveAttributesRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result RemoveAttributesRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result RemoveAttributesRes::OverrideRouteContextDestination( EntityUID to )


			Result RemoveAttributesRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				RemoveAttributesRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "RemoveAttributes:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result RemoveAttributesRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Attribute add
			const MessageID AttributeValueAddCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 5);
			Result AttributeValueAddCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_AttributeName));
				protocolCheck(input->Read(m_Value));

				return hr;

			}; // Result AttributeValueAddCmd::ParseMessage( MessageData* pIMsg )


			Result AttributeValueAddCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueAddCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueAddCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueAddCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributeName)
					, SerializedSizeOf(InValue)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueAddCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributeName));
				protocolCheck(output->Write(InValue));

				return hr;
			}; // MessageData* AttributeValueAddCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )

			Result AttributeValueAddCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueAddCmd::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueAddCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueAddCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueAdd:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, AttributeName:{6}, Value:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributeName(), parser.GetValue()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueAddCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AttributeValueAddRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 5);
			Result AttributeValueAddRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result AttributeValueAddRes::ParseMessage( MessageData* pIMsg )


			Result AttributeValueAddRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueAddRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueAddRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueAddRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueAddRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AttributeValueAddRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AttributeValueAddRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueAddRes::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueAddRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueAddRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueAdd:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueAddRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Attribute subtract
			const MessageID AttributeValueSubCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 6);
			Result AttributeValueSubCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_AttributeName));
				protocolCheck(input->Read(m_Value));

				return hr;

			}; // Result AttributeValueSubCmd::ParseMessage( MessageData* pIMsg )


			Result AttributeValueSubCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueSubCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueSubCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueSubCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributeName)
					, SerializedSizeOf(InValue)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueSubCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributeName));
				protocolCheck(output->Write(InValue));

				return hr;
			}; // MessageData* AttributeValueSubCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue )

			Result AttributeValueSubCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueSubCmd::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueSubCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueSubCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueSub:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, AttributeName:{6}, Value:{7}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributeName(), parser.GetValue()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueSubCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AttributeValueSubRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 6);
			Result AttributeValueSubRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result AttributeValueSubRes::ParseMessage( MessageData* pIMsg )


			Result AttributeValueSubRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueSubRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueSubRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueSubRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueSubRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AttributeValueSubRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AttributeValueSubRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueSubRes::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueSubRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueSubRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueSub:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueSubRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// Cmd: Compare and exchange attribute value
			const MessageID AttributeValueCASCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 7);
			Result AttributeValueCASCmd::ParseMessage( MessageData* pIMsg )
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
				protocolCheck(input->Read(m_CharacterName));
				protocolCheck(input->Read(m_AttributeName));
				protocolCheck(input->Read(m_AttributeType));
				protocolCheck(input->Read(m_Expected));
				protocolCheck(input->Read(m_NewValue));

				return hr;

			}; // Result AttributeValueCASCmd::ParseMessage( MessageData* pIMsg )


			Result AttributeValueCASCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueCASCmd(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueCASCmd::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueCASCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InPlayerID)
					, SerializedSizeOf(InCharacterName)
					, SerializedSizeOf(InAttributeName)
					, SerializedSizeOf(InAttributeType)
					, SerializedSizeOf(InExpected)
					, SerializedSizeOf(InNewValue)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueCASCmd::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InPlayerID));
				protocolCheck(output->Write(InCharacterName));
				protocolCheck(output->Write(InAttributeName));
				protocolCheck(output->Write(InAttributeType));
				protocolCheck(output->Write(InExpected));
				protocolCheck(output->Write(InNewValue));

				return hr;
			}; // MessageData* AttributeValueCASCmd::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue )

			Result AttributeValueCASCmd::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueCASCmd::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueCASCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueCASCmd parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueCAS:{0}:{1} , RouteContext:{2}, TransactionID:{3}, PlayerID:{4}, CharacterName:{5}, AttributeName:{6}, AttributeType:{7}, Expected:{8}, NewValue:{9}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetPlayerID(), parser.GetCharacterName(), parser.GetAttributeName(), parser.GetAttributeType(), parser.GetExpected(), parser.GetNewValue()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueCASCmd::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			const MessageID AttributeValueCASRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_CHARACTERDATASERVER, 7);
			Result AttributeValueCASRes::ParseMessage( MessageData* pIMsg )
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

			}; // Result AttributeValueCASRes::ParseMessage( MessageData* pIMsg )


			Result AttributeValueCASRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )
			{
 				FunctionContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) AttributeValueCASRes(std::forward<MessageDataPtr>(pIMsg)));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result AttributeValueCASRes::ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMessageBase )

			MessageData* AttributeValueCASRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )
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
					, SerializedSizeOf(InRouteContext)
					, SerializedSizeOf(InTransactionID)
					, SerializedSizeOf(InResult)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, CharacterDataServer::AttributeValueCASRes::MID, __uiMessageSize ) );
				size_t MsgDataSize = (int)((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(output->Write(InRouteContext));
				protocolCheck(output->Write(InTransactionID));
				protocolCheck(output->Write(InResult));

				return hr;
			}; // MessageData* AttributeValueCASRes::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult )

			Result AttributeValueCASRes::OverrideRouteContextDestination( EntityUID to )
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

			}; // Result AttributeValueCASRes::OverrideRouteContextDestination( EntityUID to )


			Result AttributeValueCASRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				AttributeValueCASRes parser;
				parser.ParseMessage(*pMsg);
				protocolTrace( Debug1, "AttributeValueCAS:{0}:{1} , RouteContext:{2}, TransactionID:{3}, Result:{4:X8}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetTransactionID(), parser.GetResult()); 
				return ResultCode::SUCCESS;
			}; // Result AttributeValueCASRes::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace CharacterDataServer
	}; // namespace Message
}; // namespace SF


