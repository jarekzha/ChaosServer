#####################################################
# 	生成GameServer
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=GameServer
ExternLib=-lpthread -lprotobuf -lboost_thread -lzmq
MyLib=$(LibCommon) $(LibPrototol) $(LibEngine)
Inc=$(IncProtocol) $(IncEngine) $(IncCommon) $(IncSelf)

include ../Make/MakeExecutable.mk
