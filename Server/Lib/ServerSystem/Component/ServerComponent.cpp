////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Server Component implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "ServerSystemPCH.h"
#include "String/SFStrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/SFThread.h"
#include "Net/SFNetDef.h"
#include "Entity/Entity.h"
#include "Component/ServerComponent.h"
#include "Component/ServerComponentCarrier.h"
#include "SvrTrace.h"


namespace SF {
namespace Svr {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerCOmponentCarrier class
	//

	void ServerComponentCarrier::OnAddComponent(Component* newComponent)
	{
		svrTrace(SVR_INFO, "Server Component is added {0}, {1}", typeid(*newComponent).name(), newComponent->GetComponentID());
	}



} // namespace Svr {
} // namespace SF {



