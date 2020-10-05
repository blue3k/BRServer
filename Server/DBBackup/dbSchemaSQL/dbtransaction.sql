/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbGameTransaction00
*********************************************************************
*/

/*!40101 SET NAMES utf8mb4 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbGameTransaction00` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE `dbGameTransaction00`;

/*Table structure for table `tblGameLog` */

DROP TABLE IF EXISTS `tblGameLog`;

CREATE TABLE `tblGameLog` (
  `LogTime` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `GameTime` INT DEFAULT NULL,
  `PlayerID` BIGINT NOT NULL,
  `LogCategory` CHAR(1) NOT NULL,
  `Consume` INT NOT NULL,
  `Gain` INT NOT NULL,
  `TotalValue` BIGINT DEFAULT NULL,
  `LogMessage` VARCHAR(1024) NOT NULL,
  PRIMARY KEY (`LogTime`),
  KEY `PlayerByType` (`PlayerID`,`LogCategory`)
) ENGINE=INNODB;

/*Table structure for table `tblShardInfo` */

DROP TABLE IF EXISTS `tblShardInfo`;

CREATE TABLE `tblShardInfo` (
  `ShardID` INT NOT NULL,
  `ConnectionString` VARCHAR(512) NOT NULL,
  `DBName` varchar(128) NOT NULL,
  PRIMARY KEY (`ShardID`)
) ENGINE=InnoDB;

/* Procedure structure for procedure `spAddGameLog` */

/*!50003 DROP PROCEDURE IF EXISTS  `spAddGameLog` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spAddGameLog`(
    in inPlayerID bigint,
    in inGameTime int,
    in inLogCategory char(1),
    in inConsume int,
    in inGain int,
    in inTotalValue bigint,
    in inLogMessage varchar(1024),
    out outResult int
    )
BEGIN
	DECLARE CONTINUE HANDLER FOR SQLSTATE '23000'
	BEGIN 
		-- Already in the friend list
		SET outResult = -1;
	END;
	
	SET outResult = 0;
	INSERT INTO tblGameLog
		( PlayerID, GameTime, LogCategory, Consume, Gain, TotalValue, LogMessage )
		VALUES
		( inPlayerID, inGameTime, inLogCategory, inConsume, inGain, inTotalValue, inLogMessage );
    END */$$
DELIMITER ;

/* Procedure structure for procedure `spGetShardList` */

/*!50003 DROP PROCEDURE IF EXISTS  `spGetShardList` */;

DELIMITER $$

/*!50003 CREATE PROCEDURE `spGetShardList`(
	in shardID int
    )
BEGIN
    
	select ShardID, ConnectionString, DBName from tblShardInfo;
	
    END */$$
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
