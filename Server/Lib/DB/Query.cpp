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


namespace SF {
namespace DB {

	Query::Query(IHeap& heap, Message::MessageID MsgID)
		: Svr::TransactionResult(MsgID)
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

	void Query::BuildQueryString(const char* spName)
	{
		const char* strComma = "";
		StringBuilder builder(GetHeap());
		m_OutputParameterCount = 0;

		// call sp
		builder.AppendFormat("{0}(", spName);
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
		builder.Append(");\n");

		// output binding
		if (m_OutputParameterCount > 0)
		{
			strComma = "";
			builder.Append("select ");
			for (auto& itBinding : m_ParameterBinding)
			{
				if ((itBinding.IOType&ParamIO::Output) == ParamIO::Output)
				{
					builder.AppendFormat("{0}@{1}", strComma, itBinding.Name);
					strComma = ", ";
				}
			}
			builder.Append(");\n");
		}

		//String Password;
		//ParameterInfo info("Password", ParamIO::Input, BoxingByReference(Password));
		//AddParameterBinding(std::forward<ParameterInfo>(info));
		m_QueryString = builder.ToString();
	}




} // namespace DB
} // namespace SF
