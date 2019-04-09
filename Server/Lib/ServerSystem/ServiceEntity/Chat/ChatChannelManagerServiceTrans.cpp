////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2018 The Braves
// 
// Author : KyungKun Ko
//
// Description : Chat channel entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "Memory/SFMemoryPool.h"
#include "Types/BrSvrTypes.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"
#include "Server/BrServer.h"
#include "ServerEntity/ServerEntityManager.h"
#include "Entity/EntityManager.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceTrans.h"
#include "ServiceEntity/Chat/ChatChannelManagerServiceEntity.h"




SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelManagerTransCreateChatChannel);
SF_MEMORYPOOL_IMPLEMENT(SF::Svr::ChatChannelManagerTransChatChannelDeleted);


namespace SF {
namespace Svr {




	// Start Transaction
	Result ChatChannelManagerTransCreateChatChannel::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		svrChk( GetMyOwner()->CreateChatChannel( GetChannelName(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_UID ) );

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



};// namespace Svr 
};// namespace SF 

