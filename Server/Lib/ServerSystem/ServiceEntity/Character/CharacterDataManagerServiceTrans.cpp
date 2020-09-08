////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 The Braves
// 
// Author : KyungKun Ko
//
// Description : character data manager implementation
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
#include "ServiceEntity/Character/CharacterDataManagerServiceTrans.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"





namespace SF {
namespace Svr {




	// Start Transaction
	Result CharacterDataManagerTransAddCharacterData::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk( super::StartTransaction() );

		//svrChk( GetMyOwner()->CreateGameCharacterData( (GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID ) );

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}


	// Start Transaction
	Result CharacterDataManagerTransRemoveCharacterData::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result CharacterDataManagerTransGetCharacterData::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result CharacterDataManagerTransSetAttribute::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}



	// Start Transaction
	Result CharacterDataManagerTransRemoveAttribute::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	Result CharacterDataManagerTransAttributeValueAdd::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	Result CharacterDataManagerTransAttributeValueSub::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




	// Start Transaction
	Result CharacterDataManagerTransAttributeValueCAS::StartTransaction()
	{
		Result hr = ResultCode::SUCCESS;

		svrChk(super::StartTransaction());

		//svrChk(GetMyOwner()->CreateGameCharacterData((GameID)GetGameID(), GetCreator(), GetRouteContext().GetFrom(), GetServerEntity<ServerEntity>(), m_CharacterDataUID));

	Proc_End:

		CloseTransaction(hr);

		return hr;
	}




};// namespace Svr 
};// namespace SF 

