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
#include "Common/SharedObj.h"


namespace BR
{
	typedef enum tag_PolicyID
	{
		// Policy 
		POLICY_NONE						= 0,			// Not defined policy

		POLICY_LOGIN					,
		POLICY_SVR_LOGIN				,

		POLICY_GAME						,				// Game policy
		POLICY_SVR_GAME					,				// Game policy

		POLICY_SERVER					,				// server generic policy
		POLICY_SVR_SERVER				,				// server generic policy

		POLICY_LOGINSERVER				,
		POLICY_SVR_LOGINSERVER			,

		POLICY_GAMESERVER				,				// Game server policy
		POLICY_SVR_GAMESERVER			,				// Game server policy

		POLICY_GAMEPARTY				,				// Game party server policy
		POLICY_SVR_GAMEPARTY			,				// Game party server policy

		POLICY_CLUSTERSERVER			,
		POLICY_SVR_CLUSTERSERVER		,

		POLICY_MONITORING,
		POLICY_SVR_MONITORING,

		//////////////////////////////////////////////////////////////////////////
		//
		//	Game cluster
		//

		POLICY_GAMEMASTERSERVER			,
		POLICY_SVR_GAMEMASTERSERVER		,

		POLICY_ENTITYSERVER				,				// entity server policy
		POLICY_SVR_ENTITYSERVER			,				// entity server policy

		POLICY_PARTYMATCHING			,
		POLICY_SVR_PARTYMATCHING		,

		POLICY_PARTYMATCHINGQUEUE		,
		POLICY_SVR_PARTYMATCHINGQUEUE	,

		POLICY_GAMEINSTANCE				,
		POLICY_SVR_GAMEINSTANCE			,

		POLICY_GAMEINSTANCEMANAGER		,
		POLICY_SVR_GAMEINSTANCEMANAGER	,

		POLICY_GAMEPARTYMANAGER			,
		POLICY_SVR_GAMEPARTYMANAGER		,

		POLICY_RANKING,
		POLICY_SVR_RANKING,

		POLICY_NETMAX					,				// Max Network policy value
		POLICY_CLIENTNETMAX = POLICY_SVR_GAME+1,

	} PolicyID;


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Policy base Interface
	//
	class IPolicyBase 
	{
	private:
		// Policy ID
		UINT32	m_uiPolicyID;

	public:
		IPolicyBase( UINT uiPolicyID ) : m_uiPolicyID(uiPolicyID) {}
		virtual ~IPolicyBase() {}

		// Get Policy ID
		UINT32 GetPolicyID() const	{ return m_uiPolicyID; }
	};

}; // namespace BR



