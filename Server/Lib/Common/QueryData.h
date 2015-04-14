
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2010 Bluenauts
// 
// Author : YeonJoo Choe (lucy)
// Date : 11/12/2010
//
// Description : DB Management class
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
	

namespace SR 
{
	//////////////////////////////////////////////////////////////////////////////////
	//
	//	Base query data 
	//

	class BaseQuery
	{
		HRESULT	m_hResult;

	public:
		// constructor / destructor
		BaseQuery(void);
		~BaseQuery(void);

	public:
		HRESULT RequestQuery();
	};


	//////////////////////////////////////////////////////////////////////////////////
	//
	//	QueryObject Class 
	//

	template <class DataType> 
	class QueryData: public BaseQuery
	{	
		DataType m_QueryData;

	public:
		// constructor / destructor
		QueryData(void);
		~QueryData(void);

	public:
		HRESULT RequestQuery(const char* pProcedure);
	};

} // namespace SR
