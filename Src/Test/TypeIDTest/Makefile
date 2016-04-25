###############################################################################
# 用于被Makefile包含从而生成可执行文件，需要前置定义若干变量
# author : jarek
#
#==============================================================================
# 需要可选的定义如下变量
BINDIR=./
EXENAME=test
EXTERNLIB=-lpthread 
MYLIB=../../../Src/Lib/libEngine.a
INC=-I../../../Src/Engine
#==============================================================================


# 编译参数
CXX_FLAG=-Wall -g -Wno-deprecated -Wno-write-strings -Wno-reorder -D_DEBUG
CXX=g++

# 各种相关文件的计算
TARGET=$(BINDIR)$(EXENAME)
BASEDIR=./
OBJDIR=$(BASEDIR)/Obj/

SRCDIR=$(patsubst %/, %, $(patsubst %:, % , $(shell ls -R $(BASEDIR) | grep "/")))
SRCDIR:=$(addsuffix /, $(SRCDIR))

VPATH=$(SRCDIR)

SOURCE=$(foreach subdir, ${SRCDIR}, $(wildcard ${subdir}*.cpp))

SRC=$(notdir $(SOURCE))

SRCOBJ=$(patsubst %.cpp,%.o,$(SRC))
OBJ=$(addprefix $(OBJDIR), $(SRCOBJ))

LIB=$(MYLIB) $(EXTERNLIB)

.PHONY: clean

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi

$(TARGET): $(OBJ) $(MYLIB)
	@$(CXX) -o $(TARGET) $(OBJ) $(LIB)
	@echo -e '\e[32m===== Create $(EXENAME) =====\e[m'

$(OBJDIR)%.o: %.cpp
	@$(CXX) $(CXX_FLAG) -c $< -o $@ $(INC)
	@echo -e 'Compile [$(EXENAME)] $< -> $@'

clean:
	@rm -rf $(TARGET) $(OBJDIR) 
	@echo rm $(TARGET) $(OBJDIR)

cleanbin:
	@rm -rf $(TARGET)
	@echo rm $(TARGET)

relink: cleanbin all 
rebuild: clean all 

print:
	@echo $(SRCDIR)
	@echo $(OBJ)
	@echo $(VPATH)
	@echo $(OBJDIR)
