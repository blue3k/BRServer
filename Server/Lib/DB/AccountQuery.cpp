////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Account DB
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AccountQuery.h"


BRDB_DEFINE_QUERY_IMPL(QueryCreateUser);
BRDB_DEFINE_QUERY_IMPL(QueryLogin);
BRDB_DEFINE_QUERY_IMPL(QueryLogout);

BRDB_DEFINE_QUERY_IMPL(QueryFacebookCreateUser);
BRDB_DEFINE_QUERY_IMPL(QueryFacebookLogin);

BRDB_DEFINE_QUERY_IMPL(QueryCreateRandomUser);

BRDB_DEFINE_QUERY_IMPL(QueryUserList);

BRDB_DEFINE_QUERY_IMPL(QueryUpdateGCMKeys);
BRDB_DEFINE_QUERY_IMPL(QueryUpdateUserContactInfo);

BRDB_DEFINE_QUERY_IMPL(QueryFindPlayerByEMail);
BRDB_DEFINE_QUERY_IMPL(QueryFindPlayerByPlayerID);

BRDB_DEFINE_QUERY_IMPL(QueryGetPlayerShardID);

