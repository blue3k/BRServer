////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2020 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : CharacterData service implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "SvrTrace.h"
#include "SvrConst.h"
#include "String/SFStrUtil.h"
#include "Server/BrServer.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Memory/SFMemory.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "String/SFStringFormat.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerEntity/ServerEntity.h"
#include "ServiceEntity/Character/CharacterData.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceTrans.h"


#include "PerformanceCounter/PerformanceCounterClient.h"

#include "Protocol/Message/CharacterDataServerMsgClass.h"




namespace SF {
namespace Svr {



	CharacterData::CharacterData(IHeap& heap, GameID gameID, PlayerID playerID, CharacterDataUID characterUID)
		: m_GameID(gameID)
		, m_PlayerID(playerID)
		, m_CharacterUID(characterUID)
		, m_Attributes(heap)
	{
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Entity informations
	//

	GlobalUIDGenerator CharacterDataManagerServiceEntity::stm_CharacterIDGenerator;

	CharacterDataManagerServiceEntity::CharacterDataManagerServiceEntity(GameID gameID, ClusterMembership initialMembership)
		: super(gameID, ClusterID::CharacterData, initialMembership )
		, m_CharacterDataMap(GetHeap())
		, m_CharacterDataCount("CharacterDataCount")
	{
		// Game CharacterData manager transactions
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::AddCharacterDataCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransAddCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::RemoveCharacterDataCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransRemoveCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::GetCharacterDataListCmd)	{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransGetCharacterDataList(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::GetCharacterDataCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransGetCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::SetAttributeCmd)			{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransSetAttribute(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::RemoveAttributesCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransRemoveAttribute(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::AttributeValueAddCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransAttributeValueAdd(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::AttributeValueSubCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransAttributeValueSub(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );
		BR_ENTITY_MESSAGE(Message::CharacterDataServer::AttributeValueCASCmd)		{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransAttributeValueCAS(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

	}

	CharacterDataManagerServiceEntity::~CharacterDataManagerServiceEntity()
	{
	}

	Result CharacterDataManagerServiceEntity::InitializeEntity(EntityID newEntityID)
	{
		FunctionContext hr = ResultCode::SUCCESS;

		auto pInstance = PerformanceCounterClient::GetDefaultCounterInstance();
		if (pInstance != nullptr)
		{
			pInstance->AddCounter(&m_CharacterDataCount);
		}

		svrCheck(super::InitializeEntity(newEntityID));

		stm_CharacterIDGenerator.SetServerID(Svr::BrServer::GetInstance()->GetServerUID());

		return hr;
	}

	Result CharacterDataManagerServiceEntity::RegisterServiceMessageHandler( ServerEntity *pServerEntity )
	{
		FunctionContext hr = ResultCode::SUCCESS;

		svrCheck(super::RegisterServiceMessageHandler( pServerEntity ) );

		// Game CharacterData manager transactions
		//pServerEntity->BR_ENTITY_MESSAGE(Message::CharacterDataServer::CreateCharacterDataCmd)						{ svrMemReturn(pNewTrans = new(GetHeap()) CharacterDataManagerTransCreateCharacterData(GetHeap(), pMsgData)); return ResultCode::SUCCESS; } );

		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Game CharacterData operations
	//

	// Add new Entity
	Result CharacterDataManagerServiceEntity::CreateCharacterData(GameID gameID, PlayerID playerID, CharacterDataUID characterUID, CharacterData*& pCharacterData)
	{
		FunctionContext hr([&pCharacterData](Result hr)
			{
				if (!hr && pCharacterData != nullptr)
				{
					delete pCharacterData;
					pCharacterData = nullptr;
				}
			});

		svrCheckMem(pCharacterData = new(GetHeap()) CharacterData(GetHeap(), gameID, playerID, characterUID));


		svrCheck(m_CharacterDataMap.Insert(characterUID, pCharacterData));
		pCharacterData = nullptr;

		++m_CharacterDataCount;

		return hr;
	}

	// Called when a game CharacterData is deleted
	Result CharacterDataManagerServiceEntity::FreeCharacterData( CharacterDataUID characterUID )
	{
		FunctionContext hr = ResultCode::SUCCESS;

		CharacterData* pCharacterData = nullptr;
		svrCheck(m_CharacterDataMap.Remove(characterUID, pCharacterData));

		--m_CharacterDataCount;

		delete pCharacterData;


		return hr;
	}







}; // namespace Svr {
}; // namespace SF {



