#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include <stdint.h>
#include "billing.h"

struct park_msgbuf {
    long mtype;
    char mtext[1024];
};

#pragma pack(1)

typedef struct {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
} park_time;

typedef struct {
    char c3Color[3];
    char c16Plate[16];
    park_time time;     //仅用于传输车辆进入和离开时的时间
    unsigned int u32CanParkTime;    //用来传输车辆交费信息，这里时交费所对应的允许停车时间
} _strPlateMsg;

typedef struct {
    char c4Head[4];
    union {
        _strPlateMsg    plate;
        _strBilling     billing;
    } MessageType;
} MsgQueueMessage;
#pragma pack()

#define SERVER_TYPE     0xa55a
#define CLIENT_TYPE     0x9a4f

#define MSGKEYDIR       "/home/frog/camera/duanshuai/"
#define MSGKEYFILE      "msgkey.file"

#define MESSAGE_CAR_OUT     "OUT"
#define MESSAGE_CAR_IN      "IN"
#define MESSAGE_SET_BILLING "SET"
#define MESSAGE_CHARGE      "CHA"

int createMsgQueue(void);
int getMsgQueue(void);
int destoryMsgQueue(int msg_id);
//int sendMsgQueue(int msg_id, int who, char *msg);
int sendMsgQueue(int msg_id, int who, char *msg, int len);
int recvMsgQueue(int msg_id, int recvType,char *out);

#endif
