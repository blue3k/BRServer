////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net control messages
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Net/NetDef.h"
#include "Common\Message.h"

namespace BR {
namespace Net {

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

		static tag_MsgNetCtrlBuffer NullValue;

		inline tag_MsgNetCtrlBuffer& operator = ( const tag_MsgNetCtrlBuffer& src );

		inline bool operator == ( const tag_MsgNetCtrlBuffer& src ) const;
		inline bool operator != ( const tag_MsgNetCtrlBuffer& src ) const;
	} MsgNetCtrlBuffer;


#pragma pack(pop)

	
#include "NetMessage.inl"

} // Net
} // BR


