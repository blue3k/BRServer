#!/bin/bash



REMOTE_BASEPATH=/GameServer/Server/bin/DEBUG
SSH_PARAMS=-f blue3k@161.202.74.206 -i ./LinuxBuild/Bravesprivate2.popen

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BREntityServer -n:BREntityServer1"

sleep 2s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BREntityServer -n:BREntityServer2"

sleep 2s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRLoginServer -n:BRLoginServer01"


ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRSharedModuleServer -n:BRMatchingQueue01"


sleep 1s


ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameServer -n:BRConspiracyGameServer01"
ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameServer -n:BRConspiracyGameServer02"

sleep 1s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameInstanceServer -n:BRConspiracyGameInstanceServer01"
ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameInstanceServer -n:BRConspiracyGameInstanceServer02"



