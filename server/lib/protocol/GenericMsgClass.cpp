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
#include "Common/PolicyID.h"
#include "Protocol/ProtocolMessageCPPInc.h"
#include "Net/NetDef.h"
#include "Protocol/ProtocolTrace.h"
#include "Protocol/ProtocolHelper.h"
#include "Protocol/GenericMsgClass.h"



namespace BR {
namespace Message {

		// S2C: Used for sending generial failure for not started transaction messages
		HRESULT FailResultS2CEvt::ParseIMsg( MessageData* pIMsg )
		{
 			HRESULT hr = S_OK;
			INT iMsgSize;
			BYTE* pCur;

			protocolChkPtr(pIMsg);

			iMsgSize = (UINT)pIMsg->GetMessageSize() - sizeof(MessageHeader);
			pCur = pIMsg->GetMessageData();

			if( iMsgSize == sizeof(HRESULT) ) // if result only
			{
				m_Context = 0;
			}
			else
			{
				protocolChk( Protocol::StreamParamCopy( &m_Context, pCur, iMsgSize, sizeof(Context) ) );
			}
			protocolChk( Protocol::StreamParamCopy( &m_hrRes, pCur, iMsgSize, sizeof(HRESULT) ) );

		Proc_End:

			return hr;

		}; // HRESULT MsgFailResultS2CEvt::ParseIMsg( BR::Net::MessageData* pIMsg )

		void FailResultS2CEvt::OutputMessage(const char* Prefix)
		{
 			Prefix;
			protocolTrace(Trace::TRC_DBG1, "{0}:FailResultS2CEvt: , Context:{1}, hrRes:{2:X8}",
											Prefix, m_Context, m_hrRes); 
		}; // VOID MsgFailResultS2CEvt::OutputMessage(const char* Prefix)


}; // namespace Message
}; // namespace BR


