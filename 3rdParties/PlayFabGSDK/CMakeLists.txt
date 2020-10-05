



set(SRC_FILES
	../../../gsdk/cpp/cppsdk/gsdkConfig.h
	../../../gsdk/cpp/cppsdk/ManualResetEvent.h
	../../../gsdk/cpp/cppsdk/ManualResetEvent.cpp
	../../../gsdk/cpp/cppsdk/gsdk.h
    ../../../gsdk/cpp/cppsdk/gsdk.cpp
	../../../gsdk/cpp/cppsdk/gsdkCommonPch.h
	../../../gsdk/cpp/cppsdk/gsdkInternal.h
	../../../gsdk/cpp/cppsdk/gsdkLog.h
    ../../../gsdk/cpp/cppsdk/gsdkLog.cpp
	../../../gsdk/cpp/cppsdk/gsdkUtils.h
    ../../../gsdk/cpp/cppsdk/gsdkUtils.cpp
	../../../gsdk/cpp/cppsdk/gsdkWindowsPch.h
	../../../gsdk/cpp/cppsdk/gsdkLinuxPch.h
	../../../gsdk/cpp/cppsdk/gsdkConfig.cpp
	../../../gsdk/cpp/cppsdk/gsdkConfig.h
)



if(WIN32)
	add_definitions(-DGSDK_WINDOWS)
elseif(UNIX)
	add_definitions(-DGSDK_LINUX)
endif()

include_directories(BEFORE 
	../../../StormForge/3rdParties/src/PlayFab/XPlatCppSdk/code/include
	../../../StormForge/3rdParties/src/jsoncpp/jsoncpp/include
	)

add_library(PlayFabGSDK ${SRC_FILES})

install (TARGETS PlayFabGSDK
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)



