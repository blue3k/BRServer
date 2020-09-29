


set(BRTABLE_LIB_FILES
    GameConspiracyDBPch.h
    GameConspiracyDB.cpp
    GameConspiracyDB.h
    GameConspiracyQuery.h
	GameConspiracyQuery.cpp
  )




include_directories(./ Table)



add_library(BRConspiracyGameDB STATIC ${BRTABLE_LIB_FILES})


install (TARGETS BRConspiracyGameDB
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


