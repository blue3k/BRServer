// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : EngineObject manager
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "Util/SFLog.h"
#include "Service/ServerService.h"


namespace SF {



	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	data table class
	//


	DataTable::MultiRowIndex::~MultiRowIndex()
	{
		for (auto itItem : Map)
		{
			delete itItem.second;
		}
		Map.clear();
	}

	DataTable::DataTable()
	{
		assert(IHeap::CheckMemoryHeader(this));
	}

	DataTable::~DataTable()
	{
		for (auto itItem : m_MultiRowIndices)
		{
			delete itItem.second;
		}
		m_MultiRowIndices.clear();

		for (auto itRow : m_DataByRow)
		{
			delete itRow.second;
		}
		m_DataByRow.clear();
	}

	uint64_t DataTable::VaribleToKeyValue(Variable* pValue)
	{
		Result hr;
		uint64_t keyValue = 0;
		switch (pValue->GetTypeName())
		{
		case VariableBool::TYPE_NAME:
		case VariableInt::TYPE_NAME:
		case VariableUInt::TYPE_NAME:
		case VariableInt64::TYPE_NAME:
		case VariableUInt64::TYPE_NAME:
		case VariableFloat::TYPE_NAME:
		case VariableDouble::TYPE_NAME:
			keyValue = pValue->GetValueUInt64();
			break;
		case VariableString::TYPE_NAME:
		case VariableStringCrc64::TYPE_NAME:
			keyValue = pValue->GetValueStringCrc64();
			break;
		case VariableStringCrc32::TYPE_NAME:
			keyValue = pValue->GetValueStringCrc32();
			break;
		default:
			defCheck(ResultCode::INVALID_FORMAT);
		}

		return keyValue;
	}

	Result DataTable::LoadTable(StringCrc32 keyColumnName, const Array<VariableTable>& rowList)
	{
		for (auto& rowItem : rowList)
		{
			auto keyVariable = rowItem.GetVariable(keyColumnName);
			if (keyVariable == nullptr)
				return ResultCode::INVALID_FORMAT;

			uint64_t keyValue = VaribleToKeyValue(keyVariable);
			
			auto newRowData = new VariableTable;
			*newRowData = rowItem;

			m_DataByRow.insert(std::make_pair(keyValue, newRowData));
		}

		return ResultCode::SUCCESS;
	}

	Result DataTable::MultiRowIndexing(StringCrc32 columnName)
	{
		if (m_MultiRowIndices.find(columnName) != m_MultiRowIndices.end())
			return ResultCode::SUCCESS;

		MultiRowIndex* pRowIndex = new MultiRowIndex;
		pRowIndex->ColumnName = columnName;

		for (auto itRow : m_DataByRow)
		{
			auto pRow = itRow.second;

			auto keyVariable = pRow->GetVariable(columnName);
			if (keyVariable == nullptr)
				return ResultCode::INVALID_FORMAT;

			uint64_t keyValue = VaribleToKeyValue(keyVariable);

			// Get or create row set
			Array<const VariableTable*>* pRowSet{};
			auto itFound = pRowIndex->Map.find(keyValue);
			if (itFound == pRowIndex->Map.end())
			{
				pRowSet = new DynamicArray<const VariableTable*>;
				pRowIndex->Map.insert(std::make_pair(keyValue, pRowSet));
			}
			else
			{
				pRowSet = itFound->second;
			}

			// Add new item
			pRowSet->push_back(pRow);
		}

		m_MultiRowIndices.insert(std::make_pair(columnName, pRowIndex));

		return ResultCode::SUCCESS;
	}

	VariableTable* DataTable::FindRow(uint64_t key)
	{
		auto itFound = m_DataByRow.find(key);
		if (itFound != m_DataByRow.end())
			return itFound->second;

		return nullptr;
	}

	const Array<const VariableTable*>* DataTable::FindRows(StringCrc32 columnName, uint64_t key)
	{
		auto itFound = m_MultiRowIndices.find(columnName);
		if (itFound == m_MultiRowIndices.end())
			return nullptr;

		auto pMultiIndex = itFound->second;
		if (pMultiIndex == nullptr)
			return nullptr;

		auto itRowSet = pMultiIndex->Map.find(key);
		if (itRowSet == pMultiIndex->Map.end())
			return nullptr;

		return itRowSet->second;
	}



	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	data table class
	//

	DataTableManagerService::~DataTableManagerService()
	{
		for (auto itRow : m_DataTableByName)
		{
			IHeap::Delete(itRow.second);
		}
		m_DataTableByName.clear();
	}

	DataTable* DataTableManagerService::GetDataTable(StringCrc32 tableName)
	{
		auto itFound = m_DataTableByName.find(tableName);
		if (itFound != m_DataTableByName.end())
			return itFound->second;

		return nullptr;
	}

	Result DataTableManagerService::MultiRowIndexing(StringCrc32 tableName, StringCrc32 columnName)
	{
		Result hr;
		DataTable* pTable = GetDataTable(tableName);
		defCheckPtr(pTable);

		defCheck(pTable->MultiRowIndexing(columnName));

		return hr;
	}

	const VariableTable* DataTableManagerService::FindRow(StringCrc32 tableName, uint32_t key)
	{
		return FindRow(tableName, static_cast<uint64_t>(key));
	}

	const VariableTable* DataTableManagerService::FindRow(StringCrc32 tableName, uint64_t key)
	{
		auto pTable = GetDataTable(tableName);
		if (pTable == nullptr)
			return nullptr;

		auto pRow = pTable->FindRow(key);
		return pRow;
	}

	const Array<const VariableTable*>* DataTableManagerService::FindRows(StringCrc32 tableName, StringCrc32 columnName, uint64_t key)
	{
		auto pTable = GetDataTable(tableName);
		if (pTable == nullptr)
			return nullptr;

		auto pRow = pTable->FindRows(columnName, key);
		return pRow;
	}

}

