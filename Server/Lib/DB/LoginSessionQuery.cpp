
////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Game DB
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DB/LoginSessionQuery.h"
#include "DBTrace.h"
#include "Memory/MemoryPool.h"



BRDB_DEFINE_QUERY_IMPL(QueryRegisterAuthTicket);
BRDB_DEFINE_QUERY_IMPL(QueryReplaceLoginSession);
BRDB_DEFINE_QUERY_IMPL(QueryDeleteLoginSession);

BRDB_DEFINE_QUERY_IMPL(QueryConnectedToGameServer);
BRDB_DEFINE_QUERY_IMPL(QueryValidateGameServerSession);
BRDB_DEFINE_QUERY_IMPL(QueryGameServerHeartBit);

BRDB_DEFINE_QUERY_IMPL(QueryFindPlayer);

