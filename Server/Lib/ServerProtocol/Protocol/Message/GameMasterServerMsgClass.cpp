////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : Generated
// 
// Description : GameMasterServer Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "String/ToString.h"
#include "Net/NetToString.h"
#include "Container/SFArray.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/Message/GameMasterServerMsgClass.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace GameMasterServer
		{
 			// C2S: Player entered
			const MessageID PlayerEnteredC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEMASTERSERVER, 0);
			Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;
				uint16_t uiSizeOfPlayerName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerName, pCur, iMsgSize, sizeof(uint16_t) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerName, pCur, iMsgSize, sizeof(char)*uiSizeOfPlayerName ) );


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerEnteredC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PlayerEnteredC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("PlayerID", parser.GetPlayerID());
				variableBuilder.SetVariable("PlayerName", parser.GetPlayerName());


			Proc_End:

				return hr;

			}; // Result PlayerEnteredC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PlayerEnteredC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerEnteredC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerEnteredC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerEnteredC2SEvt::Create( IMemoryManager& memoryManager, const AccountID &InPlayerID, const char* InPlayerName )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				uint16_t __uiInPlayerNameLength = InPlayerName ? (uint16_t)(strlen(InPlayerName)+1) : 1;
				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) +  + sizeof(uint16_t) + __uiInPlayerNameLength 
					+ sizeof(AccountID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameMasterServer::PlayerEnteredC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &__uiInPlayerNameLength, sizeof(uint16_t) );
				Protocol::PackParamCopy( pMsgData, InPlayerName ? InPlayerName : "", __uiInPlayerNameLength );


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerEnteredC2SEvt::Create( IMemoryManager& memoryManager, const AccountID &InPlayerID, const char* InPlayerName )



			Result PlayerEnteredC2SEvt::TraceOut(MessageData* pMsg)
			{
 				PlayerEnteredC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PlayerEntered:{0}:{1} , PlayerID:{2}, PlayerName:{3,60}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetPlayerID(), parser.GetPlayerName()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerEnteredC2SEvt::TraceOut(MessageData* pMsg)

			// C2S: Player leaved
			const MessageID PlayerLeavedC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_NONE, PROTOCOLID_GAMEMASTERSERVER, 1);
			Result PlayerLeavedC2SEvt::ParseMessage( MessageData* pIMsg )
			{
 				Result hr;

				int iMsgSize;
				uint8_t* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (unsigned)pIMsg->GetMessageSize() - sizeof(MessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(AccountID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeavedC2SEvt::ParseMessage( MessageData* pIMsg )

			Result PlayerLeavedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )
			{
 				Result hr;


				PlayerLeavedC2SEvt parser;
				protocolChk(parser.ParseMessage(pIMsg));

				variableBuilder.SetVariable("PlayerID", parser.GetPlayerID());


			Proc_End:

				return hr;

			}; // Result PlayerLeavedC2SEvt::ParseMessageTo( MessageData* pIMsg, VariableMapBuilder& variableBuilder )

			Result PlayerLeavedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )
			{
 				Result hr;

				protocolMem(pMessageBase = new(memoryManager) PlayerLeavedC2SEvt(pIMsg));
				protocolChk(pMessageBase->ParseMsg());

			Proc_End:
				return hr;

			}; // Result PlayerLeavedC2SEvt::ParseMessageToMessageBase( IMemoryManager& memoryManager, MessageData* pIMsg, MessageBase* &pMessageBase )

			MessageData* PlayerLeavedC2SEvt::Create( IMemoryManager& memoryManager, const AccountID &InPlayerID )
			{
 				MessageData *pNewMsg = nullptr;
				Result hr;

				uint8_t *pMsgData = nullptr;

				unsigned __uiMessageSize = (unsigned)(sizeof(MessageHeader) 
					+ sizeof(AccountID));


				protocolMem( pNewMsg = MessageData::NewMessage( memoryManager, GameMasterServer::PlayerLeavedC2SEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(AccountID));


			Proc_End:

				if(!hr)
				{
 					if(pNewMsg != nullptr) GetSystemMemoryManager().Delete(pNewMsg);
					pNewMsg = nullptr;
				}
				return pNewMsg;

			}; // MessageData* PlayerLeavedC2SEvt::Create( IMemoryManager& memoryManager, const AccountID &InPlayerID )



			Result PlayerLeavedC2SEvt::TraceOut(MessageData* pMsg)
			{
 				PlayerLeavedC2SEvt parser;
				parser.ParseMessage(pMsg);
				protocolTrace( Debug1, "PlayerLeaved:{0}:{1} , PlayerID:{2}",
						pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, parser.GetPlayerID()); 
				return ResultCode::SUCCESS;
			}; // Result PlayerLeavedC2SEvt::TraceOut(MessageData* pMsg)



		}; // namespace GameMasterServer
	}; // namespace Message
}; // namespace SF


