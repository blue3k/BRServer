

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Util/SFUtility.h"
#include "Util/SFStrUtil.h"
#include "Util/SFToString.h"
#include "Util/SFToStringBase.h"
#include "Net/SFNetToString.h"
#include "Net/SFMessage.h"
#include "ServerLog/SvrLog.h"
#include "Multithread/SFSynchronization.h"
#include "MemoryManager/SFMemory.h"
#include <gtest/gtest.h>

#if LINUX

#define EXPECT_HRESULT_SUCCEEDED(expr) EXPECT_GE((expr),0)

#define EXPECT_HRESULT_FAILED(expr) EXPECT_LT((expr),0)

#endif


