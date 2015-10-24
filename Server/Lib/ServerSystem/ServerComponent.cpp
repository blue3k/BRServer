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



#include "StdAfx.h"
#include "Common/StrUtil.h"
#include "Common/Trace.h"
#include "Common/Thread.h"
#include "Net/NetDef.h"
#include "ServerSystem/Entity.h"
#include "ServerSystem/ServerComponent.h"
#include "ServerSystem/SvrTrace.h"


namespace BR {
namespace Svr {


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerCOmponentCarrier class
	//

	void ServerComponentCarrier::OnAddComponent(Component* newComponent)
	{
		svrTrace(Svr::TRC_INFO, "Server Component is added %0%, %1%", typeid(*newComponent).name(), newComponent->GetComponentID());
	}



}; // namespace Svr {
}; // namespace BR {



