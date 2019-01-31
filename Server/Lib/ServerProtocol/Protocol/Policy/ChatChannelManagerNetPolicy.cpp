////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StromFOrge
// 
// Author : Generated
// 
// Description : ChatChannelManager Message debug implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "Net/SFNetDef.h"
#include "Net/SFMessage.h"
#include "Net/SFConnection.h"
#include "Protocol/SvrProtocol.h"
#include "Protocol/Policy/ChatChannelManagerNetPolicy.h"
#include "Protocol/Message/ChatChannelManagerMsgClass.h"
#include "ProtocolTrace.h"



namespace SF
{
 	namespace Policy
	{
 		// Cmd: Create a channel instance
		Result NetPolicyChatChannelManager::CreateChannelCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannelManager::CreateChannelCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InChannelName, InPasscode, InCreator);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannelManager::CreateChannelCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName, const char* InPasscode, const PlayerInformation &InCreator )
		// Cmd: Find a channel instance with name
		Result NetPolicyChatChannelManager::FindChannelCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannelManager::FindChannelCmd::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InRouteHopCount, InChannelName);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannelManager::FindChannelCmd( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const uint16_t &InRouteHopCount, const char* InChannelName )
		// C2S: Notification that a chat channel instance has deleted
		Result NetPolicyChatChannelManager::ChatChannelDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannelManager::ChatChannelDeletedC2SEvt::Create(m_pConnection->GetIOHeap(), InRouteContext, InRouteHopCount);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetPolicyChatChannelManager::ChatChannelDeletedC2SEvt( const RouteContext &InRouteContext, const uint16_t &InRouteHopCount )


		// Cmd: Create a channel instance
		Result NetSvrPolicyChatChannelManager::CreateChannelRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannelManager::CreateChannelRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InChannelUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannelManager::CreateChannelRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
		// Cmd: Find a channel instance with name
		Result NetSvrPolicyChatChannelManager::FindChannelRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )
		{
 			Result hr;

			 MessageDataPtr pMessage;
			 protocolChkPtr(m_pConnection);

			 pMessage = SF::Message::ChatChannelManager::FindChannelRes::Create(m_pConnection->GetIOHeap(), InRouteContext, InTransactionID, InResult, InChannelUID);
			 protocolChkPtr(*pMessage);

			 return m_pConnection->Send( pMessage );

		Proc_End:

			return hr;

		}; // Result NetSvrPolicyChatChannelManager::FindChannelRes( const RouteContext &InRouteContext, const TransactionID &InTransactionID, const Result &InResult, const uint64_t &InChannelUID )


	}; // namespace Policy
}; // namespace SF


