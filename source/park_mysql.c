#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#include "msgqueue.h"
#include "billing.h"
#include "park_mysql.h"

typedef struct {
    char database_name[20];
    char table_park_name[30];       //���µı�
    char table_qianfei[30];        //��ʱ��û���뿪�ı�
    char table_billing_name[30];
    char server[20];
    char user[20];
    char password[20];
} MySqlCOMMENT;

typedef struct {
    char tablename[30];
    unsigned int no;
    unsigned int qianfei_no;
} ItemInTable;

#define FREEITEM            "FreeItem"

//��ȡ��Ӧ�汾�ļƷѹ���
//��version=0ʱ����ȡ���µļƷѷ�ʽ
static int getBilling(MYSQL *mysql, int version, _strBilling *pBill)
{
    char sql[128] = {0};
    MYSQL_RES *res;
    MYSQL_ROW row;
    char tbbilling[] = "billing";

    if (version == 0) {    
        sprintf(sql, "select * from %s where u8Version=(select max(u8Version) from %s)", tbbilling, tbbilling);
    } else {
        sprintf(sql, "select * from %s where u8Version=%d", tbbilling, version);
    }

    if (!mysql_real_query(mysql, sql, strlen(sql))) {
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);
        if (row) {
            pBill->u8Version = atoi(row[0]);
            pBill->u8FreeTime = atoi(row[1]);
            pBill->u8TimingCycle = atoi(row[2]);
            pBill->u8Price = atoi(row[3]);
            pBill->u16MaxTiming = atoi(row[4]);
            pBill->u8VIPDiscount = atoi(row[5]);

            mysql_free_result(res);
            return 0;
        }
    } else {
        printf("read table[%s] error[%s]\r\n", tbbilling, mysql_error(mysql));
    }

    mysql_free_result(res);
    return -1;
}

/*
 *  ���Ƶ�ǰ���е�Ƿ�Ѽ�¼��Ƿ�ѱ���
 */
static void copy_nochargeitem_to_qianfeitable(MYSQL *mysql, MySqlCOMMENT *mysqlcomm)
{
    char sql[128] = {0};
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(sql, "select %s,%s,%s from %s where %s='N'",
            PLATE, COLOR, NUMBER, mysqlcomm->table_park_name, ISLEAVE);

    if (!mysql_real_query(mysql, sql, strlen(sql))) {
        res = mysql_store_result(mysql);
        while(row = mysql_fetch_row(res)) {
            //for ( int i = 0; i < mysql_num_fields(res); i++) {
            //    printf("%s ", row[i]);
            //}
            //printf("\r\n");
#if 1
            //����Ϣд�뵽Ƿ�ѱ���
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "insert into %s(c12Plate,c3Color,c20TableName,u32NumberInTable) values('%s','%s','%s',%d)",
                    mysqlcomm->table_qianfei, row[0], row[1], mysqlcomm->table_park_name, atoi(row[2]));
            //printf("sql=%s\r\n", sql);
            if (!mysql_real_query(mysql, sql, strlen(sql))) {
                //add success
                //printf("add to qianfei table success\r\n");
            } else {
                //add failed
                printf("add to qianfei table failed,error=%s\r\n", mysql_error(mysql));
            }
#endif
        }
    } else {
        printf("%s:%s\r\n", __func__, mysql_error(mysql));
    }
    
    mysql_free_result(res);
}


/*
 *      ������������Ϣ�������
 **/
