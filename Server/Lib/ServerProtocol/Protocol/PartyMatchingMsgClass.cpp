////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : Generated
// 
// Description : PartyMatching Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "Protocol/SFProtocol.h"
#include "Util/SFToString.h"
#include "Net/SFNetToString.h"
#include "Container/SFArray.h"
#include "Protocol/SFProtocolHelper.h"
#include "Protocol/PartyMatchingMsgClass.h"



namespace SF
{
 	namespace Message
	{
 		namespace PartyMatching
		{
 			// S2C: Event message will be sent from matching srvice when the pary game mathcing has done.
			const MessageID PartyGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHING, 0);
			Result PartyGameMatchedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);

				return hr;

			}; // Result PartyGameMatchedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PartyGameMatchedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PartyGameMatchedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PartyGameMatchedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PartyGameMatchedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InRouteHopCount)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatching::PartyGameMatchedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);

				return hr;
			}; // MessageData* PartyGameMatchedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )

			Result PartyGameMatchedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PartyGameMatchedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PartyGameMatched:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount()); 
				return ResultCode::SUCCESS;
			}; // Result PartyGameMatchedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)

			// S2C: Event message will be sent when a player game matching has successfully finished.
			const MessageID PlayerGameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_PARTYMATCHING, 1);
			Result PlayerGameMatchedS2CEvt::ParseMessage(const MessageData* pIMsg)
			{
 				ScopeContext hr;


				protocolCheckPtr(pIMsg);

				size_t MsgDataSize = ((size_t)pIMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<const uint8_t> bufferView(MsgDataSize, pIMsg->GetMessageData());
				InputMemoryStream inputStream(bufferView);
				auto* input = inputStream.ToInputStream();
				uint16_t ArrayLen = 0;(void)(ArrayLen);

				protocolCheck(*input >> m_RouteContext);
				protocolCheck(*input >> m_RouteHopCount);
				protocolCheck(*input >> m_DestPlayerID);
				protocolCheck(*input >> m_GameInsUID);
				protocolCheck(*input >> m_RequestedRole);

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::ParseMessage(const MessageData* pIMsg)


			Result PlayerGameMatchedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )
			{
 				ScopeContext hr;

				protocolCheckMem(pMessageBase = new(memHeap) PlayerGameMatchedS2CEvt(pIMsg));
				protocolCheck(pMessageBase->ParseMsg());

				return hr;

			}; // Result PlayerGameMatchedS2CEvt::ParseMessageToMessageBase( IHeap& memHeap, const MessageDataPtr& pIMsg, MessageBase* &pMessageBase )


			MessageData* PlayerGameMatchedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )
			{
 				MessageData *pNewMsg = nullptr;
				ScopeContext hr([&pNewMsg](Result hr) -> MessageData*
				{
 					if(!hr && pNewMsg != nullptr)
					{
 						IHeap::Delete(pNewMsg);
						return nullptr;
					}
					return pNewMsg;
				});

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ SerializedSizeOf(InRouteContext)
					+ SerializedSizeOf(InRouteHopCount)
					+ SerializedSizeOf(InDestPlayerID)
					+ SerializedSizeOf(InGameInsUID)
					+ SerializedSizeOf(InRequestedRole)
				);

				protocolCheckMem( pNewMsg = MessageData::NewMessage( memHeap, PartyMatching::PlayerGameMatchedS2CEvt::MID, __uiMessageSize ) );
				auto MsgDataSize = static_cast<uint>((size_t)pNewMsg->GetMessageSize() - sizeof(MessageHeader));
				ArrayView<uint8_t> BufferView(MsgDataSize, 0, pNewMsg->GetMessageData());
				OutputMemoryStream outputStream(BufferView);
				auto* output = outputStream.ToOutputStream();

				protocolCheck(*output << InRouteContext);
				protocolCheck(*output << InRouteHopCount);
				protocolCheck(*output << InDestPlayerID);
				protocolCheck(*output << InGameInsUID);
				protocolCheck(*output << InRequestedRole);

				return hr;
			}; // MessageData* PlayerGameMatchedS2CEvt::Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint16_t &InRouteHopCount, const PlayerID &InDestPlayerID, const uint64_t &InGameInsUID, const uint8_t &InRequestedRole )

			Result PlayerGameMatchedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)
			{
 				PlayerGameMatchedS2CEvt parser;
				parser.ParseMessage(*pMsg);
				SFLog(Net, Debug1, "PlayerGameMatched:{0}:{1} , RouteContext:{2}, RouteHopCount:{3}, DestPlayerID:{4}, GameInsUID:{5}, RequestedRole:{6}",
						prefix, pMsg->GetMessageHeader()->Length, parser.GetRouteContext(), parser.GetRouteHopCount(), parser.GetDestPlayerID(), parser.GetGameInsUID(), (int)parser.GetRequestedRole()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerGameMatchedS2CEvt::TraceOut(const char* prefix, const MessageDataPtr& pMsg)



		}; // namespace PartyMatching
	}; // namespace Message
}; // namespace SF


