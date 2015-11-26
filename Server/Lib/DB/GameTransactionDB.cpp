
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : DB
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common/Typedefs.h"

#include "DB/GameTransactionDB.h"
#include "DB/GameTransactionDBQuery.h"



namespace BR {
namespace DB {

	
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	GameTransactionDB Class 
	//
	
	// constructor / destructor
	GameTransactionDB::GameTransactionDB()
		:Svr::IServerComponent(ComponentID)
	{
	}

	GameTransactionDB::~GameTransactionDB()
	{
	}


	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Interface
	//

	// Add game log
	HRESULT GameTransactionDB::AddGameLog(UINT shardID, const PlayerID &playerID, TimeStampSec gameTime, TransLogCategory LogCategory, INT consume, INT gain, UINT64 totalValue, const char* logMessage)
	{
		HRESULT hr = S_OK;
		QueryAddGameLogCmd *pQuery = nullptr;

		dbMem(pQuery = new QueryAddGameLogCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->GameTime = gameTime.time_since_epoch().count();
		pQuery->LogCategory[0] = (char)LogCategory; pQuery->LogCategory[1] = 0;
		pQuery->Consume = consume;
		pQuery->Gain = gain;
		pQuery->TotalValue = totalValue;
		StrUtil::StringCpy(pQuery->LogMessage, logMessage);
		pQuery->Result = 0;

		pQuery->SetTransaction( 0 );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		if( FAILED(hr) )
			Util::SafeRelease( pQuery );

		return hr;
	}
	

} //namespace DB
} // namespace BR