static void do_work_when_car_in(MYSQL *mysql, MySqlCOMMENT *mysqlcomm, _strPlateMsg *ptrPlate, _strBilling *ptrBilling) 
{
    char sql[1024] = {0};
    
    int parknumber = 10;
    char isVIP = 'N';
    //����ǽ���
    printf("ʻ�룡\r\n");

    printf("rec:%s:%s\r\n", ptrPlate->c3Color, ptrPlate->c16Plate);
    printf("rec time:%d-%d-%d %d:%d\r\n", ptrPlate->time.year, ptrPlate->time.mon, ptrPlate->time.day,\
            ptrPlate->time.hour, ptrPlate->time.min);

    char timebuf[20] = {0};

    sprintf(timebuf, "%d-%d-%d %d:%d:%d", ptrPlate->time.year, ptrPlate->time.mon, ptrPlate->time.day,
            ptrPlate->time.hour, ptrPlate->time.min, ptrPlate->time.sec);

    sprintf(sql, 
            "insert into %s(%s,%s,%s,%s,%s,%s,%s,%s) \
            values(%d,'%s','%s','%s','%c','N',0,%d)",
            mysqlcomm->table_park_name, PARKNUMBER , COLOR, PLATE, INTIME, ISVIP, ISLEAVE, ALREADYPAY, BILLVERSION,\
            parknumber, ptrPlate->c3Color, ptrPlate->c16Plate, timebuf, isVIP, ptrBilling->u8Version);

    //printf("sql=%s\r\n", sql);
    if (mysql_real_query(mysql, sql, strlen(sql))) {
        printf("Insert into failed:%s\r\n", mysql_error(mysql));
    }
}
/*
 *  ��鳵���Ƿ���Ƿ�ѱ�͵�ǰ���д���Ƿ����Ϣ
 *  ���ؽṹ�壬���������Ƿ����Ϣ���ṹ���е�tablename����ֵΪFREEITEM
 *  ���򣬱���ֵΪ���ڱ���
 */
static ItemInTable *check_plate_qianfei_item(MYSQL *mysql, MySqlCOMMENT *mysqlcomm,  _strPlateMsg *ptrPlate)
{
    char sql[256] = {0};
    MYSQL_RES *res;
    MYSQL_ROW row;

    ItemInTable *iit = (ItemInTable *)malloc(sizeof(ItemInTable));
    if(iit == NULL)
        return NULL;

    memset(iit, 0, sizeof(ItemInTable));

    sprintf(sql, "select c20TableName,u32NumberInTable,u32Number from %s where c12Plate='%s' and c3Color='%s'",
            mysqlcomm->table_qianfei, ptrPlate->c16Plate, ptrPlate->c3Color);    

    if (!mysql_real_query(mysql, sql, strlen(sql))) {
        //��Ƿ���з�����Ƿ�Ѽ�¼
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);
        if(row) {
            printf("��Ƿ�ѱ��з���Ƿ�Ѽ�¼\r\n");
            strcpy(iit->tablename, row[0]);
            iit->no = atoi(row[1]);
            iit->qianfei_no = atoi(row[2]);
            goto FUNC_EXIT;
        } else {
            //���û����������¼��ֱ��ȥ�·�ִ��
            goto FIND_IN_CURPARKTABLE;
        }
    } else {
        printf("%s:%s\r\n", __func__, mysql_error(mysql));
    }

FIND_IN_CURPARKTABLE:
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "select %s from %s where %s='%s' and %s='%s' and %s='N'",
            NUMBER, mysqlcomm->table_park_name, COLOR, ptrPlate->c3Color, PLATE, ptrPlate->c16Plate, ISLEAVE);

    //printf("��Ƿ�Ѽ�¼���������νɷ�\r\n");
    if ( !mysql_real_query(mysql, sql, strlen(sql)) ) {
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res); 
        if (row) {
            //�ҵ���Ƿ�ѵļ�¼,��ʾ����
            printf("�ڵ�ǰ���з���Ƿ�Ѽ�¼\r\n");
            strcpy(iit->tablename, mysqlcomm->table_park_name);
            iit->no = atoi(row[0]);
            iit->qianfei_no = 0;
        } else {
            strcpy(iit->tablename, FREEITEM); 
        }
    } else {
        printf("%s:%s\r\n", __func__, mysql_error(mysql));
    }

FUNC_EXIT:
    mysql_free_result(res);

    return iit;
}

/*
 *      �������ݿ��е�ͣ����Ϣ������ͣ��ʱ��
 *      �������ͣ��ʱ�䲻��0�����ۼӡ�
 */
