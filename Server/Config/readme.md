
# Server Config XML


	DBInstances: Database instances. list all db instances and its access information. later they will be accessed by db cluster definition.
				 DB 인스턴스 정보. 모든 DB 인스턴스들과 억세스 정보들. 나중에 DB cluster를 정의할때 사용됩니다.
		- InstanceName: Instance name. Used to find instance information.
		- ConnectionString: DB connection string. exclude ID/PW
		- UserID: DB instance access ID
		- Password: DB instance access password

	DBCluster: DB cluster information. 
		- ClusterType: BRServer system support DB clustering - Normal and sharding.
			- Normal: no scale out. you can use DB engine's scale out feature
			- Sharding: Sharding scale out. Round robin is used for selecting shard partition. If you use thie type. the database with DBName should have tblshardinfo holding all db instances
		- DBInstanceName: DB instance name to use. We only assign the first db instance. Rest of them will be queried from tblshardinfo DB table dynamically
		- DBName: database name 
		
	TableDB: game table database. If a server needs game related table information, they are requested from here.
		- DBInstanceName: DB instance name to use.
		- DBName: database name 

	AccountDB: Account information DB. Server system doesn't support any scale out for this DB, so ClusterType should be Normal.
	LoginSessionDB: DBCluster type. Login session management DBCluster
	RankingDB: DBCluster type. ranking information
	
	GameCluster: Game cluster information. Each game type should have it's own game cluster. All server instances in the same gamecluster will be connected each other.
		GameDB: DBCluster type, Game specific database including Player information.
		GameTransactionLogDB: DBCluster type. Game transaction log DB
		Servers: Module servers, Generic server type which can assign modules
		Game: Game frontend servers, Custom frontend game server
		GameInstance: Game instance servers, semi-frontend game instance server
		
		
	-- Server instance types
	GenericServer: generic server instance with module customization.
		- NetPrivate: Private network information used for server to server communication.
		- Server Service Modules
		
			- ModGamePartyManager: Party service manager
			- ModMonitoring: Server monitoring module. Manages server performance counters. Suspended at this moment.
			- ModLogin: Login support module
			- ModRanking: Realtime ranking module
			- ModGame: Game service module, Game server should include this module for public network and user entity support
			- ModInstanceManager: Reserved, Not supported yet.
			- ModMatching_Game_4: 4 person game instance matching module. It requires ModMatchingQueue_Game_4x1 ~ ModMatchingQueue_Game_4x1W matching queues. Matching queues can be exist on any server instance in the same game cluster. However, all of them should be exist at least one each.
			- ModMatching_Game_8: 8 person game instance matching module. It requires ModMatchingQueue_Game_8x1 ~ ModMatchingQueue_Game_8x1W matching queues. Matching queues can be exist on any server instance in the same game cluster. However, all of them should be exist at least one each.
			- ModMatchingQueue_Game_4x1 ~ ModMatchingQueue_Game_4x1W: 4 person game instance matching queues. 
			- ModMatchingQueue_Game_8x1 ~ ModMatchingQueue_Game_8x1W: 8 person game instance matching queues. 
			- ModPurchaseValidateGoogle: google Auth information for google purchase verification
				- Account: Google account
				- P12KeyFile: P12Key file
				- AuthScopes: authrization scope
			- ModPurchaseValidateIOS: IOS auth information for ios purchase verification
				- URL: URL to request.
				- Use https://buy.itunes.apple.com/verifyReceipt for release purchase.
				- Use https://sandbox.itunes.apple.com/verifyReceipt for test purchase.
			
		- NetPublic: Some modules supporting public network service will require this setting.
			IPV6: IPV6 public address, requires IPV6 public network address. you can specify IPV4 address in case you don't need to support IPV6.
			IPV4: IPV4 public address
			ListenIP: H/W listen IP. It should be IPV6 address if you want to support IPV6 network service. This is ip address to decide which net device is going to use for public network service.
						If you don't care, you can use any IP "::".
			Port: Service listen port

	- 
			
			
# Log config file - traceConfig.cfg

Each line explains each mode's bit mask for log print

