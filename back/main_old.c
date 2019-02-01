#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
//#include "LPRCClientSDK.h"
#include "WTY.H"

#include "pthread.h"
#include "errno.h"

#define CAMERA_PORT 8080

char *pstrPlateNumber = (char *)"京AF0236";
char *pstrCamera_ip = (char *)"192.168.200.251";
char *pstrPhoto = (char *)"pohot.jpeg";

volatile int door_flag = 0;

#if 1
#define OPENDOOR()  WTY_SetRelayClose(pstrCamera_ip, CAMERA_PORT)
#define CLOSEDOOR() //WTY_DropRod(pstrCamera_ip, CAMERA_PORT)
#else
#define OPENDOOR()  
#define CLOSEDOOR()
#endif

void WTY_DATA_1_CallBackHandler( char *chWTYIP,
                                char *chPlate,
                                char *chColor,
                                char *chFullImage,
                                int nFullLen, 
                                char *chPlateImage,
                                int nPlateLen)
{
    char strTime[256];
    char strC[256];

    if (strcmp(chWTYIP, pstrCamera_ip) == 0) {

        if (nPlateLen > 0) {
            printf("\n%s: 车牌号码：%s:%s\n", 
                    chWTYIP, 
                    chColor, 
                    chPlate);
        } else {
            printf("无车牌\r\n");
        }
   }
}

void WTY_DATA_2_CallBackHandler(plate_result_ex *rec)
{
    if (strcmp(rec->chWTYIP, pstrCamera_ip) == 0) {
        if (rec->pPlateImage.nLen > 0) {
            printf("\n%s:车牌号码:%s:%s\n",
                    rec->chWTYIP, rec->chColor, rec->chLicense);
        } else {
            printf("无车牌\r\n");
        }
    }
}

void WTY_DATA_3_CallBackHandler(plate_result *rec)
{
    if (strcmp(rec->chWTYIP, pstrCamera_ip) == 0) {
        if (rec->nPlateLen > 0) {
            printf("\n%s:车牌号码:%s:%s\n",
                    rec->chWTYIP, rec->chColor, rec->chLicense);
        } else {
            printf("无车牌\r\n");
        }
    }
}

// 输出连接状态的回调函数
void ConnectStatus(char *chCLIENTIP, UINT Status)
{
	if(strcmp(chCLIENTIP, pstrCamera_ip) == 0)
	{
		if(Status == 0)
		{
			printf("%s connect fail!\n", chCLIENTIP);
		} else {
			//printf("%s connect Normal!\n", chCLIENTIP);
		}
	}
}

/*
 *  void (*CLIENT_LPRC_JpegCallback)(CLIENT_LPRC_DEVDATA_INFO *JpegInfo,LDWORD dwUser)
 */
#if 0
void JpegCallBackHandler(CLIENT_LPRC_DEVDATA_INFO *jpegInfo, LDWORD dwUser)
{
    //printf("jpeg call back\r\n");
    if(strcmp(jpegInfo->chIp, pstrCamera_ip) == 0) {
        if (jpegInfo->nStatus == 0) {
            //printf("jpeg call back\r\n");
        }
    }
}
#endif

void *pthread_door(void *arg)
{
    int delay = 0;

    while(1) {
        if (door_flag == 1) {
            OPENDOOR();
            delay++;
            if(delay >= 20) {
                delay = 0;
                //20 Seconds
                door_flag = 0;
            }
        } else {
            CLOSEDOOR();
        }

        sleep(1);
        //printf("pthread:flag = %d\r\n", door_flag);
    }
}

static void usage(char *appname)
{
	printf(
	"\nUsage:\n"
	"    %s <run time>\n",
	appname);
}

int main(int argc, char **argv)
{
	int 	ret;
	int 	nTime = 0;
	//int		quittime = 10;
    int cmd = 0;
	char 	*chPath = (char *)"/home/frog/camera/duanshuai/picture";
    pthread_t door_id;

	if(argc == 2){
		//quittime = atoi(argv[1]);
        cmd = atoi(argv[1]);
	}
	else{
		usage(argv[0]);
		return -1;
	}

	// 注册获取识别结果数据的回调函数
	//WTY_RegDataExEvent((WTYDataExCallback) WTY_DATA_1_CallBackHandler);
	WTY_RegDataEx2Event((WTYDataEx2Callback ) WTY_DATA_2_CallBackHandler);
	// 注册链接状态的回调函数
	WTY_RegWTYConnEvent((WTYConnectCallback) ConnectStatus);

    // 初始化。（多个相机的话，需要调用多次这个接口,输入不同的IP地址）
	//ret =  CLIENT_LPRC_InitSDK(CAMERA_PORT, NULL, 0, pstrCamera_ip, 1);
	ret =  WTY_InitSDK(CAMERA_PORT, NULL, 0, pstrCamera_ip);
	if (ret == 1)
	{
		printf("%s InitSDK fail\n\tthen quit\r\n", pstrCamera_ip);
        WTY_QuitDevice(pstrCamera_ip);
		return -1;
	} else {
		printf("%s InitSDK success\n", pstrCamera_ip);
	}
	
    ret = pthread_create(&door_id, NULL, pthread_door, NULL);
    if ( ret > 0) {
        printf("create pthread error!\n");
        return -1;
    }

    //CLIENT_LPRC_DropRod(pstrCamera_ip, CAMERA_PORT);
    //sleep(5);

    while(1)
	{
        //CLIENT_LPRC_SetTrigger(pstrCamera_ip, CAMERA_PORT);

        WTY_SetTrigger(pstrCamera_ip, CAMERA_PORT);
        printf("paiyizhang!\r\n");
        //CLIENT_LPRC_SnapJpegFrame(pstrCamera_ip, pstrPhoto, NULL, 10*1024*1024);
        sleep(2);
	}

    sleep(3);
	
    // 释放资源
	WTY_QuitDevice(pstrCamera_ip);

	return 0;
}
