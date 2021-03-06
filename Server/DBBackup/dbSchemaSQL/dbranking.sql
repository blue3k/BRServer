/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbConspiracyRanking
*********************************************************************
*/

/*!40101 SET NAMES utf8mb4 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbConspiracyRanking` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE `dbConspiracyRanking`;

/*Table structure for table `tblRankers` */

DROP TABLE IF EXISTS `tblRankers`;

CREATE TABLE `tblRankers` (
  `RankingID` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Ranking table PK',
  `Score` BIGINT UNSIGNED NOT NULL,
  `PlayerID` BIGINT NOT NULL,
  `FacebookUID` BIGINT NOT NULL,
  `NickName` VARCHAR(64) NOT NULL,
  `Level` INT NOT NULL,
  `LatestUpdated` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`RankingID`),
  UNIQUE KEY `ByPlayerID` (`PlayerID`),
  KEY `ByUpdated` (`LatestUpdated`)
) ENGINE=INNODB AUTO_INCREMENT=149;

/*Table structure for table `tblShardInfo` */

DROP TABLE IF EXISTS `tblShardInfo`;

CREATE TABLE `tblShardInfo` (
  `ShardID` INT NOT NULL,
  `ConnectionString` VARCHAR(512) NOT NULL,
  `DBName` VARCHAR(128) NOT NULL,
  PRIMARY KEY (`ShardID`)
) ENGINE=INNODB;

/*Table structure for table `tblWeeklyRanking` */

DROP TABLE IF EXISTS `tblWeeklyRanking`;

CREATE TABLE `tblWeeklyRanking` (
  `RankingID` INT NOT NULL,
  `Ranking` INT NOT NULL,
  `WinRate` FLOAT NOT NULL DEFAULT '0',
  `Win` INT UNSIGNED NOT NULL DEFAULT '0',
  `Lose` INT UNSIGNED NOT NULL DEFAULT '0',
  `GameNick` VARCHAR(64) NOT NULL DEFAULT '',
  `PlayerID` BIGINT NOT NULL DEFAULT '1',
  `FacebookUID` BIGINT DEFAULT '0',
  `Level` INT NOT NULL DEFAULT '1',
  `DummyRank` INT NOT NULL DEFAULT '0',
  PRIMARY KEY (`RankingID`),
  KEY `RankingKey` (`Ranking`)
) ENGINE=MYISAM MAX_ROWS=1000;

/*!50106 set global event_scheduler = 1*/;

/* Event structure for event `evtUpdateRankingTable` */

/*!50106 DROP EVENT IF EXISTS `evtUpdateRankingTable`*/;

DELIMITER $$

/*!50106 CREATE EVENT `evtUpdateRankingTable` ON SCHEDULE EVERY 1 DAY STARTS '2014-11-29 16:25:06' ON COMPLETION NOT PRESERVE ENABLE DO BEGIN
	    call spUpdateRankingTable();
	END */$$
DELIMITER ;

/* Procedure structure for procedure `spCreateTestRankers` */

