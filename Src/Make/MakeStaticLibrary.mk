###############################################################################
# 用于被包含从而生成lib，需要前置定义若干变量
# author : jarek
#
#==============================================================================
# 需要可选的定义如下变量
#LibDir=../Lib/
#LibName=libCommon.a
#Inc=-I../
#==============================================================================

Output=$(LibDir)$(LibName)

Ar=ar
Rm=rm -rf
CxxFlag=-Wall -g -Wno-deprecated -Wno-reorder -D_DEBUG -DTSF4G_TDR_DEBUG
Cxx=g++

BaseDir=./
ObjDir=$(BaseDir)/Obj/

SubDir=$(patsubst %/, %, $(patsubst %:, % , $(shell ls -R $(BaseDir) | grep "/")))
SubDir:=$(addsuffix /, $(SubDir))

VPATH=$(SubDir)

Source=$(foreach subdir, ${SubDir}, $(wildcard ${subdir}*.cpp))

Src=$(notdir $(Source))

SrcObj=$(patsubst %.cpp,%.o,$(Src))
Obj=$(addprefix $(ObjDir), $(SrcObj))

.PHONY:clean

all: $(ObjDir) $(Output)

$(ObjDir):
	@if [ ! -d $(ObjDir) ]; then mkdir $(ObjDir); fi

$(Output): $(Obj)
	@$(Ar) cr $(Output) $(Obj) 
	@echo -e '\e[33m===== Create $(LibName) =====\e[m'

$(ObjDir)%.o: %.cpp
	@$(Cxx) $(CxxFlag) -c $< -o $@ $(Inc)
	@echo -e 'Compile [$(LibName)] $< -> $@'

clean:
	@$(Rm) $(Output) $(ObjDir)
	@echo clean ... $(Output)

print:
	@echo $(LibDir)
	@echo $(ObjDir)
	@echo $(SrcObj)
	@echo $(Obj)
