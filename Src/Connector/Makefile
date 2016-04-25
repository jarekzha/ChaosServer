#####################################################
# 	生成Connector
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=Connector
ExternLib=-lpthread -lfcgi -lprotobuf -lboost_thread -lboost_system -L$(ExternLibDir) -lz -lrt -lzmq
MyLib=$(LibCommon) $(LibPrototol) $(LibEngine)
Inc=$(IncProtocol) $(IncEngine) $(IncCommon) $(IncSelf) $(SystemIncDir)

include ../Make/MakeExecutable.mk
