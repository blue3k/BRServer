
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
#include "SFTypedefs.h"

#include "DB/GameTransactionDB.h"
#include "DB/GameTransactionDBQuery.h"



namespace SF {
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

	void GameTransactionDB::TerminateComponent()
	{
		TerminateDB();
		Svr::IServerComponent::TerminateComponent();
	}

	
	/////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Interface
	//

	// Add game log
	Result GameTransactionDB::AddGameLog(uint shardID, const PlayerID &playerID, TimeStampSec gameTime, TransLogCategory LogCategory, INT consume, INT gain, uint64_t totalValue, const char* logMessage)
	{
		Result hr = ResultCode::SUCCESS;
		QueryAddGameLogCmd *pQuery = nullptr;

		dbMem(pQuery = new(GetMemoryManager()) QueryAddGameLogCmd);

		pQuery->SetPartitioningKey(shardID);

		pQuery->PlayerID = playerID;
		pQuery->GameTime = gameTime.time_since_epoch().count();
		pQuery->LogCategory[0] = (char)LogCategory; pQuery->LogCategory[1] = 0;
		pQuery->Consume = consume;
		pQuery->Gain = gain;
		pQuery->TotalValue = totalValue;
		StrUtil::StringCpy(pQuery->LogMessage, logMessage);
		pQuery->Result = 0;

		pQuery->SetTransaction( TransactionID() );

		dbChk( RequestQuery( pQuery ) );
		pQuery = nullptr;

	Proc_End:

		IMemoryManager::Delete(pQuery);

		return hr;
	}
	

} //namespace DB
} // namespace SF

