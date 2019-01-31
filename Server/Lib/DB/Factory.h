////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : DB Factory interface
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"




/////////////////////////////////////////////////////////////////////////////
//
// DB module selection
//

//#define BRDB_USE_OLEDB
#define BRDB_USE_MYSQL



namespace SF {
namespace DB {

	class DataSource;
	class Session;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Factory Class 
	//

	class Factory
	{
	protected:
		static Factory *stm_pInstance;

	public:

		Factory(){}
		virtual ~Factory(){}

		static Result InitializeDBFactory();
		static Result TerminateDBFactory();

		static Factory &GetInstance()									{ return *stm_pInstance; }

		static void ErrorLog( void* DBContext, Result hr, const char* className )		{ GetInstance().ReportError(DBContext, hr,className); }

		virtual void ReportError( void* DBContext, Result hr, const char* className ) = 0;

		// initialize DB source
		virtual Result	CreateDataSource(IHeap& memMgr, DataSource* &pDBSource ) = 0;

		// close DB source
		virtual Result	CreateSession(IHeap& memMgr, DataSource* pDBSource, Session* &pSession ) = 0;

	};

} // namespace DB
} // namespace SF
