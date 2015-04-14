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


#include "StdAfx.h"
#include "Common/Typedefs.h"
#include "Common/StrUtil.h"
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



	HRESULT Factory::InitializeDBFactory()
	{
		stm_pInstance = new DBFACTORY_CLASS;
		return S_OK;
	}

	HRESULT Factory::TerminateDBFactory()
	{
		if (stm_pInstance)
			delete stm_pInstance;

		stm_pInstance = nullptr;
		return S_OK;
	}

} // namespace DB
} // namespace BR

