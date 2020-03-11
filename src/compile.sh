#!/bin/bash

cd $(echo $0 | sed 's/\(.*\/\)[^\/]\+/\1/')
export LD_LIBRARY_PATH=$ORACLE_HOME/lib

type=non-oracle
if [ $type == 'oracle' ]
then
	sed -i -rz 's/[^\n]*(#include "oracle.h")/\1/' lib/bus.c
    sed -i -rz 's/([\ ]*)[\/]*(oraclequery\(\);)/\1\2/' lib/bus.c
	gcc -g -std=c99 -o ../bin/makedata.out makedata.c lib/*.c -I/usr/local/include -I$ORACLE_HOME/rdbms/public -L$ORACLE_HOME/lib -L/usr/local/lib -locilib
else
	sed -i -rz 's/[^\n]*(#include "oracle.h")/\/\/\1/' lib/bus.c
	sed -i -rz 's/([\ ]*)[\/]*(oraclequery\(\);)/\/\/\1\2/' lib/bus.c
	gcc -g -std=c99 -o ../bin/makedata.out makedata.c lib/bus.c lib/helper.c lib/model.c
fi

