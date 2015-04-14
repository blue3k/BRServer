////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : ShopTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "ShopTbl.h"



namespace conspiracy
{
 
	// EItemEffect structure defition
	ShopTbl::EItemEffect::EnumMap ShopTbl::EItemEffect::m_Map;

	ShopTbl::EItemEffect::EnumMap::EnumMap()
	{
 		m_Map.insert(std::make_pair("Gem", EItemEffect::Enum::Gem));
		m_Map.insert(std::make_pair("Stamina", EItemEffect::Enum::Stamina));
		m_Map.insert(std::make_pair("FriendSlot", EItemEffect::Enum::FriendSlot));
		m_Map.insert(std::make_pair("ResetRankNormal", EItemEffect::Enum::ResetRankNormal));
		m_Map.insert(std::make_pair("NickName", EItemEffect::Enum::NickName));
		m_Map.insert(std::make_pair("ReStart", EItemEffect::Enum::ReStart));
	}

	ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::operator =(int i)
	{
 		m_Enum = AsEnum((Enum)i);
		return m_Enum;
	}

	ShopTbl::EItemEffect::operator int () const
	{
 		return (int)m_Enum;
	}

	ShopTbl::EItemEffect::operator Enum () const
	{
 		return m_Enum;
	}

	ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::operator =(const char* sz)
	{
 		m_Enum = AsEnum(sz);
		return m_Enum;
	}

	bool ShopTbl::EItemEffect::operator ==(const char* sz)
	{
 		return m_Enum == AsEnum(sz);
	}

	ShopTbl::EItemEffect::operator const char* () const
	{
 		return AsName(m_Enum);
	}

	ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::AsEnum(const char* sz)
	{
 		EnumMapItr itr =  m_Map.find(sz);
		if (itr == m_Map.end())
			return EItemEffect::Enum::INVALID_ENUM;

		return itr->second;
	}

	ShopTbl::EItemEffect::Enum ShopTbl::EItemEffect::AsEnum(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->second;
		}
		return EItemEffect::Enum::INVALID_ENUM;
	}

	const char* ShopTbl::EItemEffect::AsName(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->first.c_str();
		}
		return "INVALID_ENUM";
	}


	class ShopTblLoaderElement : public BR::XML::DOMElement
	{
 	public:
		ShopTblLoaderElement::ShopTblLoaderElement() : BR::XML::DOMElement("ShopTblLoaderElement")
		{
 		}
		void AddChild( DOMElement *pChild ) override
		{
 			if( pChild->GetTypeName() == "ShopItem" )
			{
 				auto* pShopItem = dynamic_cast<ShopTbl::ShopItem*>(pChild);
				ShopTbl::m_TableMap.insert(std::make_pair(pShopItem->ShopItemID, pShopItem));
			}
		}
	}; // class ShopTblLoaderElement : public BR::XML::DOMElement

	// ShopTblParser decl/impl
	class ShopTblParser : public BR::XML::XMLParserHandler
	{
 	public:
		enum ATT_ID_ShopItem
		{
 			EATT_ShopItemID,
			EATT_ItemInfo,
			EATT_RequiredCash,
			EATT_RequiredGem,
			EATT_RequiredGameMoney,
			EATT_ItemEffect,
			EATT_Quantity,
		}; // enum ATT_ID_ShopItem


	public:
		ShopTblParser()
		{
 			RegisterElementCreator( "ShopItem", []()-> BR::XML::DOMElement* { return new ShopTbl::ShopItem; } );
			RegisterElementCreator( "ShopTbl", []()-> BR::XML::DOMElement* { return new ShopTblLoaderElement; } );
		}

		HRESULT LoadTable( const char *strFileName )
		{
 			int result = xmlSAXUserParseFile( *this, this, strFileName );

			if (result != 0)
			{
 				// error log
				return E_FAIL;
			}
			xmlCleanupParser();
			return S_OK;
		}
	}; // class ShopTblParser : public BR::XML::XMLParserHandler

	ShopTbl ShopTbl::m_Instance;
	ShopTbl::TableMap ShopTbl::m_TableMap;

	HRESULT ShopTbl::LoadTable( const char *strFileName )
	{
 		ShopTblParser parser;
		if (FAILED(parser.LoadTable(strFileName)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT ShopTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT ShopTbl::FindItem( const int& Key, ShopItem*& pRow)
	{
 		TableMapItr itr = m_TableMap.find(Key);
		if (itr == m_TableMap.end())
		{
 			// write error log
			return E_FAIL;
		}
		pRow = itr->second;
		return S_OK;
	}

	// sub class ShopItem member implementations
	ShopTbl::ShopItem::ShopItem() : BR::XML::DOMElement("ShopItem")
	{
 	}
	bool ShopTbl::ShopItem::SetAttributeValue( const std::string& name, const std::string& value )
	{
 		if(name == "ShopItemID")
		{
 			this->ShopItemID = atoi( value.c_str() );
			return true;
		}
		else if(name == "ItemInfo")
		{
 			this->ItemInfo = value;
			return true;
		}
		else if(name == "RequiredCash")
		{
 			this->RequiredCash = atoi( value.c_str() );
			return true;
		}
		else if(name == "RequiredGem")
		{
 			this->RequiredGem = atoi( value.c_str() );
			return true;
		}
		else if(name == "RequiredGameMoney")
		{
 			this->RequiredGameMoney = atoi( value.c_str() );
			return true;
		}
		else if(name == "ItemEffect")
		{
 			this->ItemEffect = value.c_str();
			if (this->ItemEffect == "INVALID_ENUM") return false;
			return true;
		}
		else if(name == "Quantity")
		{
 			this->Quantity = atoi( value.c_str() );
			return true;
		}
		return __super::SetAttributeValue(name,value);
	}
	void ShopTbl::ShopItem::AddChild( DOMElement *pChild )
	{
 		return __super::AddChild(pChild);
	}

}; // namespace conspiracy


