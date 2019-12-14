


set(BRTABLE_LIB_FILES
    targetver.h
    conspiracy/BotTalkTbl.h
    conspiracy/GameConfigTbl.h
    conspiracy/LevelTbl.h
    conspiracy/OrganicTbl.h
    conspiracy/RewardTbl.h
    conspiracy/ShopTbl.h
    conspiracy/TableVersionTbl.h
    TableQueries.h
    TableSystem.h
    conspiracy/BotTalkTbl.cpp
    conspiracy/GameConfigTbl.cpp
    conspiracy/LevelTbl.cpp
    conspiracy/OrganicTbl.cpp
    conspiracy/RewardTbl.cpp
    conspiracy/ShopTbl.cpp
    conspiracy/TableVersionTbl.cpp
    TableQueries.cpp
    TableSystem.cpp
  )




include_directories(./ conspiracy)



add_library(BRTable STATIC ${BRTABLE_LIB_FILES})


install (TARGETS BRTable
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


