#####################################################
# 	生成Robot
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=IPCTool
ExternLib=-lprotobuf $(LibEngine)  -lrt -L$(ExternLibDir) -lz -lpthread
MyLib=$(LibCommon) $(LibPrototol) 
Inc=$(IncSelf) $(IncProtocol) $(IncEngine) $(IncCommon)

include ../Make/MakeExecutable.mk
