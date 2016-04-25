#!/bin/bash

./Tool/spawn-fcgi -a 127.0.0.1 -p 9000 -f './Connector -c ./Config/ConnectorConfig.xml'
