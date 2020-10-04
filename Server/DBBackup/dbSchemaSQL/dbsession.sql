/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbSession00
*********************************************************************
*/

/*!40101 SET NAMES utf8mb4 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbSession00` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE `dbSession00`;

/*Table structure for table `tblSession` */

DROP TABLE IF EXISTS `tblSession`;

CREATE TABLE `tblSession` (
  `AuthTicket` bigint(20) unsigned NOT NULL,
  `LoginEntityUID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `GameEntityUID` bigint(20) unsigned NOT NULL DEFAULT '0',
  `Heartbit` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `PlayerID` bigint(20) NOT NULL,
  PRIMARY KEY (`PlayerID`),
  KEY `PlayerIDIndex` (`AuthTicket`)
) ENGINE=MEMORY;

/*Table structure for table `tblShardinfo` */

DROP TABLE IF EXISTS `tblShardinfo`;

CREATE TABLE `tblShardinfo` (
  `ShardID` int NOT NULL,
  `ConnectionString` varchar(512) NOT NULL,
  `DBName` varchar(128) NOT NULL,
  PRIMARY KEY (`ShardID`)
) ENGINE=InnoDB ;

/* Procedure structure for procedure `spConnectedToGameServer` */

/*!50003 DROP PROCEDURE IF EXISTS  `spConnectedToGameServer` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spConnectedToGameServer`(
	in inPlayerID bigint,
	in inAuthTicket bigint,
	in inLoginEntityUID bigint,
	in inGameEntityUID bigint,
	out outResult int
)
BEGIN
	set outResult = 0;
	Update tblSession 
		SET
			`LoginEntityUID` = 0,
			`GameEntityUID` = inGameEntityUID ,
			`Heartbit` = CURRENT_TIMESTAMP()
		where inPlayerID = `PlayerID` and inAuthTicket = `AuthTicket` and inLoginEntityUID = `LoginEntityUID`;
	IF ROW_COUNT()  = 0 THEN
	begin
		set outResult = -1;
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spDeleteLoginSession` */

/*!50003 DROP PROCEDURE IF EXISTS  `spDeleteLoginSession` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spDeleteLoginSession`(
	in inPlayerID bigint,
	in inAuthTicket bigint,
	out outResult int
)
BEGIN
	set outResult = 0;
	-- delete from tblSession where inPlayerID = `PlayerID` and inAuthTicket = `AuthTicket`;
	UPDATE tblSession
		SET
		`AuthTicket` = 0,
		`LoginEntityUID` = 0,
		`Heartbit` = NOW()
		WHERE inPlayerID = `PlayerID` and inAuthTicket = `AuthTicket`;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFindPlayer` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFindPlayer` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFindPlayer`(
	in inPlayerID bigint,
	out inLoginEntityUID bigint,
	out inGameEntityUID bigint,
	out outResult int
)
BEGIN
	select `LoginEntityUID`, `GameEntityUID` into inLoginEntityUID, inGameEntityUID from tblSession where `PlayerID` = inPlayerID;
	if row_count() != 0 then 
		set outResult = 0;
	else 
		set outResult = -1;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGameServerHeartBit` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGameServerHeartBit` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGameServerHeartBit`(
	in inPlayerID bigint,
	in inAuthTicket bigint,
	in inGameEntityUID bigint,
	out outResult int
)
BEGIN
	set outResult = 0;
	Update tblSession 
		SET
			`Heartbit` = CURRENT_TIMESTAMP()
		where inPlayerID = `PlayerID` and inAuthTicket = `AuthTicket` and inGameEntityUID = `GameEntityUID`;
	IF ROW_COUNT()  = 0 THEN
	begin
		set outResult = -1;
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetShardList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetShardList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetShardList`(
	in shardID int
    )
BEGIN
    
	select * from tblShardinfo;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spRegisterAuthTicket` */

/*!50003 DROP PROCEDURE IF EXISTS  `spRegisterAuthTicket` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spRegisterAuthTicket`(
	in inPlayerID bigint,
	in inAuthTicket bigint,
	in inLoginEntityUID bigint,
	-- OUT outAuthTicket BIGINT,
	-- OUT outLoginEntityUID BIGINT,
	out outGameEntityUID bigint,
	-- OUT outHeartbit BIGINT,
	out outResult int
)
BEGIN
	-- duplicated handler
	declare exit handler for sqlstate '23000'
	begin 
		-- set outResult = -1;
		
--		select `AuthTicket`, `LoginEntityUID`, `GameEntityUID`, `Heartbit` into outAuthTicket, outLoginEntityUID, outGameEntityUID, outHeartbit 
--			from tblSession 
--			where `PlayerID` = inPlayerID;
		UPDATE tblSession
			SET
			`AuthTicket` = inAuthTicket,
			-- `GameEntityUID` = 0,
			`LoginEntityUID` = inLoginEntityUID,
			`Heartbit` = NOW()
			WHERE inPlayerID = `PlayerID`; -- and `GameEntityUID` = 0;
		
		IF ROW_COUNT() = 0 THEN 
			SET outResult = -1;
		ELSE
			select `GameEntityUID` into outGameEntityUID
				from tblSession 
				where `PlayerID` = inPlayerID;
		END IF;
	
	end;
	SET outGameEntityUID = 0;
	set outResult = 0;
	insert into tblSession
			(`PlayerID`, `AuthTicket`, `LoginEntityUID`)
		values 
			(inPlayerID, inAuthTicket, inLoginEntityUID);
END */$$
DELIMITER ;

/* Procedure structure for procedure `spReplaceLoginSession` */

/*!50003 DROP PROCEDURE IF EXISTS  `spReplaceLoginSession` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spReplaceLoginSession`(
	IN inPlayerID BIGINT,
	IN inOldAuthTicket BIGINT,
	IN inAuthTicket BIGINT,
	IN inLoginEntityUID BIGINT,
	OUT outResult INT)
BEGIN
	SET outResult = 0;
	UPDATE tblSession
		SET
		`AuthTicket` = inAuthTicket,
		-- `GameEntityUID` = 0,
		`LoginEntityUID` = inLoginEntityUID,
		`Heartbit` = Now()
		WHERE inPlayerID = `PlayerID` AND inOldAuthTicket = `AuthTicket`;
		
	if ROW_COUNT() = 0 then 
		set outResult = -1;
	END IF;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spValidateGameServerSession` */

/*!50003 DROP PROCEDURE IF EXISTS  `spValidateGameServerSession` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spValidateGameServerSession`(
    IN inPlayerID BIGINT,
	IN inAuthTicket BIGINT,
	IN inGameEntityUID BIGINT,
	OUT outResult INT
	)
BEGIN
    
	SET outResult = -1;
	if exists( select 1 from tblSession WHERE inPlayerID = `PlayerID` AND inAuthTicket = `AuthTicket` and inGameEntityUID = `GameEntityUID`) then
		SET outResult = 0;
	end if;
	
--	SET outResult = 0;
--	UPDATE tblSession
--		SET
--		`LoginEntityUID` = 0,
--		`Heartbit` = NOW()
--		WHERE inPlayerID = `PlayerID` AND inAuthTicket = `AuthTicket` and inGameEntityUID = `GameEntityUID`;
		
--	IF ROW_COUNT() = 0 THEN 
--		SET outResult = -1;
--	END IF;
		
    END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
