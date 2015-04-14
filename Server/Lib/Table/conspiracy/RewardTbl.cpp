////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : RewardTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "RewardTbl.h"



namespace conspiracy
{
 
	// ERole structure defition
	RewardTbl::ERole::EnumMap RewardTbl::ERole::m_Map;

	RewardTbl::ERole::EnumMap::EnumMap()
	{
 		m_Map.insert(std::make_pair("Villager", ERole::Enum::Villager));
		m_Map.insert(std::make_pair("Monster", ERole::Enum::Monster));
		m_Map.insert(std::make_pair("Seer", ERole::Enum::Seer));
	}

	RewardTbl::ERole::Enum RewardTbl::ERole::operator =(int i)
	{
 		m_Enum = AsEnum((Enum)i);
		return m_Enum;
	}

	RewardTbl::ERole::operator int () const
	{
 		return (int)m_Enum;
	}

	RewardTbl::ERole::operator Enum () const
	{
 		return m_Enum;
	}

	RewardTbl::ERole::Enum RewardTbl::ERole::operator =(const char* sz)
	{
 		m_Enum = AsEnum(sz);
		return m_Enum;
	}

	bool RewardTbl::ERole::operator ==(const char* sz)
	{
 		return m_Enum == AsEnum(sz);
	}

	RewardTbl::ERole::operator const char* () const
	{
 		return AsName(m_Enum);
	}

	RewardTbl::ERole::Enum RewardTbl::ERole::AsEnum(const char* sz)
	{
 		EnumMapItr itr =  m_Map.find(sz);
		if (itr == m_Map.end())
			return ERole::Enum::INVALID_ENUM;

		return itr->second;
	}

	RewardTbl::ERole::Enum RewardTbl::ERole::AsEnum(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->second;
		}
		return ERole::Enum::INVALID_ENUM;
	}

	const char* RewardTbl::ERole::AsName(Enum e)
	{
 		for (EnumMapItr itr = m_Map.begin(); itr != m_Map.end(); ++itr)
		{
 			if (itr->second == e)
				return itr->first.c_str();
		}
		return "INVALID_ENUM";
	}


	class RewardTblLoaderElement : public BR::XML::DOMElement
	{
 	public:
		RewardTblLoaderElement::RewardTblLoaderElement() : BR::XML::DOMElement("RewardTblLoaderElement")
		{
 		}
		void AddChild( DOMElement *pChild ) override
		{
 			if( pChild->GetTypeName() == "RewardItem" )
			{
 				auto* pRewardItem = dynamic_cast<RewardTbl::RewardItem*>(pChild);
				RewardTbl::m_TableMap.insert(std::make_pair(pRewardItem->Role, pRewardItem));
			}
		}
	}; // class RewardTblLoaderElement : public BR::XML::DOMElement

	// RewardTblParser decl/impl
	class RewardTblParser : public BR::XML::XMLParserHandler
	{
 	public:
		enum ATT_ID_RewardItem
		{
 			EATT_Role,
			EATT_WinExp,
			EATT_LoseExp,
			EATT_WinMoney,
			EATT_LoseMoney,
		}; // enum ATT_ID_RewardItem


	public:
		RewardTblParser()
		{
 			RegisterElementCreator( "RewardItem", []()-> BR::XML::DOMElement* { return new RewardTbl::RewardItem; } );
			RegisterElementCreator( "RewardTbl", []()-> BR::XML::DOMElement* { return new RewardTblLoaderElement; } );
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
	}; // class RewardTblParser : public BR::XML::XMLParserHandler

	RewardTbl RewardTbl::m_Instance;
	RewardTbl::TableMap RewardTbl::m_TableMap;

	HRESULT RewardTbl::LoadTable( const char *strFileName )
	{
 		RewardTblParser parser;
		if (FAILED(parser.LoadTable(strFileName)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT RewardTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT RewardTbl::FindItem( const unsigned int& Key, RewardItem*& pRow)
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

	// sub class RewardItem member implementations
	RewardTbl::RewardItem::RewardItem() : BR::XML::DOMElement("RewardItem")
	{
 	}
	bool RewardTbl::RewardItem::SetAttributeValue( const std::string& name, const std::string& value )
	{
 		if(name == "Role")
		{
 			this->Role = value.c_str();
			if (this->Role == "INVALID_ENUM") return false;
			return true;
		}
		else if(name == "WinExp")
		{
 			this->WinExp = atoi( value.c_str() );
			return true;
		}
		else if(name == "LoseExp")
		{
 			this->LoseExp = atoi( value.c_str() );
			return true;
		}
		else if(name == "WinMoney")
		{
 			this->WinMoney = atoi( value.c_str() );
			return true;
		}
		else if(name == "LoseMoney")
		{
 			this->LoseMoney = atoi( value.c_str() );
			return true;
		}
		return __super::SetAttributeValue(name,value);
	}
	void RewardTbl::RewardItem::AddChild( DOMElement *pChild )
	{
 		return __super::AddChild(pChild);
	}

}; // namespace conspiracy


