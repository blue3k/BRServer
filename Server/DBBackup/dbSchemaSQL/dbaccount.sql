/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbaccount
*********************************************************************
*/

/*!40101 SET NAMES utf8mb4 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbAccount` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE `dbAccount`;

/*Table structure for table `tblCharacterShardGroup` */

DROP TABLE IF EXISTS `tblCharacterShardGroup`;

CREATE TABLE `tblCharacterShardGroup` (
  `ShardID` INT NOT NULL,
  `WorkLoad` INT NOT NULL DEFAULT '0',
  `DBName` VARCHAR(64) NOT NULL,
  `TableName` VARCHAR(64) NOT NULL,
  PRIMARY KEY (`ShardID`),
  KEY `LoadOrder` (`WorkLoad`)
) ENGINE=INNODB;

/*Table structure for table `tblShardInfo` */

DROP TABLE IF EXISTS `tblShardInfo`;

CREATE TABLE `tblShardInfo` (
  `ShardID` INT NOT NULL,
  `ConnectionString` VARCHAR(512) NOT NULL,
  `DBName` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`ShardID`)
) ENGINE=INNODB;

/*Table structure for table `tblUser` */

DROP TABLE IF EXISTS `tblUser`;

CREATE TABLE `tblUser` (
  `UserUID` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'User unique ID which is assigned by us',
  `UserName` VARCHAR(128) DEFAULT NULL COMMENT 'User account name',
  `UserPassword` VARCHAR(128) DEFAULT NULL COMMENT 'Password',
  `FBUserID` BIGINT DEFAULT NULL COMMENT 'Facebook user ID',
  `LatestLoggedIn` TIMESTAMP NULL DEFAULT NULL COMMENT 'Latest logged in time',
  `CreatedDate` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `LoginToken` VARCHAR(512) NOT NULL DEFAULT '0',
  `EMail` VARCHAR(45) DEFAULT NULL,
  `GCMKeys` VARCHAR(512) DEFAULT NULL,
  `ShardID` INT DEFAULT '0' COMMENT 'Sharding ID',
  `CellPhone` VARCHAR(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`UserUID`),
  UNIQUE KEY `UserID_UNIQUE` (`UserUID`),
  UNIQUE KEY `UserName_UNIQUE` (`UserName`),
  UNIQUE KEY `FBUserID_UNIQUE` (`FBUserID`),
  UNIQUE KEY `Email_UNIQUE` (`EMail`)
) ENGINE=INNODB AUTO_INCREMENT=240579 COMMENT='User account table';

/*!50106 set global event_scheduler = 1*/;

/* Event structure for event `evtUpdateCharacterShardLoad` */

/*!50106 DROP EVENT IF EXISTS `evtUpdateCharacterShardLoad`*/;

DELIMITER $$

/*!50106 CREATE EVENT `evtUpdateCharacterShardLoad` ON SCHEDULE EVERY 1 MINUTE STARTS '2015-03-21 11:00:59' ON COMPLETION NOT PRESERVE ENABLE DO BEGIN
		
		call spUpdateCharacterShardLoad();
	    
	END */$$
DELIMITER ;

/* Procedure structure for procedure `spAssignCharacterShard` */

