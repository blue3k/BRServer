using System;
using System.Net;
using BR.Message.Game;
using BR.Message;
using BR;
using BR.Net;
using System.Collections;
using System.Collections.Generic;
using Cocos2D;
using BR.GameGraphics;

namespace BR.GameSystem
{
    // Player id/name map
    public partial class PlayerInfo
    {
        private UInt64 playerID;

        public UInt64 FacebookUID;
        public string NickName;

        public UInt32 Level;


        #region Modules

        public FriendManager.FriendInformation FriendInfo;
        public PlayerManager.GameInformation GameInfo;
        public RankingManager.RankingInformation RankingInfo;

        #endregion


        public UInt64 PlayerID
        { 
            get 
            { 
                return playerID; 
            }
            set
            {
                playerID = value;
            }
        }

    };


} // namespace BR.GameSystem
