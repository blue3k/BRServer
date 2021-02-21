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

	DataTable::~DataTable()
	{
		for (auto itRow : m_DataByRow)
		{
			delete itRow.second;
		}
		m_DataByRow.clear();
	}

	Result DataTable::LoadTable(StringCrc32 keyColumnName, const Array<VariableTable>& rowList)
	{
		for (auto& rowItem : rowList)
		{
			auto keyVariable = rowItem.GetVariable(keyColumnName);
			if (keyVariable == nullptr)
				return ResultCode::INVALID_FORMAT;

			uint64_t keyValue = 0;
			switch (keyVariable->GetTypeName())
			{
			case VariableBool::TYPE_NAME:
			case VariableInt::TYPE_NAME:
			case VariableUInt::TYPE_NAME:
			case VariableInt64::TYPE_NAME:
			case VariableUInt64::TYPE_NAME:
			case VariableFloat::TYPE_NAME:
			case VariableDouble::TYPE_NAME:
				keyValue = keyVariable->GetValueUInt64();
				break;
			case VariableString::TYPE_NAME:
			case VariableStringCrc64::TYPE_NAME:
				keyValue = keyVariable->GetValueStringCrc64();
				break;
			case VariableStringCrc32::TYPE_NAME:
				keyValue = keyVariable->GetValueStringCrc32();
				break;
			default:
				return ResultCode::INVALID_FORMAT;
			}

			auto newRowData = new VariableTable;
			*newRowData = rowItem;

			m_DataByRow.insert(std::make_pair(keyValue, newRowData));
		}

		return ResultCode::SUCCESS;
	}

	VariableTable* DataTable::FindRow(uint64_t key)
	{
		auto itFound = m_DataByRow.find(key);
		if (itFound != m_DataByRow.end())
			return itFound->second;

		return nullptr;
	}




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	data table class
	//

	DataTableManagerService::~DataTableManagerService()
	{
		for (auto itRow : m_DataTableByName)
		{
			delete itRow.second;
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

	VariableTable* DataTableManagerService::FindRow(StringCrc32 tableName, uint32_t key)
	{
		return FindRow(tableName, static_cast<uint64_t>(key));
	}

	VariableTable* DataTableManagerService::FindRow(StringCrc32 tableName, uint64_t key)
	{
		auto pTable = GetDataTable(tableName);
		if (pTable == nullptr)
			return nullptr;

		auto pRow = pTable->FindRow(key);
		return pRow;
	}
}