static int do_work_charge_update_parktime(MYSQL *mysql, ItemInTable *iit, unsigned int time)
{
    char sql[128] ={0};
    MYSQL_RES *res;
    MYSQL_ROW row; 
    unsigned int parktime;

    sprintf(sql, "select %s from %s where %s=%d", 
            NUMBER, iit->tablename, NUMBER, iit->no);
    if (!mysql_real_query(mysql, sql, strlen(sql))) {
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);
        if (row) {
            parktime = atoi(row[0]);
                //������0˵������֮ǰ�н��ѵļ�¼
            memset(sql, 0, sizeof(sql));
            sprintf(sql, "update %s set %s=%d where %s=%d",
                    iit->tablename,
                    PARKTIME, (parktime + time),
                    NUMBER, iit->no);
            if (!mysql_real_query(mysql, sql, strlen(sql))) {
                return 0;
            }
            printf("[ERROR]%s:%s\r\n", __func__, mysql_error(mysql));
        }
    } else {
        printf("set charge failed:%s\r\n", mysql_error(mysql));
    }

    mysql_free_result(res);

    return -1;
}

/*
 *      �����뿪 
 *      �������ݿ��еĶ�Ӧ��Ϣ���뿪ʱ�䣬�����뿪��־ΪY��
 *      �����Ƿ�ѱ��д��ڶ�Ӧ�ļ�¼,��ɾ��
 */
static void update_parkinfo_in_table(MYSQL *mysql, MySqlCOMMENT *mysqlcomm, ItemInTable *iit, char *timebuf, unsigned int number)
{
    char sql[128] = {0};

    sprintf(sql, "update %s set %s='%s',%s='Y' where %s=%d",
            iit->tablename,
            OUTTIME, timebuf,
            ISLEAVE, 
            NUMBER, number);
    //printf("sql=%s\r\n", sql);
    if (mysql_real_query(mysql, sql, strlen(sql))) {
        printf("update failed:%s\r\n", mysql_error(mysql));
    }   

    if(iit->qianfei_no) {
        //�ѽ��ѣ���Ƿ�ѱ���ɾ��
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "delete from %s where u32Number=%d", mysqlcomm->table_qianfei, iit->qianfei_no);
        if (mysql_real_query(mysql, sql, strlen(sql))) {
            printf("update failed:%s\r\n", mysql_error(mysql));
        }   
    }  
}

/*
 *      �����뿪�Ĵ���
 *
 */
