***
 
 CopyRight (c) 2021 StormForge
 
 Description : Server Message Protocol API Document

***



Server Generic Protocol

namespace SF::Message::Server


# Protocol interface class NetPolicyServer
## GenericFailure Request
Generic failure message

1. Command interface

        Result GenericFailureCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

2. Result interface

C++: Cast message to GenericFailureRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 


## ServerConnectedC2SEvt
Server Started or Connected

        Result ServerConnectedC2SEvt(const RouteContext &InRouteContext, const uint32_t &InStartUpTime, const NetAddress &InPrivateAddress)

		- OutInRouteContext: RouteContext type. 

		- OutInStartUpTime: uint32 type. 

		- OutInPrivateAddress: NetAddress type. 








