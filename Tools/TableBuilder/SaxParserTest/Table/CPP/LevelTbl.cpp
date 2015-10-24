////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : LevelTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "LevelTbl.h"



namespace conspiracy
{
 

	class LevelTblLoaderElement : public BR::XML::DOMElement
	{
 	public:
		LevelTblLoaderElement::LevelTblLoaderElement() : BR::XML::DOMElement("LevelTblLoaderElement")
		{
 		}
		void AddChild( DOMElement *pChild ) override
		{
 			if( pChild->GetTypeName() == "LevelItem" )
			{
 				auto* pLevelItem = dynamic_cast<LevelTbl::LevelItem*>(pChild);
				LevelTbl::m_TableMap.insert(std::make_pair(pLevelItem->Level, pLevelItem));
			}
		}
	}; // class LevelTblLoaderElement : public BR::XML::DOMElement

	// LevelTblParser decl/impl
	class LevelTblParser : public BR::XML::XMLParserHandler
	{
 	public:
		enum ATT_ID_LevelItem
		{
 			EATT_Level,
			EATT_RequiredExpTotal,
		}; // enum ATT_ID_LevelItem


	public:
		LevelTblParser()
		{
 			RegisterElementCreator( "LevelItem", []()-> BR::XML::DOMElement* { return new LevelTbl::LevelItem; } );
			RegisterElementCreator( "LevelTbl", []()-> BR::XML::DOMElement* { return new LevelTblLoaderElement; } );
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
	}; // class LevelTblParser : public BR::XML::XMLParserHandler

	LevelTbl LevelTbl::m_Instance;
	LevelTbl::TableMap LevelTbl::m_TableMap;

	HRESULT LevelTbl::LoadTable( const char *strFileName )
	{
 		LevelTblParser parser;
		if (FAILED(parser.LoadTable(strFileName)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT LevelTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT LevelTbl::FindItem( const int& Key, LevelItem*& pRow)
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

	// sub class LevelItem member implementations
	LevelTbl::LevelItem::LevelItem() : BR::XML::DOMElement("LevelItem")
	{
 	}
	bool LevelTbl::LevelItem::SetAttributeValue( const std::string& name, const std::string& value )
	{
 		if(name == "Level")
		{
 			this->Level = atoi( value.c_str() );
			return true;
		}
		else if(name == "RequiredExpTotal")
		{
 			this->RequiredExpTotal = atoi( value.c_str() );
			return true;
		}
		return __super::SetAttributeValue(name,value);
	}
	void LevelTbl::LevelItem::AddChild( DOMElement *pChild )
	{
 		return __super::AddChild(pChild);
	}

}; // namespace conspiracy


