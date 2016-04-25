########################
#   分别去make项目中个各个模块
#

#TARGET= Engine Protocol ResXml Common DBProxy Connector GMConnector IPCTool Robot GameServer
TARGET= Engine Protocol ResXml Common Connector GMConnector IPCTool Robot GameServer

THIS_MAKEFILE=$(abspath $(firstword $(MAKEFILE_LIST))) 
THIS_MAKEFILE_DIR=$(shell dirname ${THIS_MAKEFILE})


#定义遍历make函数
define make_target 
	@for DIR in $(TARGET); do \
		make -C ${THIS_MAKEFILE_DIR}/$$DIR $1 -j4 || break; \
	done
endef

all:
	$(call make_target, all)
	@echo -e '\n\e[36m\e[1m|||||===== Build All Successfully =====|||||\e[0m\n'

clean:
	$(call make_target, clean)
