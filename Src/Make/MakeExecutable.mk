###############################################################################
# 用于被Makefile包含从而生成可执行文件，需要前置定义若干变量
# author : jarek
#
#==============================================================================
# 需要可选的定义如下变量
#BinDir=../../Bin/
#ExeName=GameServer
#ExternLib=-lpthread -lmysqlcppconn-static
#MyLib=../Lib/libProtocol.lib
#Inc=-I../Common/ -I../Protocol
#==============================================================================


# 编译参数
CxxFlag=-Wall -g -Wno-deprecated -Wno-write-strings -Wno-reorder -D_DEBUG
Cxx=g++

# 各种相关文件的计算
Target=$(BinDir)/$(ExeName)
BaseDir=./
ObjDir=$(BaseDir)/Obj/

SubDir=$(patsubst %/, %, $(patsubst %:, % , $(shell ls -R $(BaseDir) | grep "/")))
SubDir:=$(addsuffix /, $(SubDir))

VPATH=$(SubDir)

Source=$(foreach subdir, ${SubDir}, $(wildcard ${subdir}*.cpp))

SRC=$(notdir $(Source))

SrcObj=$(patsubst %.cpp,%.o,$(SRC))
Obj=$(addprefix $(ObjDir), $(SrcObj))

Lib=$(MyLib) $(ExternLib)

.PHONY: clean

all: $(ObjDir) $(Target)

$(ObjDir):
	@if [ ! -d $(ObjDir) ]; then mkdir $(ObjDir); fi

$(Target): $(Obj) $(MyLib)
	@$(Cxx) -o $(Target) $(Obj) $(Lib)
	@echo -e '\e[32m===== Create $(ExeName) =====\e[m'

$(ObjDir)%.o: %.cpp
	@$(Cxx) $(CxxFlag) -c $< -o $@ $(Inc)
	@echo -e 'Compile [$(ExeName)] $< -> $@'

clean:
	@rm -rf $(Target) $(ObjDir) 
	@echo rm $(Target) $(ObjDir)

cleanbin:
	@rm -rf $(Target)
	@echo rm $(Target)

relink: cleanbin all 
rebuild: clean all 

print:
	@echo $(MyLib)
	@echo $(Obj)
	@echo $(VPATH)
	@echo $(ObjDir)
