#!/bin/bash



export REMOTE_BASEPATH=/GameServer/Server/bin/DEBUG
export SSH_PARAMS='-f blue3k@10.133.29.149 -i ./Bravesprivate2.popen'

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BREntityServer -n:BREntityServer1"

sleep 2s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BREntityServer -n:BREntityServer2"

sleep 5s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRModuleServer -n:BRLoginServer01"


ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRModuleServer -n:BRMatchingQueue01"


sleep 1s


ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameServer -n:BRConspiracyGameServer01"
ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameServer -n:BRConspiracyGameServer02"

sleep 1s

ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameInstanceServer -n:BRConspiracyGameInstanceServer01"
ssh $SSH_PARAMS "$REMOTE_BASEPATH/BRGameInstanceServer -n:BRConspiracyGameInstanceServer02"



