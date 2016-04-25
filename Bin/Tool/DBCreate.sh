#!/bin/bash

DBUSER="root"
DBPASSWD="root"
DBNAME="HGame"

MYSQL_CMD="mysql -D ${DBNAME} -u${DBUSER} -p${DBPASSWD}"

# 创建DB
echo "Create database $DBNAME..."
mysql -u$DBUSER -p$DBPASSWD -e "CREATE DATABASE IF NOT EXISTS $DBNAME"


echo "Create table player..."
CREATE_STMT="DROP TABLE IF EXISTS player;
	CREATE TABLE IF NOT EXISTS player(
		account VARCHAR(64) NOT NULL PRIMARY KEY, 
		data BLOB NOT NULL,
		mailbox BLOB NOT NULL);"
echo $CREATE_STMT | $MYSQL_CMD

echo "Create table mail..."
CREATE_CMD="DROP TABLE IF EXISTS mail;
	CREATE TABLE IF NOT EXISTS mail(
		seq int unsigned NOT NULL auto_increment,
		account VARCHAR(64) NOT NULL,
		content BLOB NOT NULL,
		PRIMARY KEY(seq));"
echo $CREATE_CMD | $MYSQL_CMD
