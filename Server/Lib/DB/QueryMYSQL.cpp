

#include "DBPch.h"


#include "DBPch.h"
#include "SFTypedefs.h"
#include "Factory.h"

#ifdef BRDB_USE_MYSQL


#include "SFTypedefs.h"
#include "String/SFStrUtil.h"
#include "DBTrace.h"
#include "DataSourceMYSQL.h"
#include "SessionMYSQL.h"
#include "Query.h"
#include "QueryMYSQL.h"
#include "ResultCode/SFResultCodeSvr.h"



namespace SF {
	namespace DB {


		QueryMYSQL::QueryMYSQL(IHeap& heap, Message::MessageID MsgID)
			:Query(heap, MsgID)
		{
		}

		QueryMYSQL::~QueryMYSQL()
		{
		}

		void QueryMYSQL::BindParameters(mysqlx::SqlStatement& statement)
		{
			for (auto& itBinding : GetParameterBinding())
			{
				if ((itBinding.IOType & ParamIO::Output) == ParamIO::Output)
				{
				}
				else
				{
					auto pVariable = itBinding.VariableRef.GetVariable();
					switch (pVariable->GetTypeName())
					{
					case "double"_crc: statement.bind(pVariable->GetValueDouble()); break;
					case "float"_crc: statement.bind(pVariable->GetValueFloat()); break;
					case "int32_t"_crc: statement.bind(pVariable->GetValueInt32()); break;
					case "uint32_t"_crc: statement.bind(pVariable->GetValueUInt32()); break;
					case "int64_t"_crc: statement.bind(pVariable->GetValueInt64()); break;
					case "uint64_t"_crc: statement.bind(pVariable->GetValueUInt64()); break;
					case "int8_t"_crc: statement.bind(pVariable->GetValueInt8()); break;
					case "uint8_t"_crc: statement.bind(pVariable->GetValueUInt8()); break;
					case "int16_t"_crc: statement.bind(pVariable->GetValueInt16()); break;
					case "uint16_t"_crc: statement.bind(pVariable->GetValueUInt16()); break;
					case "const char*"_crc:
					case "String"_crc:
					{
						// x dev doesn't like nullptr, replace it with empty string
						auto pStrValue = pVariable->GetValueCharString();
						if (pStrValue == nullptr)
							pStrValue = "";
						statement.bind(pStrValue);
						break;
					}
					default:
						assert(false);// Unknown type
						break;
					}
				}
			}
		}

		static void AssignToSF(Variable& dest, const mysqlx::Value& src)
		{
			switch (src.getType())
			{
			case mysqlx::Value::VNULL: dest.SetValue(0); break;
			case mysqlx::Value::UINT64: dest.SetValue(src.get<uint64_t>()); break;
			case mysqlx::Value::INT64: dest.SetValue(src.get<int64_t>()); break;
			case mysqlx::Value::FLOAT: dest.SetValue(src.get<float>()); break;
			case mysqlx::Value::DOUBLE: dest.SetValue(src.get<double>()); break;
			case mysqlx::Value::BOOL: dest.SetValue(src.get<bool>()); break;
			case mysqlx::Value::STRING: dest.SetValue(src.get<std::string>().c_str()); break;
			case mysqlx::Value::RAW:
			{
				auto bytes = std::forward<mysqlx::bytes>(src.get<mysqlx::bytes>());
				dest.SetValue(ArrayView<uint8_t>(bytes.second, bytes.first));
				break;
			}
			case mysqlx::Value::DOCUMENT: // fall through
			case mysqlx::Value::ARRAY:  // fall through
			default:
				assert(false);// Unknown type
				break;
			}
		}

		static void AssignToSF(VariableTable& dest, StringCrc32 columnName, const mysqlx::Value& src)
		{
			switch (src.getType())
			{
			//case mysqlx::Value::VNULL: dest.SetValue(columnName, nullptr); break;
			case mysqlx::Value::UINT64: dest.SetValue(columnName, src.get<uint64_t>()); break;
			case mysqlx::Value::INT64: dest.SetValue(columnName, src.get<int64_t>()); break;
			case mysqlx::Value::FLOAT: dest.SetValue(columnName, src.get<float>()); break;
			case mysqlx::Value::DOUBLE: dest.SetValue(columnName, src.get<double>()); break;
			case mysqlx::Value::BOOL: dest.SetValue(columnName, src.get<bool>()); break;
			case mysqlx::Value::STRING: dest.SetValue(columnName, src.get<std::string>().c_str()); break;
			case mysqlx::Value::RAW:
			{
				auto bytes = std::forward<mysqlx::bytes>(src.get<mysqlx::bytes>());
				dest.SetValue(columnName, ArrayView<uint8_t>(bytes.second, bytes.first));
				break;
			}
			case mysqlx::Value::DOCUMENT: [[fallthrough]];// fall through
			case mysqlx::Value::ARRAY: [[fallthrough]];// fall through
			default:
				assert(false);// Unknown type
				break;
			}
		}

		void QueryMYSQL::ParseResult(mysqlx::SqlResult& queryResult)
		{
			//if (GetRowsetBinding().size() == 0)
			//	return;

			if (queryResult.count() == 0)
				return;

			RowsetType Attributes;

			DynamicArray<StringCrc32> columnNames(GetHeap());

			auto& columnInfos = queryResult.getColumns();
			for (auto& columnInfo : columnInfos)
			{
				std::string utf8Name;
				auto& columnName = columnInfo.getColumnName();
				StrUtil::WCSToUTF8(columnName, utf8Name);
				columnNames.push_back(utf8Name.c_str());
			}

			RowsetResults.resize(queryResult.count());

			for (uint iRow = 0; iRow < queryResult.count(); iRow++)
			{
				auto& rowResult = RowsetResults[iRow];
				mysqlx::Row row = queryResult.fetchOne();
				auto numColumn = Util::Min((int)columnNames.size(), (int)row.colCount());
				for (int iCol = 0; iCol < numColumn; iCol++)
				{
					mysqlx::Value& columnValue = row[iCol];
					AssignToSF(rowResult, columnNames[iCol], columnValue);
				}

				//for (auto& itBinding : GetRowsetBinding())
				//{
				//	mysqlx::Value& column = row[iOutput];
				//	auto pVariable = itBinding.VariableRef.GetVariable();
				//	AssignToSF(*pVariable, column);
				//	iOutput++;
				//}
				//AddRowset();
			}
		}

		void QueryMYSQL::ParseOutput(mysqlx::SqlResult& queryResult)
		{
			if (GetOutputParameterCount() == 0)
				return;

			mysqlx::Row row = queryResult.fetchOne();
			int iOutput = 0;
			for (auto& itBinding : GetParameterBinding())
			{
				if ((itBinding.IOType & ParamIO::Output) == ParamIO::Output)
				{
					auto& column = row[iOutput];
					auto pVariable = itBinding.VariableRef.GetVariable();
					AssignToSF(*pVariable, column);
					iOutput++;
				}
			}
		}



	} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
