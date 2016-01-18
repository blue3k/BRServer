﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Autogenerated file, Game
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

		// User is already in a game 
		const int E_GAME_ALREADY_IN_GAME =                                  unchecked((int)0xE7070000); 

		// Invalid player 
		const int E_GAME_INVALID_PLAYER =                                   unchecked((int)0xE7070001); 

		// Game is not ready 
		const int E_GAME_NOTREADY =                                         unchecked((int)0xE7070002); 

		// Invalid player role 
		const int E_GAME_INVALID_ROLE =                                     unchecked((int)0xE7070003); 

		// Not ready for next game play state 
		const int E_GAME_NOT_READY_FOR_NEXT_STATE =                         unchecked((int)0xE7070004); 

		// Invalid vote type 
		const int E_GAME_INVALID_VOTETYPE =                                 unchecked((int)0xE7070005); 

		// Invalid vote state 
		const int E_GAME_INVALID_VOTE_STATE =                               unchecked((int)0xE7070006); 

		// Invalid vote target 
		const int E_GAME_INVALID_VOTE_TARGET =                              unchecked((int)0xE7070007); 

		// No suffrage 
		const int E_GAME_NO_SUFFRAGE =                                      unchecked((int)0xE7070008); 

		// Invalid game state 
		const int E_GAME_INVALID_GAMESTATE =                                unchecked((int)0xE7070009); 

		// Invalid partyID 
		const int E_GAME_INVALID_PARTYID =                                  unchecked((int)0xE707000A); 

		// Invalid player count for game 
		const int E_GAME_INVALID_PLAYER_COUNT =                             unchecked((int)0xE707000B); 

		// User is already in a party 
		const int E_GAME_ALREADY_IN_PARTY =                                 unchecked((int)0xE707000C); 

		// User doesn't have eligible privilege 
		const int E_GAME_NO_PRIVILEGE =                                     unchecked((int)0xE707000D); 

		// Invalid shop item data 
		const int E_GAME_INVALID_SHOPITEM =                                 unchecked((int)0xE707000E); 

		// Invalid shop item id 
		const int E_GAME_INVALID_SHOPITEMID =                               unchecked((int)0xE707000F); 

		// Not enough money(gold, gem, etc) 
		const int E_GAME_NOTENOUGH_RESOURCE =                               unchecked((int)0xE7070010); 

		// Doesn't have enough stamina 
		const int E_GAME_LOW_STAMINA =                                      unchecked((int)0xE7070011); 

		// Invalid player game state 
		const int E_GAME_INVALID_PLAYER_STATE =                             unchecked((int)0xE7070012); 

		// Maximum notification count 
		const int E_GAME_MAX_NOTIFICATION =                                 unchecked((int)0xE7070013); 

		// Maximum try reached 
		const int E_GAME_MAX_TRY =                                          unchecked((int)0xE7070014); 

		// Invalid cost table id 
		const int E_GAME_INVALID_COSTID =                                   unchecked((int)0xE7070015); 

		// Invalid bot talk table 
		const int E_GAME_INVALID_BOTTALK_TABLE =                            unchecked((int)0xE7070016); 

	}
}

