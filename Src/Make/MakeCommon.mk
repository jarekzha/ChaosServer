###############################################################################
# 预定义若干makefile需要的变量
# author : jarek
#
#==============================================================================
# 需要预定义的定义如下变量
# ProjectDir=../
#==============================================================================

SrcDir=$(ProjectDir)/Src/
BinDir=$(ProjectDir)/Bin/
LibDir=$(SrcDir)/Lib/

ExternDir=$(SrcDir)/Extern/
ExternLibDir=$(ExternDir)/Lib/

SystemLibDir=-L/usr/local/mysql/lib/ -L/usr/local/lib/ -L/usr/lib64/ /usr/local/mysql/lib/mysql
SystemIncDir=-I/usr/local/include			 			

#Inc-----------------------------
IncSelf=-I./
IncEngine=-I$(SrcDir) -I$(SrcDir)/Engine/
IncCommon=-I$(SrcDir) -I$(SrcDir)/Common/
IncProtocol=-I$(SrcDir) -I$(SrcDir)/Protocol/

#Lib-----------------------------
LibEngine=$(LibDir)/libEngine.a
LibCurl=$(ExternLibDir)/libcurl.a -lcrypto -lidn -lssl -lldap -lzmq
LibCommon=$(LibDir)/libCommon.a
LibPrototol=$(LibDir)/libProtocol.a
