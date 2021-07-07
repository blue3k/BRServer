


set(BRDB_LIB_FILES
	DBPch.h
	MySqlHeaders.h
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
    DBClusterManager.cpp
	DBClusterManager.h
	DBConfig.h
    LoginSessionDB.cpp
	LoginSessionDB.h
    LoginSessionQuery.cpp
	LoginSessionQuery.h
	Query.cpp
	Query.h
	QueryConst.h
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
    ShardCoordinatorDBQuery.cpp
	ShardCoordinatorDBQuery.h
	DatabaseServiceComponent.h
	DatabaseServiceComponent.cpp
	DBTrace.h
	DBTrace.cpp
	DBToString.h
	DBToString.cpp
	MySQL/DBMYSQL.h
    MySQL/DataSourceMYSQL.cpp
	MySQL/DataSourceMYSQL.h
    MySQL/FactoryMYSQL.cpp
	MySQL/FactoryMYSQL.h
    MySQL/QueryMYSQL.cpp
	MySQL/QueryMYSQL.h
	MySQL/SessionMYSQL.cpp
	MySQL/SessionMYSQL.h
	OLEDB/DBOLEDB.h
    OLEDB/DataSourceOLEDB.cpp
	OLEDB/DataSourceOLEDB.h
    OLEDB/FactoryOLEDB.cpp
	OLEDB/FactoryOLEDB.h
	OLEDB/QueryOLEDB.h
    OLEDB/SessionOLEDB.cpp
	OLEDB/SessionOLEDB.h
  )




include_directories(./)



add_library(BRDB STATIC ${BRDB_LIB_FILES})


install (TARGETS BRDB
         ARCHIVE DESTINATION lib COMPONENT BRServer
         LIBRARY DESTINATION lib COMPONENT BRServer
         RUNTIME DESTINATION bin COMPONENT BRServer)


