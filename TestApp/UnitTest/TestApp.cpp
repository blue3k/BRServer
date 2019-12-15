// TestApp.cpp : Defines the entry point for the console application.
//

#include"UnitTestPch.h"
#include <gtest/gtest.h>
#include "TestBase.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::SF;




int main(int argc, char **argv)
{
	EngineInitParam initParam;
	initParam.EnableMemoryLeakDetection = true;

	Engine::Start(initParam);


	InitGoogleTest(&argc, argv);

	UnitTest& unit_test = *UnitTest::GetInstance();

	int iRes = RUN_ALL_TESTS();

	Engine::Stop();

	return iRes;
}
