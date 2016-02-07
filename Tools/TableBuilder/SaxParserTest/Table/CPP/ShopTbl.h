////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves Co.
// 
// Author : Generated
// 
// Description : ShopTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"



namespace conspiracy
{
 	class ShopTbl
	{
 	public:

		// EItemEffect enum class defition
		class EItemEffect
		{
 		public:
			enum class Enum
			{
 				Gem,
				Stamina,
				FriendSlot,
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

		// ShopItem structure defition
		struct ShopItem
		{
 			int	ShopItemID;
			char 	ItemInfo[256];
			char 	RequiredCash[256];
			int	RequiredGem;
			int	RequiredGameMoney;
			EItemEffect	ItemEffect;
			int	Quantity;
			char 	AndroidItemID[256];
			char 	iOSItemID[256];

		}; // struct ShopItem

		typedef std::unordered_map<int, ShopItem*> ShopItemIDTable;
		typedef ShopItemIDTable::iterator ShopItemIDTableItr;
		static ShopItemIDTable *m_ShopItemIDTable;
		static ShopItemIDTable *m_ShopItemIDTablePrev;



		// declare member function
		static HRESULT LoadTable( const std::list<ShopItem>& rowList );

		static HRESULT FindItem( const int& Key, ShopItem*& pRow);
	}; // class ShopTbl
}; // namespace conspiracy


