////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : MadK
//
// Description : Database session class
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/StackPool.h"

namespace BR {
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
		FORCEINLINE DataSource* GetDataSource()			{ return m_pDataSource; }

		FORCEINLINE bool IsOpened()						{ return m_bIsOpened; }

		// return context value
		virtual void* GetContext() = 0;

		// Send a query
		virtual HRESULT SendQuery( Query *pQuery ) = 0;

		// Open session
		virtual HRESULT OpenSession();

		// Close session
		virtual HRESULT CloseSession();

		// Release session after use, this call will return session to Data source
		HRESULT ReleaseSession();

	private:

		// Is session opened?
		bool m_bIsOpened;

		// Data source of the session
		DataSource *m_pDataSource;
	};

} // namespace DB
} // namespace BR