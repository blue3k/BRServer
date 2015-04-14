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
-- Table structure for table `tbluser`
--

DROP TABLE IF EXISTS `tbluser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tbluser` (
  `UserUID` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT 'User unique ID which is assigned by us',
  `UserName` varchar(60) DEFAULT NULL COMMENT 'User account name',
  `UserPassword` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL COMMENT 'Password',
  `FBUserID` bigint(20) DEFAULT NULL COMMENT 'Facebook user ID',
  `GameNick` varchar(60) NOT NULL COMMENT 'Default nick name in game',
  `LatestLoggedIn` timestamp NULL DEFAULT NULL COMMENT 'Latest logged in time',
  `CreatedDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `LoginToken` varchar(512) NOT NULL DEFAULT '0',
  `EMail` varchar(45) DEFAULT NULL,
  `GCMKeys` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`UserUID`),
  UNIQUE KEY `UserID_UNIQUE` (`UserUID`),
  UNIQUE KEY `UserName_UNIQUE` (`UserName`),
  UNIQUE KEY `FBUserID_UNIQUE` (`FBUserID`),
  UNIQUE KEY `Email_UNIQUE` (`EMail`)
) ENGINE=InnoDB AUTO_INCREMENT=22230 DEFAULT CHARSET=utf8 COMMENT='User account table';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tbluser`
--

LOCK TABLES `tbluser` WRITE;
/*!40000 ALTER TABLE `tbluser` DISABLE KEYS */;
INSERT INTO `tbluser` (`UserUID`, `UserName`, `UserPassword`, `FBUserID`, `GameNick`, `LatestLoggedIn`, `CreatedDate`, `LoginToken`, `EMail`, `GCMKeys`) VALUES (4279,'1','1',NULL,'1',NULL,'2013-07-06 13:01:55','0',NULL,NULL),(4282,'2','2',NULL,'2',NULL,'2013-07-06 13:05:56','0',NULL,NULL),(4297,'','',NULL,'','2013-07-06 14:27:38','2013-07-06 13:18:57','0',NULL,NULL),(21104,'Tester1','Tester1',NULL,'Tester1','2013-11-12 13:33:53','2013-07-06 15:55:49','0','test1@test.com',NULL),(21105,'Tester2','Tester2',NULL,'Tester2','2013-11-12 13:33:57','2013-07-06 15:55:57','0','test2@test.com',NULL),(21106,'Tester3','Tester3',NULL,'Tester3','2013-11-12 13:33:58','2013-07-06 15:56:04','0','test3@test.com',NULL),(21107,'Tester4','Tester4',NULL,'Tester4','2013-10-28 00:26:58','2013-07-06 15:56:10','0','test4@test.com',NULL),(21108,'Tester5','Tester5',NULL,'Tester5',NULL,'2013-07-06 15:56:16','0','test5@test.com',NULL),(21109,'Tester6','Tester6',NULL,'Tester6',NULL,'2013-07-06 15:56:23','0','test6@test.com',NULL),(21110,'Tester7','Tester7',NULL,'Tester7',NULL,'2013-07-06 15:56:30','0','test7@test.com',NULL),(21111,'Tester8','Tester8',NULL,'Tester8',NULL,'2013-07-06 15:56:36','0','test8@test.com',NULL),(21112,'Tester9','Tester9',NULL,'Tester9',NULL,'2013-07-06 15:56:42','0','test9@test.com',NULL),(21113,'Tester10','Tester10',NULL,'Tester10',NULL,'2013-07-06 15:56:50','0','test10@test.com',NULL),(21114,'Tester11','Tester11',NULL,'Tester11',NULL,'2013-07-06 15:56:56','0','test11@test.com',NULL),(21115,'Tester12','Tester12',NULL,'Tester12',NULL,'2013-07-06 15:57:01','0','test12@test.com',NULL),(21116,'Tester13','Tester13',NULL,'Tester13',NULL,'2013-07-06 15:57:06','0','test13@test.com',NULL),(21117,'Tester14','Tester14',NULL,'Tester14',NULL,'2013-07-06 15:57:11','0','test14@test.com',NULL),(21118,'Tester15','Tester15',NULL,'Tester15',NULL,'2013-07-06 15:57:20','0','test15@test.com',NULL),(21119,'Tester16','Tester16',NULL,'Tester16',NULL,'2013-07-06 15:57:26','0','test16@test.com',NULL),(21120,'Tester17','Tester17',NULL,'Tester17',NULL,'2013-07-06 15:57:36','0','test17@test.com',NULL),(21121,'Tester18','Tester18',NULL,'Tester18',NULL,'2013-07-06 15:57:42','0','test18@test.com',NULL),(21122,'Tester19','Tester19',NULL,'Tester19',NULL,'2013-07-06 15:57:52','0','test19@test.com',NULL),(21123,'Tester20','Tester20',NULL,'Tester20',NULL,'2013-07-06 15:58:00','0','test20@test.com',NULL),(22223,NULL,NULL,100001894965105,'케이군1','2013-11-12 13:34:33','2013-07-12 14:50:04','0','blue3k@gmail.com','APA91bH-6kz7fuBLk0RmFKBXGqwC0zTJHJhEaTnX8BCLgRVVKvpMtvSUtmvX7mV8AOvEFBmCk4yo3BAm-_KUqd2GeDX6CPzI39iMfK5Ex1xPxupEWrtgAjfZ2Ille2hoacWBV-_HAdaO'),(22224,NULL,NULL,100001854056790,'야호','2013-10-25 14:59:32','2013-07-14 07:43:33','0','yaho0620@gmail.com','APA91bE2FfnLFaPG2xSWdXW3sSS_S6YVbm0Skg3P_4RXKOsLT4cihd9fuLIzCQqyBKBpZWlnfmiWDQpLk5h85UPmvAlAZM_VWU29z0FzPEJcDSjRYhLETx7JZO0d5Z1ZECM9rA7t3NOU'),(22225,NULL,NULL,100001384095859,'catlatte','2013-10-31 15:19:22','2013-07-14 07:52:53','0','jth0927@hotmail.com','APA91bFO5WATRXVqgoh_hcltWTfFOdapMxPHnDgq9q-ikmpgCllIcE3sWcHHa2aMs_jKn8n5_rIIXNCRmOyzkIe_Aqopmygr5OnGLmRou78z4yMXtCYbDeXeFk4e-2CKDPd6CfQtf72e'),(22226,NULL,NULL,1795378172,'짱군','2013-10-31 14:47:43','2013-07-14 08:01:54','0','amiesuie@naver.com','APA91bE3rDpGtaA2Yd8VJRMyAD_0doBZeOHKniN85OCtHFn7zBG2Rljyfex8IBmEiVnC2FPLtu5jP8MwKQSDg95fDxlbY3HJtnJO9S-CDJpOoIKdarjtUKh7hW9mRMox7HEu7ufJBw8Q'),(22227,NULL,NULL,100001831377230,'바락','2013-10-19 12:32:49','2013-07-14 08:12:02','0','barakcao@gmail.com','APA91bGuWihUPZaNCpPU67oDLaSGyjEntBDOrC3l6ei78b9Ci4Lpj_GkeJrWr4LCmFaTaJZT2PZwAdc6kUe5ZS4bT076S2rttwwhMxsfygbBKa-0cMCCOxbY_GOkGBHI68iHwXQxSgFm'),(22228,NULL,NULL,1839626376,'충쿤','2013-11-11 23:15:20','2013-08-02 05:33:03','0','proib@naver.com','APA91bHGCIAOEwYxnr-W0IkLSq1OeKojocuF9ptDp6P7Ev7eh-693G19PR6-mQpk2fiJFKEdpcl83eVlYAyNuK06FLgW8-mxix5kobEUFusVfUMePTorz1R_zmJD_JPDU38QF8Mh5WCU'),(22229,NULL,NULL,100002036101529,'신띠아','2013-11-02 05:57:16','2013-08-02 13:19:26','0','durmi@hanmail.net','APA91bEvxmIyOCI49lhFr5bFnS1vMg3-FoX-xWTRU1m9utqZB8_8WN3X2tnlPik3GGJroA9AtHS78BqOGx4iemBUT_AxswO9BMPMd-OR5LBqifIpEyRkybBuOtgEtQE3kIm47I_Hwy2cTS5Wao6G9drgYSdpX-vHWw');
/*!40000 ALTER TABLE `tbluser` ENABLE KEYS */;
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
