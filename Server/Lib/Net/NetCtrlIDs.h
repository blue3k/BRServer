////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network classes definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Protocol/Protocol.h"



namespace BR {
namespace Net {



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Packetes for Network Control definitions
	//

	enum NetCtrlIDs
	{
		NetCtrlCode_None,
		NetCtrlCode_Ack,
		NetCtrlCode_Nack,
		NetCtrlCode_HeartBit,
		NetCtrlCode_Disconnect,
		NetCtrlCode_Connect,
		NetCtrlCode_ConnectPeer,
		NetCtrlCode_SyncReliable,
		NetCtrlCode_TimeSync,
		NetCtrlCode_SequenceFrame,
		NetCtrlCode_DataTransmitionTest,
	};

	static const Message::MessageID PACKET_NETCTRL_NONE				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_None);
	static const Message::MessageID PACKET_NETCTRL_ACK				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_Ack);
	static const Message::MessageID PACKET_NETCTRL_NACK				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_Nack);
	static const Message::MessageID PACKET_NETCTRL_HEARTBIT			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_HeartBit);
	static const Message::MessageID PACKET_NETCTRL_DISCONNECT		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_Disconnect);
	static const Message::MessageID PACKET_NETCTRL_CONNECT			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_Connect);
	static const Message::MessageID PACKET_NETCTRL_CONNECTPEER		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_ConnectPeer);
	static const Message::MessageID PACKET_NETCTRL_SYNCRELIABLE		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_SyncReliable);
	static const Message::MessageID PACKET_NETCTRL_TIMESYNC			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_TimeSync);
	static const Message::MessageID PACKET_NETCTRL_SEQUENCE_FRAME   = Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_SequenceFrame);
	static const Message::MessageID PACKET_NETCTRL_DATA_TRANSMITION_TEST = Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_MOBILE, PROTOCOLID_NONE, NetCtrlCode_DataTransmitionTest);


} // namespace Net
} // namespace BR



