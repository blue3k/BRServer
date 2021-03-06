////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Database session class
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Container/SFStackPool.h"

namespace SF {
namespace DB {

	class Query;
	class DataSource;

	class Session : private StackPool::Item
	{
	public:
		// constructor / destructor
		Session( DataSource* pDataSource );
		virtual ~Session() {}

		// Get data source which is linked to this session
		SF_FORCEINLINE DataSource* GetDataSource()			{ return m_pDataSource; }

		SF_FORCEINLINE bool IsOpened()						{ return m_bIsOpened; }

		// return context value
		virtual void* GetContext() = 0;

		// Send a query
		virtual Result ProcessQuery( Query *pQuery ) = 0;

		// Open session
		virtual Result OpenSession();

		// Close session
		virtual Result CloseSession();

		// Release session after use, this call will return session to Data source
		Result ReleaseSession();

		virtual Result Ping() = 0;

		virtual void OneEndUse() {}

	private:

		// Is session opened?
		bool m_bIsOpened;

		// Data source of the session
		DataSource *m_pDataSource;
	};

} // namespace DB
} // namespace SF
