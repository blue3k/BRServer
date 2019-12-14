

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
			if ((itBinding.IOType&ParamIO::Output) == ParamIO::Output)
			{
			}
			else
			{
				auto pVariable = itBinding.VariableRef.GetVariable();
				switch (pVariable->GetTypeName())
				{
				case "double"_crc: statement.bind(pVariable->GetValueDouble()); break;
				case "float"_crc: statement.bind(pVariable->GetValueFloat()); break;
				case "int32"_crc: statement.bind(pVariable->GetValueInt32()); break;
				case "uint32"_crc: statement.bind(pVariable->GetValueUInt32()); break;
				case "int64"_crc: statement.bind(pVariable->GetValueInt64()); break;
				case "uint64"_crc: statement.bind(pVariable->GetValueUInt64()); break;
				case "int8"_crc: statement.bind(pVariable->GetValueInt8()); break;
				case "uint8"_crc: statement.bind(pVariable->GetValueUInt8()); break;
				case "int16"_crc: statement.bind(pVariable->GetValueInt16()); break;
				case "uint16"_crc: statement.bind(pVariable->GetValueUInt16()); break;
				case "const char*"_crc: statement.bind(pVariable->GetValueString().data()); break;
				case "String"_crc: statement.bind(pVariable->GetValueString().data()); break;
				default:
					assert(false);// Unknown type
					break;
				}
			}
		}
	}
	
	void AssignToSF(Variable& dest, const mysqlx::Value& src)
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
			dest.SetValue(ExternalBufferArray<uint8_t>(bytes.second, bytes.first));
			break;
		}
		case mysqlx::Value::DOCUMENT: // fall through
		case mysqlx::Value::ARRAY:  // fall through
		default:
			assert(false);// Unknown type
			break;
		}
	}

	void QueryMYSQL::ParseResult(mysqlx::SqlResult& queryResult)
	{
		if (GetOutputParameterCount() > 0)
		{
			mysqlx::Row row = queryResult.fetchOne();
			int iOutput = 0;
			for (auto& itBinding : GetParameterBinding())
			{
				if ((itBinding.IOType&ParamIO::Output) == ParamIO::Output)
				{
					auto& column = row[iOutput];
					auto pVariable = itBinding.VariableRef.GetVariable();
					AssignToSF(*pVariable, column);
					iOutput++;
				}
			}
		}

		if (queryResult.count() > 0)
		{
			int iOutput = 0;
			mysqlx::Row row = queryResult.fetchOne();
			for (auto& itBinding : GetRowsetBinding())
			{
				auto& column = row[iOutput];
				auto pVariable = itBinding.VariableRef.GetVariable();
				AssignToSF(*pVariable, column);
				iOutput++;
			}
			AddRowset();
		}
	}



} // namespace DB
} // namespace SF

#endif // BRDB_USE_MYSQL
