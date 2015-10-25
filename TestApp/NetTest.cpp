// TestApp.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <gtest/gtest.h>
#include "Common/StrUtil.h"
#include "Common/TimeUtil.h"
#include "Common/StrFormat.h"
#include "Common/BrBaseTypes.h"
#include "Common/HRESSvrSys.h"
#include "Net/MessageWindow.h"
#include "Common/Message.h"

#include "ServerSystem/MessageRoute.h"

#include "NetTest.h"
#include "Net/NetToString.h"

#include "Protocol/Policy/GameIPolicy.h"
#include "Protocol/Message/GameMsgClass.h"
#include "Protocol/Policy/LoginIPolicy.h"
#include "Protocol/Message/LoginMsgClass.h"


using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest; 
using namespace ::BR;


namespace BRTest
{ 

TEST_F(NetTest, Message)
{
	BR::Message::MessageData *pMsgData = nullptr;
	const char* strID = "MyID";
	const char* strPassword = "Pas1234";

	EXPECT_HRESULT_SUCCEEDED( BR::Message::Login::LoginCmd::BuildIMsg( pMsgData, GameID::Conspiracy, strID, strPassword ) );

	EXPECT_EQ(pMsgData->GetMessageHeader()->msgID.IDSeq.MsgID, BR::Message::Login::LoginCmd::MID.IDSeq.MsgID);

	BR::Message::Login::LoginCmd msgClass;
	EXPECT_HRESULT_SUCCEEDED( msgClass.ParseIMsg( pMsgData ) );

	EXPECT_STREQ( strID, msgClass.GetID() );
	EXPECT_STREQ( strPassword, msgClass.GetPassword() );
}


template< class MessageClassType >
HRESULT HandleMessage( BR::Message::MessageData *pMsgData, MessageClassType* &pMsgInstance )
{
	pMsgInstance = nullptr;
	if( pMsgData == nullptr )
		return E_POINTER;

	pMsgInstance = new MessageClassType(pMsgData);
	HRESULT hr = pMsgInstance->ParseMsg();
	if( FAILED(hr) )
		return hr;

	return hr;
}

TEST_F(NetTest, MessageMap)
{
	const char* strID = "MyID";
	const char* strPassword = "Pas1234";
	const char* strNickName = "NickName";
	const HRESULT InResult = S_FALSE;
	const BR::AccountID InAccID = 123456;
	const BR::AuthTicket InTicket = 654321;
	const BR::NetAddress InGameInsSvr("127.0.0.1",1234);
	const BR::GameInsUID InInsUID(3,67893);

	typedef std::function<HRESULT(BR::Message::MessageData*)> MessageHandlerType;
	typedef BR::Svr::MessageHandlerTable<MessageHandlerType> TestMessageTable;
	BR::CheckCtrMemory();
	BR::StaticAllocator<10> allocator(BR::STDAllocator::GetInstance());
	TestMessageTable messageHandlers(allocator);
	BR::CheckCtrMemory();
	messageHandlers.Register<BR::Message::Login::LoginCmd>([&](BR::Message::MessageData *pMsgData)->HRESULT {
		BR::Message::Login::LoginCmd *pMsg = nullptr;
			EXPECT_HRESULT_SUCCEEDED(HandleMessage<BR::Message::Login::LoginCmd>(pMsgData, pMsg));
			if( pMsg ) 
				delete pMsg;
			return S_OK;
	});
	messageHandlers.Register<BR::Message::Login::LoginRes>(
		[&](BR::Message::MessageData *pMsgData)->HRESULT	{
		BR::Message::Login::LoginRes *pMsg = nullptr;
		EXPECT_HRESULT_SUCCEEDED(HandleMessage<BR::Message::Login::LoginRes>(pMsgData, pMsg));
			BR::CheckCtrMemory();
			if( pMsg ) 
				delete pMsg;
			BR::CheckCtrMemory();
			return S_OK;
	});
	messageHandlers.Register<BR::Message::Game::JoinGameCmd>( 
		[&](BR::Message::MessageData *pMsgData)->HRESULT	{
			BR::Message::Game::JoinGameCmd *pMsg = nullptr;
			EXPECT_HRESULT_SUCCEEDED( HandleMessage<BR::Message::Game::JoinGameCmd>( pMsgData, pMsg ) );
			if( pMsg ) 
				delete pMsg;
			return S_OK;
	});
	messageHandlers.Register<BR::Message::Game::JoinGameRes>( 
		[&](BR::Message::MessageData *pMsgData)->HRESULT	{
			BR::Message::Game::JoinGameRes *pMsg = nullptr;
			EXPECT_HRESULT_SUCCEEDED( HandleMessage<BR::Message::Game::JoinGameRes>( pMsgData, pMsg ) );
			if( pMsg ) 
				delete pMsg;
			return S_OK;
	});

	BR::CheckCtrMemory();
	const int TestCaseCount = 4;
	BR::Message::MessageData *pMsgData[TestCaseCount];
	BR::NetAddress InGameServerAddr;
	UINT64 InLoginEntityUID;

	memset(pMsgData,0,sizeof(pMsgData));
	EXPECT_HRESULT_SUCCEEDED(BR::Message::Login::LoginCmd::BuildIMsg(pMsgData[0], GameID::Conspiracy, strID, strPassword));
	EXPECT_HRESULT_SUCCEEDED(BR::Message::Login::LoginRes::BuildIMsg(pMsgData[1], InResult, InGameServerAddr, InAccID, InTicket, InLoginEntityUID));
	EXPECT_HRESULT_SUCCEEDED( BR::Message::Game::JoinGameCmd::BuildIMsg( pMsgData[2], InAccID, InTicket, 0 ) );
//	EXPECT_HRESULT_SUCCEEDED( BR::Message::Game::JoinGameRes::BuildIMsg( pMsgData[3], InResult, InInsUID, GameStateID::DefenceOfSuspects, PlayerRole::AngelOfVillagers, 0 ) );
	BR::CheckCtrMemory();
	for( int iMsg = 0; iMsg < TestCaseCount; iMsg++ )
	{
		HRESULT hr = messageHandlers.HandleMessage( pMsgData[iMsg] );
		EXPECT_HRESULT_SUCCEEDED(hr);
		BR::CheckCtrMemory();
	}
}

TEST_F(NetTest, Simple)
{
	HRESULT hr = S_OK;

	m_pNetClient = dynamic_cast<BR::Net::IClient*>(m_pNet);
	defChkPtr(m_pNetClient);

	// Connect TCP server
	EXPECT_HRESULT_SUCCEEDED(m_pNetClient->Connect( "127.0.0.1", 4001, m_pIConnection ));

	// Policy create
	EXPECT_HRESULT_SUCCEEDED( m_pIConnection->CreatePolicy( BR::POLICY_GAME ) );

	// Wait until connection done
	while( m_pIConnection->GetConnectionState() == BR::Net::IConnection::STATE_CONNECTING )
	{
		BR::Net::IConnection::Event curEvent;

		if( SUCCEEDED(m_pIConnection->DequeueConnectionEvent( curEvent ) ) )
		{
			switch( curEvent.EventType )
			{
			case BR::Net::IConnection::Event::EVT_CONNECTION_RESULT:
				defTrace( Trace::TRC_TRACE, "EVT_CONNECTION_RESULT %0%", curEvent.Value.hr );
				break;
			case BR::Net::IConnection::Event::EVT_DISCONNECTED:
				defTrace( Trace::TRC_TRACE, "EVT_DISCONNECTED %0%", curEvent.Value.hr  );
				break;
			case BR::Net::IConnection::Event::EVT_STATE_CHANGE:
				defTrace( Trace::TRC_TRACE, "EVT_STATE_CHANGE %0%", curEvent.Value.hr  );
				break;
			};
		}

		Sleep(100);
	}


	// Sending Login packet
	EXPECT_HRESULT_SUCCEEDED( m_pIConnection->GetPolicy<BR::Policy::IPolicyLogin>()->LoginCmd( GameID::Conspiracy, "1", "1234" ) );


	// Processing Connection update
	BR::Message::MessageData *pIMsg = nullptr;
	DWORD dwTimeStart = Util::Time.GetTimeMs();
	DWORD dwTime = Util::Time.GetTimeMs();
	while( 1 )
	{
		dwTime = Util::Time.GetTimeMs();

		if( m_pIConnection )
		{
			BR::Net::IConnection::Event curEvent;
			// Processing Connection event 
			while( SUCCEEDED(m_pIConnection->DequeueConnectionEvent( curEvent ) ) )
			{
				switch( curEvent.EventType )
				{
				case BR::Net::IConnection::Event::EVT_CONNECTION_RESULT:
					defTrace( Trace::TRC_TRACE, "EVT_CONNECTION_RESULT %0%", BR::ArgHex32(curEvent.Value.hr) );
					break;
				case BR::Net::IConnection::Event::EVT_DISCONNECTED:
					defTrace( Trace::TRC_TRACE, "EVT_DISCONNECTED %0%", curEvent.Value.hr  );
					EXPECT_HRESULT_SUCCEEDED( m_pNetClient->ReleaseConnection( m_pIConnection ) );
					m_pIConnection = nullptr;
					goto EndTest;
					break;
				case BR::Net::IConnection::Event::EVT_STATE_CHANGE:
					defTrace( Trace::TRC_TRACE, "EVT_STATE_CHANGE %0%", curEvent.Value.hr  );
					break;
				};
			}

			// Processing received messages
			if( SUCCEEDED(m_pIConnection->GetRecvMessage( pIMsg ) ) )
			{
				defTrace( Trace::TRC_TRACE, "Message %0%", pIMsg->GetMessageHeader()->msgID.ID  );
				//switch( pIMsg->GetMessageHeader()->msgID.ID )
				//{
				//case BR::Message::MsgID::Login::MID_LoginRes:
				//{
				//	BR::Message::Login::MsgLoginRes res(pIMsg);

				//	EXPECT_HRESULT_SUCCEEDED( res.ParseMsg() );

				//	defTrace( Trace::TRC_TRACE, "Received Login Result %0%, Acc:%1%, Ticket:%2%", BR::ArgHex32(res.GetResult()), res.GetAccID(), res.GetTicket() );


				//	if( SUCCEEDED(res.GetResult()) )
				//	{
				//		m_AccountID = res.GetAccID();
				//		m_AuthTicket = res.GetTicket();
				//		EXPECT_HRESULT_SUCCEEDED( m_pIConnection->GetPolicy<BR::Policy::IPolicyLogin>()->GetWorldStatusListCmd( ) );
				//	}
				//	break;
				//}
				//case BR::Message::MsgID::Login::MID_GetWorldStatusListRes:
				//{
				//	BR::Message::Login::MsgGetWorldStatusListRes res(pIMsg);
				//	EXPECT_HRESULT_SUCCEEDED( res.ParseMsg() );

				//	defTrace( Trace::TRC_TRACE, "GetWorldStatusListRes %0%", BR::ArgHex32(res.GetResult()) );

			
				//	for( int iSvr = 0; iSvr < res.GetuiSizeOfWorldServerID(); iSvr++ )
				//	{
				//		defTrace( Trace::TRC_TRACE, "	Server %0%", res.GetWorldServerID()[iSvr] );
				//	}

				//	if( SUCCEEDED(res.GetResult()) && res.GetuiSizeOfWorldServerID() > 0 )
				//	{
				//		EXPECT_HRESULT_SUCCEEDED( m_pIConnection->GetPolicy<BR::Policy::IPolicyLogin>()->GetWorldInfoCmd( res.GetWorldServerID()[0] ) );
				//	}
				//	break;
				//}
				//case BR::Message::MsgID::Login::MID_GetWorldInfoRes:
				//{
				//	BR::Message::Login::MsgGetWorldInfoRes res(pIMsg);

				//	EXPECT_HRESULT_SUCCEEDED(res.ParseMsg());

				//	defTrace( Trace::TRC_TRACE, "GetWorldInfoRes %0%", BR::ArgHex32(res.GetResult()) );

				//	defTrace( Trace::TRC_TRACE, "Svr Addr %0%", res.GetAddr() );

				//	if( SUCCEEDED(res.GetResult()) )
				//	{
				//		EXPECT_HRESULT_SUCCEEDED( m_pNetClient->Connect( res.GetAddr().strAddr, res.GetAddr().usPort, m_pIConnectionWorld ) );
				//		EXPECT_HRESULT_SUCCEEDED( m_pIConnectionWorld->CreatePolicy( BR::POLICY_WORLD ) );
				//		EXPECT_HRESULT_SUCCEEDED( m_pIConnectionWorld->CreatePolicy( BR::POLICY_COMMUNITY ) );
				//	}
				//	break;
				//}
				//}
			}
		}

		Sleep(100);
	}

EndTest:

	if( m_pIConnection )
		EXPECT_HRESULT_SUCCEEDED( m_pIConnection->Disconnect() );


Proc_End:

	;
}


const UINT TEST_COUNT = 4000000*TestScale;


BR::Message::MessageData* NewMessage(UINT sequenceID)
{
	BR::Message::MessageData *pResult = nullptr;
	AssertRel(SUCCEEDED(BR::Message::Login::LoginCmd::BuildIMsg(pResult, GameID::Conspiracy, "11", "11")));
	pResult->AssignSequence(sequenceID);
	return pResult;
}


TEST_F(NetTest, RecvMessageWindowSimple)
{
	Net::RecvMsgWindow recvMessage;
	UINT16 uiSequence = 0;

	for (int iTest = 0; iTest < 256; iTest++)
	{
		auto hr = recvMessage.AddMsg(NewMessage(uiSequence++));
		if (iTest < recvMessage.GetWindowSize())
		{
			AssertRel(SUCCEEDED(hr));
		}
		else
		{
			AssertRel(FAILED(hr));
		}
	}

	uiSequence = 0;
	for (int iTest = 0; iTest < 256; iTest++)
	{
		BR::Message::MessageData *pResult = nullptr;
		auto hr = recvMessage.PopMsg(pResult);
		if (iTest < recvMessage.GetWindowSize())
		{
			AssertRel(pResult != nullptr);
			AssertRel(SUCCEEDED(hr));

			AssertRel(Message::SequenceDifference(pResult->GetMessageHeader()->msgID.IDSeq.Sequence, uiSequence++) == 0);
			Util::SafeRelease(pResult);
		}
		else
		{
			AssertRel(pResult == nullptr);
			AssertRel(FAILED(hr));
		}
	}
}

TEST_F(NetTest, RecvMessageWindowSimple2)
{
	Net::RecvMsgWindow recvMessage;
	UINT16 uiSequence = 0;
	UINT16 releaseSequence = 0;
	BR::Message::MessageData *pResult = nullptr;
	HRESULT hr;

	for (unsigned iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		auto random = rand() % 2;
		switch (random)
		{
		case 0: // add
			hr = recvMessage.AddMsg(NewMessage(uiSequence));
			if (Message::SequenceDifference(uiSequence, recvMessage.GetBaseSequence()) < recvMessage.GetWindowSize())
			{
				AssertRel(SUCCEEDED(hr));
				uiSequence++;
			}
			else
			{
				AssertRel(FAILED(hr));
			}
			break;
		case 1: // release
			pResult = nullptr;
			hr = recvMessage.PopMsg(pResult);
			if (SUCCEEDED(hr))
			{
				AssertRel(pResult != nullptr);

				AssertRel(Message::SequenceDifference(pResult->GetMessageHeader()->msgID.IDSeq.Sequence, releaseSequence++) == 0);
				Util::SafeRelease(pResult);
			}
			else
			{
				AssertRel(pResult == nullptr);
				AssertRel(FAILED(hr));
			}
			break;
		}

		recvMessage.GetSyncMask();
	}

	recvMessage.ClearWindow();
}

TEST_F(NetTest, RecvMessageWindowSimple3)
{
	Net::RecvMsgWindow recvMessage;
	UINT16 uiSequence = 0;
	UINT16 releaseSequence = 0;
	BR::Message::MessageData *pResult = nullptr;
	HRESULT hr;

	for (unsigned iTest = 0; iTest < TEST_COUNT; iTest++)
	{
		auto random = rand() % 2;
		switch (random)
		{
		case 0: // add
		{
					auto testSequence = uiSequence;
					if (rand() % 2)
					{
						testSequence = std::abs(rand());
					}
					hr = recvMessage.AddMsg(NewMessage(testSequence));
					auto seqOffset = Message::SequenceDifference(testSequence, recvMessage.GetBaseSequence());
					if (seqOffset < recvMessage.GetWindowSize() && seqOffset >= 0)
					{
						AssertRel(SUCCEEDED(hr));
						uiSequence++;
					}
					else
					{
						AssertRel(seqOffset <0 || FAILED(hr));
					}
		}
			break;
		case 1: // release
			pResult = nullptr;
			hr = recvMessage.PopMsg(pResult);
			if (SUCCEEDED(hr))
			{
				AssertRel(pResult != nullptr);

				AssertRel(Message::SequenceDifference(pResult->GetMessageHeader()->msgID.IDSeq.Sequence, releaseSequence++) == 0);
				Util::SafeRelease(pResult);
			}
			else
			{
				AssertRel(pResult == nullptr);
				AssertRel(FAILED(hr));
			}
			break;
		}

		recvMessage.GetSyncMask();
	}

	recvMessage.ClearWindow();
}


TEST_F(NetTest, RecvMessageWindowMT)
{
	const int NUM_SEND_THREAD = 10;
	const int NUM_RECV_THREAD = 1;
	const UINT runningTime = TestScale * 1 * 60 * 1000;

	Net::RecvMsgWindow recvMessage;
	std::atomic<UINT16> uiSequence = 0;
	std::atomic<UINT16> releaseSequence = 0;
	std::vector<Thread*> threads;

	for (int iThread = 0; iThread < NUM_SEND_THREAD; iThread++)
	{
		auto newThread = new FunctorThread([&](Thread *pThread)
		{
			UINT16 sequence = uiSequence.fetch_add(1, std::memory_order_relaxed);
			BR::Message::MessageData* pMsg = nullptr;
			HRESULT hr;

			while (!pThread->CheckKillEvent(0))
			{
				pMsg = NewMessage(sequence);
				hr = recvMessage.AddMsg(pMsg);
				Assert(hr != S_NET_PROCESSED_SEQUENCE);
				if(FAILED(hr))
				{
					Util::SafeRelease(pMsg);
				}
				else
				{
					sequence = uiSequence.fetch_add(1, std::memory_order_relaxed);
				}

				recvMessage.GetSyncMask();
			}
		});
		newThread->Start();
		threads.push_back(newThread);
	}

	for (int iThread = 0; iThread < NUM_RECV_THREAD; iThread++)
	{
		auto newThread = new FunctorThread([&](Thread *pThread)
		{
			HRESULT hr;
			BR::Message::MessageData *pResult = nullptr;
			UINT16 sequence = releaseSequence.fetch_add(1, std::memory_order_relaxed);
			while (!pThread->CheckKillEvent(0))
			{
				pResult = nullptr;
				hr = recvMessage.PopMsg(pResult);
				if (SUCCEEDED(hr))
				{
					AssertRel(pResult != nullptr);

					AssertRel(Message::SequenceDifference(pResult->GetMessageHeader()->msgID.IDSeq.Sequence, sequence) == 0);
					Util::SafeRelease(pResult);
					sequence = releaseSequence.fetch_add(1, std::memory_order_relaxed);
				}
				else
				{
					AssertRel(pResult == nullptr);
					AssertRel(FAILED(hr));
				}
			}
		});
		newThread->Start();
		threads.push_back(newThread);
	}

	Sleep(runningTime);

	std::for_each(threads.begin(), threads.end(), [](Thread* pThread)
	{
		pThread->Stop(true);
	});

	recvMessage.ClearWindow();
}

// random bigger than current seq
TEST_F(NetTest, RecvMessageWindowMT2)
{
	const int NUM_SEND_THREAD = 10;
	const int NUM_RECV_THREAD = 1;
	const UINT runningTime = TestScale * 1 * 60 * 1000;

	Net::RecvMsgWindow recvMessage;
	std::atomic<UINT16> uiSequence = 0;
	std::atomic<UINT16> releaseSequence = 0;
	std::vector<Thread*> threads;

	for (int iThread = 0; iThread < NUM_SEND_THREAD; iThread++)
	{
		auto newThread = new FunctorThread([&](Thread *pThread)
		{
			UINT16 sequence = uiSequence.fetch_add(1, std::memory_order_relaxed);
			BR::Message::MessageData* pMsg = nullptr;
			HRESULT hr;

			while (!pThread->CheckKillEvent(0))
			{
				UINT16 testSequence = sequence;
				if (rand() % 2)
				{
					testSequence = sequence - 512 + std::abs(rand()) % 1024;
				}
				pMsg = NewMessage(testSequence);
				hr = recvMessage.AddMsg(pMsg);
				if (FAILED(hr))
				{
					Util::SafeRelease(pMsg);
				}
				else
				{
					sequence = uiSequence.fetch_add(1, std::memory_order_relaxed);
				}

				recvMessage.GetSyncMask();
			}
		});
		newThread->Start();
		threads.push_back(newThread);
	}

	for (int iThread = 0; iThread < NUM_RECV_THREAD; iThread++)
	{
		auto newThread = new FunctorThread([&](Thread *pThread)
		{
			HRESULT hr;
			BR::Message::MessageData *pResult = nullptr;
			UINT16 sequence = releaseSequence.fetch_add(1, std::memory_order_relaxed);
			while (!pThread->CheckKillEvent(0))
			{
				pResult = nullptr;
				hr = recvMessage.PopMsg(pResult);
				if (SUCCEEDED(hr))
				{
					AssertRel(pResult != nullptr);

					AssertRel(Message::SequenceDifference(pResult->GetMessageHeader()->msgID.IDSeq.Sequence, sequence) == 0);
					Util::SafeRelease(pResult);
					if ((sequence % 10000) == 0)
					{
						printf("Read Sequence: %d\n", sequence);
					}
					sequence = releaseSequence.fetch_add(1, std::memory_order_relaxed);
				}
				else
				{
					AssertRel(pResult == nullptr);
					AssertRel(FAILED(hr));
				}
			}
		});
		newThread->Start();
		threads.push_back(newThread);
	}

	Sleep(runningTime);

	std::for_each(threads.begin(), threads.end(), [](Thread* pThread)
	{
		pThread->Stop(true);
	});

	recvMessage.ClearWindow();
}




}  // namespace BRTest