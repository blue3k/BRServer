////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : RewardTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRXML.h"



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
		}; // class ERole

		// RewardItem structure defition
		class RewardItem: public BR::XML::DOMElement
		{
 		public:
			ERole	Role;
			int	WinExp;
			int	LoseExp;
			int	WinMoney;
			int	LoseMoney;

			RewardItem();
			bool SetAttributeValue( const std::string& name, const std::string& value ) override;
			void AddChild( DOMElement *pChild ) override;
		}; // class RewardItem: public BR::XML::DOMElement

		typedef std::tr1::unordered_map<unsigned int, RewardItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static RewardTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const char *strFileName );
		static HRESULT ClearTable();

		static HRESULT FindItem( const unsigned int& Key, RewardItem*& pRow);
	}; // class RewardTbl
}; // namespace conspiracy


