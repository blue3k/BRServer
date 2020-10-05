/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbconspiracy00
*********************************************************************
*/

/*!40101 SET NAMES utf8mb4 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbConspiracy00` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE `dbConspiracy00`;

/*Table structure for table `tblFriend` */

DROP TABLE IF EXISTS `tblFriend`;

CREATE TABLE `tblFriend` (
  `UserUID` BIGINT NOT NULL,
  `FriendUID` BIGINT NOT NULL,
  `FriendFacebookUID` BIGINT NOT NULL DEFAULT '0',
  `FriendStaminaTime` BIGINT NOT NULL DEFAULT '0',
  `FriendShardID` INT NOT NULL,
  PRIMARY KEY (`UserUID`,`FriendUID`),
  UNIQUE KEY `PK_Unique` (`UserUID`,`FriendUID`),
  KEY `PrimaryIndex` (`UserUID`,`FriendUID`),
  KEY `FriendID_Index` (`FriendUID`)
) ENGINE=INNODB;

/*Table structure for table `tblNotification` */

DROP TABLE IF EXISTS `tblNotification`;

CREATE TABLE `tblNotification` (
  `NotificationID` INT NOT NULL AUTO_INCREMENT,
  `UserUID` BIGINT UNSIGNED NOT NULL,
  `MessageID` SMALLINT UNSIGNED NOT NULL,
  `MessageParam0` BIGINT DEFAULT NULL,
  `MessageParam1` BIGINT DEFAULT NULL,
  `MessageText` VARCHAR(512) DEFAULT NULL,
  `IssuedTime` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Read` TINYINT NOT NULL DEFAULT '0',
  `TimeStamp` BIGINT UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY (`NotificationID`),
  UNIQUE KEY `NotificationID_UNIQUE` (`NotificationID`),
  KEY `UserID_KEY` (`UserUID`)
) ENGINE=MYISAM AUTO_INCREMENT=7;

/*Table structure for table `tblPlayer` */

DROP TABLE IF EXISTS `tblPlayer`;

CREATE TABLE `tblPlayer` (
  `PlayerID` BIGINT NOT NULL,
  `Grade` TINYINT NOT NULL DEFAULT '0',
  `Level` SMALLINT NOT NULL DEFAULT '1',
  `Exp` BIGINT NOT NULL DEFAULT '0',
  `Money` BIGINT NOT NULL DEFAULT '0',
  `gem` BIGINT NOT NULL DEFAULT '0',
  `Stamina` SMALLINT NOT NULL DEFAULT '3',
  `TotalPlayed` INT NOT NULL DEFAULT '0',
  `WinPlaySC` INT NOT NULL DEFAULT '0',
  `WinPlaySM` INT NOT NULL DEFAULT '0',
  `WinPlaySS` INT NOT NULL DEFAULT '0',
  `LosePlaySC` INT NOT NULL DEFAULT '0',
  `LosePlaySM` INT NOT NULL DEFAULT '0',
  `LosePlaySS` INT NOT NULL DEFAULT '0',
  `WinPlayNC` INT NOT NULL DEFAULT '0',
  `WinPlayNM` INT NOT NULL DEFAULT '0',
  `WInPlayNS` INT NOT NULL DEFAULT '0',
  `LosePlayNC` INT NOT NULL DEFAULT '0',
  `LosePlayNM` INT NOT NULL DEFAULT '0',
  `LosePlayNS` INT NOT NULL DEFAULT '0',
  `Updated` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `AddedFriendSlot` SMALLINT NOT NULL DEFAULT '0' COMMENT 'maximum friend slot',
  `LatestTickTime` BIGINT UNSIGNED NOT NULL DEFAULT '0' COMMENT 'In game tick time(UTC sec)',
  `LatestActiveTime` INT UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Lastest active time(UTCsec)',
  `WeeklyWin` INT UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Weekly win count',
  `WeeklyLose` INT UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Weekly lose count',
  `PlayerState` SMALLINT UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Is in game?',
  `GameNick` VARCHAR(64) NOT NULL DEFAULT '' COMMENT 'Game nick name',
  `ComplitionState` VARCHAR(16) NOT NULL DEFAULT '' COMMENT 'ComplitionState',
  PRIMARY KEY (`PlayerID`),
  UNIQUE KEY `PlayerID_UNIQUE` (`PlayerID`)
) ENGINE=INNODB;

/*Table structure for table `tblShardInfo` */

DROP TABLE IF EXISTS `tblShardInfo`;

CREATE TABLE `tblShardInfo` (
  `ShardID` INT NOT NULL,
  `ConnectionString` VARCHAR(512) NOT NULL,
  `DBName` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`ShardID`)
) ENGINE=INNODB;

/*Table structure for table `tblUsedPurchaseTokens` */

DROP TABLE IF EXISTS `tblUsedPurchaseTokens`;

CREATE TABLE `tblUsedPurchaseTokens` (
  `PurchaseTime` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `PurchaseID` VARBINARY(64) NOT NULL,
  `PurchaseToken` VARCHAR(512) NOT NULL,
  `PlayerID` BIGINT NOT NULL,
  PRIMARY KEY (`PurchaseID`),
  UNIQUE KEY `PurchaseIDIndex` (`PurchaseID`),
  KEY `PlayerIndex` (`PlayerID`)
) ENGINE=INNODB;

/* Procedure structure for procedure `spCheckPurchaseID` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCheckPurchaseID` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCheckPurchaseID`(
	IN inPurchaseID varbinary(64),
	out outResult int
    )
BEGIN
    
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ;
	IF EXISTS (SELECT 1 FROM tblUsedPurchaseTokens WHERE `PurchaseID` = inPurchaseID)
	THEN BEGIN
	 	SET outResult = -1;
	END;
	else begin
		SET outResult = 0;
	end;
	end if;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreatePlayerInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreatePlayerInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreatePlayerInfo`(
	IN inPlayerID BIGINT,
	in inInitialStamina int,
	OUT outResult INT
)
BEGIN
	SET outResult = 0;
	
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED ;
	
	if exists (select 1 from tblPlayer where PlayerID = inPlayerID)
	then begin
	 	set outResult = 1;
	end;
	else begin
		INSERT IGNORE INTO tblPlayer (PlayerID, Stamina) VALUES ( inPlayerID, inInitialStamina );
	end;
	END IF;
	SELECT `GameNick`, `Grade`, `Level`, `Exp`, `Money`, `Gem`, `Stamina`, `AddedFriendSlot`, 
		TotalPlayed, 
		WinPlaySC, WinPlaySM, WinPlaySS, LosePlaySC, LosePlaySM, LosePlaySS, 
		WinPlayNC, WinPlayNM, WinPlayNS, LosePlayNC, LosePlayNM, LosePlayNS, 
		WeeklyWin, WeeklyLose, LatestTickTime
		FROM tblPlayer WHERE PlayerID = inPlayerID;
	IF (SELECT COUNT(*) FROM tblPlayer WHERE PlayerID = inPlayerID) = 0 THEN
	BEGIN
		SET outResult = -1;
	END;
	END IF;
	commit;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFriend_Add` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFriend_Add` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFriend_Add`(
	in inUserID bigint,
	IN inFriendID bigint,
	IN inFriendShardID INT,
	IN inFacebookUID bigint,
	out result int
)
BEGIN
	declare continue handler for sqlstate '23000'
	begin 
		-- Already in the friend list
		set result = -1;
	end;
	set result = 0;
	insert into tblFriend 
		( UserUID, FriendUID, FriendFacebookUID, FriendShardID )
		Values
		( inUserID, inFriendID, inFacebookUID, inFriendShardID );
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFriend_List` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFriend_List` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFriend_List`(
	inUserUID BIGINT
)
BEGIN
	SELECT FriendUID, FriendShardID, FriendFacebookUID, FriendStaminaTime
				FROM tblFriend
				WHERE UserUID = inUserUID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFriend_Remove` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFriend_Remove` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFriend_Remove`(
	inUserID bigint,
	inFriendID bigint,
	out result int
)
BEGIN
	-- not found handler
	declare continue handler for sqlstate 'HY000'
	begin 
		set result = -1;
	end;
	set result = 0;
	delete from tblFriend where UserUID = inUserID and FriendUID = inFriendID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spFriend_UpdateStaminaTime` */

/*!50003 DROP PROCEDURE IF EXISTS  `spFriend_UpdateStaminaTime` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spFriend_UpdateStaminaTime`(
	inUserUID bigint,
	inFriendUID BIGINT,
	inTImeStamp BIGINT,
	OUT outResult int
)
BEGIN
	Update tblFriend set FriendStaminaTime = inTImeStamp where UserUID = inUserUID and FriendUID = inFriendUID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetComplitionState` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetComplitionState` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetComplitionState`(
	IN inPlayerID BIGINT,
	OUT outComplitionState varchar(16)
    )
BEGIN
    
	select `ComplitionState` into outComplitionState from tblPlayer 
		WHERE `PlayerID` = inPlayerID ;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetFriendQuickInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetFriendQuickInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetFriendQuickInfo`(
	IN inPlayerID BIGINT,
	OUT outLevel INT,
	OUT outWeeklyWin INT,
	OUT outWeeklyLose INT,
	OUT outPlayerState smallint,
	OUT outLatestActiveTime INT,
	OUT outResult INT
)
BEGIN
	SET outResult = 0;
	SELECT `Level`, `WeeklyWin`, `WeeklyLose`, `PlayerState`, `LatestActiveTime` INTO outLevel, outWeeklyWin, outWeeklyLose, outPlayerState, outLatestActiveTime
		FROM tblPlayer 
		WHERE PlayerID = inPlayerID;
		
	IF (SELECT count(*) FROM tblPlayer WHERE PlayerID = inPlayerID) = 0 THEN
	BEGIN
		SET outResult = -1;
	END;
	END IF;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetFriendQuickInfoWithNick` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetFriendQuickInfoWithNick` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetFriendQuickInfoWithNick`(
	IN inPlayerID BIGINT,
	OUT outGameNick varchar(60),
	OUT outLevel INT,
	OUT outWeeklyWin INT,
	OUT outWeeklyLose INT,
	OUT outPlayerState SMALLINT,
	OUT outLatestActiveTime INT,
	OUT outResult INT
)
BEGIN
	-- not found handler
	DECLARE CONTINUE HANDLER FOR SQLSTATE 'HY000'
	BEGIN
		SET outResult = -1;
	END;
	SET outResult = 0;
	SELECT `GameNick`, `Level`, `WeeklyWin`, `WeeklyLose`, `PlayerState`, `LatestActiveTime` INTO outGameNick, outLevel, outWeeklyWin, outWeeklyLose, outPlayerState, outLatestActiveTime
		FROM tblPlayer 
		WHERE PlayerID = inPlayerID;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetFriendSlotStatus` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetFriendSlotStatus` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetFriendSlotStatus`(
	in inPlayerID bigint,
	OUT outLevel INT,
	out outAddedFriendSlot int,
	OUT outNumFriends INT,
	OUT outResult INT
    )
BEGIN
	select count(*) into outNumFriends from tblFriend where UserUID = inPlayerID;
	set outResult = 0;
	
	select `Level`, `AddedFriendSlot` into outLevel, outAddedFriendSlot from tblPlayer where PlayerID = inPlayerID;
	IF (select count(*) from tblPlayer where PlayerID = inPlayerID) = 0 THEN
	BEGIN
		SET outResult = -1;
	END;
	END IF;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetNickName` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetNickName` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetNickName`(
	in inPlayerID bigint,
	out outGameNick varchar(60),
	out outResult int
)
BEGIN
	set outResult = 0;
	select GameNick into outGameNick from tblPlayer where PlayerID = inPlayerID;
	IF (select count(*) where PlayerID = inPlayerID) = 0 THEN
	BEGIN
		SET outResult = -1;
	END;
	END IF;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetPlayerInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetPlayerInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetPlayerInfo`(
	in inPlayerID bigint,
	out outResult int
)
BEGIN
	set outResult = 0;
	select `GameNick`, `Grade`, `Level`, `Exp`, `Money`, `Gem`, `Stamina`, `AddedFriendSlot`, 
		TotalPlayed, 
		WinPlaySC, WinPlaySM, WinPlaySS, LosePlaySC, LosePlaySM, LosePlaySS, 
		WinPlayNC, WinPlayNM, WinPlayNS, LosePlayNC, LosePlayNM, LosePlayNS, 
		WeeklyWin, WeeklyLose, LatestTickTime
		from tblPlayer where PlayerID = inPlayerID;
	if (select count(*) from tblPlayer where PlayerID = inPlayerID) = 0 then
	begin
		set outResult = -1;
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetPlayerQuickInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetPlayerQuickInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetPlayerQuickInfo`(
	IN inPlayerID BIGINT,
	OUT outLevel INT,
	OUT outWeeklyWin INT,
	OUT outWeeklyLose INT,
	OUT outResult INT
)
BEGIN
	-- not found handler
	DECLARE CONTINUE HANDLER FOR SQLSTATE 'HY000'
	BEGIN
		SET outResult = -1;
	END;
	SET outResult = 0;
	SELECT `Level`, `WeeklyWin`, `WeeklyLose` INTO outLevel, outWeeklyWin, outWeeklyLose FROM tblPlayer WHERE PlayerID = inPlayerID;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetPlayerStatus` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetPlayerStatus` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetPlayerStatus`(
	IN inPlayerID BIGINT,
	out outPlayerState smallint,
	OUT outLatestActiveTime INT,
	OUT outResult INT
    )
BEGIN
	SET outResult = 0;
	SELECT LatestActiveTime, PlayerState into outLatestActiveTime, outPlayerState
		FROM tblPlayer WHERE PlayerID = inPlayerID;
	IF (select count(*) from tblPlayer where PlayerID = inPlayerID) = 0 THEN
	BEGIN
		SET outResult = -1;
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

/* Procedure structure for procedure `spNotification_Add` */

/*!50003 DROP PROCEDURE IF EXISTS  `spNotification_Add` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spNotification_Add`(
	inUserID BIGINT,
	inIsCollapsable TINYINT,
	inMessageID SMALLINT,
	inMessageParam0 BIGINT,
	inMessageParam1 BIGINT,
	inMessageText VARCHAR(512),
	inTimeStamp BIGINT,
	OUT outNotificationID INT,
	OUT result INT
)
BEGIN
	IF inIsCollapsable = 1 AND EXISTS ( SELECT 1 FROM tblNotification WHERE UserUID = inUserID AND MessageID = inMessageID AND MessageParam0 = inMessageParam0 AND MessageParam1 = inMessageParam1 LIMIT 1 )
	THEN BEGIN
		-- Duplicated message
		SET result = -1;
		SELECT NotificationID INTO outNotificationID FROM tblNotification WHERE UserUID = inUserID AND MessageID = inMessageID AND MessageParam0 = inMessageParam0 AND MessageParam1 = inMessageParam1 LIMIT 1;
		-- refresh Issued Time
		UPDATE tblNotification SET IssuedTime = NOW(), `TimeStamp` = inTimeStamp WHERE NotificationID = outNotificationID;
	END;
	ELSE
	-- elseif
	BEGIN
		SET result = 0;
		INSERT INTO tblNotification
			( UserUID, MessageID, MessageParam0, MessageParam1, MessageText, `TimeStamp` )
			VALUES
			( inUserID, inMessageID, inMessageParam0, inMessageParam1, inMessageText, inTimeStamp );
		SET outNotificationID = LAST_INSERT_ID();
	END;
	END IF;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spNotification_GetList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spNotification_GetList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spNotification_GetList`(
	inUserID bigint,
	out result int
)
BEGIN
	set result = 0;
	select NotificationID, MessageID, MessageParam0, MessageParam1, MessageText, `Read`, `TimeStamp` from tblNotification where UserUID = inUserID order by NotificationID DESC limit 20;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spNotification_Remove` */

/*!50003 DROP PROCEDURE IF EXISTS  `spNotification_Remove` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spNotification_Remove`(
	inUserID bigint,
	inNotificationID int,
	out outResult int
)
BEGIN
	-- Not found handler
	declare continue handler for sqlstate 'HY000'
	begin
		set outResult = -1;
	end;
	set outResult = 0;
	delete from tblNotification where NotificationID = inNotificationID and UserUID = inUserID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spNotification_RemoveByMessageID` */

/*!50003 DROP PROCEDURE IF EXISTS  `spNotification_RemoveByMessageID` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spNotification_RemoveByMessageID`(
	inUserID bigint,
	inMessageID smallint,
	out outResult int
)
BEGIN
	-- Not found handler
	declare continue handler for sqlstate 'HY000'
	begin
		set outResult = -1;
	end;
	set outResult = 0;
	delete from tblNotification where UserUID = inUserID and MessageID = inMessageID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spNotification_SetRead` */

/*!50003 DROP PROCEDURE IF EXISTS  `spNotification_SetRead` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spNotification_SetRead`(
	inUserID BIGINT,
	inNotificationID INT,
	OUT outResult INT
)
BEGIN
	-- Not found handler
	DECLARE CONTINUE HANDLER FOR SQLSTATE 'HY000'
	BEGIN
		SET outResult = -1;
	END;
	SET outResult = 0;
	UPDATE tblNotification SET `Read` = 1 WHERE NotificationID = inNotificationID AND UserUID = inUserID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spSavePurchaseInfoToDB` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSavePurchaseInfoToDB` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSavePurchaseInfoToDB`(
	IN inPlayerID BIGINT,
	IN inLevel SMALLINT,
	IN inExp BIGINT,
	IN inMoney BIGINT,
	IN inGem BIGINT,
	IN inStamina SMALLINT,
	IN inAddedFriendSlot SMALLINT,
	in inPurchaseID varbinary(64),
	IN inPurchasePlatform VARCHAR(64),
	IN inPurchaseToken VARCHAR(512),
	IN inLatestActiveTime INT,
	IN inLatestTickTime BIGINT,
	OUT outResult INT
)
BEGIN
	DECLARE EXIT HANDLER FOR 1062
	BEGIN 
		-- Already in the list
		SET outResult = -2;
	END;
	
	DECLARE EXIT HANDLER FOR SQLSTATE '23000'
	begin
		SET outResult = -3;
	end;
	
	SET outResult = 0;
	
	INSERT INTO tblUsedPurchaseTokens (
			`PurchaseID`,
			`PurchaseToken`,
			`PlayerID`
			) VALUES (
			inPurchaseID,
			inPurchaseToken,
			inPlayerID
			);	
	
	UPDATE tblPlayer
		SET
		`Level` = inLevel,
		`Exp` = inExp,
		`Money` = inMoney,
		`gem` = inGem,
		`Stamina` = inStamina,
		`AddedFriendSlot` = inAddedFriendSlot, 
		`LatestTickTime` = inLatestTickTime,
		`LatestActiveTime` = inLatestActiveTime,
		`Updated` = CURRENT_TIMESTAMP()
		WHERE PlayerID = inPlayerID;
	
	IF ROW_COUNT()  = 0 THEN
	BEGIN
		
		SET outResult = -1;
	END;
	END IF;
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spSavePurchaseInfoToDBTest` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSavePurchaseInfoToDBTest` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSavePurchaseInfoToDBTest`(
	IN inPlayerID BIGINT,
	IN inPurchaseID VARBINARY(64),
	IN inPurchaseToken VARCHAR(512),
	OUT outResult INT
)
BEGIN
	DECLARE EXIT HANDLER FOR 1062
	BEGIN 
		-- Already in the list
		SET outResult = -2;
	END;
	
	DECLARE EXIT HANDLER FOR SQLSTATE '23000'
	BEGIN
		SET outResult = -3;
	END;
	
	SET outResult = 0;
	
	INSERT INTO tblUsedPurchaseTokens (
			`PurchaseID`,
			`PurchaseToken`,
			`PlayerID`
			) VALUES (
			inPurchaseID,
			inPurchaseToken,
			inPlayerID
			);	
	
	SET outResult = 1;	
	
END */$$
DELIMITER ;

/* Procedure structure for procedure `spSetComplitionState` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSetComplitionState` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSetComplitionState`(
	IN inPlayerID BIGINT,
	IN inComplitionState varchar(16)
    )
BEGIN
    
	UPDATE tblPlayer SET `ComplitionState` = inComplitionState
		WHERE `PlayerID` = inPlayerID ;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spSetNickName` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSetNickName` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSetNickName`(
	in inPlayerID bigint,
	in inNickName varchar(64),
	out outResult int
)
BEGIN
	-- Not found handler
	-- ROW_COUNT() will return affected rows than found rows, same row case will return 0
	DECLARE CONTINUE HANDLER FOR SQLSTATE 'HY000'
	BEGIN
		SET outResult = -1;
	END;
	
	SET outResult = 0;
	UPDATE tblPlayer SET GameNick = inNickName WHERE PlayerID = inPlayerID;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spSetPlayerInfo` */

/*!50003 DROP PROCEDURE IF EXISTS  `spSetPlayerInfo` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spSetPlayerInfo`(
	in inPlayerID bigint,
	in inLevel smallint,
	in inExp bigint,
	in inMoney bigint,
	in inGem bigint,
	in inStamina smallint,
	in inAddedFriendSlot smallint,
	in inTotalPlayed int,
	in inWinPlaySC int,
	in inWinPlaySM int,
	IN inWinPlaySS INT,
	in inLosePlaySC int,
	in inLosePlaySM int,
	IN inLosePlaySS INT,
	in inWinPlayNC int,
	in inWinPlayNM int,
	IN inWinPlayNS INT,
	in inLosePlayNC int,
	in inLosePlayNM int,
	IN inLosePlayNS INT,
	IN inLatestActiveTime INT,
	IN inLatestTickTime BIGINT,
	out outResult int
)
BEGIN
	set outResult = 0;
	UPDATE tblPlayer
		SET
		`Level` = inLevel,
		`Exp` = inExp,
		`Money` = inMoney,
		`gem` = inGem,
		`Stamina` = inStamina,
		`AddedFriendSlot` = inAddedFriendSlot, 
		`TotalPlayed` = inTotalPlayed,
		`WinPlaySC` = inWinPlaySC,
		`WinPlaySM` = inWinPlaySM,
		`WinPlaySS` = inWinPlaySS,
		`LosePlaySC` = inLosePlaySC,
		`LosePlaySM` = inLosePlaySM,
		`LosePlaySS` = inLosePlaySS,
		`WinPlayNC` = inWinPlayNC,
		`WinPlayNM` = inWinPlayNM,
		`WinPlayNS` = inWinPlayNS,
		`LosePlayNC` = inLosePlayNC,
		`LosePlayNM` = inLosePlayNM,
		`LosePlayNS` = inLosePlayNS,
		`LatestTickTime` = inLatestTickTime,
		`LatestActiveTime` = inLatestActiveTime,
		`Updated` = CURRENT_TIMESTAMP()
		WHERE PlayerID = inPlayerID;
	-- SELECT ROW_COUNT() into count ;
	IF ROW_COUNT()  = 0 THEN
	begin
		-- Create player information if not exist
		-- This case isn't be supposed to be happened, all the player inseartion must be happened in spGetPlayerInfo which is called on login process.
		-- If this happened you need to fix DB integraty. My guess is DB sharding is wrong.
		-- However, I'm going to save the data here so that we can restore the player data
		insert into tblPlayer (`PlayerID`, `Level`, `Exp`, `Money`, `Gem`, `Stamina`, `AddedFriendSlot`, 
					`TotalPlayed`, 
					WinPlaySC, WinPlaySM, WinPlaySS, LosePlaySC, LosePlaySM, LosePlaySS, 
					WinPlayNC, WinPlayNM, WinPlayNS, LosePlayNC, LosePlayNM, LosePlayNS, 
					`LatestActiveTime`, `LatestTickTime`)
				values ( inPlayerID, inLevel, inExp, inMoney, inGem, inStamina, inAddedFriendSlot, 
					inTotalPlayed, 
					inWinPlaySC, inWinPlaySM, inWinPlaySS, inLosePlaySC, inLosePlaySM, inLosePlaySS, 
					inWinPlayNC, inWinPlayNM, inWinPlayNS, inLosePlayNC, inLosePlayNM, inLosePlayNS, 
					inLatestActiveTime, inLatestTickTime );
		set outResult = 1;
	end;
	end if;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateGameEnd` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateGameEnd` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateGameEnd`(
	IN inPlayerID BIGINT,
	IN inLevel SMALLINT,
	IN inExp BIGINT,
	IN inMoney BIGINT,
	IN inTotalPlayed INT,
	IN inWinPlaySC INT,
	IN inWinPlaySM INT,
	IN inWinPlaySS INT,
	IN inLosePlaySC INT,
	IN inLosePlaySM INT,
	IN inLosePlaySS INT,
	IN inWinPlayNC INT,
	IN inWinPlayNM INT,
	IN inWinPlayNS INT,
	IN inLosePlayNC INT,
	IN inLosePlayNM INT,
	IN inLosePlayNS INT,
	IN inLatestActiveTime INT,
	OUT outResult INT
    )
BEGIN
    
	SET outResult = 0;
	UPDATE tblPlayer
		SET
		`Level` = inLevel,
		`Exp` = inExp,
		`Money` = inMoney,
		`TotalPlayed` = inTotalPlayed,
		`WinPlaySC` = inWinPlaySC,
		`WinPlaySM` = inWinPlaySM,
		`WinPlaySS` = inWinPlaySS,
		`LosePlaySC` = inLosePlaySC,
		`LosePlaySM` = inLosePlaySM,
		`LosePlaySS` = inLosePlaySS,
		`WinPlayNC` = inWinPlayNC,
		`WinPlayNM` = inWinPlayNM,
		`WinPlayNS` = inWinPlayNS,
		`LosePlayNC` = inLosePlayNC,
		`LosePlayNM` = inLosePlayNM,
		`LosePlayNS` = inLosePlayNS,
		`PlayerState` = 0,
		`LatestActiveTime` = inLatestActiveTime,
		`Updated` = CURRENT_TIMESTAMP()
		WHERE PlayerID = inPlayerID;
	-- SELECT ROW_COUNT() into count ;
	IF ROW_COUNT()  = 0 THEN
	BEGIN
		-- Create player information if not exist
		-- This case isn't be supposed to be happened, all the player inseartion must be happened in spGetPlayerInfo which is called on login process.
		SET outResult = -1;
	END;
	END IF;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateJoinGame` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateJoinGame` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateJoinGame`(
	IN inPlayerID BIGINT,
	IN inGem BIGINT,
	IN inStamina SMALLINT,
	IN inPlayerState SMALLINT,
	IN inLatestActiveTime INT,
	IN inLatestTickTime BIGINT,
	OUT outResult INT
    )
BEGIN
    
	SET outResult = 0;
	UPDATE tblPlayer
		SET
		`gem` = inGem,
		`Stamina` = inStamina,
		`PlayerState` = inPlayerState,
		`LatestTickTime` = inLatestTickTime,
		`LatestActiveTime` = inLatestActiveTime,
		`Updated` = CURRENT_TIMESTAMP()
		WHERE PlayerID = inPlayerID;
	IF ROW_COUNT()  = 0 THEN
	BEGIN
		-- Create player information if not exist
		-- This case isn't be supposed to be happened, all the player inseartion must be happened in spGetPlayerInfo which is called on login process.
		SET outResult = -1;
	END;
	END IF;
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateTickStatus` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateTickStatus` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateTickStatus`(
	IN inPlayerID BIGINT,
	IN inGem BIGINT,
	IN inStamina SMALLINT,
	IN inPlayerState BIGINT,
	IN inLatestActiveTime int,
	IN inLatestTickTime BIGINT,
	OUT outResult INT
    )
BEGIN
    
	SET outResult = 0;
	UPDATE tblPlayer
		SET
		`gem` = inGem,
		`Stamina` = inStamina,
		`PlayerState` = inPlayerState,
		`LatestTickTime` = inLatestTickTime,
		`LatestActiveTime` = inLatestActiveTime,
		`Updated` = CURRENT_TIMESTAMP()
		WHERE PlayerID = inPlayerID;
	-- SELECT ROW_COUNT() into count ;
	IF ROW_COUNT()  = 0 THEN
	BEGIN
		-- Create player information if not exist
		-- This case isn't be supposed to be happened, all the player inseartion must be happened in spGetPlayerInfo which is called on login process.
		SET outResult = -1;
	END;
	END IF;
    END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
