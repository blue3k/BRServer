////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Transaction/Transaction.h"
#include "Memory/SFMemoryPool.h"
#include "Container/SFArray.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"
#include "Net/SFMessage.h"
#include "Protocol/Message/CharacterDataServerMsgClass.h"
#include "Protocol/Policy/CharacterDataServerNetPolicy.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Character/CharacterDataManagerServiceEntity.h"
#include "Transaction/ServerTransaction.h"
#include "ServiceEntity/Character/CharacterDataTransaction.h"


namespace SF {
namespace Svr {


	class CharacterDataManagerTransAddCharacterData : public CharacterDataMessageTransaction<Message::CharacterDataServer::AddCharacterDataCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::AddCharacterDataCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransAddCharacterData(IHeap& heap, MessageDataPtr &pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransAddCharacterData() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransRemoveCharacterData : public CharacterDataMessageTransaction<Message::CharacterDataServer::RemoveCharacterDataCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::RemoveCharacterDataCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransRemoveCharacterData(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransRemoveCharacterData() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransGetCharacterDataList : public CharacterDataMessageTransaction<Message::CharacterDataServer::GetCharacterDataListCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::GetCharacterDataListCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransGetCharacterDataList(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransGetCharacterDataList() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};


	class CharacterDataManagerTransGetCharacterData : public CharacterDataMessageTransaction<Message::CharacterDataServer::GetCharacterDataCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::GetCharacterDataCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransGetCharacterData(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransGetCharacterData() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransSetAttribute : public CharacterDataMessageTransaction<Message::CharacterDataServer::SetAttributeCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::SetAttributeCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransSetAttribute(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransSetAttribute() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransRemoveAttribute : public CharacterDataMessageTransaction<Message::CharacterDataServer::RemoveAttributesCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::RemoveAttributesCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransRemoveAttribute(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransRemoveAttribute() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransAttributeValueAdd : public CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueAddCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueAddCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransAttributeValueAdd(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransAttributeValueAdd() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransAttributeValueSub : public CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueSubCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueSubCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransAttributeValueSub(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransAttributeValueSub() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};



	class CharacterDataManagerTransAttributeValueCAS : public CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueCASCmd>
	{
	public:
		typedef CharacterDataMessageTransaction<Message::CharacterDataServer::AttributeValueCASCmd> super;

	private:
		CharacterDataUID	m_CharacterDataUID;

	public:
		CharacterDataManagerTransAttributeValueCAS(IHeap& heap, MessageDataPtr& pIMsg) : CharacterDataMessageTransaction(heap, pIMsg) {}
		virtual ~CharacterDataManagerTransAttributeValueCAS() {}

		// Start Transaction
		virtual Result StartTransaction();


		//BR_SVR_MSGTRANS_CLOSE(Policy::NetSvrPolicyGameCharacterDataManager, CreateCharacterDataRes, RouteContext(m_CharacterDataUID,GetRouteContext().GetFrom()));
	};






	
} // namespace GameServer 
} // namespace SF 

