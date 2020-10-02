////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : GameConfigTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"



namespace conspiracy
{
 	class GameConfigTbl
	{
 	public:

		// GameConfigItem structure defition
		struct GameConfigItem
		{
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
			int	MaxAutoRefilStamina;
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

		}; // struct GameConfigItem

		typedef std::unordered_map<int, GameConfigItem*> PresetIDTable;
		typedef PresetIDTable::iterator PresetIDTableItr;
		static PresetIDTable *m_PresetIDTable;
		static PresetIDTable *m_PresetIDTablePrev;



		// declare member function
		static BR::Result LoadTable( const std::list<GameConfigItem>& rowList );

		static BR::Result FindItem( const int& Key, GameConfigItem*& pRow);
	}; // class GameConfigTbl
}; // namespace conspiracy