static void do_work_when_car_out(MYSQL *mysql, MySqlCOMMENT *mysqlcomm, _strPlateMsg *ptrPlate, ItemInTable *iit)
{
    char sql[256] = {0};
    MYSQL_RES *res;
    MYSQL_ROW row;
    struct tm tm_time;
    time_t totalSeconds;
    int last_time;
    int timeinterval;
    int canparktime;
    _strBilling tmpBilling;
    char timebuf[20];

    printf("ʻ��!\r\n");

    sprintf(sql, "select unix_timestamp(%s),%s,%s,%s,%s \
            from %s where %s=%d",
            INTIME, ISVIP, PARKTIME, NUMBER, BILLVERSION,
            iit->tablename, NUMBER, iit->no);

    //printf("sql=%s\r\n", sql);
    if ( !mysql_real_query(mysql, sql, strlen(sql)) ) {

        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);

        if ( row ) {
            //��������Ӧ��ͣ����Ϣ
            if (mysql_num_fields(res) == 5) {
                //0:���������ʱ���
                //1:�����Ƿ��ǻ�Ա
                //2:�����ܹ�ͣ���
                //3:��Ϣ���
                //4:�Ʒѹ���İ汾
                printf("%s--%s--%s--%s:%s\r\n", row[0], row[1], row[2],row[3],row[4]);
                //���泵����ȥʱ��ʱ�� 
                tm_time.tm_year = ptrPlate->time.year - 1900;
                tm_time.tm_mon = ptrPlate->time.mon - 1;
                tm_time.tm_mday = ptrPlate->time.day;
                tm_time.tm_hour = ptrPlate->time.hour;
                tm_time.tm_min = ptrPlate->time.min;
                tm_time.tm_sec = ptrPlate->time.sec;
                
                sprintf(timebuf, "%d-%d-%d %d:%d:%d", ptrPlate->time.year, ptrPlate->time.mon, ptrPlate->time.day,
                        ptrPlate->time.hour, ptrPlate->time.min, ptrPlate->time.sec);
                //��ȡͣ����ʱ���
                totalSeconds = mktime(&tm_time);
                //��ȡ��������ʱ��ʱ���
                last_time = atoi(row[0]);
                //��������ʱ���֮���ʱ������Ҳ���ǻ�ȡ��ʵ��ͣ����ʱ��,����60ת��Ϊ������
                timeinterval = difftime(totalSeconds, last_time)/60;
                //������ͣ����ʱ��
                canparktime = atoi(row[2]);
                //��ȡ�Ʒѹ���
                getBilling(mysql, atoi(row[4]), &tmpBilling);

                printf("canparktime=%d, time interval=%d\r\n", canparktime, timeinterval);
                if (( timeinterval < tmpBilling.u8FreeTime ) ||
                         (canparktime + MAX_LEAVETIME >= timeinterval)) {
                    //ʵ��ͣ��ʱ��С�����ʱ��������ȡ����ֱ�ӷ���
                    update_parkinfo_in_table(mysql, mysqlcomm, iit, timebuf, atoi(row[3]));
                    //OPEN DOOR
                } else if (canparktime < timeinterval) {
                    //����ͣ��ʱ��С��ʵ��ͣ��ʱ��,��Ҫ����
                    printf("�뽻��\r\n");
                    if ( timeinterval > tmpBilling.u16MaxTiming) {
                        //printf("------max time\r\n");
                        timeinterval = tmpBilling.u16MaxTiming;
                    }

                    int n1 = timeinterval / tmpBilling.u8TimingCycle;
                    int n2;
                    if (timeinterval % tmpBilling.u8TimingCycle) {
                        n2 = n1 + 1;
                    } else {
                        n2 = n1;
                    }
                    n2 *= tmpBilling.u8Price;
                    printf("��Ҫ�ɷ�%dԪ\r\n", n2);
                    
                    //���ͽ�������
                }
            }
        }
    } else {
        printf("error:%s\r\n", mysql_error(mysql));
    }
    
    mysql_free_result(res);
}

/*
 *      ����µļƷ���Ϣ���Ʒѱ���
 */
static void do_work_add_new_billing(MYSQL *mysql, MySqlCOMMENT *mysqlcomm, _strBilling *ptrBilling)
{
   char sql[128] = {0};

    sprintf(sql, "insert into %s(u8FreeTime, u8TimingCycle, u8Price, u16MaxTiming, u8VIPDiscount) \
            values(%d,%d,%d,%d,%d)", mysqlcomm->table_billing_name,
            ptrBilling->u8FreeTime, ptrBilling->u8TimingCycle, ptrBilling->u8Price, 
            ptrBilling->u16MaxTiming, ptrBilling->u8VIPDiscount);

    if ( !mysql_real_query(mysql, sql, strlen(sql)) ) {
        //��ӳɹ�
    } else {
        //���ʧ��
    }
}

/*
 *      ��ȡ���е������Ϣ���
 */
static unsigned int get_mysql_park_table_max_number(MYSQL *mysql, char *tablename)
{
    char sql[120] = {0};
    MYSQL_RES *res;
    MYSQL_ROW row;
    unsigned int ret = 0;

    sprintf(sql, "select %s from %s where %s=(select max(%s) from %s)", NUMBER, tablename, NUMBER, NUMBER, tablename);
    
    if ( !mysql_real_query(mysql, sql, strlen(sql)) ) {
        //�ɹ�
        res = mysql_store_result(mysql);
        row = mysql_fetch_row(res);

        if(row) {
            ret = atoi(row[0]);
        }
    } 

    mysql_free_result(res);

    return ret;
}

