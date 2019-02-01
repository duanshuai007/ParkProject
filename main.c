#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "LPRCClientSDK.h"
#include "plate.h"
#include "msgqueue.h"

#define CAMERA_PORT 8080

char *pstrCameraIN_ip = (char *)"192.168.200.251";
char *pstrCameraOUT_ip = (char *)"192.168.200.252";

static int siMsgID = 0;

#if 1
#define OPENDOOR(ip,port)  CLIENT_LPRC_SetRelayClose((ip), port)
#define CLOSEDOOR(ip,port) CLIENT_LPRC_DropRod((ip), port)
#else
#define OPENDOOR()  
#define CLOSEDOOR()
#endif

void LPRC_DataEx2CallBackHandler(CLIENT_LPRC_PLATE_RESULTEX *recResult, LDWORD dwUser)
{
    MsgQueueMessage msg = {0};;

    if (strcmp(recResult->chCLIENTIP, pstrCameraIN_ip) == 0) {
        //有车进入停车场
        if (recResult->pPlateImage.nLen > 0) {

            printf("\n门口炸鸡%s: 车牌号码：%s:%s\n", \
                    recResult->chCLIENTIP, \
                    recResult->chColor, \
                    recResult->chLicense);
            
            strcpy(msg.c4Head, MESSAGE_CAR_IN);

            goto OK;
        } 
    } else if (strcmp(recResult->chCLIENTIP, pstrCameraOUT_ip) == 0) {
        //有车离开停车场
        if (recResult->pPlateImage.nLen > 0) {

            printf("\n桌子上的%s: 车牌号码：%s:%s\n", 
                    recResult->chCLIENTIP, 
                    recResult->chColor, 
                    recResult->chLicense);

            strcpy(msg.c4Head, MESSAGE_CAR_OUT);
            
            goto OK;
        }
    }

    return;

OK:
    strcpy(msg.MessageType.plate.c3Color, recResult->chColor);
    strcpy(msg.MessageType.plate.c16Plate, recResult->chLicense);
    memcpy(&msg.MessageType.plate.time, &recResult->shootTime, sizeof(recResult->shootTime));
    sendMsgQueue(siMsgID, CLIENT_TYPE, (char *)&msg, sizeof(msg));
}

// 输出连接状态的回调函数
void ConnectStatus(char *chCLIENTIP, UINT Status, LDWORD dwUser)
{
	if(strcmp(chCLIENTIP, pstrCameraIN_ip) == 0)
	{
		if(Status == 0)
		{
			printf("%s connect fail!\n", chCLIENTIP);
		} else {
			//printf("%s connect Normal!\n", chCLIENTIP);
		}
	}
}

void AlarmCallBackHandler(CLIENT_LPRC_DEVDATA_INFO *alarmInfo, LDWORD dwUser)
{
    printf("alarm callback\r\n");
}

void GPIOCallBackHandler(char *chWTYIP, CLIENT_LPRC_GPIO_In_Statue *pGpioState)
{
    if (strcmp(chWTYIP, pstrCameraIN_ip) == 0) {
        //gpio0
        if (pGpioState->gpio_in0) {

        } else {

        }
        //gpio1
        if (pGpioState->gpio_in1) {

        } else {

        } 
        //gpio2
        if (pGpioState->gpio_in2) {

        } else {

        } 
        //gpio3
        if (pGpioState->gpio_in3) {

        } else {

        }
    } else if (strcmp(chWTYIP, pstrCameraOUT_ip) == 0) {
        //gpio0
        if (pGpioState->gpio_in0) {

        } else {

        }
        //gpio1
        if (pGpioState->gpio_in1) {

        } else {

        } 
        //gpio2
        if (pGpioState->gpio_in2) {

        } else {

        } 
        //gpio3
        if (pGpioState->gpio_in3) {

        } else {

        }       
    }
}

int main(int argc, char **argv)
{
    int 	ret;
    int 	nTime = 0;
    char 	*chPath = (char *)"/home/frog/camera/duanshuai/picture";

    if ( fork() == 0) {
        //子进程
        printf("execlp park_mysql\r\n");
        execlp("./park_mysql", "park_mysql_thread_name", NULL);
    } else {

        siMsgID = createMsgQueue();
        if (siMsgID == -1) {
            printf("crate msgqueue failed\r\n");
            return -1;
        }
        // 注册获取识别结果数据的回调函数
        CLIENT_LPRC_RegDataEx2Event((CLIENT_LPRC_DataEx2Callback) LPRC_DataEx2CallBackHandler);
        // 注册链接状态的回调函数
        CLIENT_LPRC_RegCLIENTConnEvent ((CLIENT_LPRC_ConnectCallback) ConnectStatus);

        //注册设备报警回掉函数
        //CLIENT_LPRC_RegAlarmEvent((CLIENT_LPRC_AlarmCallback) AlarmCallBackHandler);

        // 设置图片保存的路径（设置路径后，接口库会自动将识别结果保存到指定目录下）
        CLIENT_LPRC_SetSavePath(chPath);
        //设置gpio回掉函数
        CLIENT_LPRC_RegWTYGetGpioState((CLIENT_LPRC_GetGpioStateCallback) GPIOCallBackHandler);

        // 初始化。（多个相机的话，需要调用多次这个接口,输入不同的IP地址）
        ret =  CLIENT_LPRC_InitSDK(CAMERA_PORT, NULL, 0, pstrCameraIN_ip, 1);
        if (ret == 1)
        {
            printf("%s InitSDK fail\n\tthen quit\r\n", pstrCameraIN_ip);
            CLIENT_LPRC_QuitSDK();
            return -1;
        } else {
            printf("%s InitSDK success\n", pstrCameraIN_ip);
        }

        ret =  CLIENT_LPRC_InitSDK(CAMERA_PORT, NULL, 0, pstrCameraOUT_ip, 2);
        if (ret == 1)
        {
            printf("%s InitSDK fail\n\tthen quit\r\n", pstrCameraOUT_ip);
            CLIENT_LPRC_QuitSDK();
            return -1;
        } else {
            printf("%s InitSDK success\n", pstrCameraOUT_ip);
        }

        while(1)
        {
            sleep(1);
        }

        // 释放资源
        CLIENT_LPRC_QuitSDK();
        destoryMsgQueue(siMsgID);

        return 0;
    }
}
