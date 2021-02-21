
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author: Kyungkun Ko
//
// Description : DB query base
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "MemoryManager/SFMemoryPool.h"
#include "DB/Factory.h"

#include "Types/BrSvrTypes.h"
#include "Transaction/Transaction.h"
#include "SvrPolicyID.h"
#include "Net/SFMessage.h"





namespace SF {
namespace DB {

	class Session;
	class DBClusterManager;

	using RowsetType = VariableTable;
	using RowsetList = DynamicArray<RowsetType>;


	enum ParamIO : uint32_t
	{
		None = 0,
		Input = 1,
		Output = 2,
		InOut = 3,

	};

	inline ParamIO operator | (ParamIO op1, ParamIO op2) { return static_cast<ParamIO>(static_cast<uint32_t>(op1) | static_cast<uint32_t>(op2)); }


	using DBString = std::string;



	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Query Base class
	//

	class Query: public Svr::TransactionResult
	{
	public:

		// Variable parameter information
		struct ParameterInfo
		{
			// Parameter name
			String Name;
			// Parameter IO type
			ParamIO IOType;
			// Reference to variable
			VariableBox VariableRef;

			ParameterInfo() = default;
			ParameterInfo(const ParameterInfo& src) = default;
			ParameterInfo(ParameterInfo&& src) = default;
			explicit ParameterInfo(const char* name, ParamIO ioType, VariableBox&& variable )
				: Name(name), IOType(ioType), VariableRef(variable)
			{
			}

			ParameterInfo& operator = (const ParameterInfo& src) = default;
		};


	public:
		Query(IHeap& heap, Message::MessageID MsgID);

		virtual ~Query();

		IHeap& GetHeap() { return m_ParameterBinding.GetHeap(); }

		SF_FORCEINLINE TimeStampMS GetRequestedTime() const { return m_RequestedTime; }
		SF_FORCEINLINE void UpdateRequestedTime()						{ m_RequestedTime = Util::Time.GetTimeMs(); }

		SF_FORCEINLINE DBClusterManager* GetQueryManager() const { return m_pQueryManager; }
		SF_FORCEINLINE void SetQueryManager(DBClusterManager* pMgr)	{ m_pQueryManager = pMgr; }

		SF_FORCEINLINE Session* GetSession() const { return m_pSession; }
		SF_FORCEINLINE void SetSession(Session* pSes) { m_pSession = pSes; }

		// Sharding ID
		SF_FORCEINLINE uint GetPartitioningKey() const					{ return m_PartitioningKey; }
		SF_FORCEINLINE void SetPartitioningKey( uint key )				{ m_PartitioningKey = key; }

		uint GetOutputParameterCount() const									{ return m_OutputParameterCount; }

		const DBString& GetQueryString() const { return m_QueryString; }
		const DBString& GetQueryOutputString() const { return m_QueryOutputString; }

	protected:

		virtual void BuildParameters() {}
		virtual void BuildQueryString(const char* spName, bool isSP = true);

		//virtual void AddRowset() { RowsetResults.push_back(std::forward<RowsetType>(Attributes)); }

		Result AddParameterBinding(const char* name, ParamIO ioType, VariableBox&& variable) { return m_ParameterBinding.push_back(ParameterInfo(name, ioType, std::forward<VariableBox>(variable))); }
		Result AddParameterBinding(ParameterInfo&& parameterInfo)	{ return m_ParameterBinding.push_back(parameterInfo); }
		const Array<ParameterInfo>& GetParameterBinding() const		{ return m_ParameterBinding; }

		Result AddRowsetBinding(ParameterInfo&& parameterInfo)		{ return m_RowsetBinding.push_back(parameterInfo); }
		const Array<ParameterInfo>& GetRowsetBinding() const		{ return m_RowsetBinding; }


	private:

		// Query manager for the query
		DBClusterManager *m_pQueryManager = nullptr;

		// Session
		Session*	m_pSession = nullptr;

		// DB shard partitioning key
		uint		m_PartitioningKey = 0;

		// Requested time stamp
		TimeStampMS	m_RequestedTime;

		// Output parameter count
		uint m_OutputParameterCount = 0;

		// Query string
		DBString m_QueryString;
		DBString m_QueryOutputString;

		// Parameter bindings info
		DynamicArray<ParameterInfo> m_ParameterBinding;

		// Rowset binding info
		DynamicArray<ParameterInfo> m_RowsetBinding;

	public: // TODO: maybe

		RowsetList RowsetResults;
	};

	
	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Query string macro
	//



	
} // namespace DB
} // namespace SF

#include "Variable/SFVariableBoxing.h"

#ifdef BRDB_USE_OLEDB
#include "DB/DBOLEDB.h"
#include "DB/QueryOLEDB.h"
#elif defined(BRDB_USE_MYSQL)
#include "DB/DBMYSQL.h"
#include "DB/QueryMYSQL.h"
#else
#error "DB type isn't specified"
#endif



namespace SF {
	namespace DB {


		/////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	Query definitions 
		//


		class QueryWholeTable : public DB::QueryBase
		{
		public:
			String m_QueryString;

		public:

			QueryWholeTable(IHeap & heap, Message::MessageID mid, const char* tableName)
				: QueryBase(heap, mid)
			{
				m_QueryString.Format("select * from {0};", tableName);

				BuildQueryString(m_QueryString, false);
			}

		};

	} // namespace DB
} // namespace SF
