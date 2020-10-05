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

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSystem.h"
#include "Container/SFArray.h"


namespace SF {

	class VariableTable;

	namespace conspiracy
	{
		using namespace SF;


		class GameConfigTbl
		{
		public:

			// GameConfigItem structure definition
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
			static PresetIDTable* m_PresetIDTable;
			static PresetIDTable* m_PresetIDTablePrev;



			// declare member function
			static Result LoadTable(const Array<VariableTable>& rowList);

			static Result FindItem(const int& Key, GameConfigItem*& pRow);
		}; // class GameConfigTbl
	} // namespace conspiracy
}

