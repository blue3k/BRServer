////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Procees Requested DB Query
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "SFTypedefs.h"
#include "String/StrUtil.h"
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

namespace BR {
namespace DB {


	//static DBFACTORY_CLASS __DBFactoryInstance;

	Factory *Factory::stm_pInstance = nullptr;// &__DBFactoryInstance;



	Result Factory::InitializeDBFactory()
	{
		stm_pInstance = new DBFACTORY_CLASS;
		return ResultCode::SUCCESS;
	}

	Result Factory::TerminateDBFactory()
	{
		if (stm_pInstance)
			delete stm_pInstance;

		stm_pInstance = nullptr;
		return ResultCode::SUCCESS;
	}

} // namespace DB
} // namespace BR

