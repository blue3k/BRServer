////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network Policy Definitions
//	
//
////////////////////////////////////////////////////////////////////////////////



#pragma once


#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Net/NetDef.h"



namespace BR {
namespace Net {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Policy base Interface
	//
	class INetPolicy : public IPolicyBase
	{
	private:
		IConnection		*m_pConnection;

		friend class IConnection;

	public:
		INetPolicy(UINT uiPolicyID, Net::IConnection *pConn);
		virtual ~INetPolicy();

		// Get Policy ID
		Net::IConnection* GetConnection() const	{ return m_pConnection; }

		// Processing function will defined at child interface
	};

} // namespace Net
} // namespace BR



