#!/bin/bash

REMOTE_BASEPATH=/GameServer/Server/bin/DEBUG
SSH_PARAMS='-f blue3k@10.133.29.154 -i ./Bravesprivate2.popen'


ssh $SSH_PARAMS "pkill BRGameInstanceS; pkill BRModuleServer"
sleep 0.5s
ssh $SSH_PARAMS "pkill BRGameServer; pkill BRLoginServer"
sleep 0.5s
ssh $SSH_PARAMS "pkill BREntityServer"



