// TestApp.cpp : Defines the entry point for the console application.
//

#include"UnitTestPch.h"
#include <gtest/gtest.h>
#include "TestBase.h"


int main(int argc, char **argv)
{
	SF::EngineInitParam initParam;
	initParam.EnableMemoryLeakDetection = true;

	SF::Engine::Start(initParam);

	auto pEngine = SF::Engine::GetInstance();
	pEngine->AddComponent<SF::DatabaseServiceComponent>();

	InitGoogleTest(&argc, argv);

	//UnitTest& unit_test = *UnitTest::GetInstance();

	int iRes = RUN_ALL_TESTS();

	SF::Engine::Stop();

	return iRes;
}
