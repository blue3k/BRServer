////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : CharacterDataServer Service definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "ServerService/ServerServiceBase.h"
#include "Net/SFNetDef.h"
#include "Protocol/Policy/CharacterDataServerNetPolicy.h"



namespace SF
{
 	namespace Svr
	{
 		class CharacterDataServerService : public ServerServiceBase
		{
 			public:
			// Constructor
			CharacterDataServerService ( ServerServiceInformation* pService );

			// Cmd: Add character data to the account
			Result AddCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const char* InCharacterName, const VariableTable &InAttributes );
			// Cmd: Remove character data
			Result RemoveCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Get character list
			Result GetCharacterDataListCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Get character data
			Result GetCharacterDataCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );
			// Cmd: Set(add or update) attribute value
			Result SetAttributeCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );
			// Cmd: Remove an attribute value
			Result RemoveAttributesCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames );
			// Cmd: Attribute add
			Result AttributeValueAddCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );
			// Cmd: Attribute subtract
			Result AttributeValueSubCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );
			// Cmd: Compare and exchange attribute value
			Result AttributeValueCASCmd( const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue );

		}; // class CharacterDataServerService : public ServerServiceBase


	}; // namespace Svr
}; // namespace SF