/*!50003 DROP PROCEDURE IF EXISTS  `spCreateTestRankers` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spCreateTestRankers`(
	IN inUserNamePrefix VARCHAR(60),
	IN inStartNumber INT,
	IN inNumPlayer INT,
	IN inNumCipher INT
    )
BEGIN

	DECLARE X  INT;
	declare playerID int;
	DECLARE userName  VARCHAR(255);
	DECLARE userEMail  VARCHAR(255);
	SET X = 0;
	SET playerID = inStartNumber;
	SET userName =  '';
	WHILE X  <= inNumPlayer DO
		   SET userName = CONCAT(inUserNamePrefix,LPAD(X,inNumCipher,'0'));
		   CALL spUpdateRankingScore( playerID, 0, userName, RAND() * 10, RAND() * 5000 );
		   SET  X = X + 1; 
		   set playerID = playerID + 1;
	END WHILE;

	END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetRankers` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetRankers` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetRankers`(
	in inBaseIndex int,
	in inRequestCount int
    )
BEGIN

		select PlayerID, FacebookUID, NickName, `Level`, Score from tblRankers order by LatestUpdated limit inBaseIndex, inRequestCount;


	END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetShardList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetShardList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetShardList`(
	IN inDummy INT
    )
BEGIN
    
	SELECT ShardID, ConnectionString, DBName FROM tblShardInfo;
	
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetTotalRanking` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetTotalRanking` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetTotalRanking`( 
	IN inMinRanking INT,
	IN inRankingCount INT
	 )
BEGIN
	SELECT RankingID, Ranking, WinRate, win, lose, PlayerID, FacebookUID, GameNick, `Level` FROM tblWeeklyRanking WHERE Ranking >= inMinRanking ORDER BY RankingID ASC LIMIT inRankingCount;
END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateRankingScore` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateRankingScore` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateRankingScore`(
	IN inPlayerID BIGINT,
	IN inFBUID BIGINT,
	IN inNickName varchar(128),
	IN inLevel INT,
	IN inScore bigINT
    )
BEGIN

	-- duplicated player
	DECLARE EXIT HANDLER FOR SQLSTATE '23000'
	BEGIN 
		-- Try to update first
		UPDATE tblRankers 
			SET  `Level` = inLevel, `Score` = inScore, `NickName` = inNickName, `FacebookUID` = inFBUID
			WHERE `PlayerID` = inPlayerID;
	END;

	-- Try to insert new one
	INSERT INTO tblRankers	( `Score`, `PlayerID`, `FacebookUID`, `NickName`, `Level` )
			VALUES	( inScore, inPlayerID,  inFBUID,      inNickName, inLevel );



END */$$
DELIMITER ;

/* Procedure structure for procedure `spUpdateRankingTable` */

