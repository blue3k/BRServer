////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Game Message parser implementations
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
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/ProtocolTrace.h"



namespace BR
{
 	namespace Message
	{
 		namespace Game
		{
 			// C2S: Client heart bit
			const MessageID HeartBitC2SEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 0);
			Result HeartBitC2SEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result HeartBitC2SEvt::ParseIMsg( MessageData* pIMsg )

			Result HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::HeartBitC2SEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result HeartBitC2SEvt::BuildIMsg( OUT MessageData* &pMsg )



			void HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:HeartBitC2SEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void HeartBitC2SEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player connected from a login server and moved to game server
			const MessageID JoinGameServerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 1);
			Result JoinGameServerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_AccID, pCur, iMsgSize, sizeof(AccountID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LoginEntityUID, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameServerCmd::ParseIMsg( MessageData* pIMsg )

			Result JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(AccountID)
					+ sizeof(AuthTicket)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccID, sizeof(AccountID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InLoginEntityUID, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameServerCmd::BuildIMsg( OUT MessageData* &pMsg, const AccountID &InAccID, const AuthTicket &InTicket, const UINT64 &InLoginEntityUID )



			void JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameServerCmd:{1}:{2} , AccID:{3}, Ticket:{4}, LoginEntityUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_AccID, m_Ticket, m_LoginEntityUID); 
			}; // void JoinGameServerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameServerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 1);
			Result JoinGameServerRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfNickName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MatchingTicket, pCur, iMsgSize, sizeof(MatchingQueueTicket) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameServerRes::ParseIMsg( MessageData* pIMsg )

