﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated file, Login
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;


namespace BR {
	public partial struct ResultCode {

		// Invalid username 
		const uint E_LOGIN_INVALID_USERNAME =                               unchecked(0xE7060000); 

		// Invalid password 
		const uint E_LOGIN_INVALID_PASSWORD =                               unchecked(0xE7060001); 

		// Already logged in 
		const uint E_LOGIN_ALREADY_LOGGEDIN =                               unchecked(0xE7060002); 

		// Already logged in at another place, and failed to kick 
		const uint E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE =                    unchecked(0xE7060003); 

		// Invalid user signature 
		const uint E_LOGIN_INVALID_SIGNATURE =                              unchecked(0xE7060004); 

	}
}

