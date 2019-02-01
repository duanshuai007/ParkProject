#!/bin/bash

if [ $# -lt 2 ]
then
    echo "./gettime.sh [starttime] [endtime]"
    exit
fi

starttime=$1
endtime=$2

#day=`date -d "1 days ago " "+%Y%m%d"`
#year=`expr substr ${day} 1 4`
#month=`expr substr ${day} 5 2`
#s_date=$year"0101"
#
#if [ "$month" == "01" ] || [ "$month" == "02" ] || [ "$month" == "03" ]
#then
#    s_date=$year"0101"
#elif [ "$month" == "04" ] || [ "$month" == "05" ] || [ "$month" == "06" ]
#then
#    s_date=$year"0401"
#elif [ "$month" == "07" ] || [ "$month" == "08" ] || [ "$month" == "09" ]
#then
#    s_date=$year"0701"
#elif [ "$month" == "10" ] || [ "$month" == "11" ] || [ "$month" == "12" ]
#then
#    s_date=$year"1001"
#fi
#
#e_date=$day
#
#sys_s_data=`date -d  "$s_date" +%s`
#sys_e_data=`date -d   "$e_date" +%s`

#interval=`expr $sys_e_data - $sys_s_data`
#daycount=`expr $interval / 3600 / 24 + 1`

echo "starttime=${starttime},endtime=${endtime}"

starttime_sec=`date -d "${starttime}" +%s`
endtime_sec=`date -d "${endtime}" +%s`

echo "startsec=${starttime_sec},endsec=${endtime_sec}"

interval=`expr ${endtime_sec} - ${starttime_sec}`
#计算秒数对应的分钟数
mincount=`expr $interval / 60`

echo $mincount
