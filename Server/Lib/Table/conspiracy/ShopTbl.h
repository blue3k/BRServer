////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : ShopTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRXML.h"



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
				ResetRankNormal,
				NickName,
				ReStart,
				INVALID_ENUM,
			}; // enum class Enum

		private:
			class EnumMap : public std::tr1::unordered_map<std::string, Enum>
			{
 			public:
				EnumMap();
			}; // class EnumMap : public std::tr1::unordered_map<std::string, Enum>

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

		private:
			Enum m_Enum;
			static EnumMap m_Map;
		}; // class EItemEffect

		// ShopItem structure defition
		class ShopItem: public BR::XML::DOMElement
		{
 		public:
			int	ShopItemID;
			std::string	ItemInfo;
			int	RequiredCash;
			int	RequiredGem;
			int	RequiredGameMoney;
			EItemEffect	ItemEffect;
			int	Quantity;

			ShopItem();
			bool SetAttributeValue( const std::string& name, const std::string& value ) override;
			void AddChild( DOMElement *pChild ) override;
		}; // class ShopItem: public BR::XML::DOMElement

		typedef std::tr1::unordered_map<int, ShopItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static ShopTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const char *strFileName );
		static HRESULT ClearTable();

		static HRESULT FindItem( const int& Key, ShopItem*& pRow);
	}; // class ShopTbl
}; // namespace conspiracy


