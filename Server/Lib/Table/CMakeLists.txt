

source_group(INCLUDES
    conspiracy/BotTalkTbl.h
    conspiracy/GameConfigTbl.h
    conspiracy/LevelTbl.h
    conspiracy/OrganicTbl.h
    conspiracy/RewardTbl.h
    conspiracy/ShopTbl.h
    conspiracy/TableVersionTbl.h
    stdafx.h
    TableQueries.h
    TableSystem.h
    targetver.h
)


set(BRTABLE_LIB_FILES
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


