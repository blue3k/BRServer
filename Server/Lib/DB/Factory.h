////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : DB Factory interface
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"




/////////////////////////////////////////////////////////////////////////////
//
// DB module selection
//

//#define BRDB_USE_OLEDB
#define BRDB_USE_MYSQL



namespace BR {
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

		static HRESULT InitializeDBFactory();
		static HRESULT TerminateDBFactory();

		static Factory &GetInstance()									{ return *stm_pInstance; }

		static void ErrorLog( void* DBContext, HRESULT hr, const char* className )		{ GetInstance().ReportError(DBContext, hr,className); }

		virtual void ReportError( void* DBContext, HRESULT hr, const char* className ) = 0;

		// initialize DB source
		virtual HRESULT	CreateDataSource( DataSource* &pDBSource ) = 0;

		// close DB source
		virtual HRESULT	CreateSession( DataSource* pDBSource, Session* &pSession ) = 0;

	};

} // namespace DB
} // namespace BR
