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
#include "Common/HRESNet.h"
#include "Net/NetCtrl.h"



namespace BR {
namespace Net {

	static tag_MsgNetCtrlBuffer GetNullMsgNetCtrlBuffer()
	{
		tag_MsgNetCtrlBuffer buffer;
		memset(&buffer,0,sizeof(buffer));
		return buffer;
	}

	tag_MsgNetCtrlBuffer tag_MsgNetCtrlBuffer::NullValue = GetNullMsgNetCtrlBuffer();

} // namespace Net
} // namespace BR


