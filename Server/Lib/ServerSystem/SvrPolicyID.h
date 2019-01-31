////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Policy Definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"


namespace SF
{
	enum tag_SvrPolicyID
	{

		PROTOCOLID_ACCOUNTDB = PROTOCOLID_NETMAX	,		// Account DB policy
		PROTOCOLID_SHRDCOORDINATORDB,					// Shard Coordinator DB policy
		PROTOCOLID_LOGINSESSIONDB,						// Login Session DB policy
		PROTOCOLID_RANKINGDB					,		// Login Session DB policy
		PROTOCOLID_GAMEDB						,		// game DB policy
		PROTOCOLID_GAMETRANSACTIONDB,					// game DB policy
		PROTOCOLID_TABLEDB,								// Table DB policy

		PROTOCOLID_SVR_SYSTEM		,					// entity server policy

		PROTOCOLID_MAX			,		// Max policy value
	};



}; // namespace SF



