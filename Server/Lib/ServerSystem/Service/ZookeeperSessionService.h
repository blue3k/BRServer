////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Entity table
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Object/SFSharedPointer.h"
#include "String/SFStringCrcDB.h"



namespace SF {

	class Zookeeper;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Zookeeper service
	//

	class ZookeeperSessionService
	{
	public:

		virtual Zookeeper* GetZookeeperSession() { return nullptr; }

	};

	

}; // namespace SF

