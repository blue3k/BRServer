////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2019 The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Session
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "DBPch.h"
#include "SFTypedefs.h"
#include "String/SFStrUtil.h"
#include "String/SFString.h"
#include "DB/DBTrace.h"
#include "DB/DataSource.h"
#include "DB/Session.h"
#include "DB/Factory.h"
#include "DB/Query.h"
#include "String/SFStringFormat.h"

namespace SF {
namespace DB {

	Query::Query(IHeap& heap, Message::MessageID MsgID)
		: Svr::TransactionResult(MsgID, Svr::TransactionResult::ResultType::DBQuery)
		, m_ParameterBinding(heap)
		, m_RowsetBinding(heap)
	{
	}

	Query::~Query()
	{
		if (m_pSession != nullptr)
			m_pSession->ReleaseSession();
		m_pSession = nullptr;
	}

	void Query::BuildQueryString(const char* spName, bool isSP)
	{
		const char* strComma = "";
		m_OutputParameterCount = 0;

		// call sp
		if (isSP)
		{
			StringBuilder builder(GetHeap());
			builder.AppendFormat("CALL {0}(", spName);
			for (auto& itBinding : m_ParameterBinding)
			{
				if ((itBinding.IOType&ParamIO::Output) == ParamIO::Output)
				{
					m_OutputParameterCount++;
					builder.AppendFormat("{0}@{1}", strComma, itBinding.Name);
				}
				else
				{
					builder.AppendFormat("{0}?", strComma);
				}

				strComma = ", ";
			}
			builder.Append(");");
			m_QueryString = builder.ToString();

			// output binding
			StringBuilder outputBuilder(GetHeap());
			if (m_OutputParameterCount > 0)
			{
				strComma = "";
				outputBuilder.Append("select ");
				for (auto& itBinding : m_ParameterBinding)
				{
					if ((itBinding.IOType&ParamIO::Output) == ParamIO::Output)
					{
						outputBuilder.AppendFormat("{0}@{1}", strComma, itBinding.Name);
						strComma = ", ";
					}
				}
				outputBuilder.Append(";");
			}

			m_QueryOutputString = outputBuilder.ToString();
		}
		else
		{
			// just append raw query
			m_QueryString = spName;
			m_QueryOutputString = "";
		}
	}




} // namespace DB
} // namespace SF