/*
 *      ����һ����
 */
static void create_mysql_table(MYSQL *mysql, char *name)
{
    char sql[512] = {0};

    sprintf(sql, "create table %s( u32Number int(4) unsigned primary key auto_increment,\
            u8ParkNumber int(1) unsigned default 0,\
            u8BillingVersion int(1) unsigned default 0,\
            c3Color char(3) default 'NN',\
            c12Plate char(12) default 'NULL',\
            InTime datetime,\
            OutTime datetime,\
            u32CanParkTime int(4) unsigned default 0,\
            cIsVIP char(1) default 'N',\
            cIsLeave char(1) default 'N',\
            u8AlreadyPay int(1) unsigned default 0)", name); //len>=386

    //sprintf(sql, "create table %s(u8Version int(1) unsigned primary key auto_increment,\
        u8FreeTime int(1) unsigned default 0,\
        u8TimingCycle int(1) unsigned default 0,\
        u8Price int(1) unsigned default 0,\
        u16MaxTiming int(2) unsigned default 0,\
        u8VIPDiscount int(1) unsigned default 0)", name);

    if ( !mysql_real_query(mysql, sql, strlen(sql)) ) {
        //��ӳɹ�
        printf("create table[%s] success\r\n", name);
        memset(sql, 0, sizeof(sql));
        sprintf(sql, "alter table %s add index color_plate(%s(2),%s(12))", name, COLOR, PLATE);
        if (!mysql_real_query(mysql, sql, strlen(sql))) {
            printf("index create success\r\n");
        } else {
            printf("index create failed,error=%s\r\n", mysql_error(mysql));
        }
    } else {
        //���ʧ��
        printf("create table[%s] failed, error:%s\r\n", name, mysql_error(mysql));
    }
}

