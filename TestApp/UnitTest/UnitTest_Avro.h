////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : Avro test
//	
//
////////////////////////////////////////////////////////////////////////////////


#include"UnitTestPch.h"
#include <gtest/gtest.h>
#include "TestBase.h"



class AvroTest : public MyTestBase
{
public:


	// Remember that SetUp() is run immediately before a test starts.
	virtual void SetUp()
	{
		MyTestBase::SetUp();
	}

	// TearDown() is invoked immediately after a test finishes.  Here we
	virtual void TearDown()
	{
		MyTestBase::TearDown();

	}
};
