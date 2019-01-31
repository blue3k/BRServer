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
#include "String/SFFixedStringDB.h"



namespace SF {

	class ZooKeeper;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZooKeeper service
	//

	class ZooKeeperSessionService
	{
	public:

		virtual ZooKeeper* GetZooKeeperSession() { return nullptr; }

	};

	

}; // namespace SF

