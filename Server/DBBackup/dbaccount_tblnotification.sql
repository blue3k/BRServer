CREATE DATABASE  IF NOT EXISTS `dbaccount` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `dbaccount`;
-- MySQL dump 10.13  Distrib 5.6.13, for Win32 (x86)
--
-- Host: localhost    Database: dbaccount
-- ------------------------------------------------------
-- Server version	5.6.14-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tblnotification`
--

DROP TABLE IF EXISTS `tblnotification`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblnotification` (
  `NotificationID` int(11) NOT NULL AUTO_INCREMENT,
  `UserUID` bigint(20) unsigned NOT NULL,
  `MessageID` smallint(5) unsigned NOT NULL,
  `MessageParam0` bigint(20) DEFAULT NULL,
  `MessageParam1` bigint(20) DEFAULT NULL,
  `MessageText` varchar(512) DEFAULT NULL,
  `IssuedTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`NotificationID`),
  UNIQUE KEY `NotificationID_UNIQUE` (`NotificationID`),
  KEY `UserID_KEY` (`UserUID`)
) ENGINE=MyISAM AUTO_INCREMENT=58 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tblnotification`
--

LOCK TABLES `tblnotification` WRITE;
/*!40000 ALTER TABLE `tblnotification` DISABLE KEYS */;
INSERT INTO `tblnotification` (`NotificationID`, `UserUID`, `MessageID`, `MessageParam0`, `MessageParam1`, `MessageText`, `IssuedTime`) VALUES (1,10,3,22223,8594313445456,'케이군1','2013-09-08 10:36:46'),(35,13,3,22229,8594313445469,'신띠아','2013-09-08 13:39:31'),(36,22227,3,22228,8594313445460,'충쿤','2013-09-15 07:43:23'),(11,12,3,22228,8594313445465,'충쿤','2013-09-08 13:25:40'),(10,10,3,22228,8594313445465,'충쿤','2013-09-08 13:25:38'),(12,13,3,22228,8594313445465,'충쿤','2013-09-08 13:25:40'),(57,21105,3,22223,8594313445463,'케이군1','2013-10-30 14:53:52'),(34,12,3,22229,8594313445469,'신띠아','2013-09-08 13:39:30'),(44,22227,3,22225,8594313445471,'catlatte','2013-10-12 11:00:58'),(33,10,3,22229,8594313445469,'신띠아','2013-09-08 13:39:29'),(53,22223,3,22225,8594313445486,'catlatte','2013-10-21 12:01:29'),(54,22224,3,22225,8594313445486,'catlatte','2013-10-21 12:01:30'),(55,22227,3,22225,8594313445486,'catlatte','2013-10-21 12:01:30');
/*!40000 ALTER TABLE `tblnotification` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-11-14  1:24:10
