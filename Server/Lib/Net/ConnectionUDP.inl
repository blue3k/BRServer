////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves.
// 
// Author : KyungKun Ko
//
// Description : Net connection inline implementation. 
//	
//
////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
//
//	UDP Connection class
//


// Set maximum guaranted retry count
void ConnectionUDPBase::SetMaxGuarantedRetry( UINT uiMaxGuarantedRetry )
{
	m_uiMaxGuarantedRetry = uiMaxGuarantedRetry;
}


////////////////////////////////////////////////////////////////////////////////
//
//	Client UDP Connection class
//

IOBUFFER_READ* ConnectionUDPClient::GetRecvBuffer()
{
	return &m_RecvBuffer;
}



