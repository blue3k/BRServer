////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : RewardTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"



namespace conspiracy
{
 	class RewardTbl
	{
 	public:

		// ERole enum class defition
		class ERole
		{
 		public:
			enum class Enum
			{
 				Villager,
				Monster,
				Seer,
				INVALID_ENUM,
			}; // enum class Enum

		private:
			class EnumMap : public std::unordered_map<std::string, Enum>
			{
 			public:
				EnumMap();
			}; // class EnumMap : public std::unordered_map<std::string, Enum>

			typedef EnumMap::iterator EnumMapItr;

		public:
			Enum operator=(int i);
			operator int () const;
			operator Enum () const;

			Enum operator=(const char* sz);
			bool operator==(const char* sz);
			operator const char* () const;

			static Enum AsEnum(const char* sz);
			static Enum AsEnum(Enum e);
			static const char* AsName(Enum e);

			union { int m_iEnum; Enum m_Enum; };
			static EnumMap m_Map;
		}; // class ERole

		// RewardItem structure defition
		struct RewardItem
		{
 			ERole	Role;
			int	WinExp;
			int	LoseExp;
			int	WinMoney;
			int	LoseMoney;

		}; // struct RewardItem

		typedef std::unordered_map<unsigned int, RewardItem*> RoleTable;
		typedef RoleTable::iterator RoleTableItr;
		static RoleTable *m_RoleTable;
		static RoleTable *m_RoleTablePrev;



		// declare member function
		static HRESULT LoadTable( const std::list<RewardItem>& rowList );

		static HRESULT FindItem( const unsigned int& Key, RewardItem*& pRow);
	}; // class RewardTbl
}; // namespace conspiracy