int main(int argc, char *argv[])
{
    MYSQL *mysql = NULL;
    unsigned int port = 0;
    char *unix_socket = NULL;
    unsigned long client_flag = 0;
    struct tm *stime;
    time_t seconds;
    MySqlCOMMENT mysql_comm;

    memset(&mysql_comm, 0, sizeof(MySqlCOMMENT));
    strcpy(mysql_comm.database_name, "waming_park_db");
    strcpy(mysql_comm.table_park_name, "park01");
    strcpy(mysql_comm.table_billing_name, "billing");
    strcpy(mysql_comm.server, "localhost");
    strcpy(mysql_comm.user, "root");
    strcpy(mysql_comm.password, "frogshealth");
    strcpy(mysql_comm.table_qianfei, "table_qianfei");

    char recvbuff[32];

    _strPlateMsg PlateMsg;
    _strBilling Billing;

    printf("do park_mysql\r\n");

    mysql = mysql_init(NULL);
    if ( !mysql_real_connect(mysql, mysql_comm.server, mysql_comm.user, mysql_comm.password, 
                mysql_comm.database_name, port, unix_socket, client_flag)) 
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        return -1; 
    }   

    mysql_set_character_set(mysql,"gbk");

    int msgid = getMsgQueue();
    MsgQueueMessage *mqm = NULL;
    ItemInTable *itt = NULL;

    //��ȡ�������µļƷѱ�׼
    getBilling(mysql, 0, &Billing);

    while(1)
    {
        memset(&PlateMsg, 0, sizeof(PlateMsg));

        recvMsgQueue(msgid, CLIENT_TYPE, recvbuff);                                                               
        mqm = (MsgQueueMessage *)recvbuff;

        //��������
        if ( strncmp(mqm->c4Head, MESSAGE_CAR_IN, strlen(MESSAGE_CAR_IN)) == 0) {
            
            memcpy(&PlateMsg, &mqm->MessageType.plate, sizeof(mqm->MessageType.plate));

            itt = check_plate_qianfei_item(mysql, &mysql_comm, &PlateMsg);
            if (itt != NULL) {
                if(strcmp(itt->tablename, FREEITEM) == 0) {
                    //��ѯ�±��е������
                    unsigned int number = get_mysql_park_table_max_number(mysql, mysql_comm.table_park_name);
                    printf("table[%s] has %d message\r\n", mysql_comm.table_park_name, number);

                    if (number > TABLE_MAX_NUMBER) {
                        seconds = time(NULL);
                        stime = localtime(&seconds);
                        //char newname[] = "park190129";
                        char newname[20];
                        sprintf(newname, "park%02d%02d%02d", stime->tm_year+1900, stime->tm_mon+1, stime->tm_mday);
                        //�����е�Ƿ����Ϣ�������Ƶ�qianfei����
                        copy_nochargeitem_to_qianfeitable(mysql, &mysql_comm);
                        //�����򴴽��±�
                        create_mysql_table(mysql, newname);
                        //���½��ı������Ƹ��±���
                        memset(mysql_comm.table_park_name, 0, sizeof(mysql_comm.table_park_name));
                        strcpy(mysql_comm.table_park_name, newname);
                        //memcpy(mysql_comm.table_park_name, newname, strlen(newname));
                    }
                    do_work_when_car_in(mysql, &mysql_comm, &PlateMsg, &Billing);
                } else {
                    //��Ƿ�ѱ��ڷ��ָó�����Ϣ,��Ҫ�������Ѳŷ���

                }

                free(itt);
                itt = NULL;
            }
        //�����뿪
        } else if ( strncmp(mqm->c4Head, MESSAGE_CAR_OUT, strlen(MESSAGE_CAR_OUT)) == 0) {
            
            memcpy(&PlateMsg, &mqm->MessageType.plate, sizeof(mqm->MessageType.plate));
            itt = check_plate_qianfei_item(mysql, &mysql_comm, &PlateMsg);
            if (itt) {
                if ( strcmp(itt->tablename, FREEITEM) != 0) {
                    do_work_when_car_out(mysql, &mysql_comm, &PlateMsg, itt);
                }

                free(itt);
                itt = NULL;
            }
        //�����µļƷѹ���
        } else if ( strncmp(mqm->c4Head, MESSAGE_SET_BILLING, strlen(MESSAGE_SET_BILLING)) == 0) {
            //SB_xx_xx_xxx_xx_xxxx_xx
            //д��������Ϣ�����ݿ���
            memset(&Billing, 0, sizeof(Billing));
            memcpy(&Billing, &mqm->MessageType.billing, sizeof(mqm->MessageType.billing));

            printf("add billing\r\n");
            printf("u8Version=%d\r\n", Billing.u8Version);
            printf("u8FreeTime=%d\r\n", Billing.u8FreeTime);
            printf("u8TimingCycle=%d\r\n", Billing.u8TimingCycle);
            printf("u8Price=%d\r\n", Billing.u8Price);
            printf("u16MaxTiming=%d\r\n", Billing.u16MaxTiming);
            printf("u8VIPDiscount=%d\r\n", Billing.u8VIPDiscount);

            do_work_add_new_billing(mysql, &mysql_comm, &Billing);
        //���Ѽ�¼
        } else if ( strncmp(mqm->c4Head, MESSAGE_CHARGE, strlen(MESSAGE_CHARGE))) {
            //���������ˣ����¶�Ӧ���е���Ϣ
            memcpy(&PlateMsg, &mqm->MessageType.plate, sizeof(mqm->MessageType.plate));
            itt = check_plate_qianfei_item(mysql, &mysql_comm, &PlateMsg);
            if (itt) {
                if (strcmp(itt->tablename, FREEITEM) != 0) {
                    //�����Ӧ���������1:����֮����뿪��2:����֮����Ϊ����û�뿪���ٴ���Ҫ�뿪��ʱ����Ҫ����
                    if (do_work_charge_update_parktime(mysql, itt, PlateMsg.u32CanParkTime)) {
                        //�����ϱ������� 
                    }
                } else {
                    //û���ҵ���Ӧ��ͣ����Ϣ����Ҫ�ϱ���������
                }
                free(itt);
                itt = NULL;
            }
        }
    }

    mysql_close(mysql);

    return 0;
}
