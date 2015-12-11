////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : ServerSystem test
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/StrFormat.h"
#include "Common/PageQueue.h"
#include "ServerSystemTest.h"
#include "Common/OrderedLinkedList.h"
#include "Common/StaticHashTable.h"
#include "ServerSystem/SvrConfig.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;

namespace BRTest
{


TEST_F(ServerSystemTest, Config)
{
	const int MAX_NUMBER	= 100;
	const int MAX_TEST		= 10000;
	std::wstring strCfgPath = L"..\\Server\\Config\\ServerConfig.xml";

	EXPECT_HRESULT_SUCCEEDED( Svr::Config::LoadConfig( strCfgPath.c_str() ) );
}




}  // namespace BRTest
