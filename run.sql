/*
Navicat MySQL Data Transfer

Source Server         : 虚拟机
Source Server Version : 50536
Source Host           : 192.168.2.105:3306
Source Database       : test

Target Server Type    : MYSQL
Target Server Version : 50536
File Encoding         : 65001

Date: 2017-01-29 18:41:47
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for problem
-- ----------------------------
DROP TABLE IF EXISTS `problem`;
CREATE TABLE `problem` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `contest_id` int(10) NOT NULL DEFAULT '0',
  `is_public` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否公开',
  `available` tinyint(1) NOT NULL DEFAULT '1' COMMENT '是否可用',
  `special_judge` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'spj',
  `title` varchar(255) NOT NULL COMMENT '标题',
  `description` text NOT NULL COMMENT '描述',
  `hint` text COMMENT '提示',
  `limit_time` int(11) NOT NULL DEFAULT '0',
  `limit_mem` int(11) NOT NULL DEFAULT '0',
  `limit_output` int(11) NOT NULL DEFAULT '0',
  `limit_lang` int(11) NOT NULL DEFAULT '0' COMMENT '语言限制掩码',
  `sample_in` text,
  `sample_out` text,
  `creat_time` datetime NOT NULL DEFAULT '2017-01-01 00:00:00' COMMENT '题目创建时间',
  `accepted` int(11) NOT NULL DEFAULT '0' COMMENT '接受',
  `submit` int(11) NOT NULL DEFAULT '0' COMMENT '提交',
  `solved` int(11) NOT NULL DEFAULT '0' COMMENT '解决',
  `delete` tinyint(1) NOT NULL DEFAULT '0' COMMENT '是否删除',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=66 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of problem
-- ----------------------------
INSERT INTO `problem` VALUES ('65', '0', '1', '1', '0', 'TEST', 'TEST', 'TET', '10', '10', '10', '0', null, null, '2017-01-01 00:00:00', '0', '7', '0', '0');

-- ----------------------------
-- Table structure for solution
-- ----------------------------
DROP TABLE IF EXISTS `solution`;
CREATE TABLE `solution` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `problem_id` int(10) NOT NULL DEFAULT '0',
  `contest_id` int(10) NOT NULL DEFAULT '0',
  `user_id` int(10) NOT NULL DEFAULT '0',
  `status` tinyint(2) NOT NULL DEFAULT '0',
  `used_time` int(11) NOT NULL DEFAULT '0',
  `used_mem` int(11) NOT NULL DEFAULT '0',
  `lang` tinyint(2) DEFAULT NULL,
  `code` text,
  `stdout` text,
  `err_msg` varchar(255) DEFAULT NULL,
  `code_length` int(11) NOT NULL DEFAULT '0',
  `time` datetime NOT NULL DEFAULT '2017-01-01 00:00:00',
  `delete` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of solution
-- ----------------------------
INSERT INTO `solution` VALUES ('1', '65', '0', '1000', '3', '0', '0', '0', '#include<stdio.h>\nint main(){;p555rintf(\"Hello Wo2rld\");}', '', '/tmp/cch0paBZ.o: In function `main\':\nmain.c:(.text.startup+0xc): undefined reference to `p555rintf\'\ncollect2: error: ld returned 1 exit status\n', '57', '2017-01-01 00:00:00', '0');
