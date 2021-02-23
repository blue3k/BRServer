////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Database service component
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "SFTypedefs.h"
#include "DBTrace.h"
#include "Service/ServerService.h"
#include "DB/DatabaseServiceComponent.h"

#ifdef BRDB_USE_OLEDB
	#include "DB/FactoryOLEDB.h"
#elif defined (BRDB_USE_MYSQL)
	#include "DB/FactoryMYSQL.h"
#else
	#error "DB type must be specified"
#endif

namespace SF {

#ifdef BRDB_USE_OLEDB
	using DataBaseServiceClass = DB::FactoryOLEDB;
#elif defined (BRDB_USE_MYSQL)
	using DataBaseServiceClass = DB::FactoryMYSQL;
#else
	#error "DB type must be specified"
#endif

	DatabaseServiceComponent::DatabaseServiceComponent()
		: LibraryComponent("DatabaseServiceComponent")
	{
	}

	DatabaseServiceComponent::~DatabaseServiceComponent()
	{

	}

	Result DatabaseServiceComponent::InitializeComponent()
	{
		Result hr;

		dbCheck(LibraryComponent::InitializeComponent());

		m_DBService.reset(new(GetSystemHeap()) DataBaseServiceClass);
		dbCheckPtr(m_DBService);

		Service::Database = m_DBService.get();

		return hr;
	}

	void DatabaseServiceComponent::DeinitializeComponent()
	{
		LibraryComponent::DeinitializeComponent();

		Service::Database = nullptr;
		m_DBService.reset();
	}


} // namespace SF

