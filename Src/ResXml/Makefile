#=============================================================
# makefile for generating server resouce header file 
#=============================================================
TDRTOOL=../Extern/Tool/tdr
RESXMLDIR=.

RESOURCE_XML=$(wildcard $(RESXMLDIR)/*.xml)
HEADER_FILE = $(patsubst %.xml, %.h, $(RESOURCE_XML))

header : $(RESOURCE_XML)
	$(TDRTOOL) -H $^ -O .
#	$(TDR) -H --add_custom_prefix="m_" $^ -O .

all : header

.PHONY: clean

clean:
	rm -f $(HEADER_FILE)

