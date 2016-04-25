#####################################################
# 	生成GMConnector
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=GMConnector
ExternLib=-lpthread -lfcgi -lprotobuf -lboost_thread -lboost_system -L$(ExternLibDir) -lz -lrt -lzmq
MyLib=$(LibCommon) $(LibPrototol) $(LibEngine)
Inc=$(IncProtocol) $(IncEngine) $(IncCommon) $(IncSelf)

include ../Make/MakeExecutable.mk
