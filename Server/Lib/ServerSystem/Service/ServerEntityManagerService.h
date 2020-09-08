////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Entity manager
//
////////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "SFTypedefs.h"
#include "Object/SFSharedPointer.h"
#include "String/SFStringCrcDB.h"



namespace SF {

	namespace Svr
	{
		class Transaction;
		class Entity;
		class ServerEntity;
	}



	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ServerEntity Manager service
	//

	class ServerEntityManagerService
	{
	public:

		virtual void Clear() {}


		virtual Result GetOrRegisterServer(ServerID serverID, NetClass netClass, const NetAddress& netAddress, Svr::ServerEntity* &pServerEntity) { return ResultCode::NOT_IMPLEMENTED; }

		// Get remote entity
		virtual Result GetServerEntity(ServerID svrID, Svr::ServerEntity* &pServerEntity) { return ResultCode::NOT_IMPLEMENTED; }

		// Get available firstEntity server entity
		virtual Result GetEntityManagerServerEntity(Svr::ServerEntity* &pServerEntity) { return ResultCode::NOT_IMPLEMENTED; }


		// Add new remote entity
		virtual Result AddOrGetServerEntity(ServerID serverID, NetClass netClass, Svr::ServerEntity* &pServerEntity) { return ResultCode::NOT_IMPLEMENTED; }

		virtual const SharedPointerAtomicT<Net::Connection>& GetServerConnection(ServerID svrID) { static SharedPointerAtomicT<Net::Connection> Dummy; return Dummy; }


	};

	

}; // namespace SF

