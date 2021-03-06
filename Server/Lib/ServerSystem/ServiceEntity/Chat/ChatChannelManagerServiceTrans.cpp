////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Chat channel entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceTrans.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"






namespace SF {
namespace Svr {




	// Start Transaction
	Result ChatChannelManagerTransCreateChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateChatChannel( GetChannelName(), GetCreator(), GetRouteContext().GetFrom(), GetRemoteEndpoint(), m_UID ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result ChatChannelManagerTransFindChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FindChatChannel(GetChannelName(), m_UID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result ChatChannelManagerTransChatChannelDeleted::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		svrChk(GetMyOwner()->FreeChatChannel(GetRouteContext().GetFrom()));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



}// namespace Svr 
}// namespace SF 

