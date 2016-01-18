﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated file, DB
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

		// Database connection failed 
		const int E_DB_INVALID_CONFIG =                                     unchecked((int)0xE7050000); 

		// Database connection failed 
		const int E_DB_CONNECTION_FAILED =                                  unchecked((int)0xE7050001); 

		// Database connection lost 
		const int E_DB_CONNECTION_LOST =                                    unchecked((int)0xE7050002); 

		// Database statement prepare is failed 
		const int E_DB_STATEMENT_PREPARE_FAILED =                           unchecked((int)0xE7050003); 

		// Database parameter binding is failed 
		const int E_DB_PARAMETER_BIND_FAILED =                              unchecked((int)0xE7050004); 

		// Database result patch is failed 
		const int E_DB_RESULT_PATCH_FAILED =                                unchecked((int)0xE7050005); 

		// Database result column count is mismatched 
		const int E_DB_RESULT_COUNT_MISMATCH =                              unchecked((int)0xE7050006); 

		// Database result column type is mismatched 
		const int E_DB_RESULT_FIELDTYPE_MISMATCH =                          unchecked((int)0xE7050007); 

		// Duplicated Database source is specified 
		const int E_DB_DUPLICATED_SOURCE =                                  unchecked((int)0xE7050008); 

		// Invalid partitioning ID 
		const int E_DB_INVALID_PARTITIONINGID =                             unchecked((int)0xE7050009); 

	}
}

