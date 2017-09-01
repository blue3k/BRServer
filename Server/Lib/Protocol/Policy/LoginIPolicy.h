////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2015 The Braves
// 
// Author : Generated
// 
// Description : Login Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Types/BrBaseTypes.h"
#include "Types/BrGameTypes.h"
#include "Protocol/Protocol.h"
#include "Net/NetDef.h"
#include "Net/NetPolicy.h"



namespace BR
{
 	namespace Policy
	{
 
		class ISvrPolicyLogin : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_SVR_LOGIN };

			// Constructor
			ISvrPolicyLogin( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Login request
			virtual Result LoginRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;
			// Cmd: Login request with Facebook UID
			virtual Result LoginByFacebookRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;
			// Cmd: Login request
			virtual Result CreateRandomUserRes( const Result &InResult, const NetAddress &InGameServerAddr, const NetAddress &InGameServerAddrIPV4, const AccountID &InAccID, const AuthTicket &InTicket, const uint64_t &InLoginEntityUID ) = 0;
			// Cmd: Update my score and Get Ranking list
			virtual Result UpdateMyScoreRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: Get Ranking lise
			virtual Result GetRankingListRes( const Result &InResult, const Array<TotalRankingPlayerInformation>& InRanking ) = 0;
			// Cmd: For network test
			virtual Result DataTestRes( const Result &InResult, const Array<uint8_t>& InTestData ) = 0;
			// C2S: Heartbit
			// Cmd: For network test
			virtual Result DebugPrintALLRankingRes( const Result &InResult ) = 0;

		}; // class ISvrPolicyLogin : public Net::INetPolicy


		class IPolicyLogin : public Net::INetPolicy
		{
 			public:
			enum { ID_POLICY = PROTOCOLID_LOGIN };

			// Constructor
			IPolicyLogin( Net::Connection* pConn ) : Net::INetPolicy(ID_POLICY, pConn)
			{}

			// Cmd: Login request
			virtual Result LoginCmd( const GameID &InGameID, const char* InID, const char* InPassword ) = 0;
			// Cmd: Login request with Facebook UID
			virtual Result LoginByFacebookCmd( const GameID &InGameID, const uint64_t &InUID, const char* InFaceBookName, const char* InEMail, const char* InFacebookToken ) = 0;
			// Cmd: Login request
			virtual Result CreateRandomUserCmd( const GameID &InGameID, const char* InCellPhone ) = 0;
			// Cmd: Update my score and Get Ranking list
			virtual Result UpdateMyScoreCmd( const uint64_t &InRankingScore, const RankingType &InRankingType, const uint16_t &InCount ) = 0;
			// Cmd: Get Ranking lise
			virtual Result GetRankingListCmd( const RankingType &InRankingType, const uint8_t &InBaseRanking, const uint8_t &InCount ) = 0;
			// Cmd: For network test
			virtual Result DataTestCmd( const Array<uint8_t>& InTestData ) = 0;
			// C2S: Heartbit
			virtual Result HeartBitC2SEvt(  ) = 0;
			// Cmd: For network test
			virtual Result DebugPrintALLRankingCmd( const char* InFileName ) = 0;

		}; // class IPolicyLogin : public Net::INetPolicy


	}; // namespace Policy
}; // namespace BR


