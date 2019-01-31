***
 
 CopyRight (c) 2018 StormForge
 
 Description : Monitoring Message Protocol API Document

***



Monitoring Server Protocol

namespace SF::Message::Monitoring


# Protocol interface class NetPolicyMonitoring
## GetInstanceList Request
[deprecated]

1. Command interface

        Result GetInstanceListCmd(const TransactionID &InTransactionID)

		- OutInTransactionID: TransactionID type. 

2. Result interface

C++: Cast message to GetInstanceListRes to access values


		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutCounterInstances: PerformanceCounterInstanceInfo type. 

		- OutTotalInstanceCount: uint32 type. 


## RequestCounterValues Request
[deprecated]

1. Command interface

        Result RequestCounterValuesCmd(const TransactionID &InTransactionID, const uint64_t &InInstanceUID)

		- OutInTransactionID: TransactionID type. 

		- OutInInstanceUID: EntityUID type. 

2. Result interface

C++: Cast message to RequestCounterValuesRes to access values


		- OutTransactionID: TransactionID type. 

		- OutResult: Result type. 

		- OutInstanceUID: EntityUID type. 

		- OutCounterValues: uint64 type. 


## PerformanceCounterNewC2SEvt
Message when new performance counter is added.

        Result PerformanceCounterNewC2SEvt(const char* InInstanceName, const uint64_t &InInstanceUID, const Array<PerformanceCounterInfo>& InNewCounters)

		- OutInInstanceName: String type. 

		- OutInInstanceUID: EntityUID type. 

		- OutInNewCounters: PerformanceCounterInfo type. 


## PerformanceCounterFreeC2SEvt
Counter instance is deleted

        Result PerformanceCounterFreeC2SEvt(const Array<uint64_t>& InFreeInstances)

		- OutInFreeInstances: EntityUID type. 


## PerformanceCounterUpdateC2SEvt
Counter update broadcast

        Result PerformanceCounterUpdateC2SEvt(const uint64_t &InInstanceUID, const Array<uint64_t>& InCounterValues)

		- OutInInstanceUID: EntityUID type. 

		- OutInCounterValues: uint64 type. 


## PerformanceCounterUpdateCounterInfoS2CEvt
Server will send this message to an instance to get performance counters.

        Result PerformanceCounterUpdateCounterInfoS2CEvt(const uint64_t &InInstanceUID)

		- OutInInstanceUID: EntityUID type. 








