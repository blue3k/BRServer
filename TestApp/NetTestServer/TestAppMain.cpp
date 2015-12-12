// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "../TestCommon/TestBaseCommon.h"


using namespace BR;

using ::testing::InitGoogleTest;




int main(int argc, char **argv)
{
	Trace::Initialize();

	BR::MemoryPoolManager::Initialize();

	InitGoogleTest(&argc, argv);


	int iRes = RUN_ALL_TESTS();


	Trace::Uninitialize();

	BR::MemoryPoolManager::Terminate();

	Trace::Uninitialize();

	return iRes;
}
