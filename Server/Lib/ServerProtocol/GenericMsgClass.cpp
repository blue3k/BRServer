////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : Generic Message parser implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Protocol/Protocol.h"
#include "Protocol/ProtocolHelper.h"
#include "Net/NetDef.h"
#include "ProtocolTrace.h"
#include "Protocol/GenericMsgClass.h"



namespace SF {
namespace Message {

		// S2C: Used for sending generic failure for not started transaction messages
		Result FailResultS2CEvt::ParseMessage( MessageData* pIMsg )
		{
 			Result hr = ResultCode::SUCCESS;
			int iMsgSize;
			uint8_t* pCur;

			protocolChkPtr(pIMsg);

			iMsgSize = (int)pIMsg->GetMessageSize() - (int)sizeof(MessageHeader);
			pCur = pIMsg->GetMessageData();

			if( iMsgSize == sizeof(Result) ) // if result only
			{
				m_Context = 0;
			}
			else
			{
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
			}
			protocolChk( Protocol::StreamParamCopy( &m_hrRes, pCur, iMsgSize, sizeof(Result) ) );

		Proc_End:

			return hr;

		}; // Result MsgFailResultS2CEvt::ParseMessage( SF::Net::MessageData* pIMsg )

		void FailResultS2CEvt::OutputMessage(const char* Prefix)
		{
 			unused(Prefix);
			protocolTrace(Debug1, "{0}:FailResultS2CEvt: , Context:{1}, hrRes:{2:X8}",
											Prefix, m_Context, m_hrRes); 
		}; // VOID MsgFailResultS2CEvt::OutputMessage(const char* Prefix)


}; // namespace Message
}; // namespace SF


