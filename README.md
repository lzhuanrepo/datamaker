## Introduction
* 本软件由C语言开发，适用于具有二维关系模式的数据构造。
* 功能：可批量生成自然数、定长字段、前补、后补、起始字符串等等。
* 可配合shell脚本实现多并发，尤其适合高性能大数据量要求的场景。
* 支持命令行参数（config.ini中所有参数均可转化为命令行参数），使用灵活。

## Structure
* bin目录：编译后二进制可执行文件为makedata.out，配置文件config.ini、data.cnf、config.ini为全局配置文件，data.cnf为字段配置文件。
* src目录：C源代码。
* soft目录：本软件需要的安装包。
* test目录：用于测试。
* README.md ：说明文档。

## Install
>非Oracle环境，可以不执行以下命令<br />
>Red Hat Enterprise Linux Server release 7.4 (Maipo)测试通过

`$ su - root`<br />
`$ yum -y install automake libtool`<br />
`$ cd soft`<br />
`$ tar -zxvf ocilib-4.6.2.tar.gz`<br />
`$ cd ocilib-4.6.2`<br />
`$ chmod +x configure`<br />
>根据实际环境修改路径参数

`$ ./configure --with-oracle-lib-path=/oracle/product/12.2.0/db/lib --with-oracle-headers-path=**/oracle/product/12.2.0/db/rdbms/public**`<br />
`$ autoreconf -ivf`<br />
`$ make`<br />
`$ make install`<br />
`$ echo /usr/local/lib >> /etc/ld.so.conf`<br />
`$ ldconfig`<br />

## Usage
>所有配置文件均以空行结尾
>config.ini说明详见bin目录下config.ini
>以下为data.cnf说明

* 字段名称以英文字母开头
* 字段类型: id | array | file
* NVL: 填入值为空
* id类型: id|NVL,0,0,0/id|head,6,-10,
>共4个参数，含义分别为：字段固定值部分、字段值长度(不包含补足部分)、字段总长度(包含补足部分，正数在值前补足，负数在值后补足)、占位符(补足时使用0或空格等)
* array类型: array|-10, ,5,z1,z2,NVL,z4,z5/array|-10, ,5,z1,z2,z3,z4,z5,r
>共5个参数，含义分别为：字段总长度(包含补足部分，正数在值前补足，负数在值后补足)、占位符(补足时使用0或空格等)、可取值的个数(n)、可取值的内容(Z1-Zn、整体视为一个参数)。"r"代表随机，为可选项，且必须放置在末尾
* file类型: file|assetacc,-20, ,2,3,5/file|assetacc,-20, ,2,3,5,r
>共7个参数，含义分别为：从文件assetacc取值、字段总长度(包含补足部分，正数在值前补足，负数在值后补足)、占位符(补足时使用0或空格等)、从第N1行开始取、取第N2列(以","分隔)、循环取前N3行。"r"代表随机，为可选项，且必须放置在末尾

## Example
data.cnf文件内容：

`field1:id|NVL,0,0,0`<br />
>代表自增型自然数:1,2,3,....

`field2:id|head,6,-10,`<br />
>代表字段固定值部分为head,字段值长度为6,字段总长度为10,字段值长度不足在字段值的后面使用空格补足.
>
`field3:array|-10, ,5,z1,z2,NVL,z4,z5`<br />
>代表字段总长度为10,字段值长度不足在字段值的后面使用空格补足,顺序取值5个:z1-z5,其中NVL代表取值为空.
>
`field4:array|-10, ,5,z1,z2,z3,z4,z5,r`<br />
>代表字段总长度为10,字段值长度不足在字段值的后面使用空格补足,随机取值5个:z1-z5.
>
`field5:file|assetacc,-20, ,2,3,5`<br />
> 代表字段从文件assetacc中取值,字段总长度为20,字段值长度不足在字段值的后面使用空格补足,从第2行开始取,取第3列(以","分隔判断列),循环顺序取前5行.
> 
`field6:file|assetacc,-20, ,2,3,5,r`<br />
>代表字段从文件assetacc中取值,字段总长度为20,字段值长度不足在字段值的后面使用空格补足,从第2行开始取,取第3列(以","分隔判断列),随机取前5行.

assetacc文件内容:<br />
`field1_line01,field2_line01,field3_line01`<br />
`field1_line02,field2_line02,field3_line02`<br />
`field1_line03,field2_line03,field3_line03`<br />
`field1_line04,field2_line04,field3_line04`<br />
`field1_line05,field2_line05,field3_line05`<br />
`field1_line06,field2_line06,field3_line06`<br />
`field1_line07,field2_line07,field3_line07`<br />
`field1_line08,field2_line08,field3_line08`<br />
`field1_line09,field2_line09,field3_line09`<br />
`field1_line10,field2_line10,field3_line10`

生成数据文件内容(前7行):<br />
`1,head01    ,z1        ,z4        ,field3_line02        ,field3_line03`<br />
`2,head02    ,z2        ,z5        ,field3_line03        ,field3_line04`<br />
`3,head03    ,          ,z2        ,field3_line04        ,field3_line02`<br />
`4,head04    ,z4        ,z4        ,field3_line05        ,field3_line05`<br />
`5,head05    ,z5        ,z1        ,field3_line06        ,field3_line06`<br />
`6,head06    ,z1        ,z5        ,field3_line02        ,field3_line04`<br />
`7,head07    ,z2        ,z3        ,field3_line03        ,field3_line03`<br />
## 编译(非Oracle环境):
`$ cd bin`<br />
`$ chmod +x ../src/compile.sh`<br />
`$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=non-oracle/' ../src/compile.sh`<br />
`$ ../src/compile.sh`<br />
## 编译(Oracle环境):
`$ cd bin`<br />
`$ chmod +x ../src/compile.sh`<br />
`$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=oracle/' ../src/compile.sh`<br />
`$ ../src/compile.sh`<br />
## 执行(非Oracle环境):
`$ cd bin`<br />
`$ ./makedata.out`<br />
>or

`$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718`<br />
## 执行(Oracle环境):
`$ export LD_LIBRARY_PATH=$ORACLE_HOME/lib`<br />
`$ cd bin`<br />
`$ ./makedata.out`<br />
>or
>
`$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718`

## FAQ
报错"Segmentation fault (core dumped)"：首先检查config.ini文件，是否相关长度配置不足，其次检查data.cnf，是否按要求配置相关参数、参数值是否合法，设置的起始值或数量是否越界。

## Support
QQ: 1842344861

## License
Copyright@2022
