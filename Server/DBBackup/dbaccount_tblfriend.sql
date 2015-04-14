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
-- Table structure for table `tblfriend`
--

DROP TABLE IF EXISTS `tblfriend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tblfriend` (
  `UserUID` bigint(20) NOT NULL,
  `FriendUID` bigint(20) NOT NULL,
  `FriendFacebookUID` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`UserUID`,`FriendUID`),
  UNIQUE KEY `PK_Unique` (`UserUID`,`FriendUID`) USING HASH,
  KEY `PrimaryIndex` (`UserUID`,`FriendUID`),
  KEY `FriendID_Index` (`FriendUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tblfriend`
--

LOCK TABLES `tblfriend` WRITE;
/*!40000 ALTER TABLE `tblfriend` DISABLE KEYS */;
INSERT INTO `tblfriend` (`UserUID`, `FriendUID`, `FriendFacebookUID`) VALUES (21104,22223,100001894965105),(21105,22223,100001894965105),(22223,21104,0),(22223,21105,0),(22223,22224,100001854056790),(22223,22225,100001384095859),(22223,22227,100001831377230),(22223,22228,1839626376),(22224,22223,100001894965105),(22224,22225,100001384095859),(22224,22227,100001831377230),(22224,22228,1839626376),(22225,22223,100001894965105),(22225,22224,100001854056790),(22225,22227,100001831377230),(22225,22228,1839626376),(22227,22223,100001894965105),(22227,22224,100001854056790),(22227,22225,100001384095859),(22227,22228,1839626376),(22228,22223,100001894965105),(22228,22224,100001854056790),(22228,22225,100001384095859),(22228,22227,100001831377230);
/*!40000 ALTER TABLE `tblfriend` ENABLE KEYS */;
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
