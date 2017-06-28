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
#include "DB/Factory.h"

namespace BR {
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
		virtual Result	CreateDataSource( DataSource* &pDBSource );

		// close DB source
		virtual Result	CreateSession( DataSource* pDBSource, Session* &pSession );
	};

} // namespace DB
} // namespace BR
