////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "SFTypedefs.h"
#include "String/SFStrUtil.h"
#include "QueryWorker.h"
#include "DBTrace.h"
#include "Factory.h"

#ifdef BRDB_USE_OLEDB
	#include "DB/FactoryOLEDB.h"
	#define DBFACTORY_CLASS FactoryOLEDB
#elif defined (BRDB_USE_MYSQL)
	#include "DB/FactoryMYSQL.h"
	#define DBFACTORY_CLASS FactoryMYSQL
#else
	#error "DB type must be specified"
#endif

namespace SF {
namespace DB {


	//static DBFACTORY_CLASS __DBFactoryInstance;

	Factory *Factory::stm_pInstance = nullptr;// &__DBFactoryInstance;



	Result Factory::InitializeDBFactory()
	{
		stm_pInstance = new(GetSystemHeap()) DBFACTORY_CLASS;
		return ResultCode::SUCCESS;
	}

	Result Factory::TerminateDBFactory()
	{
		IHeap::Delete(stm_pInstance);
		stm_pInstance = nullptr;
		return ResultCode::SUCCESS;
	}

} // namespace DB
} // namespace SF

