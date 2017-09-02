////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server Component implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "String/StrUtil.h"
#include "ServerLog/SvrLog.h"
#include "Thread/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/SvrTrace.h"


namespace SF {
namespace Svr {



	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerCOmponentCarrier class
	//

	void ServerComponentCarrier::OnAddComponent(Component* newComponent)
	{
		svrTrace(Svr::TRC_INFO, "Server Component is added {0}, {1}", typeid(*newComponent).name(), newComponent->GetComponentID());
	}



}; // namespace Svr {
}; // namespace SF {



