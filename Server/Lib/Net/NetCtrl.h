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

#include "Common/Typedefs.h"
#include "Net/NetDef.h"
#include "Net/NetConst.h"




namespace BR {
namespace Net {

	class Connection;

	

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
	};

	static const Message::MessageID PACKET_NETCTRL_NONE				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_None);
	static const Message::MessageID PACKET_NETCTRL_ACK				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Ack);
	static const Message::MessageID PACKET_NETCTRL_NACK				= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Nack);
	static const Message::MessageID PACKET_NETCTRL_HEARTBIT			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_HeartBit);
	static const Message::MessageID PACKET_NETCTRL_DISCONNECT		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Disconnect);
	static const Message::MessageID PACKET_NETCTRL_CONNECT			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_Connect);
	static const Message::MessageID PACKET_NETCTRL_CONNECTPEER		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_ConnectPeer);
	static const Message::MessageID PACKET_NETCTRL_SYNCRELIABLE		= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_SyncReliable);
	static const Message::MessageID PACKET_NETCTRL_TIMESYNC			= Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_NONE,	 Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_TimeSync);
	static const Message::MessageID PACKET_NETCTRL_SEQUENCE_FRAME   = Message::MessageID(Message::MSGTYPE_NETCONTROL, Message::MSGTYPE_RELIABLE, Message::MSGTYPE_MOBILE, BR::POLICY_NONE, NetCtrlCode_SequenceFrame);


#pragma pack(push)
#pragma pack(4)


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network control Message base Header
	//

	typedef struct tag_MsgNetCtrl : public Message::MessageHeader
	{
		Message::MessageID	rtnMsgID;
	} MsgNetCtrl;


	typedef struct tag_MsgNetCtrlConnect : public MsgNetCtrl
	{
		NetAddress	Address;
		UINT64		PeerUID;
	} MsgNetCtrlConnect;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network control Message base Header
	//

	struct MsgMobileNetCtrl : public Message::MobileMessageHeader
	{
		Message::MessageID	rtnMsgID;
	};

	struct MsgMobileNetCtrlSync : public Message::MobileMessageHeader
	{
		UINT64	MessageMask;
	};


	// Just some big enough structure to contain all net ctrl type data
	typedef struct tag_MsgNetCtrlBuffer : public tag_MsgNetCtrlConnect
	{
		UINT64	Dummy;

		inline tag_MsgNetCtrlBuffer& operator = (const tag_MsgNetCtrlBuffer& src);
		inline tag_MsgNetCtrlBuffer& operator = (void* src);

		inline bool operator == (const tag_MsgNetCtrlBuffer& src) const;
		inline bool operator != (const tag_MsgNetCtrlBuffer& src) const;
		inline bool operator == (void* src) const;
		inline bool operator != (void* src) const;
	} MsgNetCtrlBuffer;


	struct MsgMobileNetCtrlSequenceFrame : public Message::MobileMessageHeader
	{
		UINT32 SubSequence	: 16;
		UINT32 TotalSize	: 16;
	};


#pragma pack(pop)


#include "NetCtrl.inl"

} // namespace Net
} // namespace BR



