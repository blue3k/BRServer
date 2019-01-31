

#include <gtest/gtest.h>
#include "Common/Trace.h"
#include "Common/StrUtil.h"
#include "Common/MemoryPool.h"


#if LINUX

#define EXPECT_HRESULT_SUCCEEDED(expr) EXPECT_GE((expr),0)

#define EXPECT_HRESULT_FAILED(expr) EXPECT_LT((expr),0)

#endif


