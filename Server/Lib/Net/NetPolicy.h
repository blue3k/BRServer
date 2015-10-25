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
	class INetPolicy : public BR::IPolicyBase
	{
	private:
		IConnection		*m_pConnection;

		friend class IConnection;

	public:
		INetPolicy(UINT uiPolicyID, BR::Net::IConnection *pConn);
		virtual ~INetPolicy();

		// Get Policy ID
		BR::Net::IConnection* GetConnection() const	{ return m_pConnection; }

		// Processing function will defined at child interface
	};

} // namespace Net
} // namespace BR


