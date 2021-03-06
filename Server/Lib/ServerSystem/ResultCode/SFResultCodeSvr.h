﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Autogenerated header, Svr
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>



namespace SF {
	namespace ResultCode {



			// Server service failed 
			constexpr Result 			SVR_SERVICE_FAILED                                                 ((int32_t)0xE7120000L);

			// Timed out 
			constexpr Result 			SVR_TIMEOUT                                                        ((int32_t)0xE7120001L);

			// Transaction has invalid state transition 
			constexpr Result 			SVR_TRANSACTION_INVALID_STATE                                      ((int32_t)0xE7120002L);

			// Server Transaction timed out 
			constexpr Result 			SVR_INVALID_TRANSITION                                             ((int32_t)0xE7120003L);

			// Transaction process Time out 
			constexpr Result 			SVR_TRANSACTION_TIMEOUT                                            ((int32_t)0xE7120004L);

			// Failed to route message 
			constexpr Result 			SVR_ROUTEMSG_FAILED                                                ((int32_t)0xE7120005L);

			// No result handler 
			constexpr Result 			SVR_NO_RESULT_HANDLER                                              ((int32_t)0xE7120006L);

			// No message handler 
			constexpr Result 			SVR_NO_MESSAGE_HANDLER                                             ((int32_t)0xE7120007L);

			// Invalid server ID 
			constexpr Result 			SVR_INVALID_SERVERID                                               ((int32_t)0xE7120008L);

			// Player not found 
			constexpr Result 			SVR_PLAYER_NOT_FOUND                                               ((int32_t)0xE7120009L);

			// Player state is invalid 
			constexpr Result 			SVR_INVALID_PLAYER_STATE                                           ((int32_t)0xE712000AL);

			// Invalid player game db data 
			constexpr Result 			SVR_INVALID_PLAYER_GAMEDB                                          ((int32_t)0xE712000BL);

			// Not expected server message 
			constexpr Result 			SVR_NOTEXPECTED_MESSAGE                                            ((int32_t)0xE712000CL);

			// Invalid entity UID 
			constexpr Result 			SVR_INVALID_ENTITYUID                                              ((int32_t)0xE712000DL);

			// Cluster service not ready 
			constexpr Result 			SVR_CLUSTER_NOTREADY                                               ((int32_t)0xE712000EL);

			// Invalid cluster type 
			constexpr Result 			SVR_INVALID_CLUSTERTYPE                                            ((int32_t)0xE712000FL);

			// Invalid cluster ID 
			constexpr Result 			SVR_INVALID_CLUSTERID                                              ((int32_t)0xE7120010L);

			// Invalid queue item 
			constexpr Result 			SVR_INVALID_QUEUEITEM                                              ((int32_t)0xE7120011L);

			// Queue item is canceled 
			constexpr Result 			SVR_QUEUEITEM_CANCELED                                             ((int32_t)0xE7120012L);

			// There is no item in queue 
			constexpr Result 			SVR_NOITEM_INQUEUE                                                 ((int32_t)0xE7120013L);

			// Already in queue 
			constexpr Result 			SVR_ALREADY_INQUEUE                                                ((int32_t)0xE7120014L);

			// Queue item is reserved 
			constexpr Result 			SVR_RESERVED_QUEUEITEM                                             ((int32_t)0xE7120015L);

			// Cluster doesn't have any assigned master 
			constexpr Result 			SVR_CLUSTER_NOMASTER                                               ((int32_t)0xE7120016L);

			// Queue item is reserved for cancel 
			constexpr Result 			SUCCESS_SVR_QUEUEITEM_CANCEL_PENDED                                ((int32_t)0x27120017L);

			// Purchase canceled 
			constexpr Result 			SVR_PURCHASE_CANCELED                                              ((int32_t)0xE7120018L);

			// Invalid receipt info 
			constexpr Result 			SVR_INVALID_PURCHASE_INFO                                          ((int32_t)0xE7120019L);

			// Invalid duplicated purchase 
			constexpr Result 			SVR_INVALID_PURCHASE_DUPLICATED                                    ((int32_t)0xE712001AL);

			// External auth fail 
			constexpr Result 			SVR_INVALID_EXTERNAL_AUTH                                          ((int32_t)0xE712001BL);

			// External server is not available 
			constexpr Result 			SVR_EXTERNAL_SERVER_UNAVALIABLE                                    ((int32_t)0xE712001CL);

			// Invalid receipt result from server 
			constexpr Result 			SVR_INVALID_PURCHASE_FORMAT                                        ((int32_t)0xE712001DL);

			// The protocol is not supported. 
			constexpr Result 			SVR_CURL_UNSUPPORTED_PROTOCOL                                      ((int32_t)0xE712001EL);

			// CURL initialization is failed. 
			constexpr Result 			SVR_CURL_FAILED_INIT                                               ((int32_t)0xE712001FL);

			// Invalid URL format. 
			constexpr Result 			SVR_CURL_URL_MALFORMAT                                             ((int32_t)0xE7120020L);

			// Not a build-in feature. 
			constexpr Result 			SVR_CURL_NOT_BUILT_IN                                              ((int32_t)0xE7120021L);

			// Failed to resolve URL. 
			constexpr Result 			SVR_CURL_COULDNT_RESOLVE_PROXY                                     ((int32_t)0xE7120022L);

			// Failed to resolve URL. 
			constexpr Result 			SVR_CURL_COULDNT_RESOLVE_HOST                                      ((int32_t)0xE7120023L);

			// Connection is failed 
			constexpr Result 			SVR_CURL_COULDNT_CONNECT                                           ((int32_t)0xE7120024L);

			// HTTP Post can't be done. 
			constexpr Result 			SVR_CURL_HTTP_POST_ERROR                                           ((int32_t)0xE7120025L);

			// Not supported SSL type. 
			constexpr Result 			SVR_CURL_SSL_ENGINE_NOTFOUND                                       ((int32_t)0xE7120026L);

			// Not supported SSL type. 
			constexpr Result 			SVR_CURL_SSL_ENGINE_SETFAILED                                      ((int32_t)0xE7120027L);

			// Network send failed 
			constexpr Result 			SVR_CURL_SEND_ERROR                                                ((int32_t)0xE7120028L);

			// Network receive failed 
			constexpr Result 			SVR_CURL_RECV_ERROR                                                ((int32_t)0xE7120029L);

			// Invalid SSL setting. 
			constexpr Result 			SVR_CURL_USE_SSL_FAILED                                            ((int32_t)0xE712002AL);

			// No Connection is available now. 
			constexpr Result 			SVR_CURL_NO_CONNECTION_AVAILABLE                                   ((int32_t)0xE712002BL);

			// Invalid state transition 
			constexpr Result 			SVR_INVALID_STATE                                                  ((int32_t)0xE712002CL);

			// Invalid purchase environment(public/test) 
			constexpr Result 			SVR_INVALID_PURCHASE_MODE                                          ((int32_t)0xE712002DL);

			// Invalid Task group ID 
			constexpr Result 			SVR_INVALID_TASK_GROUPID                                           ((int32_t)0xE712002EL);

	}//namespace ResultCode 
}//namespace SF 





