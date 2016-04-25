#####################################################
# 	生成Robot
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=Robot
ExternLib=-lpthread -lprotobuf -lboost_thread -lboost_system $(LibEngine) $(LibCurl) $(ExternLibDir)/libz.a
MyLib=$(LibCommon) $(LibPrototol) 
Inc=$(IncSelf) $(IncProtocol) $(IncEngine) $(IncCommon)

include ../Make/MakeExecutable.mk
