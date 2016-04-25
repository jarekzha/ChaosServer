#####################################################
# 	生成DBProxy
#	author: jarek

ProjectDir=../../
include ../Make/MakeCommon.mk

ExeName=DBProxy

ExternLib=$(SystemLibDir) -lpthread -lprotobuf -lmysqlcppconn-static \
		 -L$(ExternLibDir) -lmysqlclient_r -lz -lrt -lboost_thread -lboost_system -lzmq
MyLib=$(LibCommon) $(LibPrototol) $(LibEngine)
Inc=$(IncProtocol) $(IncEngine) $(IncCommon) $(IncSelf) 

include ../Make/MakeExecutable.mk
