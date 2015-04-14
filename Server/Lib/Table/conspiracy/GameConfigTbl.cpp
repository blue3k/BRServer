////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : GameConfigTbl implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "GameConfigTbl.h"



namespace conspiracy
{
 

	class GameConfigTblLoaderElement : public BR::XML::DOMElement
	{
 	public:
		GameConfigTblLoaderElement::GameConfigTblLoaderElement() : BR::XML::DOMElement("GameConfigTblLoaderElement")
		{
 		}
		void AddChild( DOMElement *pChild ) override
		{
 			if( pChild->GetTypeName() == "GameConfigItem" )
			{
 				auto* pGameConfigItem = dynamic_cast<GameConfigTbl::GameConfigItem*>(pChild);
				GameConfigTbl::m_TableMap.insert(std::make_pair(pGameConfigItem->PresetID, pGameConfigItem));
			}
		}
	}; // class GameConfigTblLoaderElement : public BR::XML::DOMElement

	// GameConfigTblParser decl/impl
	class GameConfigTblParser : public BR::XML::XMLParserHandler
	{
 	public:
		enum ATT_ID_GameConfigItem
		{
 			EATT_PresetID,
			EATT_DefaultMail,
			EATT_DefaultGem,
			EATT_MaxGem,
			EATT_DefaultMoney,
			EATT_MaxMoney,
			EATT_DefaultFriend,
			EATT_MaxFriend,
			EATT_DefaultStamina,
			EATT_MaxStamina,
			EATT_StaminaRecoveryTime,
			EATT_StaminaForGame,
			EATT_FreeDiscussion,
			EATT_RolePlayTime,
			EATT_RolePlayAndKillingTime,
			EATT_FirstVote,
			EATT_DefenceTime,
			EATT_SecondVote,
			EATT_MorningDiscussion,
			EATT_ResultNoticeTime,
			EATT_PlayerDisplayOffline,
			EATT_MaxCharacterSet,
			EATT_ForceProgressCount,
			EATT_PlayerAutoLogout,
		}; // enum ATT_ID_GameConfigItem


	public:
		GameConfigTblParser()
		{
 			RegisterElementCreator( "GameConfigItem", []()-> BR::XML::DOMElement* { return new GameConfigTbl::GameConfigItem; } );
			RegisterElementCreator( "GameConfigTbl", []()-> BR::XML::DOMElement* { return new GameConfigTblLoaderElement; } );
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
	}; // class GameConfigTblParser : public BR::XML::XMLParserHandler

	GameConfigTbl GameConfigTbl::m_Instance;
	GameConfigTbl::TableMap GameConfigTbl::m_TableMap;

	HRESULT GameConfigTbl::LoadTable( const char *strFileName )
	{
 		GameConfigTblParser parser;
		if (FAILED(parser.LoadTable(strFileName)))
			return E_FAIL;

		return S_OK;
	}

	HRESULT GameConfigTbl::ClearTable()
	{
 		for (TableMapItr itr = m_TableMap.begin(); itr != m_TableMap.end(); ++itr)
			delete itr->second;

		m_TableMap.clear();
		return S_OK;
	}

	HRESULT GameConfigTbl::FindItem( const int& Key, GameConfigItem*& pRow)
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

	// sub class GameConfigItem member implementations
	GameConfigTbl::GameConfigItem::GameConfigItem() : BR::XML::DOMElement("GameConfigItem")
	{
 	}
	bool GameConfigTbl::GameConfigItem::SetAttributeValue( const std::string& name, const std::string& value )
	{
 		if(name == "PresetID")
		{
 			this->PresetID = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefaultMail")
		{
 			this->DefaultMail = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefaultGem")
		{
 			this->DefaultGem = atoi( value.c_str() );
			return true;
		}
		else if(name == "MaxGem")
		{
 			this->MaxGem = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefaultMoney")
		{
 			this->DefaultMoney = atoi( value.c_str() );
			return true;
		}
		else if(name == "MaxMoney")
		{
 			this->MaxMoney = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefaultFriend")
		{
 			this->DefaultFriend = atoi( value.c_str() );
			return true;
		}
		else if(name == "MaxFriend")
		{
 			this->MaxFriend = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefaultStamina")
		{
 			this->DefaultStamina = atoi( value.c_str() );
			return true;
		}
		else if(name == "MaxStamina")
		{
 			this->MaxStamina = atoi( value.c_str() );
			return true;
		}
		else if(name == "StaminaRecoveryTime")
		{
 			this->StaminaRecoveryTime = atoi( value.c_str() );
			return true;
		}
		else if(name == "StaminaForGame")
		{
 			this->StaminaForGame = atoi( value.c_str() );
			return true;
		}
		else if(name == "FreeDiscussion")
		{
 			this->FreeDiscussion = atoi( value.c_str() );
			return true;
		}
		else if(name == "RolePlayTime")
		{
 			this->RolePlayTime = atoi( value.c_str() );
			return true;
		}
		else if(name == "RolePlayAndKillingTime")
		{
 			this->RolePlayAndKillingTime = atoi( value.c_str() );
			return true;
		}
		else if(name == "FirstVote")
		{
 			this->FirstVote = atoi( value.c_str() );
			return true;
		}
		else if(name == "DefenceTime")
		{
 			this->DefenceTime = atoi( value.c_str() );
			return true;
		}
		else if(name == "SecondVote")
		{
 			this->SecondVote = atoi( value.c_str() );
			return true;
		}
		else if(name == "MorningDiscussion")
		{
 			this->MorningDiscussion = atoi( value.c_str() );
			return true;
		}
		else if(name == "ResultNoticeTime")
		{
 			this->ResultNoticeTime = atoi( value.c_str() );
			return true;
		}
		else if(name == "PlayerDisplayOffline")
		{
 			this->PlayerDisplayOffline = atoi( value.c_str() );
			return true;
		}
		else if(name == "MaxCharacterSet")
		{
 			this->MaxCharacterSet = atoi( value.c_str() );
			return true;
		}
		else if(name == "ForceProgressCount")
		{
 			this->ForceProgressCount = atoi( value.c_str() );
			return true;
		}
		else if(name == "PlayerAutoLogout")
		{
 			this->PlayerAutoLogout = atoi( value.c_str() );
			return true;
		}
		return __super::SetAttributeValue(name,value);
	}
	void GameConfigTbl::GameConfigItem::AddChild( DOMElement *pChild )
	{
 		return __super::AddChild(pChild);
	}

}; // namespace conspiracy


