// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Common/Utility.h"
#include "String/StrUtil.h"
#include "Net/Message.h"
#include "ServerLog/SvrLog.h"
#include "Common/Synchronization.h"
#include "Memory/SFMemory.h"
#include "String/Argument.h"
#include "String/ToString.h"
#include "String/ToStringBase.h"

#include "Net/NetDef.h"
#include "Net/Connection.h"
#include "Net/NetServer.h"
#include "Net/NetServerPeer.h"

#include "ServerSystem/Transaction.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/MessageRoute.h"
#include "ServerSystem/GameSystem.h"
#include "ServerSystem/BrServer.h"
#include "ServerSystem/BrServerUtil.h"
#include "ServerSystem/SvrToString.h"
#include "ServerSystem/ServerEntity/EntityServerEntity.h"
#include "ServerSystem/ServerEntity/GameMasterServerEntity.h"
#include "ServerSystem/ServerEntity/GameServerEntity.h"
#include "ServerSystem/SvrLoopbackConnection.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "ServerSystem/ServerTransaction.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/TransactionSteper.h"
#include "ServerSystem/TransactionUtil.h"

// TODO: reference additional headers your program requires here
