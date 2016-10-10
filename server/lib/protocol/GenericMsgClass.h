////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Generic Message
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Net/NetDef.h"



namespace BR {
namespace Message {

		// S2C: Used for sending generial failure for not started transaction messages
		class FailResultS2CEvt : public MessageBase
		{
 		private:
			Context m_Context;
			Result m_hrRes;
		public:
			FailResultS2CEvt()
				{}

			FailResultS2CEvt( MessageData* &pMsg )
				:MessageBase(pMsg)
				{}

			const Context& GetContext() const	{ return m_Context; };
			const Result& GethrRes() const	{ return m_hrRes; };

			void OutputMessage(const char* Prefix);

			virtual Result ParseIMsg( MessageData* pIMsg );

		}; // class FailResultS2CEvt : public BR::Net::MessageBase

	
}; // namespace Message
}; // namespace BR



