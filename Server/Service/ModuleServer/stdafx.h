// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "Common/Typedefs.h"
#include "Common/ResultCode/BRResultCodeSvr.h"
#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "Common/Message.h"
#include "Common/Trace.h"
#include "Common/Synchronization.h"
#include "Common/BrMemory.h"
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
