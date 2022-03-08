## Introduction
* This software is developed by C language and is suitable for data structure with two-dimensional relational schema.
* Function: It can generate natural numbers, fixed-length fields, pre-complement, post-complement, starting string, etc. in batches.
* It can cooperate with shell script to achieve multi-concurrency, especially suitable for scenarios with high performance and large data volume requirements.
* Support command line parameters (all parameters in config.ini can be converted into command line parameters), flexible use.

## Structure
* bin directory: The compiled binary executable file is makedata.out. The configuration files are config.ini and data.cnf. config.ini is the global configuration files, and data.cnf is the field configuration file.
* src directory: C source code.
* soft directory: the installation package required by this software.
* test directory: for testing.
* README.md : Documentation.

## Install
>In a non-Oracle environment, you can do not execute the following commands.<br />
>Red Hat Enterprise Linux Server release 7.4 (Maipo) test passed.

`$ su - root`<br />
`$ yum -y install automake libtool`<br />
`$ cd soft`<br />
`$ tar -zxvf ocilib-4.6.2.tar.gz`<br />
`$ cd ocilib-4.6.2`<br />
`$ chmod +x configure`<br />
>Modify the path parameters according to the actual environment.

`$ ./configure --with-oracle-lib-path=/oracle/product/12.2.0/db/lib --with-oracle-headers-path=/oracle/product/12.2.0/db/rdbms/public`<br />
`$ autoreconf -ivf`<br />
`$ make`<br />
`$ make install`<br />
`$ echo /usr/local/lib >> /etc/ld.so.conf`<br />
`$ ldconfig`<br />

## Usage
>all configuration files are terminated with blank lines
>config.ini description, see config.ini in the bin directory
>The following is the description of data.cnf

* Field names start with English letters
* Field type: id | array | file
* NVL: Filled value is empty
* id type: id|NVL,0,0,0/id|head,6,-10,
>A total of 4 parameters, the meanings are: field fixed value part, field value length (excluding supplementary part), total field length (including supplementary part, positive numbers are supplemented before the value, negative numbers are supplemented after the value), placeholder (Use 0 or space when making up, etc.)
* array type: array|-10, ,5,z1,z2,NVL,z4,z5/array|-10, ,5,z1,z2,z3,z4,z5,r
>A total of 5 parameters, the meanings are: the total length of the field (including the supplementary part, positive numbers are supplemented before the value, and negative numbers are supplemented after the value), placeholder (use 0 or space, etc. when supplementing), the number of possible values Number (n), content of possible values (z1-zn, the whole is regarded as a parameter). "r" stands for random, is optional, and must be placed at the end.
* file type: file|assetacc,-20, ,2,3,5/file|assetacc,-20, ,2,3,5,r
>A total of 7 parameters, the meanings are: the value from the file assetacc, the total length of the field (including the supplementary part, positive numbers are supplemented before the value, and negative numbers are supplemented after the value), placeholder (use 0 or space, etc. when supplementing) , Start from row N1, take column N2 (separated by ","), and loop to take the first N3 rows. "r" stands for random, is optional, and must be placed at the end.

## Example
data.cnf file content:

`field1:id|NVL,0,0,0`<br />
> Represents an auto-incrementing natural number: 1,2,3,....

`field2:id|head,6,-10,`<br />
> Represents that the fixed value part of the field is head, the length of the field value is 6, the total length of the field is 10, and the length of the field value is insufficient, and the space is used to fill in the back of the field value.
>
`field3:array|-10, ,5,z1,z2,NVL,z4,z5`<br />
> Represents that the total length of the field is 10. If the length of the field value is insufficient, use a space after the field value to make up for it. The sequence takes 5 values: z1-z5, where NVL represents the value is empty.
>
`field4:array|-10, ,5,z1,z2,z3,z4,z5,r`<br />
> Represents that the total length of the field is 10, the length of the field value is insufficient, and the space is used to make up after the field value, and 5 values ​​are randomly selected: z1-z5.
>
`field5:file|assetacc,-20, ,2,3,5`<br />
> The representative field is taken from the file assetacc. The total length of the field is 20. If the length of the field value is insufficient, use a space after the field value. Take the first 5 rows in loop order.
> 
`field6:file|assetacc,-20, ,2,3,5,r`<br />
> The representative field takes the value from the file assetacc, the total length of the field is 20, and the field value is insufficient, use a space after the field value to make up, start from the second row, take the third column (separate the judgment column with ","), Randomly take the first 5 rows.

assetacc file content:<br />
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

Generate data file content (first 7 lines):<br />
`1,head01    ,z1        ,z4        ,field3_line02        ,field3_line03`<br />
`2,head02    ,z2        ,z5        ,field3_line03        ,field3_line04`<br />
`3,head03    ,          ,z2        ,field3_line04        ,field3_line02`<br />
`4,head04    ,z4        ,z4        ,field3_line05        ,field3_line05`<br />
`5,head05    ,z5        ,z1        ,field3_line06        ,field3_line06`<br />
`6,head06    ,z1        ,z5        ,field3_line02        ,field3_line04`<br />
`7,head07    ,z2        ,z3        ,field3_line03        ,field3_line03`<br />
## Compile (non-Oracle environment):
`$ cd bin`<br />
`$ chmod +x ../src/compile.sh`<br />
`$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=non-oracle/' ../src/compile.sh`<br />
`$ ../src/compile.sh`<br />
## Compile (Oracle environment):
`$ cd bin`<br />
`$ chmod +x ../src/compile.sh`<br />
`$ sed -i -rz 's/[^\n]*(type=[^\n]*)/type=oracle/' ../src/compile.sh`<br />
`$ ../src/compile.sh`<br />
## Execute (non-Oracle environment):
`$ cd bin`<br />
`$ ./makedata.out`<br />
>or

`$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718`<br />
## Execute (Oracle environment):
`$ export LD_LIBRARY_PATH=$ORACLE_HOME/lib`<br />
`$ cd bin`<br />
`$ ./makedata.out`<br />
>or
>
`$ ./makedata.out -reccnt=100 -cfgpath=./data.cnf -outname=tsa-fund-buy-check-20190718`

## FAQ
Error "Segmentation fault (core dumped)": First check the config.ini file to see if the relevant length is insufficiently configured, and then check the data.cnf, whether the relevant parameters are configured as required, whether the parameter values are legal, and whether the set starting value or quantity is out of bounds.

## Support
QQ: 1842344861

## License
Copyright@2022
