////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Co.
// 
// Author : Madk
// 
// Description : Table system definitions
// 
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "Common/Trace.h"
#include "TableSystem.h"
#include "conspiracy\RewardTbl.h"
#include "conspiracy\LevelTbl.h"
#include "conspiracy\ShopTbl.h"
#include "conspiracy\GameConfigTbl.h"
#include "conspiracy\OrganicTbl.h"


#define TABLE_BASEPATH "../../conspiracy/Table/"

#define LOADTABLE( TblName, PathName ) \
	if( FAILED(TblName##::LoadTable( TABLE_BASEPATH PathName )) )\
	{\
		defTrace( Trace::TRC_ERROR, #TblName " load failed!");\
		return E_FAIL;\
	}


namespace BR {
namespace conspiracy {

	// Initialize tables
	HRESULT InitializeTable()
	{
		HRESULT hr = S_OK;

		defChk(::conspiracy::OrganicTbl::LoadTable(TABLE_BASEPATH "organicTable.xml"));

		defChk(::conspiracy::RewardTbl::LoadTable(TABLE_BASEPATH "rewardTable.xml"));

		defChk(::conspiracy::LevelTbl::LoadTable(TABLE_BASEPATH "levelTable.xml") );

		defChk(::conspiracy::ShopTbl::LoadTable(TABLE_BASEPATH "shopTable.xml") );

		defChk(::conspiracy::GameConfigTbl::LoadTable(TABLE_BASEPATH "gameConfigTable.xml") );

	Proc_End:

		return hr;
	}

	// Release all tables
	HRESULT TerminateTable()
	{

		return S_OK;
	}



}; // namespace conspiracy
}



