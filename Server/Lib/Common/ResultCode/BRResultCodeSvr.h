﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated header, Svr
//
////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>



// Server service failed 
#define E_SVR_SERVICE_FAILED                                          ((int32_t)0xE7040000L) 

// Timed out 
#define E_SVR_TIMEOUT                                                 ((int32_t)0xE7040001L) 

// Transaction has invalid state transition 
#define E_SVR_TRANSACTION_INVALID_STATE                               ((int32_t)0xE7040002L) 

// Server Transaction timed out 
#define E_SVR_INVALID_TRANSITION                                      ((int32_t)0xE7040003L) 

// Transaction process Time out 
#define E_SVR_TRANSACTION_TIMEOUT                                     ((int32_t)0xE7040004L) 

// Failed to route message 
#define E_SVR_ROUTEMSG_FAILED                                         ((int32_t)0xE7040005L) 

// No result handler 
#define E_SVR_NO_RESULT_HANDLER                                       ((int32_t)0xE7040006L) 

// No message handler 
#define E_SVR_NO_MESSAGE_HANDLER                                      ((int32_t)0xE7040007L) 

// Invalid server ID 
#define E_SVR_INVALID_SERVERID                                        ((int32_t)0xE7040008L) 

// Player not found 
#define E_SVR_PLAYER_NOT_FOUND                                        ((int32_t)0xE7040009L) 

// Player state is invalid 
#define E_SVR_INVALID_PLAYER_STATE                                    ((int32_t)0xE704000AL) 

// Invalid player game db data 
#define E_SVR_INVALID_PLAYER_GAMEDB                                   ((int32_t)0xE704000BL) 

// Not expected server message 
#define E_SVR_NOTEXPECTED_MESSAGE                                     ((int32_t)0xE704000CL) 

// Invalid entity UID 
#define E_SVR_INVALID_ENTITYUID                                       ((int32_t)0xE704000DL) 

// Cluster service not ready 
#define E_SVR_CLUSTER_NOTREADY                                        ((int32_t)0xE704000EL) 

// Invalid cluster type 
#define E_SVR_INVALID_CLUSTERTYPE                                     ((int32_t)0xE704000FL) 

// Invalid cluster ID 
#define E_SVR_INVALID_CLUSTERID                                       ((int32_t)0xE7040010L) 

// Invalid queue item 
#define E_SVR_INVALID_QUEUEITEM                                       ((int32_t)0xE7040011L) 

// Queue item is canceled 
#define E_SVR_QUEUEITEM_CANCELED                                      ((int32_t)0xE7040012L) 

// There is no item in queue 
#define E_SVR_NOITEM_INQUEUE                                          ((int32_t)0xE7040013L) 

// Already in queue 
#define E_SVR_ALREADY_INQUEUE                                         ((int32_t)0xE7040014L) 

// Queue item is reserved 
#define E_SVR_RESERVED_QUEUEITEM                                      ((int32_t)0xE7040015L) 

// Cluster doesn't have any assigned master 
#define E_SVR_CLUSTER_NOMASTER                                        ((int32_t)0xE7040016L) 

// Queue item is reserved for cancel 
#define S_SVR_QUEUEITEM_CANCEL_PENDED                                 ((int32_t)0x27040017L) 

// Thread is wrong, running on wrong thread? 
#define E_SVR_INVALID_THREAD                                          ((int32_t)0xE7040018L) 

// Purchase canceled 
#define E_SVR_PURCHASE_CANCELED                                       ((int32_t)0xE7040019L) 

// Invalid receipt info 
#define E_SVR_INVALID_PURCHASE_INFO                                   ((int32_t)0xE704001AL) 

// Invalid duplicated purchase 
#define E_SVR_INVALID_PURCHASE_DUPLICATED                             ((int32_t)0xE704001BL) 

// External auth fail 
#define E_SVR_INVALID_EXTERNAL_AUTH                                   ((int32_t)0xE704001CL) 

// External server is not available 
#define E_SVR_EXTERNAL_SERVER_UNAVALIABLE                             ((int32_t)0xE704001DL) 

// Invalid receipt result from server 
#define E_SVR_INVALID_PURCHASE_FORMAT                                 ((int32_t)0xE704001EL) 

// The protocol is not supported. 
#define E_SVR_CURL_UNSUPPORTED_PROTOCOL                               ((int32_t)0xE704001FL) 

// CURL initialization is failed. 
#define E_SVR_CURL_FAILED_INIT                                        ((int32_t)0xE7040020L) 

// Invalid URL format. 
#define E_SVR_CURL_URL_MALFORMAT                                      ((int32_t)0xE7040021L) 

// Not a build-in feature. 
#define E_SVR_CURL_NOT_BUILT_IN                                       ((int32_t)0xE7040022L) 

// Failed to resolve URL. 
#define E_SVR_CURL_COULDNT_RESOLVE_PROXY                              ((int32_t)0xE7040023L) 

// Failed to resolve URL. 
#define E_SVR_CURL_COULDNT_RESOLVE_HOST                               ((int32_t)0xE7040024L) 

// Connection is failed 
#define E_SVR_CURL_COULDNT_CONNECT                                    ((int32_t)0xE7040025L) 

// HTTP Post can't be done. 
#define E_SVR_CURL_HTTP_POST_ERROR                                    ((int32_t)0xE7040026L) 

// Not supported SSL type. 
#define E_SVR_CURL_SSL_ENGINE_NOTFOUND                                ((int32_t)0xE7040027L) 

// Not supported SSL type. 
#define E_SVR_CURL_SSL_ENGINE_SETFAILED                               ((int32_t)0xE7040028L) 

// Network send failed 
#define E_SVR_CURL_SEND_ERROR                                         ((int32_t)0xE7040029L) 

// Network receive failed 
#define E_SVR_CURL_RECV_ERROR                                         ((int32_t)0xE704002AL) 

// Invalid SSL setting. 
#define E_SVR_CURL_USE_SSL_FAILED                                     ((int32_t)0xE704002BL) 

// No Connection is available now. 
#define E_SVR_CURL_NO_CONNECTION_AVAILABLE                            ((int32_t)0xE704002CL) 


