////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : OrganicTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "OrganicTbl.h"



namespace conspiracy
{
 
	// EItemEffect structure defition
	OrganicTbl::EItemEffect::EnumMap OrganicTbl::EItemEffect::m_Map;

	OrganicTbl::EItemEffect::EnumMap::EnumMap()
	{
 		m_Map.insert(std::make_pair("ResetRankNormal", EItemEffect::Enum::ResetRankNormal));
		m_Map.insert(std::make_pair("NickName", EItemEffect::Enum::NickName));
		m_Map.insert(std::make_pair("ReStart", EItemEffect::Enum::ReStart));
		m_Map.insert(std::make_pair("Revival", EItemEffect::Enum::Revival));
		m_Map.insert(std::make_pair("Check", EItemEffect::Enum::Check));
		m_Map.insert(std::make_pair("RoleChoice", EItemEffect::Enum::RoleChoice));
	}

	OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::operator =(int i)
	{
 		m_Enum = AsEnum((Enum)i);
		return m_Enum;
	}

	OrganicTbl::EItemEffect::operator int () const
	{
 		return (int)m_Enum;
	}

	OrganicTbl::EItemEffect::operator Enum () const
	{
 		return m_Enum;
	}

	OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::operator =(const char* sz)
	{
 		m_Enum = AsEnum(sz);
		return m_Enum;
	}

	bool OrganicTbl::EItemEffect::operator ==(const char* sz)
	{
 		return m_Enum == AsEnum(sz);
	}

	OrganicTbl::EItemEffect::operator const char* () const
	{
 		return AsName(m_Enum);
	}

	OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::AsEnum(const char* sz)
	{
 		EnumMapItr itr =  m_Map.find(sz);
		if (itr == m_Map.end())
			return EItemEffect::Enum::INVALID_ENUM;

		return itr->second;
	}

	OrganicTbl::EItemEffect::Enum OrganicTbl::EItemEffect::AsEnum(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->second;
		}
		return EItemEffect::Enum::INVALID_ENUM;
	}

	const char* OrganicTbl::EItemEffect::AsName(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->first.c_str();
		}
		return "INVALID_ENUM";
	}


	class OrganicTblLoaderElement : public BR::XML::DOMElement
	{
 	public:
		OrganicTblLoaderElement::OrganicTblLoaderElement() : BR::XML::DOMElement("OrganicTblLoaderElement")
		{
 		}
		void AddChild( DOMElement *pChild ) override
		{
 			if( pChild->GetTypeName() == "OrganicItem" )
			{
 				auto* pOrganicItem = dynamic_cast<OrganicTbl::OrganicItem*>(pChild);
				OrganicTbl::m_TableMap.insert(std::make_pair(pOrganicItem->ItemEffect, pOrganicItem));
			}
		}
	}; // class OrganicTblLoaderElement : public BR::XML::DOMElement

	// OrganicTblParser decl/impl
	class OrganicTblParser : public BR::XML::XMLParserHandler
	{
 	public:
		enum ATT_ID_OrganicItem
		{
 			EATT_OrganicItemID,
			EATT_ItemInfo,
			EATT_RequiredGem,
			EATT_RequiredGameMoney,
			EATT_ItemEffect,
		}; // enum ATT_ID_OrganicItem


	public:
		OrganicTblParser()
		{
 			RegisterElementCreator( "OrganicItem", []()-> BR::XML::DOMElement* { return new OrganicTbl::OrganicItem; } );
			RegisterElementCreator( "OrganicTbl", []()-> BR::XML::DOMElement* { return new OrganicTblLoaderElement; } );
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
	}; // class OrganicTblParser : public BR::XML::XMLParserHandler

	OrganicTbl OrganicTbl::m_Instance;
	OrganicTbl::TableMap OrganicTbl::m_TableMap;

	HRESULT OrganicTbl::LoadTable( const char *strFileName )
	{
 		OrganicTblParser parser;
		if (FAILED(parser.LoadTable(strFileName)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT OrganicTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT OrganicTbl::FindItem( const unsigned int& Key, OrganicItem*& pRow)
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

	// sub class OrganicItem member implementations
	OrganicTbl::OrganicItem::OrganicItem() : BR::XML::DOMElement("OrganicItem")
	{
 	}
	bool OrganicTbl::OrganicItem::SetAttributeValue( const std::string& name, const std::string& value )
	{
 		if(name == "OrganicItemID")
		{
 			this->OrganicItemID = atoi( value.c_str() );
			return true;
		}
		else if(name == "ItemInfo")
		{
 			this->ItemInfo = value;
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
		return __super::SetAttributeValue(name,value);
	}
	void OrganicTbl::OrganicItem::AddChild( DOMElement *pChild )
	{
 		return __super::AddChild(pChild);
	}

}; // namespace conspiracy


