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
#include "Object/SharedPointer.h"
#include "String/FixedStringDB.h"
#include "ZooKeeper/SFZooKeeper.h"



namespace SF {


	
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	//	ZooKeeper service
	//

	class ZooKeeperSessionService
	{
	protected:
		String m_ServerAddresses;
		ZooKeeper m_zkInstance;

	public:

		ZooKeeperSessionService()
			: m_zkInstance(GetSystemMemoryManager()) 
		{}

		virtual ~ZooKeeperSessionService()
		{
			if (m_zkInstance.IsConnected())
			{
				m_zkInstance.Close();
				m_zkInstance.WaitForDisconnected();
			}
		}

		virtual ZooKeeper& GetZooKeeperSession()
		{
			return m_zkInstance;
		}

	};

	

}; // namespace SF

