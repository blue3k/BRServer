////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 The Braves
// 
// Author : KyungKun Ko
//
// Description : Connection implementations
//	
//
////////////////////////////////////////////////////////////////////////////////

#include "ServerSystemPCH.h"
#include "Thread/SFThread.h"
#include "SFAssert.h"
#include "ResultCode/SFResultCodeEngine.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "Util/SFLog.h"
#include "Util/SFTimeUtil.h"
#include "String/SFToString.h"
#include "String/SFHasher32.h"
#include "Protocol/SFProtocol.h"

#include "Net/SFConnectionMUDP.h"
#include "Net/SFNetDef.h"
#include "Net/SFNetSvrDef.h"
#include "Net/SFNetCtrl.h"
#include "Net/SFNetConst.h"
#include "Net/SFNetCtrl.h"
#include "Net/SFNetToString.h"






namespace SF {
namespace Net {


	

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Server Mobile UDP Network connection class
	//

	// Constructor
	ConnectionMUDPServer::ConnectionMUDPServer(IHeap& memoryManager, SocketIO* ioHandler)
		: ConnectionMUDP(memoryManager, ioHandler)
	{
		SetNetCtrlAction(NetCtrlCode_SyncReliable, &m_HandleSyncReliable);

		AddStateAction(ConnectionState::CONNECTED, &m_ActSync);
	}

	ConnectionMUDPServer::~ConnectionMUDPServer()
	{

	}
/*
	Result ConnectionMUDPServer::ProcSendReliable()
	{
		Result result = ConnectionMUDP::ProcSendReliable();
#ifdef _DEBUG
		if ((GetSendGuaQueue().size() + GetSendReliableWindow().GetMsgCount()) > 0)
		{
			if (m_SendQueueTime == TimeStampMS())
			{
				m_SendQueueTime = Util::Time.GetTimeMs();
				m_Reported = false;
			}

			if (!m_Reported && Util::TimeSince(m_SendQueueTime) > DurationMS(30*1000))
			{
				m_Reported = true;
			}

		}
		else
		{
			m_SendQueueTime = TimeStampMS();
			m_Reported = false;
		}
#endif
		return result;
	}
*/

	Result ConnectionMUDPServer::UpdateSendQueue()
	{
		Result hr;
		if (GetConnectionState() == ConnectionState::DISCONNECTED)
			return ResultCode::SUCCESS;

		MutexScopeLock localLock(GetUpdateLock());

		// Force update send
		m_ActSendReliableQueue.Run();
		m_ActSendReliableRetry.Run();



		// Flush sync message asap
		SendFlush();

		return hr;
	}





} // namespace Net
} // namespace SF


