////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServiceEntity/ServiceEntity.h"
#include "ServerEntity/ServerEntityManager.h"
#include "SvrTrace.h"
#include "Task/ServerTaskEvent.h"




namespace SF {
namespace Svr {


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ClusteredServiceEntity class
	//

	ServiceEntity::ServiceEntity()
	{

	}

	ServiceEntity::~ServiceEntity()
	{

	}


}; // namespace Svr
}; // namespace SF



