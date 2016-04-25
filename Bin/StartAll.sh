#!/bin/sh

./DBProxy -d -c./Config/DBProxyConfig.xml 
./GameServer -d -c./Config/GameServerConfig.xml 
./StartConnector.sh
./StartGMConnector.sh

./Monitor.sh
