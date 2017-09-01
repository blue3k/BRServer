////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : String manipulation utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/StrParserUtil.h"




namespace SF {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String parsing helper
	//

	// Pass all white spaces(space or tab) and return first non white space pointer of the input string
	char* SkipWhiteSpace(char* str, bool newLineIsWhiteSpace)
	{
		return (char*)SkipWhiteSpace((const char*)str, newLineIsWhiteSpace);
	}

	const char* SkipWhiteSpace(const char* str, bool newLineIsWhiteSpace)
	{
		if (str == nullptr) return nullptr;

		char processingChar = *str++;
		if (newLineIsWhiteSpace)
		{
			while (processingChar != '\0')
			{
				switch (processingChar)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					break;
				default:
					return str - 1;
				}
				processingChar = *str++;
			}
		}
		else
		{
			while (processingChar != '\0')
			{
				switch (processingChar)
				{
				case ' ':
				case '\t':
					break;
				default:
					return str - 1;
				}
				processingChar = *str++;
			}
		}

		return nullptr;
	}

	// Pass all non - white spaces, and return first white space pointer of the input string
	char* NextWhiteSpace(char* str, bool newLineIsWhiteSpace)
	{
		return (char*)NextWhiteSpace((const char*)str, newLineIsWhiteSpace);
	}

	const char* NextWhiteSpace(const char* str, bool newLineIsWhiteSpace)
	{
		if (str == nullptr) return nullptr;

		char processingChar = *str++;
		if (newLineIsWhiteSpace)
		{
			while (processingChar != '\0')
			{
				switch (processingChar)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					return str - 1;
				default:
					break;
				}
				processingChar = *str++;
			}
		}
		else
		{
			while (processingChar != '\0')
			{
				switch (processingChar)
				{
				case ' ':
				case '\t':
					return str - 1;
				default:
					break;
				}
				processingChar = *str++;
			}
		}

		return nullptr;

	}


};	// namespace Svr
} // namespace BR

