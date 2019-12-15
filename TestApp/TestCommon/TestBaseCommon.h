

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeSvr.h"
#include "Util/SFUtility.h"
#include "String/SFStrUtil.h"
#include "String/SFToString.h"
#include "String/SFToStringBase.h"
#include "Net/SFNetToString.h"
#include "Net/SFMessage.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFSynchronization.h"
#include "Memory/SFMemory.h"
#include <gtest/gtest.h>

#if LINUX

#define EXPECT_HRESULT_SUCCEEDED(expr) EXPECT_GE((expr),0)

#define EXPECT_HRESULT_FAILED(expr) EXPECT_LT((expr),0)

#endif


