#ifndef _PARK_MYSQL_H_
#define _PARK_MYSQL_H_

#define NUMBER          "u32Number"
#define NUMBER_ATTR     "int(4) unsigned"
#define PARKNUMBER      "u8ParkNumber"
#define PARKNUMBER_ATTR "int(1) unsigned default 0"
#define BILLVERSION     "u8BillingVersion"
#define BILLVErSION_ATTR "int(1) unsigned default 0"
#define COLOR           "c3Color"
#define COLOR_ATTR      "char(3) default 'NN'"
#define PLATE           "c12Plate"
#define PLATE_ATTR      "char(12) default 'NULL'"
#define INTIME          "InTime"
#define INTIME_ATTR     "datetime"
#define OUTTIME         "OutTime"
#define OUTTIME_ATTR    "datetime"
#define PARKTIME        "u32CanParkTime"
#define PARKTIME_ATTR   "int(4) unsigned default 0"
#define ISVIP           "cIsVIP"
#define ISVIP_ATTR      "char(1) default 'N'"
#define ISLEAVE         "cIsLeave"
#define ISLEAVE_ATTR    "char(1) default 'N'"
#define ALREADYPAY      "u8AlreadyPay"
#define ALREADYPAY_ATTR "int(1) unsigned default 0"


//每个表内最大的信息数量
#define TABLE_MAX_NUMBER    5
//交费之后不计入收费的离开时间，即在此时间内离开
#define MAX_LEAVETIME       15

#endif
