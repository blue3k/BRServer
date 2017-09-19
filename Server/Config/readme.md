DBInstances: Database instances. list all db instances and its access information. later they will be accessed by db cluster definition.
             DB 인스턴스 정보. 모든 DB 인스턴스들과 억세스 정보들. 나중에 DB cluster를 정의할때 사용됩니다.
	- InstanceName: Instance name. Used to find instance information.
	- ConnectionString: DB connection string. exclude ID/PW
	- UserID: DB instance access ID
	- Password: DB instance access password

MonitoringServer: Monitoring server information. All servers send performance counter information to monitoring server.
                  모니터링 서버 정보. 모든 서버들은 모니터링 서버로 performance counter정보를 전송합니다.

TableDB: game table database. If a server needs game related table information, they are requested to here.


-- Server instance types
GenericServer: generic server instance.
	- ComponentGoogle: google Auth information for google purchase verification
		- Account: Google account
		- P12KeyFile: P12Key file
		- AuthScopes: authrization scope
	- ComponentIOS: IOS auth information for ios purchase verification
		- URL: URL to request.
		- Use https://buy.itunes.apple.com/verifyReceipt for release purchase.
		- Use https://sandbox.itunes.apple.com/verifyReceipt for test purchase.
	- NetPrivate: Private network information used for server to server communication.

PublicServer: old regacy server with public IP specification.
	
ModuleServer: Server with service module. Except some special old server module, all server services are modulized. ModuleServer is an server instance can hold those server services.
		The basic idea doint this is for 

		
-- Server instance components		
NetPublic: public network information
	- IPV4: IPV4 IP address
	- IPV6: IPV6 IP address
	- ListenIP: Actual listening IP. you can use "::" for dual stack service.
	- Potr: Port

ModMatchingQueue_Game_*: matching queue modules. Matching service is queue based, and for each team member need separate queue per team member count.

ModMatching_Game_4: 4 member game matching service
ModMatching_Game_8: 8 member game matching service

ModGamePartyManager: Party manager module. Support party service.

 