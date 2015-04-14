CREATE DATABASE  IF NOT EXISTS `dbconspiracy00` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `dbconspiracy00`;
-- MySQL dump 10.13  Distrib 5.6.13, for Win32 (x86)
--
-- Host: localhost    Database: dbconspiracy00
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
-- Table structure for table `tblplayer`
--

DROP TABLE IF EXISTS `tblplayer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblplayer` (
  `PlayerID` bigint(20) NOT NULL,
  `Grade` tinyint(4) NOT NULL DEFAULT '0',
  `Level` smallint(6) NOT NULL DEFAULT '0',
  `Exp` bigint(20) NOT NULL DEFAULT '0',
  `Money` bigint(20) NOT NULL DEFAULT '0',
  `gem` bigint(20) NOT NULL DEFAULT '0',
  `Stamina` smallint(6) NOT NULL DEFAULT '0',
  `TotalPlayed` int(11) NOT NULL DEFAULT '0',
  `WinPlaySC` int(11) NOT NULL DEFAULT '0',
  `WinPlaySM` int(11) NOT NULL DEFAULT '0',
  `LosePlaySC` int(11) NOT NULL DEFAULT '0',
  `LosePlaySM` int(11) NOT NULL DEFAULT '0',
  `WinPlayNC` int(11) NOT NULL DEFAULT '0',
  `WinPlayNM` int(11) NOT NULL DEFAULT '0',
  `LosePlayNC` int(11) NOT NULL DEFAULT '0',
  `LosePlayNM` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerID`),
  UNIQUE KEY `PlayerID_UNIQUE` (`PlayerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tblplayer`
--

LOCK TABLES `tblplayer` WRITE;
/*!40000 ALTER TABLE `tblplayer` DISABLE KEYS */;
/*!40000 ALTER TABLE `tblplayer` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-11-14  1:24:09
