


set(BRDB_LIB_FILES
	DBPch.h
	Game/GameDB.h
	Game/GameDB.cpp
	Game/GameQuery.h
	Game/GameQuery.cpp
    AccountDB.cpp
	AccountDB.h
    AccountQuery.cpp
	AccountQuery.h
    DataSource.cpp
	DataSource.h
    DataSourceMYSQL.cpp
	DataSourceMYSQL.h
    DataSourceOLEDB.cpp
	DataSourceOLEDB.h
    DBClusterManager.cpp
	DBClusterManager.h
    Factory.cpp
	Factory.h
    FactoryMYSQL.cpp
	FactoryMYSQL.h
    FactoryOLEDB.cpp
	FactoryOLEDB.h
    GameTransactionDB.cpp
	GameTransactionDB.h
    GameTransactionDBQuery.cpp
	GameTransactionDBQuery.h
    LoginSessionDB.cpp
	LoginSessionDB.h
    LoginSessionQuery.cpp
	LoginSessionQuery.h
	Query.cpp
	Query.h
	QueryConst.h
    QueryMYSQL.cpp
	QueryMYSQL.h
	QueryOLEDB.h
    QueryWorker.cpp
	QueryWorker.h
    QueryWorkerManager.cpp
	QueryWorkerManager.h
    RankingDB.cpp
    RankingDB.h
	RankingDBQuery.cpp
	RankingDBQuery.h
    Session.cpp
    Session.h
	SessionMYSQL.cpp
	SessionMYSQL.h
    SessionOLEDB.cpp
	SessionOLEDB.h
    ShardCoordinatorDBQuery.cpp
	ShardCoordinatorDBQuery.h
	DBTrace.h
	DBTrace.cpp
	DBToString.h
	DBToString.cpp
	DBMYSQL.h
  )




include_directories(./)



add_library(BRDB STATIC ${BRDB_LIB_FILES})


install (TARGETS BRDB
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


