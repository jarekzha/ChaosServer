#!/bin/bash

# 部分安装在VMware的共享目录中执行时，编译会报错

# env param
BOOST=boost_1_59_0
PROTOBUF=protobuf-2.5.0
FCGI_TAR=fcgi-2.4.1.tar.gz
FCGI_UNTAR=fcgi-2.4.1-SNAP-0311112127
MYSQL_CONNECTOR_CPP=mysql-connector-c++-1.1.1
ZERO_MQ=zeromq-4.0.4

# inner param
CUR_PATH=$(cd `dirname $0`; pwd)

# ================================================
# setup boost
function_setup_boost()
{
	yum install -y python.x86_64
	cd ${CUR_PATH}
	rm -rf ${CUR_PATH}/${BOOST}
	tar -zvxf ${CUR_PATH}/${BOOST}.tar.gz
	
	cd ${CUR_PATH}/${BOOST}
	./bootstrap.sh
	./b2 install
	ldconfig
	
	rm -rf ${CUR_PATH}/${BOOST}
}

# setup protobuf
function_setup_protobuf()
{
	cd ${CUR_PATH}
	rm -rf ${CUR_PATH}/${PROTOBUF}
	tar -zvxf ${CUR_PATH}/${PROTOBUF}.tar.gz 
	cd ${CUR_PATH}/${PROTOBUF}
	
	./configure && make && make install && ldconfig
	
	rm -rf ${CUR_PATH}/${PROTOBUF}
}

# setup fcgi
function_setup_fcgi()
{
	cd ${CUR_PATH}
	rm -rf ${CUR_PATH}/${FCGI_UNTAR}
	tar -zvxf ${CUR_PATH}/${FCGI_TAR}
	cd ${CUR_PATH}/${FCGI_UNTAR}
	
	./configure && make && make install && ldconfig
	
	rm -rf ${CUR_PATH}/${FCGI_UNTAR}
}

# setup mysqlconnector
function_setup_mysqlconnector()
{
	cd ${CUR_PATH}
	rm -rf ${CUR_PATH}/${MYSQL_CONNECTOR_CPP}
	tar -zvxf ${CUR_PATH}/${MYSQL_CONNECTOR_CPP}.tar.gz 
	cd ${CUR_PATH}/${MYSQL_CONNECTOR_CPP}
	
	cmake . && make && make install && ldconfig && rm -rf ${CUR_PATH}/${MYSQL_CONNECTOR_CPP}
}

# setup zero mq
function_setup_zeromq()
{
	cd ${CUR_PATH}
	rm -rf ${CUR_PATH}/${ZERO_MQ}
	tar -zvxf ${CUR_PATH}/${ZERO_MQ}.tar.gz
	cd ${CUR_PATH}/${ZERO_MQ}
	
	./configure && make && make install && ldconfig
	
	rm -rf ${CUR_PATH}/${ZERO_MQ}	
}

function_setup_other()
{
	yum -y install openldap-devel.x86_64
}

# ================================================
case "$1" in
    protobuf )
        function_setup_protobuf
		;;
    fcgi )
        function_setup_fcgi
		;;
	boost )
		function_setup_boost
		;;
	mysqlconnector )
		function_setup_mysqlconnector
		;; 
	zeromq )
		function_setup_zeromq
		;;
	all )
		function_setup_other
		function_setup_protobuf
		function_setup_fcgi
		function_setup_boost
		function_setup_mysqlconnector
		function_setup_zeromq
		;;
	* )
		echo "$0 all|protobuf|fcgi|boost|mysqlconnector|zeromq"
esac



