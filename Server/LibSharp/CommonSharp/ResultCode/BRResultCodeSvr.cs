﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated file, Svr
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;


namespace BR {
	public partial struct ResultCode {

		// Server service failed 
		public const int E_SVR_SERVICE_FAILED =                             unchecked((int)0xE7040000); 

		// Timed out 
		public const int E_SVR_TIMEOUT =                                    unchecked((int)0xE7040001); 

		// Transaction has invalid state transition 
		public const int E_SVR_TRANSACTION_INVALID_STATE =                  unchecked((int)0xE7040002); 

		// Server Transaction timed out 
		public const int E_SVR_INVALID_TRANSITION =                         unchecked((int)0xE7040003); 

		// Transaction process Time out 
		public const int E_SVR_TRANSACTION_TIMEOUT =                        unchecked((int)0xE7040004); 

		// Failed to route message 
		public const int E_SVR_ROUTEMSG_FAILED =                            unchecked((int)0xE7040005); 

		// No result handler 
		public const int E_SVR_NO_RESULT_HANDLER =                          unchecked((int)0xE7040006); 

		// No message handler 
		public const int E_SVR_NO_MESSAGE_HANDLER =                         unchecked((int)0xE7040007); 

		// Invalid server ID 
		public const int E_SVR_INVALID_SERVERID =                           unchecked((int)0xE7040008); 

		// Player not found 
		public const int E_SVR_PLAYER_NOT_FOUND =                           unchecked((int)0xE7040009); 

		// Player state is invalid 
		public const int E_SVR_INVALID_PLAYER_STATE =                       unchecked((int)0xE704000A); 

		// Invalid player game db data 
		public const int E_SVR_INVALID_PLAYER_GAMEDB =                      unchecked((int)0xE704000B); 

		// Not expected server message 
		public const int E_SVR_NOTEXPECTED_MESSAGE =                        unchecked((int)0xE704000C); 

		// Invalid entity UID 
		public const int E_SVR_INVALID_ENTITYUID =                          unchecked((int)0xE704000D); 

		// Cluster service not ready 
		public const int E_SVR_CLUSTER_NOTREADY =                           unchecked((int)0xE704000E); 

		// Invalid cluster type 
		public const int E_SVR_INVALID_CLUSTERTYPE =                        unchecked((int)0xE704000F); 

		// Invalid cluster ID 
		public const int E_SVR_INVALID_CLUSTERID =                          unchecked((int)0xE7040010); 

		// Invalid queue item 
		public const int E_SVR_INVALID_QUEUEITEM =                          unchecked((int)0xE7040011); 

		// Queue item is canceled 
		public const int E_SVR_QUEUEITEM_CANCELED =                         unchecked((int)0xE7040012); 

		// There is no item in queue 
		public const int E_SVR_NOITEM_INQUEUE =                             unchecked((int)0xE7040013); 

		// Already in queue 
		public const int E_SVR_ALREADY_INQUEUE =                            unchecked((int)0xE7040014); 

		// Queue item is reserved 
		public const int E_SVR_RESERVED_QUEUEITEM =                         unchecked((int)0xE7040015); 

		// Cluster doesn't have any assigned master 
		public const int E_SVR_CLUSTER_NOMASTER =                           unchecked((int)0xE7040016); 

		// Queue item is reserved for cancel 
		public const int S_SVR_QUEUEITEM_CANCEL_PENDED =                    unchecked((int)0x27040017); 

		// Thread is wrong, running on wrong thread? 
		public const int E_SVR_INVALID_THREAD =                             unchecked((int)0xE7040018); 

		// Purchase canceled 
		public const int E_SVR_PURCHASE_CANCELED =                          unchecked((int)0xE7040019); 

		// Invalid receipt info 
		public const int E_SVR_INVALID_PURCHASE_INFO =                      unchecked((int)0xE704001A); 

		// Invalid duplicated purchase 
		public const int E_SVR_INVALID_PURCHASE_DUPLICATED =                unchecked((int)0xE704001B); 

		// External auth fail 
		public const int E_SVR_INVALID_EXTERNAL_AUTH =                      unchecked((int)0xE704001C); 

		// External server is not available 
		public const int E_SVR_EXTERNAL_SERVER_UNAVALIABLE =                unchecked((int)0xE704001D); 

		// Invalid receipt result from server 
		public const int E_SVR_INVALID_PURCHASE_FORMAT =                    unchecked((int)0xE704001E); 

		// The protocol is not supported. 
		public const int E_SVR_CURL_UNSUPPORTED_PROTOCOL =                  unchecked((int)0xE704001F); 

		// CURL initialization is failed. 
		public const int E_SVR_CURL_FAILED_INIT =                           unchecked((int)0xE7040020); 

		// Invalid URL format. 
		public const int E_SVR_CURL_URL_MALFORMAT =                         unchecked((int)0xE7040021); 

		// Not a build-in feature. 
		public const int E_SVR_CURL_NOT_BUILT_IN =                          unchecked((int)0xE7040022); 

		// Failed to resolve URL. 
		public const int E_SVR_CURL_COULDNT_RESOLVE_PROXY =                 unchecked((int)0xE7040023); 

		// Failed to resolve URL. 
		public const int E_SVR_CURL_COULDNT_RESOLVE_HOST =                  unchecked((int)0xE7040024); 

		// Connection is failed 
		public const int E_SVR_CURL_COULDNT_CONNECT =                       unchecked((int)0xE7040025); 

		// HTTP Post can't be done. 
		public const int E_SVR_CURL_HTTP_POST_ERROR =                       unchecked((int)0xE7040026); 

		// Not supported SSL type. 
		public const int E_SVR_CURL_SSL_ENGINE_NOTFOUND =                   unchecked((int)0xE7040027); 

		// Not supported SSL type. 
		public const int E_SVR_CURL_SSL_ENGINE_SETFAILED =                  unchecked((int)0xE7040028); 

		// Network send failed 
		public const int E_SVR_CURL_SEND_ERROR =                            unchecked((int)0xE7040029); 

		// Network receive failed 
		public const int E_SVR_CURL_RECV_ERROR =                            unchecked((int)0xE704002A); 

		// Invalid SSL setting. 
		public const int E_SVR_CURL_USE_SSL_FAILED =                        unchecked((int)0xE704002B); 

		// No Connection is available now. 
		public const int E_SVR_CURL_NO_CONNECTION_AVAILABLE =               unchecked((int)0xE704002C); 

		// Invalid state transition 
		public const int E_SVR_INVALID_STATE =                              unchecked((int)0xE704002D); 

		// Invalid purchase environment(public/test) 
		public const int E_SVR_INVALID_PURCHASE_MODE =                      unchecked((int)0xE704002E); 

		// Invalid Task group ID 
		public const int E_SVR_INVALID_TASK_GROUPID =                       unchecked((int)0xE704002F); 

	}
}

