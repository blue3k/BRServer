

$SetPathExist = (Test-Path ".\SetParam.ps1")

if (!($SetPathExist))
{
	copy _SetParam.ps1 SetParam.ps1
	Start-process notepad -ArgumentList {"SetParam.ps1"} -Wait
}


.\SetParam.ps1

$MasterServerList=$env:MasterServerList
$ServerList=$env:ServerList


echo "Stopping servers"
foreach ( $Server in $ServerList )
{
	stop-process -Name $Server
}

# echo "Stopping master servers"
# foreach ( $Server in $MasterServerList )
# {
	# stop-process -Name $Server
# }

Start-Sleep -s 3
