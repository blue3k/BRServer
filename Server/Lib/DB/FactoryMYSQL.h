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
		static HRESULT Instanciate();

		virtual void ReportError( void* DBContext, HRESULT hr, const char* className );

		// initialize DB source
		virtual HRESULT	CreateDataSource( DataSource* &pDBSource );

		// close DB source
		virtual HRESULT	CreateSession( DataSource* pDBSource, Session* &pSession );
	};

} // namespace DB
} // namespace BR