			Result JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInNickNameLength = InNickName ? (UINT16)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInNickNameLength 
					+ sizeof(Result)
					+ sizeof(GameInsUID)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(MatchingQueueTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameServerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InGameUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InMatchingTicket, sizeof(MatchingQueueTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameServerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InNickName, const GameInsUID &InGameUID, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const MatchingQueueTicket &InMatchingTicket )



			void JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameServerRes:{1}:{2} , Result:{3:X8}, NickName:{4}, GameUID:{5}, PartyUID:{6}, PartyLeaderID:{7}, MatchingTicket:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NickName, m_GameUID, m_PartyUID, m_PartyLeaderID, m_MatchingTicket); 
			}; // void JoinGameServerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: player complition statues
			const MessageID GetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 2);
			Result GetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )

			Result GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetComplitionStateCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 2);
			Result GetComplitionStateRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfComplitionState = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // Result GetComplitionStateRes::ParseIMsg( MessageData* pIMsg )

			Result GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InComplitionState )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInComplitionStateLength = InComplitionState ? (UINT16)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInComplitionStateLength 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const char* InComplitionState )



			void GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetComplitionStateRes:{1}:{2} , Result:{3:X8}, ComplitionState:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ComplitionState); 
			}; // void GetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player complition state
			const MessageID SetComplitionStateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 3);
			Result SetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfComplitionState = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfComplitionState, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ComplitionState, pCur, iMsgSize, sizeof(char)*uiSizeOfComplitionState ) );


			Proc_End:

				return hr;

			}; // Result SetComplitionStateCmd::ParseIMsg( MessageData* pIMsg )

			Result SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInComplitionStateLength = InComplitionState ? (UINT16)(strlen(InComplitionState)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInComplitionStateLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInComplitionStateLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InComplitionState ? InComplitionState : "", __uiInComplitionStateLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetComplitionStateCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InComplitionState )



			void SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetComplitionStateCmd:{1}:{2} , ComplitionState:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ComplitionState); 
			}; // void SetComplitionStateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetComplitionStateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 3);
			Result SetComplitionStateRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result SetComplitionStateRes::ParseIMsg( MessageData* pIMsg )

			Result SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetComplitionStateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetComplitionStateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetComplitionStateRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void SetComplitionStateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Register Google notification service ID, after this, the player will get notification from google. Only one notification ID can be active at a time
			const MessageID RegisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 4);
			Result RegisterGCMCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfGCMRegisteredID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // Result RegisterGCMCmd::ParseIMsg( MessageData* pIMsg )

			Result RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (UINT16)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			void RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterGCMCmd:{1}:{2} , GCMRegisteredID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // void RegisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RegisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 4);
			Result RegisterGCMRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RegisterGCMRes::ParseIMsg( MessageData* pIMsg )

			Result RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RegisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RegisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RegisterGCMRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void RegisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Unregister Google notification service ID
			const MessageID UnregisterGCMCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 5);
			Result UnregisterGCMCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfGCMRegisteredID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfGCMRegisteredID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_GCMRegisteredID, pCur, iMsgSize, sizeof(char)*uiSizeOfGCMRegisteredID ) );


			Proc_End:

				return hr;

			}; // Result UnregisterGCMCmd::ParseIMsg( MessageData* pIMsg )

			Result UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInGCMRegisteredIDLength = InGCMRegisteredID ? (UINT16)(strlen(InGCMRegisteredID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInGCMRegisteredIDLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInGCMRegisteredIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InGCMRegisteredID ? InGCMRegisteredID : "", __uiInGCMRegisteredIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterGCMCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InGCMRegisteredID )



			void UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterGCMCmd:{1}:{2} , GCMRegisteredID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GCMRegisteredID); 
			}; // void UnregisterGCMCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID UnregisterGCMRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 5);
			Result UnregisterGCMRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result UnregisterGCMRes::ParseIMsg( MessageData* pIMsg )

			Result UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::UnregisterGCMRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result UnregisterGCMRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:UnregisterGCMRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void UnregisterGCMRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite friend
			const MessageID InviteFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 6);
			Result InviteFriendCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result InviteFriendCmd::ParseIMsg( MessageData* pIMsg )

			Result InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result InviteFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:InviteFriendCmd:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void InviteFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID InviteFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 6);
			Result InviteFriendRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result InviteFriendRes::ParseIMsg( MessageData* pIMsg )

			Result InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::InviteFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result InviteFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:InviteFriendRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void InviteFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept friend request
			const MessageID AcceptFriendRequestCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 7);
			Result AcceptFriendRequestCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterFacebookUID, pCur, iMsgSize, sizeof(FacebookUID) ) );


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestCmd::ParseIMsg( MessageData* pIMsg )

			Result AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(FacebookUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InInviterFacebookUID, sizeof(FacebookUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const FacebookUID &InInviterFacebookUID )



			void AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptFriendRequestCmd:{1}:{2} , InviterID:{3}, InviterFacebookUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterFacebookUID); 
			}; // void AcceptFriendRequestCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptFriendRequestRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 7);
			Result AcceptFriendRequestRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewFriend, pCur, iMsgSize, sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestRes::ParseIMsg( MessageData* pIMsg )

			Result AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const FriendInformation &InNewFriend )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptFriendRequestRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNewFriend, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptFriendRequestRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const FriendInformation &InNewFriend )



			void AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptFriendRequestRes:{1}:{2} , Result:{3:X8}, NewFriend:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NewFriend); 
			}; // void AcceptFriendRequestRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notification for friend request is accepted
			const MessageID FriendRequestAcceptedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 8);
			Result FriendRequestAcceptedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Accepter, pCur, iMsgSize, sizeof(FriendInformation) ) );


			Proc_End:

				return hr;

			}; // Result FriendRequestAcceptedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(FriendInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRequestAcceptedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InAccepter, sizeof(FriendInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendRequestAcceptedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const FriendInformation &InAccepter )



			void FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendRequestAcceptedS2CEvt:{1}:{2} , Accepter:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Accepter); 
			}; // void FriendRequestAcceptedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Remove friden form the friend list
			const MessageID RemoveFriendCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 9);
			Result RemoveFriendCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemoveFriendCmd::ParseIMsg( MessageData* pIMsg )

			Result RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemoveFriendCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemoveFriendCmd:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void RemoveFriendCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RemoveFriendRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 9);
			Result RemoveFriendRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result RemoveFriendRes::ParseIMsg( MessageData* pIMsg )

			Result RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InFriendID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RemoveFriendRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RemoveFriendRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InFriendID )



			void RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RemoveFriendRes:{1}:{2} , Result:{3:X8}, FriendID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_FriendID); 
			}; // void RemoveFriendRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Friend removed
			const MessageID FriendRemovedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 10);
			Result FriendRemovedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FriendID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FriendRemovedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FriendRemovedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFriendID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FriendRemovedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InFriendID )



			void FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FriendRemovedS2CEvt:{1}:{2} , FriendID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FriendID); 
			}; // void FriendRemovedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get friend list
			const MessageID GetFriendListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 11);
			Result GetFriendListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(UINT16) ) );


			Proc_End:

				return hr;

			}; // Result GetFriendListCmd::ParseIMsg( MessageData* pIMsg )

			Result GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT16 &InStartIndex, const UINT16 &InCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT16)
					+ sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(UINT16));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetFriendListCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT16 &InStartIndex, const UINT16 &InCount )



			void GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetFriendListCmd:{1}:{2} , StartIndex:{3}, Count:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_StartIndex, m_Count); 
			}; // void GetFriendListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetFriendListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 11);
			Result GetFriendListRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofFriendList = 0; FriendInformation* pFriendList = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxFriendSlot, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalNumberOfFriends, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_StartIndex, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofFriendList, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pFriendList, pCur, iMsgSize, sizeof(FriendInformation)*numberofFriendList ) );
				m_FriendList.SetLinkedBuffer(numberofFriendList, numberofFriendList, pFriendList);


			Proc_End:

				return hr;

			}; // Result GetFriendListRes::ParseIMsg( MessageData* pIMsg )

			Result GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(UINT16)
					+ sizeof(FriendInformation)*InFriendList.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInFriendList = (UINT16)InFriendList.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetFriendListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InMaxFriendSlot, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InTotalNumberOfFriends, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &InStartIndex, sizeof(UINT16));
				Protocol::PackParamCopy( pMsgData, &numberOfInFriendList, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InFriendList.data(), (INT)(sizeof(FriendInformation)*InFriendList.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetFriendListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT16 &InMaxFriendSlot, const UINT16 &InTotalNumberOfFriends, const UINT16 &InStartIndex, const Array<FriendInformation>& InFriendList )



			void GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetFriendListRes:{1}:{2} , Result:{3:X8}, MaxFriendSlot:{4}, TotalNumberOfFriends:{5}, StartIndex:{6}, FriendList:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_MaxFriendSlot, m_TotalNumberOfFriends, m_StartIndex, m_FriendList); 
			}; // void GetFriendListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query notification list
			const MessageID GetNotificationListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 12);
			Result GetNotificationListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetNotificationListCmd::ParseIMsg( MessageData* pIMsg )

			Result GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetNotificationListCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetNotificationListCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetNotificationListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetNotificationListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 12);
			Result GetNotificationListRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GetNotificationListRes::ParseIMsg( MessageData* pIMsg )

			Result GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetNotificationListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetNotificationListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetNotificationListRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void GetNotificationListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Delete notification
			const MessageID DeleteNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 13);
			Result DeleteNotificationCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result DeleteNotificationCmd::ParseIMsg( MessageData* pIMsg )

			Result DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DeleteNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			void DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DeleteNotificationCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void DeleteNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID DeleteNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 13);
			Result DeleteNotificationRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result DeleteNotificationRes::ParseIMsg( MessageData* pIMsg )

			Result DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::DeleteNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result DeleteNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )



			void DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:DeleteNotificationRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void DeleteNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Set notification is read
			const MessageID SetNotificationReadCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 14);
			Result SetNotificationReadCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result SetNotificationReadCmd::ParseIMsg( MessageData* pIMsg )

			Result SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNotificationReadCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			void SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNotificationReadCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void SetNotificationReadCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNotificationReadRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 14);
			Result SetNotificationReadRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result SetNotificationReadRes::ParseIMsg( MessageData* pIMsg )

			Result SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNotificationReadRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNotificationReadRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )



			void SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNotificationReadRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void SetNotificationReadRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Accept notification
			const MessageID AcceptNotificationCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 15);
			Result AcceptNotificationCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result AcceptNotificationCmd::ParseIMsg( MessageData* pIMsg )

			Result AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptNotificationCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID )



			void AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptNotificationCmd:{1}:{2} , NotificationID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID); 
			}; // void AcceptNotificationCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AcceptNotificationRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 15);
			Result AcceptNotificationRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result AcceptNotificationRes::ParseIMsg( MessageData* pIMsg )

			Result AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AcceptNotificationRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AcceptNotificationRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InNotificationID )



			void AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AcceptNotificationRes:{1}:{2} , Result:{3:X8}, NotificationID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_NotificationID); 
			}; // void AcceptNotificationRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Notify new notification
			const MessageID NotifyS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 16);
			Result NotifyS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfMessageText = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NotificationID, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageID, pCur, iMsgSize, sizeof(NotificationType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam0, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MessageParam1, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfMessageText, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_MessageText, pCur, iMsgSize, sizeof(char)*uiSizeOfMessageText ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsRead, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result NotifyS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInMessageTextLength = InMessageText ? (UINT16)(strlen(InMessageText)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInMessageTextLength 
					+ sizeof(UINT32)
					+ sizeof(NotificationType)
					+ sizeof(UINT64)
					+ sizeof(UINT64)
					+ sizeof(BYTE)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNotificationID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InMessageID, sizeof(NotificationType));
				Protocol::PackParamCopy( pMsgData, &InMessageParam0, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InMessageParam1, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &__uiInMessageTextLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InMessageText ? InMessageText : "", __uiInMessageTextLength );
				Protocol::PackParamCopy( pMsgData, &InIsRead, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InNotificationID, const NotificationType &InMessageID, const UINT64 &InMessageParam0, const UINT64 &InMessageParam1, const char* InMessageText, const BYTE &InIsRead, const UINT64 &InTimeStamp )



			void NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyS2CEvt:{1}:{2} , NotificationID:{3}, MessageID:{4}, MessageParam0:{5}, MessageParam1:{6}, MessageText:{7}, IsRead:{8}, TimeStamp:{9}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NotificationID, m_MessageID, m_MessageParam0, m_MessageParam1, m_MessageText, m_IsRead, m_TimeStamp); 
			}; // void NotifyS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByEMailCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 17);
			Result FindPlayerByEMailCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfPlayerEMail = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlayerEMail, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PlayerEMail, pCur, iMsgSize, sizeof(char)*uiSizeOfPlayerEMail ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailCmd::ParseIMsg( MessageData* pIMsg )

			Result FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPlayerEMailLength = InPlayerEMail ? (UINT16)(strlen(InPlayerEMail)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPlayerEMailLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInPlayerEMailLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPlayerEMail ? InPlayerEMail : "", __uiInPlayerEMailLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InPlayerEMail )



			void FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByEMailCmd:{1}:{2} , PlayerEMail:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerEMail); 
			}; // void FindPlayerByEMailCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByEMailRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 17);
			Result FindPlayerByEMailRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailRes::ParseIMsg( MessageData* pIMsg )

			Result FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByEMailRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByEMailRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )



			void FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByEMailRes:{1}:{2} , Result:{3:X8}, Player:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Player); 
			}; // void FindPlayerByEMailRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Query playerID list
			const MessageID FindPlayerByPlayerIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 18);
			Result FindPlayerByPlayerIDCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDCmd::ParseIMsg( MessageData* pIMsg )

			Result FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			void FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByPlayerIDCmd:{1}:{2} , PlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // void FindPlayerByPlayerIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID FindPlayerByPlayerIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 18);
			Result FindPlayerByPlayerIDRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Player, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDRes::ParseIMsg( MessageData* pIMsg )

			Result FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::FindPlayerByPlayerIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result FindPlayerByPlayerIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerInformation &InPlayer )



			void FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:FindPlayerByPlayerIDRes:{1}:{2} , Result:{3:X8}, Player:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Player); 
			}; // void FindPlayerByPlayerIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: *Request Player Status Update
			const MessageID RequestPlayerStatusUpdateCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 19);
			Result RequestPlayerStatusUpdateCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateCmd::ParseIMsg( MessageData* pIMsg )

			Result RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInTargetPlayerID = (UINT16)InTargetPlayerID.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			void RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestPlayerStatusUpdateCmd:{1}:{2} , TargetPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayerID); 
			}; // void RequestPlayerStatusUpdateCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestPlayerStatusUpdateRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 19);
			Result RequestPlayerStatusUpdateRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateRes::ParseIMsg( MessageData* pIMsg )

			Result RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestPlayerStatusUpdateRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestPlayerStatusUpdateRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestPlayerStatusUpdateRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void RequestPlayerStatusUpdateRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: *Notify Player Status Updated
			const MessageID NotifyPlayerStatusUpdatedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 20);
			Result NotifyPlayerStatusUpdatedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LatestActiveTime, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsInGame, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(UINT32)
					+ sizeof(BYTE));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::NotifyPlayerStatusUpdatedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLatestActiveTime, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InIsInGame, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result NotifyPlayerStatusUpdatedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const UINT32 &InLatestActiveTime, const BYTE &InIsInGame )



			void NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:NotifyPlayerStatusUpdatedS2CEvt:{1}:{2} , PlayerID:{3}, LatestActiveTime:{4}, IsInGame:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_LatestActiveTime, m_IsInGame); 
			}; // void NotifyPlayerStatusUpdatedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Get Ranking lise
			const MessageID GetRankingListCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 21);
			Result GetRankingListCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RankingType, pCur, iMsgSize, sizeof(RankingType) ) );
				protocolChk( Protocol::StreamParamCopy( &m_BaseRanking, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Count, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // Result GetRankingListCmd::ParseIMsg( MessageData* pIMsg )

			Result GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(RankingType)
					+ sizeof(BYTE)
					+ sizeof(BYTE));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRankingType, sizeof(RankingType));
				Protocol::PackParamCopy( pMsgData, &InBaseRanking, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InCount, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingListCmd::BuildIMsg( OUT MessageData* &pMsg, const RankingType &InRankingType, const BYTE &InBaseRanking, const BYTE &InCount )



			void GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingListCmd:{1}:{2} , RankingType:{3}, BaseRanking:{4}, Count:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RankingType, m_BaseRanking, m_Count); 
			}; // void GetRankingListCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetRankingListRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 21);
			Result GetRankingListRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofRanking = 0; TotalRankingPlayerInformation* pRanking = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRanking, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRanking, pCur, iMsgSize, sizeof(TotalRankingPlayerInformation)*numberofRanking ) );
				m_Ranking.SetLinkedBuffer(numberofRanking, numberofRanking, pRanking);


			Proc_End:

				return hr;

			}; // Result GetRankingListRes::ParseIMsg( MessageData* pIMsg )

			Result GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(TotalRankingPlayerInformation)*InRanking.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInRanking = (UINT16)InRanking.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetRankingListRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRanking, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRanking.data(), (INT)(sizeof(TotalRankingPlayerInformation)*InRanking.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetRankingListRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking )



			void GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetRankingListRes:{1}:{2} , Result:{3:X8}, Ranking:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Ranking); 
			}; // void GetRankingListRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Game user game play information
			const MessageID GetUserGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 22);
			Result GetUserGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )

			Result GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetUserGamePlayerInfoCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GetUserGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetUserGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 22);
			Result GetUserGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Exp, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameMoney, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Gem, pCur, iMsgSize, sizeof(INT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LastUpdateTime, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )

			Result GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(INT16)
					+ sizeof(INT64)
					+ sizeof(INT64)
					+ sizeof(INT64)
					+ sizeof(INT16)
					+ sizeof(UINT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetUserGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InExp, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InGameMoney, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InGem, sizeof(INT64));
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InLastUpdateTime, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetUserGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const INT16 &InLevel, const INT64 &InExp, const INT64 &InGameMoney, const INT64 &InGem, const INT16 &InStamina, const UINT32 &InLastUpdateTime, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )



			void GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetUserGamePlayerInfoRes:{1}:{2} , Result:{3:X8}, Level:{4}, Exp:{5}, GameMoney:{6}, Gem:{7}, Stamina:{8}, LastUpdateTime:{9}, TotalPlayed:{10}, WinPlaySC:{11}, WinPlaySM:{12}, WinPlaySS:{13}, LosePlaySC:{14}, LosePlaySM:{15}, LosePlaySS:{16}, WinPlayNC:{17}, WinPlayNM:{18}, WinPlayNS:{19}, LosePlayNC:{20}, LosePlayNM:{21}, LosePlayNS:{22}, WeeklyWin:{23}, WeeklyLose:{24}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_Level, m_Exp, m_GameMoney, m_Gem, m_Stamina, m_LastUpdateTime, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // void GetUserGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Game game play information
			const MessageID GetGamePlayerInfoCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 23);
			Result GetGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoCmd::ParseIMsg( MessageData* pIMsg )

			Result GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID )



			void GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetGamePlayerInfoCmd:{1}:{2} , PlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID); 
			}; // void GetGamePlayerInfoCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GetGamePlayerInfoRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 23);
			Result GetGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Level, pCur, iMsgSize, sizeof(INT16) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalPlayed, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlaySS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WinPlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNC, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNM, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LosePlayNS, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyWin, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_WeeklyLose, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoRes::ParseIMsg( MessageData* pIMsg )

			Result GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)
					+ sizeof(INT16)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GetGamePlayerInfoRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InLevel, sizeof(INT16));
				Protocol::PackParamCopy( pMsgData, &InTotalPlayed, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlaySS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWinPlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNC, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNM, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InLosePlayNS, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyWin, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InWeeklyLose, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GetGamePlayerInfoRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InPlayerID, const INT16 &InLevel, const INT32 &InTotalPlayed, const INT32 &InWinPlaySC, const INT32 &InWinPlaySM, const INT32 &InWinPlaySS, const INT32 &InLosePlaySC, const INT32 &InLosePlaySM, const INT32 &InLosePlaySS, const INT32 &InWinPlayNC, const INT32 &InWinPlayNM, const INT32 &InWinPlayNS, const INT32 &InLosePlayNC, const INT32 &InLosePlayNM, const INT32 &InLosePlayNS, const INT32 &InWeeklyWin, const INT32 &InWeeklyLose )



			void GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GetGamePlayerInfoRes:{1}:{2} , Result:{3:X8}, PlayerID:{4}, Level:{5}, TotalPlayed:{6}, WinPlaySC:{7}, WinPlaySM:{8}, WinPlaySS:{9}, LosePlaySC:{10}, LosePlaySM:{11}, LosePlaySS:{12}, WinPlayNC:{13}, WinPlayNM:{14}, WinPlayNS:{15}, LosePlayNC:{16}, LosePlayNM:{17}, LosePlayNS:{18}, WeeklyWin:{19}, WeeklyLose:{20}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PlayerID, m_Level, m_TotalPlayed, m_WinPlaySC, m_WinPlaySM, m_WinPlaySS, m_LosePlaySC, m_LosePlaySM, m_LosePlaySS, m_WinPlayNC, m_WinPlayNM, m_WinPlayNS, m_LosePlayNC, m_LosePlayNM, m_LosePlayNS, m_WeeklyWin, m_WeeklyLose); 
			}; // void GetGamePlayerInfoRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player level up event
			const MessageID LevelUpS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 24);
			Result LevelUpS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_CurrentTotalExp, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_CurrentLevel, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result LevelUpS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT64)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LevelUpS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InCurrentTotalExp, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InCurrentLevel, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LevelUpS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const UINT64 &InCurrentTotalExp, const UINT32 &InCurrentLevel )



			void LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LevelUpS2CEvt:{1}:{2} , CurrentTotalExp:{3}, CurrentLevel:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_CurrentTotalExp, m_CurrentLevel); 
			}; // void LevelUpS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Change NickName
			const MessageID SetNickNameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 25);
			Result SetNickNameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfNickName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfNickName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_NickName, pCur, iMsgSize, sizeof(char)*uiSizeOfNickName ) );
				protocolChk( Protocol::StreamParamCopy( &m_IsCostFree, pCur, iMsgSize, sizeof(BYTE) ) );


			Proc_End:

				return hr;

			}; // Result SetNickNameCmd::ParseIMsg( MessageData* pIMsg )

			Result SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const BYTE &InIsCostFree )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInNickNameLength = InNickName ? (UINT16)(strlen(InNickName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInNickNameLength 
					+ sizeof(BYTE));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInNickNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InNickName ? InNickName : "", __uiInNickNameLength );
				Protocol::PackParamCopy( pMsgData, &InIsCostFree, sizeof(BYTE));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNickNameCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InNickName, const BYTE &InIsCostFree )



			void SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNickNameCmd:{1}:{2} , NickName:{3}, IsCostFree:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NickName, m_IsCostFree); 
			}; // void SetNickNameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetNickNameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 25);
			Result SetNickNameRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result SetNickNameRes::ParseIMsg( MessageData* pIMsg )

			Result SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetNickNameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetNickNameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetNickNameRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void SetNickNameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Create Party
			const MessageID CreatePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 26);
			Result CreatePartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::ParseIMsg( MessageData* pIMsg )

			Result CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreatePartyCmd::BuildIMsg( OUT MessageData* &pMsg )



			void CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreatePartyCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void CreatePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CreatePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 26);
			Result CreatePartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::ParseIMsg( MessageData* pIMsg )

			Result CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CreatePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CreatePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID )



			void CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CreatePartyRes:{1}:{2} , Result:{3:X8}, PartyUID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PartyUID); 
			}; // void CreatePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join party
			const MessageID JoinPartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 27);
			Result JoinPartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result JoinPartyCmd::ParseIMsg( MessageData* pIMsg )

			Result JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinPartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InInviterID )



			void JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinPartyCmd:{1}:{2} , PartyUID:{3}, InviterID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_InviterID); 
			}; // void JoinPartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinPartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 27);
			Result JoinPartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);


			Proc_End:

				return hr;

			}; // Result JoinPartyRes::ParseIMsg( MessageData* pIMsg )

			Result JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinPartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPartyLeaderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinPartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PartyUID &InPartyUID, const PlayerID &InPartyLeaderID, const Array<BYTE>& InChatHistoryData )



			void JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinPartyRes:{1}:{2} , Result:{3:X8}, PartyUID:{4}, PartyLeaderID:{5}, ChatHistoryData:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_PartyUID, m_PartyLeaderID, m_ChatHistoryData); 
			}; // void JoinPartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined event
			const MessageID PartyPlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 28);
			Result PartyPlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerInformation));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerInformation &InJoinedPlayer )



			void PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerJoinedS2CEvt:{1}:{2} , PartyUID:{3}, JoinedPlayer:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_JoinedPlayer); 
			}; // void PartyPlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party leader changed event
			const MessageID PartyLeaderChangedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 29);
			Result PartyLeaderChangedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_NewLeaderID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyLeaderChangedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InNewLeaderID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyLeaderChangedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InNewLeaderID )



			void PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyLeaderChangedS2CEvt:{1}:{2} , PartyUID:{3}, NewLeaderID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_NewLeaderID); 
			}; // void PartyLeaderChangedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave party command
			const MessageID LeavePartyCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 30);
			Result LeavePartyCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyCmd::ParseIMsg( MessageData* pIMsg )

			Result LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeavePartyCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID )



			void LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeavePartyCmd:{1}:{2} , PartyUID:{3}, PlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID); 
			}; // void LeavePartyCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeavePartyRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 30);
			Result LeavePartyRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeavePartyRes::ParseIMsg( MessageData* pIMsg )

			Result LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeavePartyRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeavePartyRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeavePartyRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void LeavePartyRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player left event
			const MessageID PartyPlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 31);
			Result PartyPlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeftPlayerID )



			void PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerLeftS2CEvt:{1}:{2} , PartyUID:{3}, LeftPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeftPlayerID); 
			}; // void PartyPlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player from the party
			const MessageID PartyKickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 32);
			Result PartyKickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			Result PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			void PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyKickPlayerCmd:{1}:{2} , PartyUID:{3}, PlayerID:{4}, PlayerToKick:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_PlayerID, m_PlayerToKick); 
			}; // void PartyKickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyKickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 32);
			Result PartyKickPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerRes::ParseIMsg( MessageData* pIMsg )

			Result PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyKickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyKickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyKickPlayerRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyKickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Player kicked message
			const MessageID PartyPlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 33);
			Result PartyPlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyPlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyPlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyPlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InKickedPlayerID )



			void PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyPlayerKickedS2CEvt:{1}:{2} , PartyUID:{3}, KickedPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_KickedPlayerID); 
			}; // void PartyPlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Invite a player to the party
			const MessageID PartyInviteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 34);
			Result PartyInviteCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviteTargetID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteCmd::ParseIMsg( MessageData* pIMsg )

			Result PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviteTargetID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviteTargetID )



			void PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteCmd:{1}:{2} , InviteTargetID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviteTargetID); 
			}; // void PartyInviteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyInviteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 34);
			Result PartyInviteRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteRes::ParseIMsg( MessageData* pIMsg )

			Result PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyInviteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party invite requested
			const MessageID PartyInviteRequestedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 35);
			Result PartyInviteRequestedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfInviterName = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InviterID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfInviterName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_InviterName, pCur, iMsgSize, sizeof(char)*uiSizeOfInviterName ) );
				protocolChk( Protocol::StreamParamCopy( &m_PartyToJoinUID, pCur, iMsgSize, sizeof(PartyUID) ) );


			Proc_End:

				return hr;

			}; // Result PartyInviteRequestedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInInviterNameLength = InInviterName ? (UINT16)(strlen(InInviterName)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInInviterNameLength 
					+ sizeof(PlayerID)
					+ sizeof(PartyUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyInviteRequestedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInviterID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInInviterNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InInviterName ? InInviterName : "", __uiInInviterNameLength );
				Protocol::PackParamCopy( pMsgData, &InPartyToJoinUID, sizeof(PartyUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyInviteRequestedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InInviterID, const char* InInviterName, const PartyUID &InPartyToJoinUID )



			void PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyInviteRequestedS2CEvt:{1}:{2} , InviterID:{3}, InviterName:{4}, PartyToJoinUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InviterID, m_InviterName, m_PartyToJoinUID); 
			}; // void PartyInviteRequestedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send Party quick chat message
			const MessageID PartyQuickChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 36);
			Result PartyQuickChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			Result PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InQuickChatID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InQuickChatID )



			void PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageCmd:{1}:{2} , QuickChatID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_QuickChatID); 
			}; // void PartyQuickChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyQuickChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 36);
			Result PartyQuickChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageRes::ParseIMsg( MessageData* pIMsg )

			Result PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyQuickChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyQuickChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 37);
			Result PartyQuickChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_QuickChatID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const UINT32 &InQuickChatID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyQuickChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InQuickChatID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyQuickChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const UINT32 &InQuickChatID )



			void PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyQuickChatMessageS2CEvt:{1}:{2} , SenderID:{3}, QuickChatID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_QuickChatID); 
			}; // void PartyQuickChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Party chatting
			const MessageID PartyChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 38);
			Result PartyChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			Result PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInChatMessageLength );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage )



			void PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageCmd:{1}:{2} , ChatMessage:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage); 
			}; // void PartyChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID PartyChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 38);
			Result PartyChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageRes::ParseIMsg( MessageData* pIMsg )

			Result PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void PartyChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Party Chatting message event
			const MessageID PartyChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 39);
			Result PartyChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfSenderName = 0;
				UINT16 uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result PartyChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInSenderNameLength = InSenderName ? (UINT16)(strlen(InSenderName)+1) : 1;
				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInSenderNameLength + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PartyChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PartyChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const char* InSenderName, const char* InChatMessage )



			void PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PartyChatMessageS2CEvt:{1}:{2} , SenderID:{3}, SenderName:{4}, ChatMessage:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_SenderName, m_ChatMessage); 
			}; // void PartyChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Join to a game
			const MessageID JoinGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 40);
			Result JoinGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::ParseIMsg( MessageData* pIMsg )

			Result JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(GameInsUID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InPlayerID, const AuthTicket &InTicket, const GameInsUID &InInsUID )



			void JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameCmd:{1}:{2} , PlayerID:{3}, Ticket:{4}, InsUID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PlayerID, m_Ticket, m_InsUID); 
			}; // void JoinGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID JoinGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 40);
			Result JoinGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				UINT16 numberofGameLogData = 0; BYTE* pGameLogData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(BYTE)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);


			Proc_End:

				return hr;

			}; // Result JoinGameRes::ParseIMsg( MessageData* pIMsg )

			Result JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16)
					+ sizeof(BYTE)*InGameLogData.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				UINT16 numberOfInGameLogData = (UINT16)InGameLogData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::JoinGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(BYTE)*InGameLogData.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result JoinGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData )



			void JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:JoinGameRes:{1}:{2} , Result:{3:X8}, InsUID:{4}, TimeStamp:{5}, GameState:{6}, Day:{7}, MaxPlayer:{8}, PlayerIndex:{9}, PlayerCharacter:{10}, Role:{11}, Dead:{12}, ChatHistoryData:{13}, GameLogData:{14}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData); 
			}; // void JoinGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Joined in the game
			const MessageID PlayerJoinedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 41);
			Result PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayer, pCur, iMsgSize, sizeof(PlayerInformation) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerRole, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerDead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_JoinedPlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerInformation)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerJoinedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayer, sizeof(PlayerInformation));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InJoinedPlayerCharacter, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerJoinedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerInformation &InJoinedPlayer, const PlayerRole &InJoinedPlayerRole, const UINT8 &InJoinedPlayerDead, const UINT8 &InJoinedPlayerIndex, const UINT8 &InJoinedPlayerCharacter )



			void PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerJoinedS2CEvt:{1}:{2} , GameInsUID:{3}, JoinedPlayer:{4}, JoinedPlayerRole:{5}, JoinedPlayerDead:{6}, JoinedPlayerIndex:{7}, JoinedPlayerCharacter:{8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_JoinedPlayer, m_JoinedPlayerRole, m_JoinedPlayerDead, m_JoinedPlayerIndex, m_JoinedPlayerCharacter); 
			}; // void PlayerJoinedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Leave Game
			const MessageID LeaveGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 42);
			Result LeaveGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::ParseIMsg( MessageData* pIMsg )

			Result LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeaveGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeaveGameCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void LeaveGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID LeaveGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 42);
			Result LeaveGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::ParseIMsg( MessageData* pIMsg )

			Result LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::LeaveGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result LeaveGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:LeaveGameRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void LeaveGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player left event
			const MessageID PlayerLeftS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 43);
			Result PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeftPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerLeftS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InLeftPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerLeftS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InLeftPlayerID )



			void PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerLeftS2CEvt:{1}:{2} , GameInsUID:{3}, LeftPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_LeftPlayerID); 
			}; // void PlayerLeftS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Kick player
			const MessageID KickPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 44);
			Result KickPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerToKick, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::ParseIMsg( MessageData* pIMsg )

			Result KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InPlayerToKick, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result KickPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerID &InPlayerToKick )



			void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, PlayerToKick:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_PlayerToKick); 
			}; // void KickPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID KickPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 44);
			Result KickPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::ParseIMsg( MessageData* pIMsg )

			Result KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::KickPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result KickPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:KickPlayerRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void KickPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player kicked
			const MessageID PlayerKickedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 45);
			Result PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KickedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKickedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKickedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerKickedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKickedPlayerID )



			void PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerKickedS2CEvt:{1}:{2} , GameInsUID:{3}, KickedPlayerID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KickedPlayerID); 
			}; // void PlayerKickedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Assign role + Game state reset
			const MessageID AssignRoleCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 46);
			Result AssignRoleCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::ParseIMsg( MessageData* pIMsg )

			Result AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AssignRoleCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AssignRoleCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void AssignRoleCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AssignRoleRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 46);
			Result AssignRoleRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::ParseIMsg( MessageData* pIMsg )

			Result AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AssignRoleRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AssignRoleRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AssignRoleRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void AssignRoleRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Role assigned event
			const MessageID RoleAssignedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 47);
			Result RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RoleAssignedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RoleAssignedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const PlayerRole &InRole )



			void RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RoleAssignedS2CEvt:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Role:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Role); 
			}; // void RoleAssignedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Send chatting message to the game
			const MessageID ChatMessageCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 48);
			Result ChatMessageCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageCmd::ParseIMsg( MessageData* pIMsg )

			Result ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageCmd::BuildIMsg( OUT MessageData* &pMsg, const char* InChatMessage, const PlayerRole &InRole )



			void ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageCmd:{1}:{2} , ChatMessage:{3}, Role:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ChatMessage, m_Role); 
			}; // void ChatMessageCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID ChatMessageRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 48);
			Result ChatMessageRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageRes::ParseIMsg( MessageData* pIMsg )

			Result ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void ChatMessageRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Chatting message event 
			const MessageID ChatMessageS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 49);
			Result ChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfSenderName = 0;
				UINT16 uiSizeOfChatMessage = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_SenderID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfSenderName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_SenderName, pCur, iMsgSize, sizeof(char)*uiSizeOfSenderName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfChatMessage, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_ChatMessage, pCur, iMsgSize, sizeof(char)*uiSizeOfChatMessage ) );


			Proc_End:

				return hr;

			}; // Result ChatMessageS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInSenderNameLength = InSenderName ? (UINT16)(strlen(InSenderName)+1) : 1;
				UINT16 __uiInChatMessageLength = InChatMessage ? (UINT16)(strlen(InChatMessage)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInSenderNameLength + sizeof(UINT16) + __uiInChatMessageLength 
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::ChatMessageS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InSenderID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &__uiInSenderNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InSenderName ? InSenderName : "", __uiInSenderNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInChatMessageLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InChatMessage ? InChatMessage : "", __uiInChatMessageLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result ChatMessageS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InSenderID, const PlayerRole &InRole, const char* InSenderName, const char* InChatMessage )



			void ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:ChatMessageS2CEvt:{1}:{2} , SenderID:{3}, Role:{4}, SenderName:{5}, ChatMessage:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_SenderID, m_Role, m_SenderName, m_ChatMessage); 
			}; // void ChatMessageS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Advance game
			const MessageID AdvanceGameCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 50);
			Result AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::ParseIMsg( MessageData* pIMsg )

			Result AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AdvanceGameCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AdvanceGameCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void AdvanceGameCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID AdvanceGameRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 50);
			Result AdvanceGameRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::ParseIMsg( MessageData* pIMsg )

			Result AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::AdvanceGameRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result AdvanceGameRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:AdvanceGameRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void AdvanceGameRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: The game state is advanced
			const MessageID GameAdvancedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 51);
			Result GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvancedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameAdvancedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay )



			void GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameAdvancedS2CEvt:{1}:{2} , GameInsUID:{3}, TimeStamp:{4}, GameState:{5}, Day:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_TimeStamp, m_GameState, m_Day); 
			}; // void GameAdvancedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game is ended
			const MessageID GameEndedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 52);
			Result GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Winner, pCur, iMsgSize, sizeof(GameWinner) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedExp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GainedGameMoney, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(GameWinner)
					+ sizeof(UINT32)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameEndedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InWinner, sizeof(GameWinner));
				Protocol::PackParamCopy( pMsgData, &InGainedExp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGainedGameMoney, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameEndedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const GameWinner &InWinner, const UINT32 &InGainedExp, const UINT32 &InGainedGameMoney )



			void GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameEndedS2CEvt:{1}:{2} , GameInsUID:{3}, Winner:{4}, GainedExp:{5}, GainedGameMoney:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Winner, m_GainedExp, m_GainedGameMoney); 
			}; // void GameEndedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote game advance
			const MessageID VoteGameAdvanceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 53);
			Result VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::ParseIMsg( MessageData* pIMsg )

			Result VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket )



			void VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteGameAdvanceCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket); 
			}; // void VoteGameAdvanceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteGameAdvanceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 53);
			Result VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::ParseIMsg( MessageData* pIMsg )

			Result VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteGameAdvanceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteGameAdvanceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteGameAdvanceRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void VoteGameAdvanceRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: GameAdvance is Voted
			const MessageID GameAdvanceVotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 54);
			Result GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameAdvanceVotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameAdvanceVotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter )



			void GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameAdvanceVotedS2CEvt:{1}:{2} , GameInsUID:{3}, Voter:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter); 
			}; // void GameAdvanceVotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Vote
			const MessageID VoteCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 55);
			Result VoteCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Ticket, pCur, iMsgSize, sizeof(AuthTicket) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VoteTarget, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ActionSerial, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result VoteCmd::ParseIMsg( MessageData* pIMsg )

			Result VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(AuthTicket)
					+ sizeof(PlayerID)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTicket, sizeof(AuthTicket));
				Protocol::PackParamCopy( pMsgData, &InVoteTarget, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InActionSerial, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteCmd::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InPlayerID, const AuthTicket &InTicket, const PlayerID &InVoteTarget, const UINT32 &InActionSerial )



			void VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteCmd:{1}:{2} , GameInsUID:{3}, PlayerID:{4}, Ticket:{5}, VoteTarget:{6}, ActionSerial:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_PlayerID, m_Ticket, m_VoteTarget, m_ActionSerial); 
			}; // void VoteCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID VoteRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 55);
			Result VoteRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result VoteRes::ParseIMsg( MessageData* pIMsg )

			Result VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void VoteRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID VotedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 56);
			Result VotedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Voter, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_VotedTarget, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::VotedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InVoter, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InVotedTarget, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VotedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InVoter, const PlayerID &InVotedTarget )



			void VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VotedS2CEvt:{1}:{2} , GameInsUID:{3}, Voter:{4}, VotedTarget:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voter, m_VotedTarget); 
			}; // void VotedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Vote is ended
			const MessageID VoteEndS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 57);
			Result VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofVoted = 0; PlayerID* pVoted = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofVoted, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pVoted, pCur, iMsgSize, sizeof(PlayerID)*numberofVoted ) );
				m_Voted.SetLinkedBuffer(numberofVoted, numberofVoted, pVoted);


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)*InVoted.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInVoted = (UINT16)InVoted.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::VoteEndS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &numberOfInVoted, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InVoted.data(), (INT)(sizeof(PlayerID)*InVoted.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result VoteEndS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const Array<PlayerID>& InVoted )



			void VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:VoteEndS2CEvt:{1}:{2} , GameInsUID:{3}, Voted:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_Voted); 
			}; // void VoteEndS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Killed
			const MessageID PlayerKilledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 58);
			Result PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_KilledPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerKilledReason) ) );


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerKilledReason));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerKilledS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InKilledPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerKilledReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerKilledS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InKilledPlayer, const PlayerKilledReason &InReason )



			void PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerKilledS2CEvt:{1}:{2} , GameInsUID:{3}, KilledPlayer:{4}, Reason:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_KilledPlayer, m_Reason); 
			}; // void PlayerKilledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player Voted
			const MessageID PlayerRevealedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 59);
			Result PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_GameInsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Reason, pCur, iMsgSize, sizeof(PlayerRevealedReason) ) );


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(PlayerID)
					+ sizeof(PlayerRole)
					+ sizeof(PlayerRevealedReason));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::PlayerRevealedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InGameInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InRevealedPlayerID, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InReason, sizeof(PlayerRevealedReason));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result PlayerRevealedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InGameInsUID, const PlayerID &InRevealedPlayerID, const PlayerRole &InRole, const PlayerRevealedReason &InReason )



			void PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:PlayerRevealedS2CEvt:{1}:{2} , GameInsUID:{3}, RevealedPlayerID:{4}, Role:{5}, Reason:{6}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_GameInsUID, m_RevealedPlayerID, m_Role, m_Reason); 
			}; // void PlayerRevealedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Play again with the current players
			const MessageID GamePlayAgainCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 60);
			Result GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::ParseIMsg( MessageData* pIMsg )

			Result GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayAgainCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayAgainRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 60);
			Result GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::ParseIMsg( MessageData* pIMsg )

			Result GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayAgainRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Somebody pressed play again. Only one of PartyUID and GameInsUID can have a value
			const MessageID GamePlayAgainS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 61);
			Result GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PartyUID, pCur, iMsgSize, sizeof(PartyUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_LeadPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PartyUID)
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayAgainS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPartyUID, sizeof(PartyUID));
				Protocol::PackParamCopy( pMsgData, &InLeadPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayAgainS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PartyUID &InPartyUID, const PlayerID &InLeadPlayer )



			void GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayAgainS2CEvt:{1}:{2} , PartyUID:{3}, LeadPlayer:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PartyUID, m_LeadPlayer); 
			}; // void GamePlayAgainS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reveal a player
			const MessageID GameRevealPlayerCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 62);
			Result GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofTargetPlayerID = 0; PlayerID* pTargetPlayerID = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &numberofTargetPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pTargetPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofTargetPlayerID ) );
				m_TargetPlayerID.SetLinkedBuffer(numberofTargetPlayerID, numberofTargetPlayerID, pTargetPlayerID);


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::ParseIMsg( MessageData* pIMsg )

			Result GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID)*InTargetPlayerID.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInTargetPlayerID = (UINT16)InTargetPlayerID.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &numberOfInTargetPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InTargetPlayerID.data(), (INT)(sizeof(PlayerID)*InTargetPlayerID.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerCmd::BuildIMsg( OUT MessageData* &pMsg, const Array<PlayerID>& InTargetPlayerID )



			void GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameRevealPlayerCmd:{1}:{2} , TargetPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayerID); 
			}; // void GameRevealPlayerCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GameRevealPlayerRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 62);
			Result GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofRevealedPlayerID = 0; PlayerID* pRevealedPlayerID = nullptr;
				UINT16 numberofRevealedRole = 0; PlayerRole* pRevealedRole = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedPlayerID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedPlayerID, pCur, iMsgSize, sizeof(PlayerID)*numberofRevealedPlayerID ) );
				m_RevealedPlayerID.SetLinkedBuffer(numberofRevealedPlayerID, numberofRevealedPlayerID, pRevealedPlayerID);
				protocolChk( Protocol::StreamParamCopy( &numberofRevealedRole, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pRevealedRole, pCur, iMsgSize, sizeof(PlayerRole)*numberofRevealedRole ) );
				m_RevealedRole.SetLinkedBuffer(numberofRevealedRole, numberofRevealedRole, pRevealedRole);
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::ParseIMsg( MessageData* pIMsg )

			Result GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)*InRevealedPlayerID.GetSize() + sizeof(UINT16)
					+ sizeof(PlayerRole)*InRevealedRole.GetSize() + sizeof(UINT16)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInRevealedPlayerID = (UINT16)InRevealedPlayerID.GetSize(); 
				UINT16 numberOfInRevealedRole = (UINT16)InRevealedRole.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameRevealPlayerRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedPlayerID, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedPlayerID.data(), (INT)(sizeof(PlayerID)*InRevealedPlayerID.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInRevealedRole, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InRevealedRole.data(), (INT)(sizeof(PlayerRole)*InRevealedRole.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameRevealPlayerRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const Array<PlayerID>& InRevealedPlayerID, const Array<PlayerRole>& InRevealedRole, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameRevealPlayerRes:{1}:{2} , Result:{3:X8}, RevealedPlayerID:{4}, RevealedRole:{5}, TotalGem:{6}, TotalGameMoney:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_RevealedPlayerID, m_RevealedRole, m_TotalGem, m_TotalGameMoney); 
			}; // void GameRevealPlayerRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. revive himself
			const MessageID GamePlayerReviveCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 63);
			Result GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::ParseIMsg( MessageData* pIMsg )

			Result GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerReviveCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayerReviveCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerReviveRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 63);
			Result GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::ParseIMsg( MessageData* pIMsg )

			Result GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerReviveRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerReviveRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerReviveRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayerReviveRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Player is revived
			const MessageID GamePlayerRevivedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 64);
			Result GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_RevivedPlayerID, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerRevivedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InRevivedPlayerID, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerRevivedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InRevivedPlayerID )



			void GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerRevivedS2CEvt:{1}:{2} , RevivedPlayerID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_RevivedPlayerID); 
			}; // void GamePlayerRevivedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Player. reset ranking
			const MessageID GamePlayerResetRankCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 65);
			Result GamePlayerResetRankCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankCmd::ParseIMsg( MessageData* pIMsg )

			Result GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankCmd::BuildIMsg( OUT MessageData* &pMsg )



			void GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerResetRankCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GamePlayerResetRankCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GamePlayerResetRankRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 65);
			Result GamePlayerResetRankRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankRes::ParseIMsg( MessageData* pIMsg )

			Result GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GamePlayerResetRankRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GamePlayerResetRankRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GamePlayerResetRankRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void GamePlayerResetRankRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Request Game match
			const MessageID RequestGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 66);
			Result RequestGameMatchCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_NumPlayer, pCur, iMsgSize, sizeof(BYTE) ) );
				protocolChk( Protocol::StreamParamCopy( &m_RequestRole, pCur, iMsgSize, sizeof(PlayerRole) ) );


			Proc_End:

				return hr;

			}; // Result RequestGameMatchCmd::ParseIMsg( MessageData* pIMsg )

			Result RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const BYTE &InNumPlayer, const PlayerRole &InRequestRole )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(BYTE)
					+ sizeof(PlayerRole));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InNumPlayer, sizeof(BYTE));
				Protocol::PackParamCopy( pMsgData, &InRequestRole, sizeof(PlayerRole));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg, const BYTE &InNumPlayer, const PlayerRole &InRequestRole )



			void RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestGameMatchCmd:{1}:{2} , NumPlayer:{3}, RequestRole:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_NumPlayer, m_RequestRole); 
			}; // void RequestGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID RequestGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 66);
			Result RequestGameMatchRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result RequestGameMatchRes::ParseIMsg( MessageData* pIMsg )

			Result RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::RequestGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result RequestGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:RequestGameMatchRes:{1}:{2} , Result:{3:X8}, TotalGem:{4}, TotalGameMoney:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TotalGem, m_TotalGameMoney); 
			}; // void RequestGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matched
			const MessageID GameMatchedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 67);
			Result GameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 numberofChatHistoryData = 0; BYTE* pChatHistoryData = nullptr;
				UINT16 numberofGameLogData = 0; BYTE* pGameLogData = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_InsUID, pCur, iMsgSize, sizeof(GameInsUID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_GameState, pCur, iMsgSize, sizeof(GameStateID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Day, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_MaxPlayer, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerIndex, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_PlayerCharacter, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Role, pCur, iMsgSize, sizeof(PlayerRole) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Dead, pCur, iMsgSize, sizeof(UINT8) ) );
				protocolChk( Protocol::StreamParamCopy( &numberofChatHistoryData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pChatHistoryData, pCur, iMsgSize, sizeof(BYTE)*numberofChatHistoryData ) );
				m_ChatHistoryData.SetLinkedBuffer(numberofChatHistoryData, numberofChatHistoryData, pChatHistoryData);
				protocolChk( Protocol::StreamParamCopy( &numberofGameLogData, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pGameLogData, pCur, iMsgSize, sizeof(BYTE)*numberofGameLogData ) );
				m_GameLogData.SetLinkedBuffer(numberofGameLogData, numberofGameLogData, pGameLogData);
				protocolChk( Protocol::StreamParamCopy( &m_Stamina, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGem, pCur, iMsgSize, sizeof(UINT64) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TotalGameMoney, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GameMatchedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(GameInsUID)
					+ sizeof(UINT32)
					+ sizeof(GameStateID)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(UINT8)
					+ sizeof(PlayerRole)
					+ sizeof(UINT8)
					+ sizeof(BYTE)*InChatHistoryData.GetSize() + sizeof(UINT16)
					+ sizeof(BYTE)*InGameLogData.GetSize() + sizeof(UINT16)
					+ sizeof(UINT32)
					+ sizeof(UINT64)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInChatHistoryData = (UINT16)InChatHistoryData.GetSize(); 
				UINT16 numberOfInGameLogData = (UINT16)InGameLogData.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InInsUID, sizeof(GameInsUID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InGameState, sizeof(GameStateID));
				Protocol::PackParamCopy( pMsgData, &InDay, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InMaxPlayer, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerIndex, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InPlayerCharacter, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &InRole, sizeof(PlayerRole));
				Protocol::PackParamCopy( pMsgData, &InDead, sizeof(UINT8));
				Protocol::PackParamCopy( pMsgData, &numberOfInChatHistoryData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InChatHistoryData.data(), (INT)(sizeof(BYTE)*InChatHistoryData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &numberOfInGameLogData, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InGameLogData.data(), (INT)(sizeof(BYTE)*InGameLogData.GetSize())); 
				Protocol::PackParamCopy( pMsgData, &InStamina, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &InTotalGem, sizeof(UINT64));
				Protocol::PackParamCopy( pMsgData, &InTotalGameMoney, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const GameInsUID &InInsUID, const UINT32 &InTimeStamp, const GameStateID &InGameState, const UINT8 &InDay, const UINT8 &InMaxPlayer, const UINT8 &InPlayerIndex, const UINT8 &InPlayerCharacter, const PlayerRole &InRole, const UINT8 &InDead, const Array<BYTE>& InChatHistoryData, const Array<BYTE>& InGameLogData, const UINT32 &InStamina, const UINT64 &InTotalGem, const UINT64 &InTotalGameMoney )



			void GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchedS2CEvt:{1}:{2} , InsUID:{3}, TimeStamp:{4}, GameState:{5}, Day:{6}, MaxPlayer:{7}, PlayerIndex:{8}, PlayerCharacter:{9}, Role:{10}, Dead:{11}, ChatHistoryData:{12}, GameLogData:{13}, Stamina:{14}, TotalGem:{15}, TotalGameMoney:{16}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_InsUID, m_TimeStamp, m_GameState, m_Day, m_MaxPlayer, m_PlayerIndex, m_PlayerCharacter, m_Role, m_Dead, m_ChatHistoryData, m_GameLogData, m_Stamina, m_TotalGem, m_TotalGameMoney); 
			}; // void GameMatchedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game match failed
			const MessageID GameMatchFailedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 68);
			Result GameMatchFailedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_FailedReason, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GameMatchFailedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const Result &InFailedReason )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchFailedS2CEvt::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InFailedReason, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchFailedS2CEvt::BuildIMsg( OUT MessageData* &pMsg, const Result &InFailedReason )



			void GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchFailedS2CEvt:{1}:{2} , FailedReason:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_FailedReason); 
			}; // void GameMatchFailedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: Game matching started
			const MessageID GameMatchingStartedS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 69);
			Result GameMatchingStartedS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GameMatchingStartedS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingStartedS2CEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchingStartedS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			void GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchingStartedS2CEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GameMatchingStartedS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Cancel Game match
			const MessageID CancelGameMatchCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 70);
			Result CancelGameMatchCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result CancelGameMatchCmd::ParseIMsg( MessageData* pIMsg )

			Result CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchCmd::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CancelGameMatchCmd::BuildIMsg( OUT MessageData* &pMsg )



			void CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelGameMatchCmd:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void CancelGameMatchCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID CancelGameMatchRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 70);
			Result CancelGameMatchRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result CancelGameMatchRes::ParseIMsg( MessageData* pIMsg )

			Result CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::CancelGameMatchRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result CancelGameMatchRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:CancelGameMatchRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void CancelGameMatchRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// S2C: game matching canceled
			const MessageID GameMatchingCanceledS2CEvt::MID = MessageID(MSGTYPE_EVENT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 71);
			Result GameMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;


				protocolChkPtr(pIMsg);




			Proc_End:

				return hr;

			}; // Result GameMatchingCanceledS2CEvt::ParseIMsg( MessageData* pIMsg )

			Result GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )
			{
 				Result hr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) );

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GameMatchingCanceledS2CEvt::MID, __uiMessageSize ) );


				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GameMatchingCanceledS2CEvt::BuildIMsg( OUT MessageData* &pMsg )



			void GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GameMatchingCanceledS2CEvt:{1}:{2} ",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32); 
			}; // void GameMatchingCanceledS2CEvt::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item prepare
			const MessageID BuyShopItemPrepareCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 72);
			Result BuyShopItemPrepareCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareCmd::ParseIMsg( MessageData* pIMsg )

			Result BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID )



			void BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemPrepareCmd:{1}:{2} , ShopItemID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID); 
			}; // void BuyShopItemPrepareCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemPrepareRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 72);
			Result BuyShopItemPrepareRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfPurchaseID = 0;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseID, pCur, iMsgSize, sizeof(char)*uiSizeOfPurchaseID ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareRes::ParseIMsg( MessageData* pIMsg )

			Result BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPurchaseIDLength = InPurchaseID ? (UINT16)(strlen(InPurchaseID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPurchaseIDLength 
					+ sizeof(Result)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemPrepareRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPurchaseID ? InPurchaseID : "", __uiInPurchaseIDLength );

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemPrepareRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InShopItemID, const char* InPurchaseID )



			void BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemPrepareRes:{1}:{2} , Result:{3:X8}, ShopItemID:{4}, PurchaseID:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ShopItemID, m_PurchaseID); 
			}; // void BuyShopItemPrepareRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Buy shop item
			const MessageID BuyShopItemCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 73);
			Result BuyShopItemCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;
				UINT16 uiSizeOfPlatform = 0;
				UINT16 uiSizeOfPackageName = 0;
				UINT16 uiSizeOfPurchaseTransactionID = 0;
				UINT16 numberofPurchaseToken = 0; BYTE* pPurchaseToken = nullptr;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPlatform, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_Platform, pCur, iMsgSize, sizeof(char)*uiSizeOfPlatform ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPackageName, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PackageName, pCur, iMsgSize, sizeof(char)*uiSizeOfPackageName ) );
				protocolChk( Protocol::StreamParamCopy( &uiSizeOfPurchaseTransactionID, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( m_PurchaseTransactionID, pCur, iMsgSize, sizeof(char)*uiSizeOfPurchaseTransactionID ) );
				protocolChk( Protocol::StreamParamCopy( &numberofPurchaseToken, pCur, iMsgSize, sizeof(UINT16) ) );
				protocolChk( Protocol::StreamParamLnk( pPurchaseToken, pCur, iMsgSize, sizeof(BYTE)*numberofPurchaseToken ) );
				m_PurchaseToken.SetLinkedBuffer(numberofPurchaseToken, numberofPurchaseToken, pPurchaseToken);


			Proc_End:

				return hr;

			}; // Result BuyShopItemCmd::ParseIMsg( MessageData* pIMsg )

			Result BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT16 __uiInPlatformLength = InPlatform ? (UINT16)(strlen(InPlatform)+1) : 1;
				UINT16 __uiInPackageNameLength = InPackageName ? (UINT16)(strlen(InPackageName)+1) : 1;
				UINT16 __uiInPurchaseTransactionIDLength = InPurchaseTransactionID ? (UINT16)(strlen(InPurchaseTransactionID)+1) : 1;
				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) +  + sizeof(UINT16) + __uiInPlatformLength + sizeof(UINT16) + __uiInPackageNameLength + sizeof(UINT16) + __uiInPurchaseTransactionIDLength 
					+ sizeof(UINT32)
					+ sizeof(BYTE)*InPurchaseToken.GetSize() + sizeof(UINT16));

				MessageData *pNewMsg = nullptr;

				UINT16 numberOfInPurchaseToken = (UINT16)InPurchaseToken.GetSize(); 
				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));
				Protocol::PackParamCopy( pMsgData, &__uiInPlatformLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPlatform ? InPlatform : "", __uiInPlatformLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPackageNameLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPackageName ? InPackageName : "", __uiInPackageNameLength );
				Protocol::PackParamCopy( pMsgData, &__uiInPurchaseTransactionIDLength, sizeof(UINT16) );
				Protocol::PackParamCopy( pMsgData, InPurchaseTransactionID ? InPurchaseTransactionID : "", __uiInPurchaseTransactionIDLength );
				Protocol::PackParamCopy( pMsgData, &numberOfInPurchaseToken, sizeof(UINT16)); 
				Protocol::PackParamCopy( pMsgData, InPurchaseToken.data(), (INT)(sizeof(BYTE)*InPurchaseToken.GetSize())); 

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InShopItemID, const char* InPlatform, const char* InPackageName, const char* InPurchaseTransactionID, const Array<BYTE>& InPurchaseToken )



			void BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemCmd:{1}:{2} , ShopItemID:{3}, Platform:{4}, PackageName:{5}, PurchaseTransactionID:{6}, PurchaseToken:{7}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_ShopItemID, m_Platform, m_PackageName, m_PurchaseTransactionID, m_PurchaseToken); 
			}; // void BuyShopItemCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID BuyShopItemRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 73);
			Result BuyShopItemRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_ShopItemID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result BuyShopItemRes::ParseIMsg( MessageData* pIMsg )

			Result BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InShopItemID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::BuyShopItemRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InShopItemID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result BuyShopItemRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const UINT32 &InShopItemID )



			void BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:BuyShopItemRes:{1}:{2} , Result:{3:X8}, ShopItemID:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_ShopItemID); 
			}; // void BuyShopItemRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: Give my stamina to other player
			const MessageID GiveStaminaCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 74);
			Result GiveStaminaCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );


			Proc_End:

				return hr;

			}; // Result GiveStaminaCmd::ParseIMsg( MessageData* pIMsg )

			Result GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(PlayerID));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GiveStaminaCmd::BuildIMsg( OUT MessageData* &pMsg, const PlayerID &InTargetPlayer )



			void GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GiveStaminaCmd:{1}:{2} , TargetPlayer:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_TargetPlayer); 
			}; // void GiveStaminaCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GiveStaminaRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 74);
			Result GiveStaminaRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TargetPlayer, pCur, iMsgSize, sizeof(PlayerID) ) );
				protocolChk( Protocol::StreamParamCopy( &m_TimeStamp, pCur, iMsgSize, sizeof(UINT64) ) );


			Proc_End:

				return hr;

			}; // Result GiveStaminaRes::ParseIMsg( MessageData* pIMsg )

			Result GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result)
					+ sizeof(PlayerID)
					+ sizeof(UINT64));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GiveStaminaRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));
				Protocol::PackParamCopy( pMsgData, &InTargetPlayer, sizeof(PlayerID));
				Protocol::PackParamCopy( pMsgData, &InTimeStamp, sizeof(UINT64));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GiveStaminaRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult, const PlayerID &InTargetPlayer, const UINT64 &InTimeStamp )



			void GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GiveStaminaRes:{1}:{2} , Result:{3:X8}, TargetPlayer:{4}, TimeStamp:{5}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result, m_TargetPlayer, m_TimeStamp); 
			}; // void GiveStaminaRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For debug, Change configue preset
			const MessageID SetPresetGameConfigIDCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 75);
			Result SetPresetGameConfigIDCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_PresetID, pCur, iMsgSize, sizeof(UINT32) ) );


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDCmd::ParseIMsg( MessageData* pIMsg )

			Result SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InPresetID )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(UINT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InPresetID, sizeof(UINT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDCmd::BuildIMsg( OUT MessageData* &pMsg, const UINT32 &InPresetID )



			void SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetPresetGameConfigIDCmd:{1}:{2} , PresetID:{3}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_PresetID); 
			}; // void SetPresetGameConfigIDCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID SetPresetGameConfigIDRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 75);
			Result SetPresetGameConfigIDRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDRes::ParseIMsg( MessageData* pIMsg )

			Result SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::SetPresetGameConfigIDRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result SetPresetGameConfigIDRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:SetPresetGameConfigIDRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void SetPresetGameConfigIDRes::TraceOut(const char* Prefix, MessageData* pMsg)

			// Cmd: For Debug
			const MessageID GainGameResourceCmd::MID = MessageID(MSGTYPE_COMMAND, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 76);
			Result GainGameResourceCmd::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Resource, pCur, iMsgSize, sizeof(INT32) ) );
				protocolChk( Protocol::StreamParamCopy( &m_Value, pCur, iMsgSize, sizeof(INT32) ) );


			Proc_End:

				return hr;

			}; // Result GainGameResourceCmd::ParseIMsg( MessageData* pIMsg )

			Result GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const INT32 &InResource, const INT32 &InValue )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(INT32)
					+ sizeof(INT32));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceCmd::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResource, sizeof(INT32));
				Protocol::PackParamCopy( pMsgData, &InValue, sizeof(INT32));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GainGameResourceCmd::BuildIMsg( OUT MessageData* &pMsg, const INT32 &InResource, const INT32 &InValue )



			void GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GainGameResourceCmd:{1}:{2} , Resource:{3}, Value:{4}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Resource, m_Value); 
			}; // void GainGameResourceCmd::TraceOut(const char* Prefix, MessageData* pMsg)

			const MessageID GainGameResourceRes::MID = MessageID(MSGTYPE_RESULT, MSGTYPE_RELIABLE, MSGTYPE_MOBILE, POLICY_GAME, 76);
			Result GainGameResourceRes::ParseIMsg( MessageData* pIMsg )
			{
 				Result hr;

				INT iMsgSize;
				BYTE* pCur;

				protocolChkPtr(pIMsg);

				iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MobileMessageHeader);
				pCur = pIMsg->GetMessageData();

				protocolChk( Protocol::StreamParamCopy( &m_Result, pCur, iMsgSize, sizeof(Result) ) );


			Proc_End:

				return hr;

			}; // Result GainGameResourceRes::ParseIMsg( MessageData* pIMsg )

			Result GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )
			{
 				Result hr;

				BYTE *pMsgData = nullptr;

				UINT __uiMessageSize = (UINT)(sizeof(MobileMessageHeader) 
					+ sizeof(Result));

				MessageData *pNewMsg = nullptr;

				protocolMem( pNewMsg = MessageData::NewMessage( Game::GainGameResourceRes::MID, __uiMessageSize ) );

				pMsgData = pNewMsg->GetMessageData();

				Protocol::PackParamCopy( pMsgData, &InResult, sizeof(Result));

				pMsg = pNewMsg;


			Proc_End:

				return hr;

			}; // Result GainGameResourceRes::BuildIMsg( OUT MessageData* &pMsg, const Result &InResult )



			void GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)
			{
 				unused(Prefix);
				protocolTrace(Trace::TRC_DBG1, "{0}:GainGameResourceRes:{1}:{2} , Result:{3:X8}",
												Prefix, pMsg->GetMessageHeader()->Length, pMsg->GetMessageHeader()->Crc32, m_Result); 
			}; // void GainGameResourceRes::TraceOut(const char* Prefix, MessageData* pMsg)



		}; // namespace Game
	}; // namespace Message
}; // namespace BR