/*!50003 DROP PROCEDURE IF EXISTS  `spAssignCharacterShard` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spAssignCharacterShard`(
	out outShardID int
    )
BEGIN
    
	declare numberOfShard int;
	declare shardOrder int;
	DECLARE shardRand int;
	DECLARE shardShifted INT;
	
	SELECT count(*) into numberOfShard  from tblCharacterShardGroup;
	
	-- random shard selection
	-- top most shard will have twice possibility than others
	set shardRand = FLOOR((RAND() * (numberOfShard * 2)));
	set shardShifted = shardRand - numberOfShard;
	set shardOrder = GREATEST( shardShifted, 0 );
	-- select numberOfShard, shardRand, shardShifted, shardOrder;
	SELECT ShardID into outShardID FROM tblCharacterShardGroup ORDER BY WorkLoad LIMIT shardOrder,1;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateDebugTestPlayer` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateDebugTestPlayer` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateDebugTestPlayer`()
BEGIN
	CALL spCreateSystemPlayer('Tester', 0, 40, 2);
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateRandomUser` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateRandomUser` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateRandomUser`(
	IN inUserName VARCHAR(128),
	IN inCellPhone VARCHAR(128),
	OUT outUID BIGINT,
	out outFBUserID bigint,
	OUT outShardID INT,
	OUT result INT
)
BEGIN
	DECLARE randomShardID INT;
	SET result = 0;
	SELECT UserUID, `FBUserID`, ShardID INTO outUID, outFBUserID, outShardID FROM tblUser WHERE UserName = inUserName;
	SELECT UserUID, `FBUserID`, ShardID INTO outUID, outFBUserID, outShardID FROM tblUser WHERE UserName = inUserName;
	IF row_COUNT() = 0
	THEN BEGIN
		DECLARE EXIT HANDLER FOR SQLSTATE '23000'
		BEGIN 
			SET result = -1;
		END;
		-- Update table
		
		CALL spAssignCharacterShard(randomShardID);
		
		INSERT INTO tblUser 
			(`UserName`, `UserPassword`, `ShardID`)
			VALUES
			(
			inUserName, inUserName, randomShardID
			);
		SELECT UserUID, `FBUserID`, ShardID INTO outUID, outFBUserID, outShardID FROM tblUser WHERE UserName = inUserName;
		
	END;
	END IF;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateStressTestPlayer` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateStressTestPlayer` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateStressTestPlayer`()
BEGIN
	call spCreateTestPlayer('bottt', 0, 2000, 5);
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateSystemPlayer` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateSystemPlayer` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateSystemPlayer`(
	IN inUserNamePrefix VARCHAR(60),
	IN inStartNumber INT,
	IN inNumPlayer INT,
	in inNumCipher int
    )
BEGIN
	-- CALL spCreateUser( 'bot0001', 'bot0001', 'bot0001', @result );
	
	DECLARE X  INT;
	DECLARE userName  VARCHAR(255);
	DECLARE userEMail  VARCHAR(255);
	SET X = inStartNumber;
	SET userName =  '';
	WHILE X  <= inNumPlayer DO
		   SET userName = CONCAT(inUserNamePrefix,LPAD(X,inNumCipher,'0'));
		   set userEMail = CONCAT(userName,'@test.com');
		   CALL spCreateUser( userName, userName, userEMail, @result );
		   SET  X = X + 1; 
	END WHILE;
    
END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateUser` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateUser` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateUser`(
	in inUserName varchar(128),
	in inPassword varchar(128),
	IN inEMail VARCHAR(128),
	out result int
)
BEGIN
	declare shardID int;
	-- invalid user name
	if inUserName is null or "" then
	begin
		set result = -2;
	end;
	else begin
		-- duplicated player
		declare exit handler for sqlstate '23000'
		begin 
			set result = -1;
		end;
		-- Update table
		set result = 0;
		CALL spAssignCharacterShard(shardID);
		
		INSERT INTO tblUser 
			(`UserName`, `UserPassword`, `EMail`, `ShardID`)
			VALUES
			(
			inUserName, inPassword, inEMail, shardID
			);
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFacebookCreateUser` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFacebookCreateUser` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFacebookCreateUser`(
	in inFBUserID bigint,
	in inEMail varchar(128),
	IN inCellPhone VARCHAR(64),
	out outUID bigint,
	out outShardID int,
	out result int
)
BEGIN
	DECLARE shardID INT;
	declare continue handler for sqlstate '23000'
	begin 
		set result = -1;
	end;
	-- check fbname first which is not check by insert into
	if exists ( select 1 from tblUser where FBUserID = inFBUserID limit 1 )
	then begin
		set result = -2;
	end;
	else
	-- elseif
	begin
		declare exit handler for sqlstate '23000'
		begin 
			set result = -1;
		end;
		-- Update table
		set result = 0;
		
		CALL spAssignCharacterShard(shardID);
		
		INSERT INTO tblUser 
			(`FBUserID`, `EMail`, `CellPhone`, `ShardID`)
			VALUES
			(
			inFBUserID, inEMail, inCellPhone, shardID
			);
		select UserUID, ShardID into outUID, outShardID from tblUser where FBUserID = inFBUserID;
		
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFacebookLogin` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFacebookLogin` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFacebookLogin`(
	in inFBUserID bigint,
	out outUID bigint,
	out outEMail VARCHAR(128),
	OUT outCellPhone VARCHAR(20),
	out outGCMKeys varchar(512),
	out outShardID int,
	out result int
)
BEGIN
	select UserUID , GCMKeys, EMail, CellPhone, shardID into outUID, outGCMKeys, outEMail, outCellPhone, outShardID from tblUser where FBUserID = inFBUserID;
	if outUID is null or outUID = 0 THEN
		-- User not found
		set result = -1;
	else begin
		update tblUser set LatestLoggedIn = NOW() where UserUID = outUID;
		set result = 0;
	end;
	end if;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFindPlayerByEMail` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFindPlayerByEMail` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFindPlayerByEMail`(
	in inEMail varchar(128),
	out outUID bigint,
	out outShardID int,
	out outFBUserID bigint,
	out outResult int
)
BEGIN
	select UserUID , ShardID, FBUserID into outUID, outShardID, outFBUserID from tblUser where EMail = inEMail limit 1;
	if outUID is null or outUID = 0 THEN
		-- User not found
		set outResult = -1;
	else begin
		set outResult = 0;
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFindPlayerByPlayerID` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFindPlayerByPlayerID` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFindPlayerByPlayerID`(
	in inUID BIGINT,
	OUT outShardID INT,
	OUT outFBUserID BIGINT,
	OUT outResult INT
)
BEGIN
	SELECT ShardID, FBUserID INTO outShardID, outFBUserID FROM tblUser WHERE UserUID = inUID LIMIT 1;
	IF outShardID IS NULL THEN
		-- User not found
		SET outResult = -1;
	ELSE BEGIN
		SET outResult = 0;
	END;
	END IF;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetPlayerShardID` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetPlayerShardID` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetPlayerShardID`(
	in inPlayerID BIGINT,
	out outShardID INT,
	out outResult int
    )
BEGIN
	set outShardID = null;
	select ShardID into outShardID from tblUser where UserUID = inPlayerID;
	if outShardID = null THEN
		-- User not found
		SET outResult = -1;
	ELSE BEGIN
		SET outResult = 0;
	END;
	END IF;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetShardList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetShardList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetShardList`(
	in shardID int
    )
BEGIN
    
	select * from tblShardInfo;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spLogin` */

