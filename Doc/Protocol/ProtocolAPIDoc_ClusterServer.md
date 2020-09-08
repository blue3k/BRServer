***
 
 CopyRight (c) 2020 StormForge
 
 Description : ClusterServer Message Protocol API Document

***



Server Generic Protocol

namespace SF::Message::ClusterServer


# Protocol interface class NetPolicyClusterServer
## GetClusterMemberList Request
Cluster member list query

1. Command interface

        Result GetClusterMemberListCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInClusterID: ClusterID type. 

2. Result interface

C++: Cast message to GetClusterMemberListRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutMemberList: ServiceInformation type. 


## RequestDataSync Request
Join to the cluster

1. Command interface

        Result RequestDataSyncCmd(const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const ClusterID &InClusterID)

		- OutInRouteContext: RouteContext type. 

		- OutInTransactionID: TransactionID type. 

		- OutInRouteHopCount: uint16 type. 

		- OutInClusterID: ClusterID type. 

2. Result interface

C++: Cast message to RequestDataSyncRes to access values


		- OutRouteContext: RouteContext type. 

		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 








