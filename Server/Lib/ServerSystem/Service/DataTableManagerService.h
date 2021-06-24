////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Server cluster service
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Util/SFStringCrcDB.h"
#include "Variable/SFVariableTable.h"



namespace SF {

	class VariableTable;

	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	data table class
	//

	class DataTable
	{
	private:

		struct MultiRowIndex
		{
			MultiRowIndex() {}
			~MultiRowIndex();

			StringCrc32 ColumnName;
			std::unordered_map<uint64_t, Array<const VariableTable*>*> Map;
		};

		std::unordered_map<uint64_t, VariableTable*> m_DataByRow;

		std::unordered_map<uint32_t, MultiRowIndex*> m_MultiRowIndices;

	private:

		uint64_t VaribleToKeyValue(Variable* pValue);

	public:

		DataTable();
		virtual ~DataTable();

		Result LoadTable(StringCrc32 keyColumnName, const Array<VariableTable>& rowList);

		Result MultiRowIndexing(StringCrc32 columnName);

		VariableTable* FindRow(uint64_t key);
		const Array<const VariableTable*>* FindRows(StringCrc32 columnName, uint64_t key);

		auto begin() { return m_DataByRow.begin(); }
		auto end() { return m_DataByRow.end(); }
	};



	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	data table manager service
	//

	class DataTableManagerService
	{
	protected:

		std::unordered_map<uint32_t, DataTable*> m_DataTableByName;

	public:

		DataTableManagerService() = default;
		virtual ~DataTableManagerService();

		// Load data table
		virtual Result LoadDataTable(const char* tableName, StringCrc32 keyColumnName, const char* tableSrcPath) { return ResultCode::NOT_IMPLEMENTED; }

		// Get data table
		virtual DataTable* GetDataTable(StringCrc32 tableName);

		virtual Result MultiRowIndexing(StringCrc32 tableName, StringCrc32 columnName);

		virtual const VariableTable* FindRow(StringCrc32 tableName, uint32_t key);
		virtual const VariableTable* FindRow(StringCrc32 tableName, uint64_t key);

		virtual const Array<const VariableTable*>* FindRows(StringCrc32 tableName, StringCrc32 columnName, uint64_t key);
	};


} // namespace SF

