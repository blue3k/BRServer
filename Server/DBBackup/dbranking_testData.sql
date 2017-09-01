/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 5.7.18-0ubuntu0.16.04.1 : Database - dbconspiracyranking
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`dbconspiracyranking` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `dbconspiracyranking`;

/*Data for the table `tblrankers` */


insert  into `tblrankers`(`RankingID`,`Score`,`PlayerID`,`FacebookUID`,`NickName`,`Level`,`LatestUpdated`) values 
(1,1570,1,12,'testName',4,'2017-07-03 15:09:43'),
(3,2683,240578,0,'tesalkjdsf',0,'2017-07-03 16:31:31'),
(43,850,2,0,'TestPlayer01',2,'2017-07-03 15:09:43'),
(44,2016,3,0,'TestPlayer02',2,'2017-07-03 15:09:43'),
(45,659,4,0,'TestPlayer03',5,'2017-07-03 15:09:43'),
(46,4370,5,0,'TestPlayer04',3,'2017-07-03 15:09:43'),
(47,2096,6,0,'TestPlayer05',6,'2017-07-03 15:09:43'),
(48,497,7,0,'TestPlayer06',3,'2017-07-03 15:09:43'),
(49,579,8,0,'TestPlayer07',7,'2017-07-03 15:09:43'),
(50,1534,9,0,'TestPlayer08',5,'2017-07-03 15:09:43'),
(51,3925,10,0,'TestPlayer09',9,'2017-07-03 15:09:43'),
(52,769,11,0,'TestPlayer10',1,'2017-07-03 15:09:43'),
(53,4292,12,0,'TestPlayer11',5,'2017-07-03 15:09:43'),
(54,4626,13,0,'TestPlayer12',9,'2017-07-03 15:09:43'),
(55,4259,14,0,'TestPlayer13',9,'2017-07-03 15:09:43'),
(56,4294,15,0,'TestPlayer14',5,'2017-07-03 15:09:43'),
(57,3244,16,0,'TestPlayer15',8,'2017-07-03 15:09:43'),
(58,3003,17,0,'TestPlayer16',7,'2017-07-03 15:09:43'),
(59,2849,18,0,'TestPlayer17',9,'2017-07-03 15:09:43'),
(60,2114,19,0,'TestPlayer18',2,'2017-07-03 15:09:43'),
(61,4516,20,0,'TestPlayer19',4,'2017-07-03 15:09:43'),
(62,1782,21,0,'TestPlayer20',2,'2017-07-03 15:09:43'),
(63,3211,22,0,'TestPlayer21',1,'2017-07-03 15:09:43'),
(64,4969,23,0,'TestPlayer22',8,'2017-07-03 15:09:43'),
(65,522,24,0,'TestPlayer23',6,'2017-07-03 15:09:43'),
(66,191,25,0,'TestPlayer24',7,'2017-07-03 15:09:43'),
(67,3930,26,0,'TestPlayer25',2,'2017-07-03 15:09:43'),
(68,2958,27,0,'TestPlayer26',4,'2017-07-03 15:09:43'),
(69,835,28,0,'TestPlayer27',8,'2017-07-03 15:09:43'),
(70,4244,29,0,'TestPlayer28',5,'2017-07-03 15:09:43'),
(71,3127,30,0,'TestPlayer29',8,'2017-07-03 15:09:43'),
(72,1209,31,0,'TestPlayer30',6,'2017-07-03 15:09:43'),
(73,4826,32,0,'TestPlayer31',3,'2017-07-03 15:09:43'),
(74,811,33,0,'TestPlayer32',8,'2017-07-03 15:09:43'),
(75,1975,34,0,'TestPlayer33',4,'2017-07-03 15:09:43'),
(76,227,35,0,'TestPlayer34',8,'2017-07-03 15:09:43'),
(77,1532,36,0,'TestPlayer35',7,'2017-07-03 15:09:43'),
(78,2076,37,0,'TestPlayer36',5,'2017-07-03 15:09:43'),
(79,644,38,0,'TestPlayer37',7,'2017-07-03 15:09:43'),
(80,3553,39,0,'TestPlayer38',6,'2017-07-03 15:09:43'),
(81,1715,40,0,'TestPlayer39',7,'2017-07-03 15:09:43'),
(82,610,41,0,'TestPlayer40',6,'2017-07-03 15:09:43'),
(83,1154,42,0,'TestPlayer41',7,'2017-07-03 15:09:43'),
(84,1445,43,0,'TestPlayer42',10,'2017-07-03 15:09:43'),
(85,2059,44,0,'TestPlayer43',5,'2017-07-03 15:09:43'),
(86,1124,45,0,'TestPlayer44',7,'2017-07-03 15:09:43'),
(87,2818,46,0,'TestPlayer45',0,'2017-07-03 15:09:43'),
(88,3435,47,0,'TestPlayer46',7,'2017-07-03 15:09:43'),
(89,4893,48,0,'TestPlayer47',4,'2017-07-03 15:09:43'),
(90,2053,49,0,'TestPlayer48',7,'2017-07-03 15:09:43'),
(91,4978,50,0,'TestPlayer49',0,'2017-07-03 15:09:43'),
(92,1108,51,0,'TestPlayer50',8,'2017-07-03 15:09:43'),
(93,1238,52,0,'TestPlayer51',6,'2017-07-03 15:09:43'),
(94,3549,53,0,'TestPlayer52',5,'2017-07-03 15:09:43'),
(95,1288,54,0,'TestPlayer53',1,'2017-07-03 15:09:43'),
(96,2533,55,0,'TestPlayer54',1,'2017-07-03 15:09:43'),
(97,1532,56,0,'TestPlayer55',4,'2017-07-03 15:09:43'),
(98,1312,57,0,'TestPlayer56',4,'2017-07-03 15:09:43'),
(99,1149,58,0,'TestPlayer57',0,'2017-07-03 15:09:43'),
(100,26,59,0,'TestPlayer58',1,'2017-07-03 15:09:43'),
(101,144,60,0,'TestPlayer59',6,'2017-07-03 15:09:43'),
(102,2501,61,0,'TestPlayer60',3,'2017-07-03 15:09:43'),
(103,1229,62,0,'TestPlayer61',5,'2017-07-03 15:09:43'),
(104,849,63,0,'TestPlayer62',6,'2017-07-03 15:09:43'),
(105,60,64,0,'TestPlayer63',1,'2017-07-03 15:09:43'),
(106,3501,65,0,'TestPlayer64',7,'2017-07-03 15:09:43'),
(107,986,66,0,'TestPlayer65',3,'2017-07-03 15:09:43'),
(108,2213,67,0,'TestPlayer66',2,'2017-07-03 15:09:43'),
(109,3170,68,0,'TestPlayer67',6,'2017-07-03 15:09:43'),
(110,307,69,0,'TestPlayer68',4,'2017-07-03 15:09:43'),
(111,2316,70,0,'TestPlayer69',1,'2017-07-03 15:09:43'),
(112,1240,71,0,'TestPlayer70',9,'2017-07-03 15:09:43'),
(113,2711,72,0,'TestPlayer71',5,'2017-07-03 15:09:43'),
(114,358,73,0,'TestPlayer72',3,'2017-07-03 15:09:43'),
(115,2398,74,0,'TestPlayer73',3,'2017-07-03 15:09:43'),
(116,2347,75,0,'TestPlayer74',4,'2017-07-03 15:09:43'),
(117,3007,76,0,'TestPlayer75',2,'2017-07-03 15:09:43'),
(118,1008,77,0,'TestPlayer76',4,'2017-07-03 15:09:43'),
(119,2096,78,0,'TestPlayer77',8,'2017-07-03 15:09:44'),
(120,745,79,0,'TestPlayer78',7,'2017-07-03 15:09:44'),
(121,296,80,0,'TestPlayer79',7,'2017-07-03 15:09:44'),
(122,3925,81,0,'TestPlayer80',2,'2017-07-03 15:09:44'),
(123,1905,82,0,'TestPlayer81',3,'2017-07-03 15:09:44'),
(124,874,83,0,'TestPlayer82',9,'2017-07-03 15:09:44'),
(125,4463,84,0,'TestPlayer83',3,'2017-07-03 15:09:44'),
(126,1933,85,0,'TestPlayer84',6,'2017-07-03 15:09:44'),
(127,1563,86,0,'TestPlayer85',1,'2017-07-03 15:09:44'),
(128,2305,87,0,'TestPlayer86',3,'2017-07-03 15:09:44'),
(129,4729,88,0,'TestPlayer87',5,'2017-07-03 15:09:44'),
(130,4100,89,0,'TestPlayer88',3,'2017-07-03 15:09:44'),
(131,334,90,0,'TestPlayer89',1,'2017-07-03 15:09:44'),
(132,4450,91,0,'TestPlayer90',0,'2017-07-03 15:09:44'),
(133,1535,92,0,'TestPlayer91',4,'2017-07-03 15:09:44'),
(134,4128,93,0,'TestPlayer92',3,'2017-07-03 15:09:44'),
(135,4612,94,0,'TestPlayer93',1,'2017-07-03 15:09:44'),
(136,479,95,0,'TestPlayer94',4,'2017-07-03 15:09:44'),
(137,2589,96,0,'TestPlayer95',4,'2017-07-03 15:09:44'),
(138,4885,97,0,'TestPlayer96',5,'2017-07-03 15:09:44'),
(139,4508,98,0,'TestPlayer97',4,'2017-07-03 15:09:44'),
(140,1695,99,0,'TestPlayer98',4,'2017-07-03 15:09:44'),
(141,1623,100,0,'TestPlayer99',5,'2017-07-03 15:09:44'),
(142,595,101,0,'TestPlayer10',2,'2017-07-03 15:09:44');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
