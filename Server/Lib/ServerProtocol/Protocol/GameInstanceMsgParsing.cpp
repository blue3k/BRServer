////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 Kyungkun Ko
// 
// Author : Generated
// 
// Description : GameInstance Message parsing implementations
// 
////////////////////////////////////////////////////////////////////////////////


#include "ServerProtocolPCH.h"
#include "SFTypedefs.h"
#include "Protocol/SFProtocol.h"
#include "Net/SFNetDef.h"
#include "Protocol/GameInstanceMsgParsing.h"
#include "Protocol/GameInstanceMsgClass.h"



namespace SF
{
 	namespace Protocol
	{
 
		///////////////////////////////////////////////////////////////
		// GameInstance message parser mapping

		extern std::unordered_map<uint32_t,HandleParseMessageToMessageBase> MessageParseToMessageBaseMap;

		void RegisterParserGameInstance()
		{
 			// Cmd: Join to a game instance. You can call multiple times, but it would be a waste
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::JoinGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::JoinGameInstanceRes::ParseMessageToMessageBase));
			// Cmd: Leave game instance.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceCmd::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::LeaveGameInstanceRes::MID.IDSeq.MsgID,&Message::GameInstance::LeaveGameInstanceRes::ParseMessageToMessageBase));
			// S2C: Player left event.
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::PlayerLeftS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::PlayerLeftS2CEvt::ParseMessageToMessageBase));
			// Cmd: Player. revive himself
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveCmd::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveCmd::ParseMessageToMessageBase));
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerReviveRes::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerReviveRes::ParseMessageToMessageBase));
			// S2C: Player is revived
			MessageParseToMessageBaseMap.insert(std::make_pair(Message::GameInstance::GamePlayerRevivedS2CEvt::MID.IDSeq.MsgID,&Message::GameInstance::GamePlayerRevivedS2CEvt::ParseMessageToMessageBase));
		}; // void RegisterParserGameInstance()


	}; // namespace Protocol
}; // namespace SF


