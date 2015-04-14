////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : GameConfigTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRXML.h"



namespace conspiracy
{
 	class GameConfigTbl
	{
 	public:

		// GameConfigItem structure defition
		class GameConfigItem: public BR::XML::DOMElement
		{
 		public:
			int	PresetID;
			int	DefaultMail;
			int	DefaultGem;
			int	MaxGem;
			int	DefaultMoney;
			int	MaxMoney;
			int	DefaultFriend;
			int	MaxFriend;
			int	DefaultStamina;
			int	MaxStamina;
			int	StaminaRecoveryTime;
			int	StaminaForGame;
			int	FreeDiscussion;
			int	RolePlayTime;
			int	RolePlayAndKillingTime;
			int	FirstVote;
			int	DefenceTime;
			int	SecondVote;
			int	MorningDiscussion;
			int	ResultNoticeTime;
			int	PlayerDisplayOffline;
			int	MaxCharacterSet;
			int	ForceProgressCount;
			int	PlayerAutoLogout;

			GameConfigItem();
			bool SetAttributeValue( const std::string& name, const std::string& value ) override;
			void AddChild( DOMElement *pChild ) override;
		}; // class GameConfigItem: public BR::XML::DOMElement

		typedef std::tr1::unordered_map<int, GameConfigItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static GameConfigTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const char *strFileName );
		static HRESULT ClearTable();

		static HRESULT FindItem( const int& Key, GameConfigItem*& pRow);
	}; // class GameConfigTbl
}; // namespace conspiracy


