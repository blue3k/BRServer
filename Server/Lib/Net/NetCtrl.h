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
#include "Net/NetDef.h"
#include "Net/NetCtrlIDs.h"
#include "Net/NetConst.h"




namespace BR {
namespace Net {

	class Connection;



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
		uint64_t		PeerUID;
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
		uint64_t	MessageMask;
	};


	// Just some big enough structure to contain all net ctrl type data
	typedef struct tag_MsgNetCtrlBuffer : public tag_MsgNetCtrlConnect
	{
		uint64_t	Dummy;

		tag_MsgNetCtrlBuffer() { msgID.ID = 0; }
		tag_MsgNetCtrlBuffer(void* ptr) { assert(ptr == nullptr); msgID.ID = 0; }
		tag_MsgNetCtrlBuffer(const tag_MsgNetCtrlBuffer& src);

		tag_MsgNetCtrlBuffer& operator = (const tag_MsgNetCtrlBuffer& src);

		bool operator == (const tag_MsgNetCtrlBuffer& src) const;
		bool operator != (const tag_MsgNetCtrlBuffer& src) const;
	} MsgNetCtrlBuffer;


	struct MsgMobileNetCtrlSequenceFrame : public Message::MobileMessageHeader
	{
		uint32_t SubSequence	: 16;
		uint32_t TotalSize	: 16;
	};


#pragma pack(pop)


#include "NetCtrl.inl"

} // namespace Net
} // namespace BR