/*!50003 DROP PROCEDURE IF EXISTS  `spUpdateRankingTable` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spUpdateRankingTable`(
    )
BEGIN
	set @TableName = CONCAT("tblWeeklyRanking_", CURDATE() + 0);
	-- select @TableName;
	
	-- Create new table
	IF EXISTS (SELECT 1	FROM INFORMATION_SCHEMA.TABLES 
				WHERE TABLE_TYPE='BASE TABLE' 
				AND TABLE_NAME=@TableName) 
	then begin
		set @dropTable = concat("drop table ", @TableName);
		PREPARE stmt FROM @dropTable;
		EXECUTE stmt;
		DEALLOCATE PREPARE stmt;
	end;
	end if;
	
	set @sql = 
		concat("CREATE TABLE ", @TableName, " (
		  `RankingID` INT NOT NULL,
		  `Ranking` INT NOT NULL,
		  `WinRate` FLOAT NOT NULL DEFAULT '0',
		  `Win` INT UNSIGNED NOT NULL DEFAULT '0',
		  `Lose` INT UNSIGNED NOT NULL DEFAULT '0',
		  `GameNick` VARCHAR(64) NOT NULL DEFAULT '',
		  `PlayerID` BIGINT NOT NULL DEFAULT '1',
		  `FacebookUID` BIGINT DEFAULT '0',
		  `Level` INT NOT NULL DEFAULT '1',
		  `DummyRank` INT NOT NULL DEFAULT '0',
		  PRIMARY KEY (`RankingID`),
		  KEY `RankingKey` (`Ranking`)
		) ENGINE=MYISAM DEFAULT MAX_ROWS=1000;");
		
	-- SELECT @sql;
	PREPARE stmt FROM @sql;
	EXECUTE stmt;
	DEALLOCATE PREPARE stmt;
	-- Update Ranking 
	START TRANSACTION;
	CREATE TEMPORARY TABLE IF NOT EXISTS tblRankingMerged (
		  `RankingID` INT NOT NULL AUTO_INCREMENT,
		  `PlayerID` INT NOT NULL,
		  `GameNick` varchar(64) not null,
		  `Level` INT NOT NULL DEFAULT '1',
		  `WinRate` FLOAT NOT NULL DEFAULT '0',
		  `Win` INT UNSIGNED NOT NULL DEFAULT '0',
		  `Lose` INT UNSIGNED NOT NULL DEFAULT '0',
		  `Ranking` int NOT NULL DEFAULT '0',
		  PRIMARY KEY (`RankingID`)
		  
		) ENGINE=MYISAM
		-- WeeklyWin is supposed to hold 
		AS	(SELECT PlayerID, `GameNick`, `Level`, ((WinPlaySC+WinPlayNC+WinPlayNM)-WeeklyWin) AS Win, ((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose) AS Lose, IF(((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose)=0, 1, ((WinPlaySC+WinPlayNC+WinPlayNM)-WeeklyWin)/((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose)) AS WinRate FROM dbconspiracy00.tblPlayer ORDER BY Win DESC, WinRate DESC LIMIT 100) 
		UNION	(SELECT PlayerID, `GameNick`, `Level`, ((WinPlaySC+WinPlayNC+WinPlayNM)-WeeklyWin) AS Win, ((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose) AS Lose, IF(((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose)=0, 1, ((WinPlaySC+WinPlayNC+WinPlayNM)-WeeklyWin)/((LosePlaySC+LosePlayNC+LosePlayNM)-WeeklyLose)) AS WinRate FROM dbconspiracy01.tblPlayer ORDER BY Win DESC, WinRate DESC LIMIT 100) 
		ORDER BY Win DESC, WinRate DESC LIMIT 100;
	-- Weekly ranking is updated update weekly values
	UPDATE dbconspiracy00.tblPlayer SET WeeklyWin = WinPlaySC+WinPlayNC+WinPlayNM, WeeklyLose = LosePlaySC+LosePlayNC+LosePlayNM;
	UPDATE dbconspiracy01.tblPlayer SET WeeklyWin = WinPlaySC+WinPlayNC+WinPlayNM, WeeklyLose = LosePlaySC+LosePlayNC+LosePlayNM;
	-- Leave use total win/lose count for test
	-- UPDATE dbconspiracy00.tblPlayer SET WeeklyWin = 0, WeeklyLose = 0;
	-- UPDATE dbconspiracy01.tblPlayer SET WeeklyWin = 0, WeeklyLose = 0;
	COMMIT;
	SET @sql = 
		CONCAT("INSERT INTO ", @TableName, "  (
		  `RankingID`,
		  `Ranking`,
		  `WinRate`,
		  `Win`,
		  `Lose`,
		  `PlayerID`,
		  `FacebookUID`,
		  `GameNick`,
		  `Level`,
		  `DummyRank`
		)
			SELECT	RankingID,
				@curRank := IF(@prevWinRate=WinRate, @curRank, RankingID),
				WinRate,
				Win,
				Lose,
				PlayerID,
				userTable.FBUserID,
				`GameNick`,
				`Level`,
				@prevWinRate := WinRate
				FROM 	tblRankingMerged LEFT JOIN dbaccount.`tbluser` userTable ON tblRankingMerged.PlayerID = userTable.UserUID,
					(SELECT @curRank :=0, @prevWinRate:=NULL) r
				ORDER BY RankingID ASC LIMIT 100;");
	-- SELECT @sql;
	PREPARE stmt FROM @sql;
	EXECUTE stmt;
	DEALLOCATE PREPARE stmt;
	-- Swap tables
	SET @sql = 
		CONCat("RENAME TABLE tblWeeklyRanking TO tmp_table, ", @TableName, " TO tblWeeklyRanking, tmp_table TO ",@TableName,"_bak;");
	-- SELECT @sql;
	PREPARE stmt FROM @sql;
	EXECUTE stmt;
	DEALLOCATE PREPARE stmt;
    END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
