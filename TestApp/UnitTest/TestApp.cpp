// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest/gtest.h>
#include "TestBase.h"
#include "Common/Trace.h"
#include "Common/Memory.h"
#include "Common/MemoryPool.h"
#include "Common/TraceComponent.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using namespace ::BR;


namespace BRTest{

// Provides alternative output mode which produces minimal amount of
// information about tests.
class TersePrinter : public EmptyTestEventListener
{
 private:
  // Called before any test activity starts.
  virtual void OnTestProgramStart(const UnitTest& /* unit_test */) {}

  // Called after all test activities have ended.
  virtual void OnTestProgramEnd(const UnitTest& unit_test) {
    fprintf(stdout, "TEST %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
    fflush(stdout);
  }

  // Called before a test starts.
  virtual void OnTestStart(const TestInfo& test_info) {
    fprintf(stdout,
            "*** Test %s.%s starting.\n",
            test_info.test_case_name(),
            test_info.name());
    fflush(stdout);
  }

  // Called after a failed assertion or a SUCCESS().
  virtual void OnTestPartResult(const TestPartResult& test_part_result) {
    fprintf(stdout,
            "%s in %s:%d\n%s\n",
            test_part_result.failed() ? "*** Failure" : "Success",
            test_part_result.file_name(),
            test_part_result.line_number(),
            test_part_result.summary());
    fflush(stdout);
  }

  // Called after a test ends.
  virtual void OnTestEnd(const TestInfo& test_info) {
    fprintf(stdout,
            "*** Test %s.%s ending.\n",
            test_info.test_case_name(),
            test_info.name());
    fflush(stdout);
  }
};  // class TersePrinter


//////////////////////////////////////////////////////////////////////
//
//	MyTestBase
//

bool MyTestBase::cfg_MemLog = true;


}  // namespace

int main(int argc, char **argv)
{
	LibComponentCarrier libComponents;

	libComponents.AddComponent<LibComponentTrace>();
	libComponents.AddComponent<Util::LibComponentTime>();
	libComponents.AddComponent<MemoryPoolManager>();

	libComponents.InitializeComponents();


	InitGoogleTest(&argc, argv);

	//BR::MemoryPoolManager::Initialize();

	//BR::InitializeMemLogger( MemLog::LOG_ALL, 0 );


	bool terse_output = false;
	if (argc > 1 && strcmp(argv[1], "--terse_output") == 0 )
		terse_output = true;
	else
		printf("%s\n", "Run this program with --terse_output to change the way "
			"it prints its output.");

	UnitTest& unit_test = *UnitTest::GetInstance();

	// If we are given the --terse_output command line flag, suppresses the
	// standard output and attaches own result printer.
	if (terse_output)
	{
		TestEventListeners& listeners = unit_test.listeners();

		// Removes the default console output listener from the list so it will
		// not receive events from Google Test and won't print any output. Since
		// this operation transfers ownership of the listener to the caller we
		// have to delete it as well.
		delete listeners.Release(listeners.default_result_printer());

		// Adds the custom output listener to the list. It will now receive
		// events from Google Test and print the alternative output. We don't
		// have to worry about deleting it since Google Test assumes ownership
		// over it after adding it to the list.
		listeners.Append(new BRTest::TersePrinter);
	}

	// --gtest_catch_exceptions=0 to disable exception catch

	int iRes = RUN_ALL_TESTS();

	libComponents.TerminateComponents();


	return iRes;
}
