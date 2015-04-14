////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Network value definitions
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"




namespace BR {
namespace Net {

////////////////////////////////////////////////////////////////////////////////
//
//	Category : Network
//

	namespace Const
	{
		enum NetConstEnum {
		PACKET_GATHER_MAX				= 36,
		PACKET_GATHER_SIZE_MAX			= 1024,

		PACKET_SIZE_MAX					= 1024,
		INTER_PACKET_SIZE_MAX			= 30*1024,

		CONMGR_THREAD_INTERVAL			= 1,		// Connection manager thread interval


		CLI_RECV_BUFFER_SIZE			= 1024*16,
		CLI_SEND_BUFFER_SIZE			= 1024*8,

		CLI_NUM_NETCTRL_THREAD			= 2,
		CLI_NUM_RECV_THREAD				= 2,
		CLI_CONNECTION_TASK_THREAD		= 1,
		CLI_NUM_RECV					= 4,
		CLI_CONNECTION_MAX				= 10,

		CLI_OVERBUFFER_COUNT			= 128, // 66*2

		CLI_GUARANTEDRETRY_ATONCE_MAX	= 4,

		CLI_PEERPUNCH_TIME				= 500,
		CLI_PEERPUNCH_COUNT				= 10,

		CLI_CONMAP_BUCKET_SIZE			= 16,


		SVR_RECV_BUFFER_SIZE			= 1024*8,
		SVR_SEND_BUFFER_SIZE			= 1024*8,

		SVR_INTERNAL_CONNECTION_MAX		= 256,

		SVR_NUM_NETCONTROL_THREAD		= 4,
		SVR_NUM_RECV_THREAD				= 4,
		SVR_CONNECTION_AUTORECOVERY		= 10*1000,
		SVR_CONNECTION_TIMEOUT			= 10 * 60 * 1000,

		SVR_OVERBUFFER_COUNT			= 8196,

		SVR_HEARTBIT_TIME_UDP			= 5*1000,
		//SVR_HEARTBIT_TIME_TCP			= 10* 60 * 1000,
		SVR_HEARTBIT_TIME_PEER			= 30 * 1000,
		//SVR_HEARTBIT_TIME_PEER			= 500,
		SVR_MAX_CONNECT_SEND			= 5,

#ifdef _DEBUG
		SVR_PRIVATE_CONNECTION_POOLCACHE	= 1,
		SVR_PUBLIC_CONNECTION_POOLCACHE	= 1,
#else
		SVR_PRIVATE_CONNECTION_POOLCACHE	= 128,
		SVR_PUBLIC_CONNECTION_POOLCACHE	= 50*1024,
#endif
		SVR_PUBLIC_CONNECTION_BUCKET_SIZE	= 100*1024,
		SVR_PRIVATE_CONNECTION_BUCKET_SIZE	= 256,

		TCP_GUARANT_PENDING_MAX			= 10,
		TCP_ACCEPT_PENDING_MAX			= 10,
		TCP_ACCEPT_TRYALLOC_MAX			= 1000,

		UDP_CLI_RETRY_ONETIME_MAX		= 4,
		UDP_SVR_RETRY_ONETIME_MAX		= 64,

#ifdef _DEBUG
		CONNECTION_TIMEOUT				= 60*1000,
#else
		CONNECTION_TIMEOUT				= 15*1000,
#endif
		CONNECTION_ZEROPACKET_MAX		= 15,
		CONNECTION_RETRY				= 700,

		SEND_RETRY_TIME					= 1000,
		MUDP_SEND_RETRY_TIME			= 500,
		CONNECTION_RETRY_TIME			= 500,

		DISCONNECT_RETRY_TIME			= 700,
		DISCONNECT_TIMEOUT				= 2200,
		SVR_DISCONNECT_TIMEOUT			= 2000,

		UDP_HEARTBIT_START_TIME			= 1300,
		TCP_HEARTBIT_START_TIME			= 7*1000,
		HEARTBIT_TIMEOUT				= 1000*60,

		MUDP_HEARTBIT_START_TIME		= 1000*60,
		MUDP_HEARTBIT_TIMEOUT			= 1000*60*10,

		};
	};

} // namespace Net
} // namespace BR



