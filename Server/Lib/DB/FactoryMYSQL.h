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

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "DB/Factory.h"

namespace SF {
namespace DB {

	class DataSource;
	class Session;


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Factory Class 
	//

	class FactoryMYSQL : public Factory
	{
	public:

		FactoryMYSQL();
		virtual ~FactoryMYSQL();

		// Make this factory as the DB factory
		static Result Instanciate();

		virtual void ReportError( void* DBContext, Result hr, const char* className );

		// initialize DB source
		virtual Result	CreateDataSource(IMemoryManager& memMgr, DataSource* &pDBSource );

		// close DB source
		virtual Result	CreateSession(IMemoryManager& memMgr, DataSource* pDBSource, Session* &pSession );
	};

} // namespace DB
} // namespace SF
