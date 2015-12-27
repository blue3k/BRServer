// TestApp.cpp : Defines the entry point for the console application.
//


#include "Common/Typedefs.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Common/TraceComponent.h"
#include "../TestCommon/TestBaseCommon.h"


using namespace BR;

using ::testing::InitGoogleTest;




int main(int argc, char **argv)
{
	LibComponentCarrier libComponents;

	libComponents.AddComponent<LibComponentTrace>();
	libComponents.AddComponent<Util::LibComponentTime>();
	libComponents.AddComponent<MemoryPoolManager>();

	libComponents.InitializeComponents();

	InitGoogleTest(&argc, argv);


	int iRes = RUN_ALL_TESTS();


	libComponents.TerminateComponents();

	return iRes;
}
