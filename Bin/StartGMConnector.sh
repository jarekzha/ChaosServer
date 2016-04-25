#!/bin/bash

./Tool/spawn-fcgi -a 127.0.0.1 -p 9001 -f './GMConnector -c ./Config/GMConnectorConfig.xml'
