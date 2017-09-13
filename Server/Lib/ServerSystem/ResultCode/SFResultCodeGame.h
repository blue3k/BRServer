﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 StormForge
// 
// Author : KyungKun Ko
//
// Description : Autogenerated header, Game
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>



namespace SF {
	namespace ResultCode {



			// User is already in a game 
			constexpr Result 			GAME_ALREADY_IN_GAME                                               (0xE7140000L);

			// Invalid player 
			constexpr Result 			GAME_INVALID_PLAYER                                                (0xE7140001L);

			// Game is not ready 
			constexpr Result 			GAME_NOTREADY                                                      (0xE7140002L);

			// Invalid player role 
			constexpr Result 			GAME_INVALID_ROLE                                                  (0xE7140003L);

			// Not ready for next game play state 
			constexpr Result 			GAME_NOT_READY_FOR_NEXT_STATE                                      (0xE7140004L);

			// Invalid vote type 
			constexpr Result 			GAME_INVALID_VOTETYPE                                              (0xE7140005L);

			// Invalid vote state 
			constexpr Result 			GAME_INVALID_VOTE_STATE                                            (0xE7140006L);

			// Invalid vote target 
			constexpr Result 			GAME_INVALID_VOTE_TARGET                                           (0xE7140007L);

			// No suffrage 
			constexpr Result 			GAME_NO_SUFFRAGE                                                   (0xE7140008L);

			// Invalid game state 
			constexpr Result 			GAME_INVALID_GAMESTATE                                             (0xE7140009L);

			// Invalid partyID 
			constexpr Result 			GAME_INVALID_PARTYID                                               (0xE714000AL);

			// Invalid player count for game 
			constexpr Result 			GAME_INVALID_PLAYER_COUNT                                          (0xE714000BL);

			// User is already in a party 
			constexpr Result 			GAME_ALREADY_IN_PARTY                                              (0xE714000CL);

			// User doesn't have eligible privilege 
			constexpr Result 			GAME_NO_PRIVILEGE                                                  (0xE714000DL);

			// Invalid shop item data 
			constexpr Result 			GAME_INVALID_SHOPITEM                                              (0xE714000EL);

			// Invalid shop item id 
			constexpr Result 			GAME_INVALID_SHOPITEMID                                            (0xE714000FL);

			// Not enough money(gold, gem, etc) 
			constexpr Result 			GAME_NOTENOUGH_RESOURCE                                            (0xE7140010L);

			// Doesn't have enough stamina 
			constexpr Result 			GAME_LOW_STAMINA                                                   (0xE7140011L);

			// Invalid player game state 
			constexpr Result 			GAME_INVALID_PLAYER_STATE                                          (0xE7140012L);

			// Maximum notification count 
			constexpr Result 			GAME_MAX_NOTIFICATION                                              (0xE7140013L);

			// Maximum try reached 
			constexpr Result 			GAME_MAX_TRY                                                       (0xE7140014L);

			// Invalid cost table id 
			constexpr Result 			GAME_INVALID_COSTID                                                (0xE7140015L);

			// Invalid bot talk table 
			constexpr Result 			GAME_INVALID_BOTTALK_TABLE                                         (0xE7140016L);

	}//namespace ResultCode 
}//namespace SF 




