



set(SF_UNIT_TEST_FILES
	../TestCommon/TestBaseCommon.h
	TestApp.cpp
	TestBase.cpp
	TestBase.h
    DBTest.cpp
	DBTest.h
    UnitTest_Avro.cpp
	UnitTest_Avro.h
	UnitTestPch.h
  )


include_directories(
	./
	../../../StormForge/3rdParties/src/gtest/gtest/googletest/include
	../../../StormForge/3rdParties/src/gtest/gtest/googlemock/include
	)


add_executable(BRUnitTest ${SF_UNIT_TEST_FILES})


add_dependencies(BRUnitTest BRServerSystem BRDB BRServerProtocol)


target_link_libraries(BRUnitTest ${ENGINE_LINK_LIBS} ${PLATFORM_LIBS} gtest)


install (TARGETS BRUnitTest
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


