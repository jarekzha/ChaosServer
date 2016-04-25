#####################################################
#   生成libProtocol.a
#   author: jarek

TARGETLIB=../Lib/libProtocol.a
PROTOFILE=$(shell ls *.proto)
PROTOOUTDIR=./

all: $(TARGETLIB)

$(TARGETLIB) : $(PROTOFILE)
	@protoc --cpp_out=$(PROTOOUTDIR) $(PROTOFILE)
	@rename .cc .cpp *.cc
	@$(MAKE) -f MakefileReal.mk

#------------------------------------------------------
clean:
	rm -rf *.cc *.cpp *.h README.txt
	@$(MAKE) -f MakefileReal.mk clean