/*!50003 DROP PROCEDURE IF EXISTS  `spLogin` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spLogin`(
	in inUserName varchar(128),
	in inPassword char(128),
	out outUID bigint,
	out outFacebookUID bigint,
	OUT outShardID INT,
	out result int
)
BEGIN
	declare pwd char(128);
	select UserUID, FBUserID, UserPassword, shardID into outUID, outFacebookUID, pwd, outShardID from tblUser where UserName = inUserName;
	-- select UserUID , Password, GameNick from tblUser where UserName = inUserName;
	-- select outUID = UserUID , pwd = Password, outGameNick = GameNick from tblUser where UserName = inUserName;
	if pwd is null THEN
		-- User not found
		set result = -1;
	elseif pwd != inPassword then
		-- Invalid password
		set result = -2;
	else begin
		update tblUser set LatestLoggedIn = NOW() where UserUID = outUID;
		set result = 0;
	end;
	end if;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spSampleInitializeDB` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSampleInitializeDB` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSampleInitializeDB`()
BEGIN
	DELETE FROM dbaccount.`tblUser`;
	DELETE FROM dbconspiracy00.`tblfriend`;
	DELETE FROM dbconspiracy00.`tblnotification`;
	DELETE FROM dbconspiracy00.`tblplayer`;
	DELETE FROM dbconspiracy01.`tblfriend`;
	DELETE FROM dbconspiracy01.`tblnotification`;
	DELETE FROM dbconspiracy01.`tblplayer`;
	-- bot uses player id 1~8
	CALL spCreateSystemPlayer('Bot', 0, 10, 2);
	
	CALL spCreateSystemPlayer('Tester', 0, 40, 2);
	
	-- shard initialize
	DELETE FROM  dbaccount.`tblCharacterShardGroup`;
	delete from  dbsession00.`tblShardInfo`;
	DELETE FROM  dbconspiracy00.`tblShardInfo`;
	DELETE FROM  gametransaction00.`tblShardInfo`;
	
	INSERT INTO dbsession00.`tblCharacterShardGroup` (`ShardID`, `WorkLoad`,`DBName`, `TableName`) VALUES (0 , 0,  'dbconspiracy00',  'tblPlayer' );
	INSERT INTO dbsession00.`tblCharacterShardGroup` (`ShardID`, `WorkLoad`,`DBName`, `TableName`) VALUES (1 , 0,  'dbconspiracy01',  'tblPlayer' );
	INSERT INTO dbsession00.`tblShardInfo` (`ShardID`, `ConnectionString`,`DBName`, `UserID`,`Password`) VALUES (1 , '127.0.0.1,11000',  'dbsession01',  'braves',  'dydwk12#' );
	INSERT INTO dbconspiracy00.`tblShardInfo` (`ShardID`, `ConnectionString`,`DBName`, `UserID`,`Password`) VALUES (1 , '127.0.0.1,11000',  'dbsession01',  'braves',  'dydwk12#' );
	INSERT INTO gametransaction00.`tblShardInfo` (`ShardID`, `ConnectionString`,`DBName`, `UserID`,`Password`) VALUES (1 , '127.0.0.1,11000',  'dbsession01',  'braves',  'dydwk12#' );
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateCharacterShardLoad` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateCharacterShardLoad` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateCharacterShardLoad`()
BEGIN
	DECLARE currentShard INT;
	DECLARE currentLoad INT;
	DECLARE currentDBName varchar(64);
	DECLARE currentTableName VARCHAR(64);
	-- It's going to be a remote query
	DECLARE numShard INT;
	DECLARE iShard INT;
	
	SELECT COUNT(*)INTO numShard FROM tblCharacterShardGroup;
	
	SET iShard = 0;
	WHILE iShard < numShard DO 
	START TRANSACTION ;
	begin
	
		select ShardID, DBName, TableName into  currentShard, currentDBName, currentTableName from tblCharacterShardGroup where shardID = iShard;
		-- select currentShard, currentDBName, currentTableName;
		SELECT TABLE_ROWS into currentLoad FROM INFORMATION_SCHEMA.TABLES WHERE STRCMP(TABLE_SCHEMA, LOWER(currentDBName)) = 0 AND STRCMP(TABLE_NAME, LOWER(currentTableName)) = 0;
		-- SELECT TABLE_ROWS FROM INFORMATION_SCHEMA.TABLES WHERE STRCMP(TABLE_SCHEMA, currentDBName) = 0 AND STRCMP(TABLE_NAME, currentTableName) = 0;;
		-- select iShard, currentShard, currentDBName, currentTableName, currentLoad;
		UPDATE tblCharacterShardGroup SET workload = currentLoad WHERE ShardID = currentShard;
		
		set iShard = iShard+1;
	COMMIT ;
	end;
	END WHILE;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateGCMKeys` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateGCMKeys` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateGCMKeys`(
	in inUID bigint,
	in inGCMKeys varchar(512),
	out result int
)
BEGIN
	-- not found handler
	declare exit handler for sqlstate 'HY000'
	begin 
		set result = -1;
	end;
	set result = 0;
	update tblUser set GCMKeys = inGCMKeys where UserUID = inUID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateUserContactInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateUserContactInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateUserContactInfo`(
    IN inUID BIGINT,
    IN inEMail VARCHAR(128),
    IN inCellPhone VARCHAR(20)
    )
BEGIN
    
	UPDATE tblUser SET EMail = inEMail, CellPhone = inCellPhone WHERE UserUID = inUID;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateUserEmail` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateUserEmail` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateUserEmail`(
    IN inUID BIGINT,
    IN inEMail VARCHAR(128)
    )
BEGIN
    
	UPDATE tblUser SET EMail = inEMail WHERE UserUID = inUID;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spUserList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUserList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUserList`(out result int)
BEGIN
	select UserUID, UserName, FBUserID from tblUser;
	set result = ROW_COUNT();
END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;


CALL spCreateSystemPlayer('Bot', 0, 10, 4);

