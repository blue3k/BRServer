////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Net message implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "ResultCode/SFResultCodeNet.h"
#include "Net/NetCtrl.h"



namespace BR {
namespace Net {

	tag_MsgNetCtrlBuffer GetNullMsgNetCtrlBuffer()
	{
		tag_MsgNetCtrlBuffer buffer;
		memset(&buffer,0,sizeof(buffer));
		return buffer;
	}

} // namespace Net
} // namespace BR


