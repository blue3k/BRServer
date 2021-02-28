// GameInstancePCH.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Util/SFUtility.h"
#include "Util/SFStrUtil.h"
#include "Net/SFMessage.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFSynchronization.h"
#include "MemoryManager/SFMemory.h"
#include "Util/SFToString.h"
#include "Util/SFToStringBase.h"

#include "Net/SFNetDef.h"
#include "Net/SFConnection.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetServerPeerTCP.h"


#include "Transaction/Transaction.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "Util/ToStringSvr.h"
#include "ServerEntity/SvrGameServerEntity.h"
#include "ServerEntity/SvrLoopbackConnection.h"
#include "Entity/SimpleUserEntity.h"
#include "Transaction/ServerTransaction.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/TransactionSubAction.h"
#include "Transaction/TransactionUtil.h"

// TODO: reference additional headers your program requires here
