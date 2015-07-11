////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Message Definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/BRBaseTypes.h"
#include "Common/PolicyID.h"


namespace BR {
namespace Message {
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Packet Message ID definition
	//

	//
	//  Bit assignment of messageID
	//    2  1 1 1      7                 9                  11
	//   1 0 9 8 7  6 5 4 3 2 1 0  9 8 7 6 5 4 3 2 1  0 9 8 7 6 5 4 3 2 1 0
	//  +---+-+-+-+---------------+--------------------+----------------------+
	//  | T |G|M|E|   Policy      |       MsgCode      |        Seq           |
	//  +---+-+-+-+---------------+--------------------+----------------------+
	//
	//  where
	//
	//      T - Packet type
	//          00 - Net Control
	//          01 - Event
	//          10 - Command
	//          11 - Result
	//
	//      G - Reliability control bit - Only enabled when using TCP or connection based UDP
	//          0 - Not Guaranteed
	//          1 - Reliable message.
	//
	//      Encrypted
	//      Mobile - Mobility
	//      Policy - Protocol Policy ID
	//      MsgCode - Message ID, 0 is not used
	//      Seq - Sequence of the message
	//

#define NET_SEQUENCE_BITS 11
#define NET_SEQUENCE_MASK ((1<<NET_SEQUENCE_BITS)-1)

#if !defined(SWIG)
	// Because we pass net class in sequence id, NetClass::Max must be fit in the bits
	static_assert((UINT)NetClass::Max <= NET_SEQUENCE_MASK, "Too big net class value");
#endif

	typedef union tag_MessageID
	{
		struct {
			UINT32 Sequence : NET_SEQUENCE_BITS;
			UINT32 MsgCode		: 9;
			UINT32 Policy		: 7;
			UINT32 Encrypted	: 1;
			UINT32 Mobile		: 1;
			UINT32 Reliability	: 1;
			UINT32 Type			: 2;
			UINT32				: 0;
		} IDs;
		struct {
			UINT32 Sequence : NET_SEQUENCE_BITS;
			UINT32 MsgID	    : 21;
		} IDSeq;
		UINT32 ID;

		inline tag_MessageID();
		//inline tag_MessageID( const tag_MessageID& src );
		inline tag_MessageID( UINT32 uiID );
		inline tag_MessageID( UINT uiType, UINT uiReliability, UINT uiMobility, UINT uiPolicy, UINT uiCode );

		inline UINT32 SetMessageID( UINT uiType, UINT uiReliability, UINT uiMobility, UINT uiPolicy, UINT uiCode );

		// Only MsgID part, no sequence or length
		UINT GetMsgID() const;
#ifndef SWIG
		inline operator UINT32() const;
#endif
	} MessageID;



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Message Constants
	//
	enum
	{
		MAX_MESSAGE_SIZE = (1 << 14) - 1,
		MAX_SUBFRAME_SIZE = 1024 - 64,

		MSGTYPE_NONE			= 0,	

		// Packet type
		MSGTYPE_NETCONTROL		= 0,	// Net Control
		MSGTYPE_EVENT			= 1,	// Event
		MSGTYPE_COMMAND			= 2,	// Command
		MSGTYPE_RESULT			= 3,	// Result

		MSGTYPE_RELIABLE		= 1,	// Reliable messages

		MSGTYPE_MOBILE			= 1,	// Mobile protocol
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	// Message Usage Constants
	//
	enum MessageUsage
	{
		MessageUsage_None,
		MessageUsage_ClusterDataRead,
		MessageUsage_ClusterDataWrite,
		MessageUsage_ClusterStatusRead,
		MessageUsage_ClusterStatusWrite,
	};

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Message Interface
	//

#pragma pack(push)
#pragma pack(4)


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Packet Message base Header
	//
	struct MessageHeader
	{
		// Message ID
		MessageID	msgID;

		// Data length
		UINT32		Length	: 16;

		// lower 16 bit of crc32 or sub frame sequence
		UINT32		Crc32		: 16;

		// bit field termination
		UINT32					: 0;

	};

	static INT SequenceDifference(UINT seq1, UINT seq2);

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network Packet Message base Header
	//

	struct MobileMessageHeader : public MessageHeader
	{
		// Mobile peer ID
		UINT64		PeerID;
	};


#pragma pack(pop)


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Network message object
	//

	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Message data class
	//

	class MessageData : public ISharedObj
	{
	private:
		// Message Buffer Pointer
		union {
		MessageHeader*			m_pMsgHeader;
		MobileMessageHeader*	m_pMobileMsgHeader;
		};

		bool					m_bIsSequenceAssigned;

		// Encryption status

	private:
		MessageData( UINT uiMsgBufSize, const BYTE* pData = NULL );

	public:
		virtual ~MessageData();

		inline MessageHeader*	GetMessageHeader();
		inline MobileMessageHeader*	GetMobileMessageHeader();
		inline BYTE*	GetMessageBuff();
		inline UINT		GetMessageSize() const;
		inline BYTE*	GetMessageData();

		void GetLengthNDataPtr( UINT& length, BYTE* &pDataPtr);
		UINT GetDataLength();

		void ClearAssignedSequence();
		void AssignSequence( UINT sequence );
		inline bool GetIsSequenceAssigned()					{ return m_bIsSequenceAssigned; }

		// Initialize message buffer
		static MessageData* NewMessage( UINT32 uiMsgID, UINT uiMsgBufSize, const BYTE* pData = nullptr );

		virtual MessageData* Clone();

		// Update checksume and encrypt
		void UpdateChecksum();
		void UpdateChecksumNEncrypt();

		HRESULT ValidateChecksum();
		HRESULT ValidateChecksumNDecrypt();

		// From ISharedObj
		virtual void DeleteThis() const;
	};
	
	typedef MessageData* PMessageData;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Message base class
	//

	class MessageBase
	{
	private:
		// Message handler
		MessageData	*m_pIMsg;
		
		// Parsing result
		HRESULT		m_hrParsing;

		bool		m_bIsParsed;

	public:
		inline MessageBase();
		inline MessageBase(MessageData* &pIMsg);
		inline virtual ~MessageBase();

		// Get Message
		inline MessageData* GetMessage();

		// Get Parsing Result
		inline HRESULT GetParsingResult();

		virtual HRESULT ParseMsg();
		virtual HRESULT ParseIMsg( MessageData* pIMsg ) = 0;

		virtual HRESULT OverrideRouteContextDestination( EntityUID to ) { to; AssertRel(false); return S_OK; }
		virtual HRESULT OverrideRouteInfomation( EntityUID to, UINT hopCount ) { to; hopCount; AssertRel(false); return S_OK; }
	};



#include "Message.inl"

} // Message
} // BR



