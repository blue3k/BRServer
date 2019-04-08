


set(BRDB_LIB_FILES
    AccountDB.cpp
    AccountQuery.cpp
    DataSource.cpp
    DataSourceMYSQL.cpp
    DataSourceOLEDB.cpp
    DBClusterManager.cpp
    Factory.cpp
    FactoryMYSQL.cpp
    FactoryOLEDB.cpp
    GameConspiracyDB.cpp
    GameConspiracyQuery.cpp
    GameTransactionDB.cpp
    GameTransactionDBQuery.cpp
    LoginSessionDB.cpp
    LoginSessionQuery.cpp
    QueryMYSQL.cpp
    QueryWorker.cpp
    QueryWorkerManager.cpp
    RankingDB.cpp
    RankingDBQuery.cpp
    Session.cpp
    SessionMYSQL.cpp
    SessionOLEDB.cpp
    ShardCoordinatorDBQuery.cpp

  )




include_directories(./)



add_library(BRDB STATIC ${BRDB_LIB_FILES})


install (TARGETS BRDB
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


