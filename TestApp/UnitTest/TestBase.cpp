// TestApp.cpp : Defines the entry point for the console application.
//

#include"UnitTestPch.h"
#include "SFTypedefs.h"
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



// --gtest_catch_exceptions=0 --gtest_break_on_failure to disable exception catch and stop on failure
//	--gtest_filter = ThreadTest.EntityTaskManager --gtest_catch_exceptions=0

//./foo_test --gtest_filter=* Also runs everything, due to the single match-everything * value.
//./foo_test --gtest_filter=FooTest.* Runs everything in test case FooTest.
//./foo_test --gtest_filter=*Null*:*Constructor* Runs any test whose full name contains either "Null" or "Constructor".
//./foo_test --gtest_filter=-*DeathTest.* Runs all non-death tests.
//./foo_test --gtest_filter=FooTest.*-FooTest.Bar Runs everything in test case FooTest except 





