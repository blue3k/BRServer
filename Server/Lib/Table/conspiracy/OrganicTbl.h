////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : OrganicTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"



namespace conspiracy
{
 	class OrganicTbl
	{
 	public:

		// EItemEffect enum class defition
		class EItemEffect
		{
 		public:
			enum class Enum
			{
 				ResetRankNormal,
				NickName,
				ReStart,
				Revival,
				Check,
				RoleChoice,
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
		}; // class EItemEffect

		// OrganicItem structure defition
		struct OrganicItem
		{
 			int	OrganicItemID;
			char 	ItemInfo[256];
			int	RequiredGem;
			int	RequiredGameMoney;
			EItemEffect	ItemEffect;

		}; // struct OrganicItem

		typedef std::unordered_map<unsigned int, OrganicItem*> ItemEffectTable;
		typedef ItemEffectTable::iterator ItemEffectTableItr;
		static ItemEffectTable *m_ItemEffectTable;
		static ItemEffectTable *m_ItemEffectTablePrev;



		// declare member function
		static HRESULT LoadTable( const std::list<OrganicItem>& rowList );

		static HRESULT FindItem( const unsigned int& Key, OrganicItem*& pRow);
	}; // class OrganicTbl
}; // namespace conspiracy


