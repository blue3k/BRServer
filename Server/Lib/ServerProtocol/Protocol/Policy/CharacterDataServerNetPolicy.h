////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : Generated
// 
// Description : CharacterDataServer Message debug definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"



namespace SF
{
 	namespace Policy
	{
 		class NetPolicyCharacterDataServer 
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetPolicyCharacterDataServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetPolicyCharacterDataServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add character data to the account
			Result AddCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const char* InCharacterName, const VariableTable &InAttributes );
			// Cmd: Remove character data
			Result RemoveCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Get character list
			Result GetCharacterDataListCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Get character data
			Result GetCharacterDataCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Set(add or update) attribute value
			Result SetAttributeCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );
			// Cmd: Remove an attribute value
			Result RemoveAttributesCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames );
			// Cmd: Attribute add
			Result AttributeValueAddCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );
			// Cmd: Attribute subtract
			Result AttributeValueSubCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );
			// Cmd: Compare and exchange attribute value
			Result AttributeValueCASCmd( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue );

		}; // class NetPolicyCharacterDataServer 


		class NetSvrPolicyCharacterDataServer
		{
 			private:
				SharedPointerT<Net::Connection> m_pConnection;
			public:
			// Constructor
			NetSvrPolicyCharacterDataServer ( const SharedPointerT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}
			NetSvrPolicyCharacterDataServer ( const SharedPointerAtomicT<Net::Connection>& pConn ) : m_pConnection(pConn)
			{}

			// Cmd: Add character data to the account
			Result AddCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InCharacterUID );
			// Cmd: Remove character data
			Result RemoveCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Get character list
			Result GetCharacterDataListRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );
			// Cmd: Get character data
			Result GetCharacterDataRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );
			// Cmd: Set(add or update) attribute value
			Result SetAttributeRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Remove an attribute value
			Result RemoveAttributesRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Attribute add
			Result AttributeValueAddRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Attribute subtract
			Result AttributeValueSubRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );
			// Cmd: Compare and exchange attribute value
			Result AttributeValueCASRes( const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

		}; // class NetSvrPolicyCharacterDataServer


	}; // namespace Policy
}; // namespace SF


