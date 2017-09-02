////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server service Component entity implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "ServerService/ServerServiceBase.h"
#include "ServerSystem/ServiceEntity/ServiceEntity.h"
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



