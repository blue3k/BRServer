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


#include "Common/Typedefs.h"
#include "Common/PolicyID.h"


namespace BR
{
	enum tag_SvrPolicyID
	{

		POLICY_ACCOUNTDB = POLICY_NETMAX	,		// Account DB policy
		POLICY_SHRDCOORDINATORDB,					// Shard Coordinator DB policy
		POLICY_LOGINSESSIONDB,						// Login Session DB policy
		POLICY_RANKINGDB					,		// Login Session DB policy
		POLICY_GAMEDB						,		// game DB policy
		POLICY_GAMETRANSACTIONDB,					// game DB policy

		POLICY_SVR_SYSTEM		,					// entity server policy

		POLICY_MAX			,		// Max policy value
	};



}; // namespace BR



