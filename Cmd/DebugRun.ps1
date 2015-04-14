

$SetPathExist = (Test-Path ".\SetParam.ps1")

if (!($SetPathExist))
{
	copy _SetParam.ps1 SetParam.ps1
	Start-process notepad -ArgumentList {"SetParam.ps1"} -Wait
}


.\SetParam.ps1



$MasterServerList=$env:MasterServerList
$ServerList=$env:ServerList


$BRRoot="..\..\"
$BRBase=$BRRoot+"\Server\"
$BRDbgBin=$BRBase+"\bin\x64Debug"
$BRCfgFile=$env:SRCfgFile

start-transaction

use-transaction -TransactedScript { Remove-Item ($BRDbgBin+"\*_log.txt") -force } -UseTransaction
use-transaction -TransactedScript { Remove-Item ($BRDbgBin+"\*_logdbg.txt") -force } -UseTransaction
use-transaction -TransactedScript { Remove-Item ($BRDbgBin+"\*.dmp") -force } -UseTransaction


complete-transaction


$Parameters = "-debug"

# echo "Starting master servers"
# foreach ( $Server in $MasterServerList )
# {
	# $ServerBinName=$BRDbgBin+"\" + $Server  + ".exe"
	# echo $ServerBinName
	# start-process $ServerBinName -ArgumentList $Parameters
# }

Start-Sleep -s 3

echo "Starting servers"
foreach ( $Server in $ServerList )
{
	$ServerBinName=$BRDbgBin+"\" + $Server  + ".exe"
	echo $ServerBinName
	start-process $ServerBinName -ArgumentList $Parameters
}





