////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : String Format utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSystem.h"


namespace BR {
namespace StrUtil {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	String parsing helper
	//

	// Pass all white spaces(space or tab) and return first non white space pointer of the input string
	const char* SkipWhiteSpace(const char* str, bool newLineIsWhiteSpace = true);
	char* SkipWhiteSpace(char* str, bool newLineIsWhiteSpace = true);

	// Pass all non - white spaces, and return first white space pointer of the input string
	const char* NextWhiteSpace(const char* str, bool newLineIsWhiteSpace = true);
	char* NextWhiteSpace(char* str, bool newLineIsWhiteSpace = true);


} // namespace StrUtil
} // namespace BR


