////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : OrganicTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRXML.h"



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

		// OrganicItem structure defition
		class OrganicItem: public BR::XML::DOMElement
		{
 		public:
			int	OrganicItemID;
			std::string	ItemInfo;
			int	RequiredGem;
			int	RequiredGameMoney;
			EItemEffect	ItemEffect;

			OrganicItem();
			bool SetAttributeValue( const std::string& name, const std::string& value ) override;
			void AddChild( DOMElement *pChild ) override;
		}; // class OrganicItem: public BR::XML::DOMElement

		typedef std::tr1::unordered_map<unsigned int, OrganicItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static OrganicTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const char *strFileName );
		static HRESULT ClearTable();

		static HRESULT FindItem( const unsigned int& Key, OrganicItem*& pRow);
	}; // class OrganicTbl
}; // namespace conspiracy


