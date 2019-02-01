#!/bin/sh

HOSTNAME="localhost"
PORT="0"
USERNAME="root"
PASSWORD="frogshealth"
DBNAME="waming_park_db"
TABLENAME="park01"
BILLINGTABLE="billing"

PREFIX="mysql -h${HOSTNAME}  -P${PORT}  -u${USERNAME} -p${PASSWORD}"

cmd=$1
curdb=$2
para=$3

if [ $# -eq 0 ]
then
    echo "Usage: ./script [cmd]"
    echo "          [create]:create database and table park01"
    echo "          [insert (Table name)]:insert 1 message into this table"
    echo "          [select (Table name)]:display table"
    echo "          [show (Table name)]:display some message"
    exit
fi

if [ ${cmd} = "create" ]
then
#创建数据库
    sql="create database IF NOT EXISTS ${DBNAME} default charset gbk"
    res=`${PREFIX} -e "${sql}" 2>&1`

    #echo ${res}
    echo ${res} | grep "ERROR"
    if [ $? -eq 0 ]
    then
        echo "create database[${DBNAME}] failed"
        exit
    fi

    #创建表
    sql="create table ${TABLENAME}(\
         u32Number int(4) unsigned primary key auto_increment,\
         u8ParkNumber int(1) unsigned default 0,\
         u8BillingVersion int(1) unsigned default 0,\
         c3Color char(3) default 'NN',\
         c12Plate char(12) default 'NULL',\
         InTime datetime,\
         OutTime datetime,\
         u32CanParkTime int(4) unsigned default 0,\
         cIsVIP char(1) default 'N',\
         cIsLeave char(1) default 'N',\
         u8AlreadyPay int(1) unsigned default 0)"

    res=`${PREFIX} ${DBNAME} -e "${sql}" 2>&1`

    echo ${res}
    echo ${res} | grep "ERROR" > /dev/null
    if [ $? -ne 0 ]
    then
        #为了支持中文，需要设置为gbk编码
        #    set_zifuji="alter table ${TABLENAME} change Color Color char(4) character set gbk"
        #    ${PREFIX} ${DBNAME} -e "${set_zifuji}" 2>&1

        #设置普通索引
#sql="alter table ${TABLENAME} add index SPLATE (c20Plate)"
#res=`${PREFIX} ${DBNAME} -e "${sql}" 2>&1`

#echo ${res}
#echo ${res} | grep "ERROR" > /dev/null
#if [ $? -eq 0 ]
#then
#echo "set index failed\r\n"
#exit
#fi
#创建组合索引
        sql="alter table ${TABLENAME} add index color_plate(c3Color(2),c12Plate(12))"
        res=`${PREFIX} ${DBNAME} -e "${sql}" 2>&1`

#echo ${res}
        echo ${res} | grep "ERROR" > /dev/null
        if [ $? -eq 0 ]
        then
            echo "set zuhe index failed\r\n"
            exit
        fi

        echo "table[${TABLENAME} create success]"
    else
        echo ${res} | grep "already exists" > /dev/null
        if [ $? -eq 0 ]
        then 
            echo table[${TABLENAME}] exists
        fi
        echo "create table[${TABLENAME}] failed"
        exit
    fi 

    #创建收费表
    sql="create table ${BILLINGTABLE}(\
         u8Version int(1) unsigned primary key auto_increment,\
         u8FreeTime int(1) unsigned default 0,\
         u8TimingCycle int(1) unsigned default 0,\
         u8Price int(1) unsigned default 0,\
         u16MaxTiming int(2) unsigned default 0,\
         u8VIPDiscount int(1) unsigned default 0)"

     res=`${PREFIX} ${DBNAME} -e "${sql}" 2>&1`

     echo ${res} | grep "ERROR" > /dev/null
     if [ $? -eq 0 ]
     then
        echo ${res} | grep "already exists" > /dev/null
        if [ $? -eq 0 ]
        then
            echo "table [${BILLINGTABLE}] exists"
            echo "create table[${BILLINGTABLE}] failed"
            exit
        fi
     else
        echo "create table[${BILLINGTABLE}] success"
     fi

    sql="create table table_qianfei(\
          u32Number int(4) unsigned primary key auto_increment,\
          c12Plate char(12) default 'NULL',\
          c3Color char(3) default 'NN',\
          c20TableName char(20) default 'NULL',\
          u32NumberInTable int(4) unsigned default 0)"

    res=`${PREFIX} ${DBNAME} -e "${sql}" 2>&1`

    echo ${res} | grep "ERROR" > /dev/null
    if [ $? -eq 0 ]
    then
        echo ${res} | grep "already exists" > /dev/null
        if [ $? -eq 0 ]
        then
            echo "table [table_qianfei] exists"
            echo "create table[table_qianfei] failed"
            exit
        fi
    else
        echo "create table[table_qianfei] success"
    fi

elif [ ${cmd} = "insert" ]
then
#插入数据
    if [ $# -lt 2 ] 
    then
        echo "  参数太少"
        echo "  insert [表名]"
        exit
    fi
    insert_sql="insert into ${curdb}(ParkNumber, Plate, InTime, IsCharge) values(1,'辽A12349',1901231754, 'N')"
    ${PREFIX} ${DBNAME} -e "${insert_sql}" 2>&1
elif [ ${cmd} = "select" ]
then
#查询
    if [ $# -lt 2 ] 
    then
        echo "  参数太少"
        echo "  select [表名]"
        exit
    fi
    select_sql="select * from ${curdb}"
    ${PREFIX} ${DBNAME} -e "${select_sql}" 2>&1
elif [ ${cmd} = "show" ]
then
#测试
    if [ $# -lt 2 ] 
    then
        echo "  参数太少"
        echo "  show [表名]"
        exit
    fi

    sql="show databases"
    res=`${PREFIX}  -e "${sql}"`
    echo ${res} | grep ${DBNAME}
    if [ $? -eq 0 ]
    then
        sql="use ${DBNAME}"
        ${PREFIX} ${DBNAME} -e "${sql}" 2>&1
        sql="show tables"
        ${PREFIX} ${DBNAME} -e "${sql}" 2>&1
        sql="select * from ${curdb}"
        ${PREFIX} ${DBNAME} -e "${sql}" 2>&1
    fi
fi

