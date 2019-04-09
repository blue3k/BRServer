////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves Corporation.
// 
// Author : KyungKun Ko
//
// Description : Entity server implementation
//
////////////////////////////////////////////////////////////////////////////////



#include "ModuleServerPCH.h"
#include "Util/SFTimeUtil.h"
#include "SvrConst.h"
#include "Server/BrServer.h"
#include "SvrTrace.h"
#include "Service/ServerService.h"
#include "ServerEntity/ServerEntityManager.h"


#include "ServiceEntity/Game/PlayerManagerServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceManagerServiceEntity.h"
#include "ServiceEntity/Login/LoginServiceEntity.h"
#include "ServiceEntity/Game/GameServiceEntity.h"
#include "ServiceEntity/Game/GameInstanceServiceEntity.h"

#include "ServiceEntity/MatchingQueueServiceEntity.h"
#include "ServiceEntity/MatchingServiceEntity.h"
#include "ServiceEntity/GamePartyManagerServiceEntity.h"
#include "ServiceEntity/RankingServiceEntity.h"
#include "ServiceEntity/MonitoringServiceEntity.h"

#include "Table/TableSystem.h"

#include "Transaction/ExternalTransactionManager.h"

#include "Protocol/Policy/GameServerNetPolicy.h"
#include "Protocol/Policy/GameInstanceNetPolicy.h"


#include "Net/SFNetServerPeerTCP.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerUDP.h"
#include "Entity/EntityManager.h"


#include "SharedModuleSvrConst.h"
#include "SharedModuleServerClass.h"

#include "Transaction/SharedModuleServerTrans.h"

#include "DB/GameConspiracyDB.h"
#include "DB/AccountDB.h"






namespace SF {
namespace SharedModuleServer {



}; // namespace SharedModuleServer
}; // namespace SF







