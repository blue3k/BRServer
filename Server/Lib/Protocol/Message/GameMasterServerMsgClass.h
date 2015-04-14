////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : GameMasterServer Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Common/PolicyID.h"
#include "Common/Message.h"
#include "Common/BrSvrTypes.h"
#include "Common/BrGameTypes.h"



namespace BR
{
 	namespace Message
	{
 		namespace GameMasterServer
		{
 			// C2S: Player entered
			class PlayerEnteredC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				AccountID m_PlayerID;
				const char* m_PlayerName;
			public:
				PlayerEnteredC2SEvt()
				:m_PlayerName(nullptr)
					{}

				PlayerEnteredC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
				,m_PlayerName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const AccountID& GetPlayerID() const	{ return m_PlayerID; };
				const char* GetPlayerName() const	{ return m_PlayerName; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID, const char* InPlayerName );

			}; // class PlayerEnteredC2SEvt : public MessageBase

			// C2S: Player leaved
			class PlayerLeavedC2SEvt : public MessageBase
			{
 			public:
				static const MessageID MID;
			private:
				AccountID m_PlayerID;
			public:
				PlayerLeavedC2SEvt()
					{}

				PlayerLeavedC2SEvt( MessageData* &pMsg )
					:MessageBase(pMsg)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const AccountID& GetPlayerID() const	{ return m_PlayerID; };

				void TraceOut(const char* Prefix, MessageData* pMsg);

				virtual HRESULT ParseIMsg( IN MessageData* pIMsg );

				static HRESULT BuildIMsg( OUT MessageData* &pMsg, const AccountID &InPlayerID );

			}; // class PlayerLeavedC2SEvt : public MessageBase





		}; // namespace GameMasterServer
	}; // namespace Message
}; // namespace BR


