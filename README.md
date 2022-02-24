[Scenario]
1,本软件由C语言开发,适用于具有二维关系模式的数据构造
2,功能:可批量生成自然数、定长字段、前补、后补、起始字符串等等
3,可配合shell脚本实现多并发,尤其适合高性能大数据量要求的场景
4,支持命令行参数(config.ini中所有参数均可转化),使用灵活
---
[Structure]
1,bin目录:编译后二进制可执行文件makedata.out,及配置文件config.ini与data.cnf,config.ini为全局配置文件,data.cnf为字段配置文件
2,src目录:C源代码
3,soft目录:本软件需要的安装包
4,test目录:用于测试
5,README.md:说明文档

[Install]
# 非Oracle环境,可以不执行以下命令
# Red Hat Enterprise Linux Server release 7.4 (Maipo)测试通过
$ su - root
$ yum -y install automake libtool
$ cd soft
$ tar -zxvf ocilib-4.6.2.tar.gz
$ cd ocilib-4.6.2
$ chmod +x configure
# 根据实际环境修改路径参数
$ ./configure --with-oracle-lib-path=/oracle/product/12.2.0/db/lib --with-oracle-headers-path=/oracle/product/12.2.0/db/rdbms/public
$ autoreconf -ivf
$ make
$ make install
$ echo /usr/local/lib >> /etc/ld.so.conf
$ ldconfig

[Usage]
# 所有配置文件均以空行结尾
# config.ini说明详见bin目录下config.ini
# data.cnf说明:
字段名称以英文字母开头
字段类型:(id/array/file)
NVL:填入值为空
1.id类型:id|NVL,0,0,0/id|head,6,-10,
共四个参数,含义分别为:字段固定值部分,字段值长度(不包含补足部分),字段总长度(包含补足部分,正数在值前补足,负数在值后补足),占位符(补足时使用0或空格等)
2.array类型:array|-10, ,5,z1,z2,NVL,z4,z5/array|-10, ,5,z1,z2,z3,z4,z5,r
共五个参数,含义分别为:字段总长度(包含补足部分,正数在值前补足,负数在值后补足),占位符(补足时使用0或空格等),可取值的个数(n),可取值的内容(Z1-Zn,整体视为一个参数),"r"代表随机，为可选项，且必须放置在末尾
3.file类型:file|assetacc,-20, ,2,3,5/file|assetacc,-20, ,2,3,5,r
共七个参数,含义分别为:从文件assetacc取值,字段总长度(包含补足部分,正数在值前补足,负数在值后补足),占位符(补足时使用0或空格等),从第N1行开始取,取第N2列(以","分隔判断列),循环取前N3行,"r"代表随机,为可选项,且必须放置在末尾

实例:
data.cnf文件内容：
zhiduanming01:id|NVL,0,0,0                    代表自增型自然数:1,2,3,...
zhiduanming02:id|head,6,-10,                  代表字段固定值部分为head,字段值长度为6,字段总长度为10,字段值长度不足在字段值的后面使用空格补足.
zhiduanming03:array|-10, ,5,z1,z2,NVL,z4,z5   代表字段总长度为10,字段值长度不足在字段值的后面使用空格补足,顺序取值5个:z1-z5,其中NVL代表取值为空.
zhiduanming04:array|-10, ,5,z1,z2,z3,z4,z5,r  代表字段总长度为10,字段值长度不足在字段值的后面使用空格补足,随机取值5个:z1-z5.
zhuduanming05:file|assetacc,-20, ,2,3,5       代表字段从文件assetacc中取值,字段总长度为20,字段值长度不足在字段值的后面使用空格补足,从第2行开始取,取第3列(以","分隔判断列),循环顺序取前5行.
zhuduanming06:file|assetacc,-20, ,2,3,5,r     代表字段从文件assetacc中取值,字段总长度为20,字段值长度不足在字段值的后面使用空格补足,从第2行开始取,取第3列(以","分隔判断列),随机取前5行.

文件assetacc内容(前10行):
param01_line01,param02_line01,param03_line01
param01_line02,param02_line02,param03_line02
param01_line03,param02_line03,param03_line03
param01_line04,param02_line04,param03_line04
param01_line05,param02_line05,param03_line05
param01_line06,param02_line06,param03_line06
param01_line07,param02_line07,param03_line07
param01_line08,param02_line08,param03_line08
param01_line09,param02_line09,param03_line09
param01_line10,param02_line10,param03_line10

生成数据文件内容(前7行):
1,head01    ,z1        ,z4        ,param03_line02        ,param03_line03
2,head02    ,z2        ,z5        ,param03_line03        ,param03_line04
3,head03    ,          ,z2        ,param03_line04        ,param03_line02
4,head04    ,z4        ,z4        ,param03_line05        ,param03_line05
5,head05    ,z5        ,z1        ,param03_line06        ,param03_line06
6,head06    ,z1        ,z5        ,param03_line02        ,param03_line04
7,head07    ,z2        ,z3        ,param03_line03        ,param03_line03
# 编译(非Oracle环境):
$ cd bin
$ chmod +x ../src/compile.sh
$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=non-oracle/' ../src/compile.sh
$ ../src/compile.sh
# 编译(Oracle环境):
$ cd bin
$ chmod +x ../src/compile.sh
$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=oracle/' ../src/compile.sh
$ ../src/compile.sh
# 执行(非Oracle环境):
$ cd bin
$ ./makedata.out
or
$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718
# 执行(Oracle环境):
$ export LD_LIBRARY_PATH=$ORACLE_HOME/lib
$ cd bin
$ ./makedata.out
or
$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718

[FAQ]
1,报错"Segmentation fault (core dumped)",首先检查config.ini文件,是否相关长度配置不足,其次检查data.cnf,是否按要求配置相关参数、参数值是否合法,设置的起始值或数量是否越界

[License]
Copyright@2019
