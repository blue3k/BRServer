***
 
 CopyRight (c) 2020 StormForge
 
 Description : CharacterDataServer Message Protocol API Document

***



CharacterData Server Protocol

namespace SF::Message::CharacterDataServer


# Protocol interface class NetPolicyCharacterDataServer
## AddCharacterData Request
Add character data to the account

1. Command interface

        Result AddCharacterDataCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributes: VariableTable type. Character attributes

2. Result interface

C++: Cast message to AddCharacterDataRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## RemoveCharacterData Request
Remove character data

1. Command interface

        Result RemoveCharacterDataCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

2. Result interface

C++: Cast message to RemoveCharacterDataRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## GetCharacterData Request
Get character data

1. Command interface

        Result GetCharacterDataCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

2. Result interface

C++: Cast message to GetCharacterDataRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutCharacterName: StringCrc32 type. CharacterName

		- OutAttributes: VariableTable type. Character attributes


## SetAttribute Request
Set(add or update) attribute value

1. Command interface

        Result SetAttributeCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributes: VariableTable type. Character attributes

2. Result interface

C++: Cast message to SetAttributeRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## RemoveAttributes Request
Remove an attribute value

1. Command interface

        Result RemoveAttributesCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributeNames: StringCrc32 type. attributes to remove

2. Result interface

C++: Cast message to RemoveAttributesRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## AttributeValueAdd Request
Attribute add

1. Command interface

        Result AttributeValueAddCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributeName: StringCrc32 type. attributes name

		- OutInValue: FLOAT type. value to add

2. Result interface

C++: Cast message to AttributeValueAddRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## AttributeValueSub Request
Attribute subtract

1. Command interface

        Result AttributeValueSubCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const FLOAT &InValue)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributeName: StringCrc32 type. attributes name

		- OutInValue: FLOAT type. value to subtract

2. Result interface

C++: Cast message to AttributeValueSubRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## AttributeValueCAS Request
Compare and exchange attribute value

1. Command interface

        Result AttributeValueCASCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInPlayerID: PlayerID type. Owner PlayerID

		- OutInCharacterName: StringCrc32 type. CharacterName

		- OutInAttributeName: StringCrc32 type. attributes name

		- OutInAttributeType: StringCrc32 type. attributes type

		- OutInExpected: uint64 type. Expected Attribute value

		- OutInNewValue: uint64 type. Attribute value to set

2. Result interface

C++: Cast message to AttributeValueCASRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








