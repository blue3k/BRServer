////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2014 The Braves Co.
// 
// Author : Generated
// 
// Description : LevelTbl  definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/Typedefs.h"
#include "Common/BRXML.h"



namespace conspiracy
{
 	class LevelTbl
	{
 	public:

		// LevelItem structure defition
		class LevelItem: public BR::XML::DOMElement
		{
 		public:
			int	Level;
			int	RequiredExpTotal;

			LevelItem();
			bool SetAttributeValue( const std::string& name, const std::string& value ) override;
			void AddChild( DOMElement *pChild ) override;
		}; // class LevelItem: public BR::XML::DOMElement

		typedef std::tr1::unordered_map<int, LevelItem*> TableMap;
		typedef TableMap::iterator TableMapItr;
		static TableMap m_TableMap;
		static LevelTbl m_Instance;



		// declare member function
		static HRESULT LoadTable( const char *strFileName );
		static HRESULT ClearTable();

		static HRESULT FindItem( const int& Key, LevelItem*& pRow);
	}; // class LevelTbl
}; // namespace conspiracy


