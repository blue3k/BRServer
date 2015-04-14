// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "Common/Utility.h"
#include "Common/StrUtil.h"
#include "Common/Message.h"
#include "Common/Trace.h"
#include "Common/Synchronize.h"
#include "Common/Memory.h"
#include "Common/Argument.h"
#include "Common/ToString.h"
#include "Common/ToStringBase.h"
#include "Common/BrXML.h"

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
#include "ServerSystem/LoopbackConnection.h"
#include "ServerSystem/SimpleUserEntity.h"
#include "ServerSystem/ServerTransaction.h"
#include "ServerSystem/ServerEntity.h"
#include "ServerSystem/TransactionSteper.h"
#include "ServerSystem/TransactionUtil.h"


// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

// TODO: reference additional headers your program requires here
