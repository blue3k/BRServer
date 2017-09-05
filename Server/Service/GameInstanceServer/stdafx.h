// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Util/Utility.h"
#include "String/StrUtil.h"
#include "Net/Message.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Synchronization.h"
#include "Memory/SFMemory.h"
#include "String/Argument.h"
#include "String/ToString.h"
#include "String/ToStringBase.h"

#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"


#include "Transaction/Transaction.h"
#include "Entity/Entity.h"
#include "Transaction/MessageRoute.h"
#include "ServiceEntity/Game/GameSystem.h"
#include "Server/BrServer.h"
#include "Server/BrServerUtil.h"
#include "String/SvrToString.h"
#include "ServerEntity/SvrEntityServerEntity.h"
#include "ServerEntity/SvrGameServerEntity.h"
#include "ServerEntity/SvrLoopbackConnection.h"
#include "Entity/SimpleUserEntity.h"
#include "Transaction/ServerTransaction.h"
#include "ServerEntity/ServerEntity.h"
#include "Transaction/TransactionSteper.h"
#include "Transaction/TransactionUtil.h"

// TODO: reference additional headers your program requires here
