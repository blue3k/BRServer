﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated file, Common
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

		// Called without initialization. 
		public const int E_NOT_INITIALIZED =                                unchecked((int)0xE7020000); 

		// The version not available or miss match with system. 
		public const int E_VERSION_NOT_AVAIL =                              unchecked((int)0xE7020001); 

		// Invalid Cluster Master server 
		public const int E_INVALID_CLUSTERMASTER =                          unchecked((int)0xE7020002); 

		// The version not available or miss match with system. 
		public const int E_INVALID_STATE =                                  unchecked((int)0xE7020003); 

		// Invalid string format. 
		public const int E_INVALID_STR_FORMAT =                             unchecked((int)0xE7020004); 

		// ServerID is duplicated. 
		public const int E_DUPLICATED_SERVERID =                            unchecked((int)0xE7020005); 

		// Invalid entity 
		public const int E_INVALID_ENTITY =                                 unchecked((int)0xE7020006); 

		// Invalid duplicated entity 
		public const int E_DUPLICATED_ENTITY =                              unchecked((int)0xE7020007); 

		// Invalid duplicated entity 
		public const int E_DUPLICATED_ENTITYID =                            unchecked((int)0xE7020008); 

		// Duplicated Account ID 
		public const int E_DUPLICATED_ACCOUNTID =                           unchecked((int)0xE7020009); 

		// Duplicated Player ID 
		public const int E_DUPLICATED_PLAYERID =                            unchecked((int)0xE702000A); 

		// Duplicated name 
		public const int E_DUPLICATED_NAME =                                unchecked((int)0xE702000B); 

		// Duplicated ID 
		public const int E_DUPLICATED_ID =                                  unchecked((int)0xE702000C); 

		// Invalid duplicated entity 
		public const int E_INVALID_ACCOUNTID =                              unchecked((int)0xE702000D); 

		// Invalid ticket 
		public const int E_INVALID_TICKET =                                 unchecked((int)0xE702000E); 

		// Invalid player ID 
		public const int E_INVALID_PLAYERID =                               unchecked((int)0xE702000F); 

		// Invalid value 
		public const int E_INVALID_VALUE =                                  unchecked((int)0xE7020010); 

		// Invalid class value 
		public const int E_INVALID_CLASS =                                  unchecked((int)0xE7020011); 

		// Invalid Difficulty 
		public const int E_INVALID_DIFFICULTY =                             unchecked((int)0xE7020012); 

		// Invalid registration to play 
		public const int E_INVALID_REGISTRATION =                           unchecked((int)0xE7020013); 

		// Invalid Channel ID 
		public const int E_INVALID_CHANNELID =                              unchecked((int)0xE7020014); 

		// Invalid Zone instance ID 
		public const int E_INVALID_INSTANCEID =                             unchecked((int)0xE7020015); 

		// Duplicated component 
		public const int E_DUPLICATED_COMPONENT =                           unchecked((int)0xE7020016); 

		// Maximum friend 
		public const int E_MAX_FRIEND =                                     unchecked((int)0xE7020017); 

		// Invalid player count for service 
		public const int E_INVALID_PLAYER_COUNT =                           unchecked((int)0xE7020018); 

		// Invalid player level 
		public const int E_INVALID_PLAYER_LEVEL =                           unchecked((int)0xE7020019); 

		// Invalid player nick 
		public const int E_INVALID_PLAYER_NICK =                            unchecked((int)0xE702001A); 

		// Invalid Notification ID 
		public const int E_INVALID_NOTIFICATIONID =                         unchecked((int)0xE702001B); 

		// Invalid Ranking range 
		public const int E_INVALID_RANKING_RANGE =                          unchecked((int)0xE702001C); 

		// Target Maximum friend 
		public const int E_TARGET_MAX_FRIEND =                              unchecked((int)0xE702001D); 

		// Invalid string data. 
		public const int E_INVALID_STR_DATA =                               unchecked((int)0xE702001E); 

		// Not authorized 
		public const int E_NOT_AUTORIZED =                                  unchecked((int)0xE702001F); 

		// No permition 
		public const int E_NO_PERMITION =                                   unchecked((int)0xE7020020); 

		// No file or directory 
		public const int E_NO_FILE_OR_DIR =                                 unchecked((int)0xE7020021); 

		// No such process 
		public const int E_NO_SUCH_PROCESS =                                unchecked((int)0xE7020022); 

		// Interrupted system call 
		public const int E_INTERRUPTED_SYSCALL =                            unchecked((int)0xE7020023); 

		// I/O error 
		public const int E_IO_ERROR =                                       unchecked((int)0xE7020024); 

		// No such device or address 
		public const int E_NOT_DEVICE =                                     unchecked((int)0xE7020025); 

		// Arg list too long 
		public const int E_TOO_MANY_ARGLIST =                               unchecked((int)0xE7020026); 

		// Exec format error 
		public const int E_INVALID_EXEC_FROMAT =                            unchecked((int)0xE7020027); 

		// Bad file number 
		public const int E_INVALID_FILE_HANDLE =                            unchecked((int)0xE7020028); 

		// No child process 
		public const int E_NO_CHILD_PROCESS =                               unchecked((int)0xE7020029); 

		// Try again 
		public const int E_TRY_AGAIN =                                      unchecked((int)0xE702002A); 

		// Bad address 
		public const int E_INVALID_POINTER =                                unchecked((int)0xE702002B); 

		// Block device required 
		public const int E_NOT_BLOCK =                                      unchecked((int)0xE702002C); 

		// Busy 
		public const int E_BUSY =                                           unchecked((int)0xE702002D); 

		// File exists 
		public const int E_FILE_EXISTS =                                    unchecked((int)0xE702002E); 

		// Cross-device link 
		public const int E_CROSS_LINK =                                     unchecked((int)0xE702002F); 

		// No such device 
		public const int E_INVALID_DEVICE =                                 unchecked((int)0xE7020030); 

		// Not a directory 
		public const int E_INVALID_DIRECTORY =                              unchecked((int)0xE7020031); 

		// Is a directory 
		public const int E_DIRECTORY =                                      unchecked((int)0xE7020032); 

		// Invalid argument 
		public const int E_INVALID_ARG =                                    unchecked((int)0xE7020033); 

		// File table overflow 
		public const int E_NOT_ENOUGH_FILE_TABLE =                          unchecked((int)0xE7020034); 

		// Too many open files 
		public const int E_TOO_MANY_OPENED_FILE =                           unchecked((int)0xE7020035); 

		// Not a typewriter 
		public const int E_INVALID_TYPEWRITER =                             unchecked((int)0xE7020036); 

		// Text file busy 
		public const int E_BUSY_FILE =                                      unchecked((int)0xE7020037); 

		// File too large 
		public const int E_TOO_BIG_FILE =                                   unchecked((int)0xE7020038); 

		// No space left on device 
		public const int E_NOT_ENOUGH_SPACE =                               unchecked((int)0xE7020039); 

		// Illegal seek 
		public const int E_INVALID_SEEK =                                   unchecked((int)0xE702003A); 

		// Read-only file system 
		public const int E_READ_ONLY =                                      unchecked((int)0xE702003B); 

		// Too many links 
		public const int E_TOO_MANY_LINK =                                  unchecked((int)0xE702003C); 

		// Broken pipe 
		public const int E_INVALID_PIPE =                                   unchecked((int)0xE702003D); 

		// Math argument out of domain of func 
		public const int E_INVALID_MATH_DOMAIN =                            unchecked((int)0xE702003E); 

		// Math result not representable 
		public const int E_INVALID_NUMERIC =                                unchecked((int)0xE702003F); 

		// Invalid password 
		public const int E_INVALID_PASSWORD =                               unchecked((int)0xE7020040); 

		// Target already in friends list 
		public const int E_ALREADY_IN_FRIEND =                              unchecked((int)0xE7020041); 

		// Thread is wrong, running on wrong thread? 
		public const int E_INVALID_THREAD =                                 unchecked((int)0xE7020042); 

	}
}

