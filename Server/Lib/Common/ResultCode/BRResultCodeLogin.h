﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated header, Login
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>



namespace BR {
	namespace ResultCode {

		enum {


			// Invalid username 
			E_LOGIN_INVALID_USERNAME                                           = 0xE7060000L, 

			// Invalid password 
			E_LOGIN_INVALID_PASSWORD                                           = 0xE7060001L, 

			// Already logged in 
			E_LOGIN_ALREADY_LOGGEDIN                                           = 0xE7060002L, 

			// Already logged in at another place, and failed to kick 
			E_LOGIN_ALREADY_LOGGEDIN_OTHERPLACE                                = 0xE7060003L, 

			// Invalid user signature 
			E_LOGIN_INVALID_SIGNATURE                                          = 0xE7060004L, 

		};//enum 
	}//namespace ResultCode 
}//namespace SF 



