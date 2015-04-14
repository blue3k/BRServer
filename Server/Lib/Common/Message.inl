////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Net base type definitions. 
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	Message ID
//



tag_MessageID::tag_MessageID()
	:ID(0)
{
}

//tag_MessageID::tag_MessageID( const tag_MessageID& src )
//	:ID(src.ID)
//{
//}

tag_MessageID::tag_MessageID( UINT32 uiID )
	:ID(uiID)
{
}

tag_MessageID::tag_MessageID( UINT uiType, UINT uiReliability, UINT uiMobility, UINT uiPolicy, UINT uiCode )
{
	IDs.MsgCode = uiCode;
	IDs.Policy = uiPolicy;
	IDs.Mobile = uiMobility;
	IDs.Encrypted = 0;
	IDs.Reliability = uiReliability;
	IDs.Type = uiType;
	IDs.Sequence = 0;// uiSeq;
}

UINT32 tag_MessageID::SetMessageID( UINT uiType, UINT uiReliability, UINT uiMobility, UINT uiPolicy, UINT uiCode )
{
	IDs.MsgCode = uiCode;
	IDs.Policy = uiPolicy;
	IDs.Mobile = uiMobility;
	IDs.Encrypted = 0;
	IDs.Reliability = uiReliability;
	IDs.Type = uiType;
	IDs.Sequence = 0;// uiSeq;

	return ID;
}

// Only MsgID part, no sequence or length
inline UINT tag_MessageID::GetMsgID() const
{
	return IDSeq.MsgID;
}

tag_MessageID::operator UINT32() const
{
	return ID;
}

inline INT SequenceDifference(UINT seq1, UINT seq2)
{
	const int SEQDIFF_MAX = NET_SEQUENCE_MASK >> 1;
	const int SEQDIFF_MIN = (-SEQDIFF_MAX - 1);

	//NET_SEQUEUCN_BITS
	seq1 = NET_SEQUENCE_MASK & seq1;
	seq2 = NET_SEQUENCE_MASK & seq2;
	auto diff = (INT)(seq1 - seq2);
	if (diff > SEQDIFF_MAX)
		diff -= NET_SEQUENCE_MASK;
	else if (diff < SEQDIFF_MIN)
		diff += NET_SEQUENCE_MASK + 1;

	return diff;
}



////////////////////////////////////////////////////////////////////////////////
//
//	MessageData
//

MessageHeader* MessageData::GetMessageHeader()
{
	return m_pMsgHeader;
}

MobileMessageHeader* MessageData::GetMobileMessageHeader()
{
	AssertRel(m_pMsgHeader->msgID.IDs.Mobile);
	return m_pMobileMsgHeader;
}

BYTE* MessageData::GetMessageBuff()
{
	return (BYTE*)m_pMsgHeader;
}

UINT MessageData::GetMessageSize() const
{
	return m_pMsgHeader ? m_pMsgHeader->Length : 0;
}

BYTE* MessageData::GetMessageData()
{
	if( m_pMsgHeader->msgID.IDs.Mobile )
		return (BYTE*)(m_pMobileMsgHeader + 1);
	else
		return (BYTE*)(m_pMsgHeader + 1);
}

////////////////////////////////////////////////////////////////////////////////
//
//	Message data 
//




////////////////////////////////////////////////////////////////////////////////
//
//	Network Message parser class
//


MessageBase::MessageBase()
	:m_pIMsg(NULL),
	m_hrParsing(S_FALSE),
	m_bIsParsed(false)
{
}

MessageBase::MessageBase(MessageData* &pIMsg)
	:m_pIMsg(pIMsg),
	m_hrParsing(S_FALSE),
	m_bIsParsed(false)
{
	pIMsg = nullptr;
}

MessageBase::~MessageBase()
{
	if( m_pIMsg )
		m_pIMsg->Release();
}


// Get Message
MessageData* MessageBase::GetMessage()
{
	return m_pIMsg;
}



// Get Parsing Result
HRESULT MessageBase::GetParsingResult()
{
	return m_hrParsing;
}

