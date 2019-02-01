#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "LPRCClientSDK.h"
//#include "WTY.H"

#include "pthread.h"
#include "errno.h"

#define CAMERA_PORT 8080

char *pstrPlateNumber = (char *)"京AF0236";
char *pstrCamera_ip = (char *)"192.168.200.251";
char *pstrPhoto = (char *)"pohot.jpeg";

volatile int door_flag = 0;

#if 1
#define OPENDOOR()  CLIENT_LPRC_SetRelayClose(pstrCamera_ip, CAMERA_PORT)
#define CLOSEDOOR() CLIENT_LPRC_DropRod(pstrCamera_ip, CAMERA_PORT)
#else
#define OPENDOOR()  
#define CLOSEDOOR()
#endif

void LPRC_DataEx2CallBackHandler(CLIENT_LPRC_PLATE_RESULTEX *recResult, LDWORD dwUser)
{
    char strTime[256];
    char strC[256];

    if (strcmp(recResult->chCLIENTIP, pstrCamera_ip) == 0) {

        if (recResult->pPlateImage.nLen > 0) {
			// 识别的时间
			sprintf(strTime,"%d-%d-%d %d:%d:%d.%d", recResult->shootTime.Year, 
				recResult->shootTime.Month,
				recResult->shootTime.Day,
				recResult->shootTime.Hour,
				recResult->shootTime.Minute,
				recResult->shootTime.Second,
				recResult->shootTime.Millisecond);
			// 车牌坐标
			//sprintf(strC, "left = %d, top = %d, right = %d, bottom = %d", 
                //recResult->pcLocation.Left, 
                //recResult->pcLocation.Top, 
                //recResult->pcLocation.Right, 
                //recResult->pcLocation.Bottom);

			printf("\n%s: 车牌号码：%s:%s\n", 
                    recResult->chCLIENTIP, 
                    recResult->chColor, 
                    recResult->chLicense);
			
            printf("%s: 识别时间：%s\n", recResult->chCLIENTIP, strTime);
			printf("%s: 车牌坐标：%s\n\n", recResult->chCLIENTIP, strC);
            if (strcmp(recResult->chLicense, pstrPlateNumber) == 0) {
                printf("this plate number is in database\r\n");
                door_flag = 1;
            }
		} else {// 表示无识别结果
			printf("\n%s: 无牌车\n", recResult->chCLIENTIP);
		}
   }
}

// 输出连接状态的回调函数
void ConnectStatus(char *chCLIENTIP, UINT Status, LDWORD dwUser)
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
#if 1
void JpegCallBackHandler(CLIENT_LPRC_DEVDATA_INFO *jpegInfo, LDWORD dwUser)
{
    char *JPEGBuf;
    char JPEGBufLen = 0;
    char *plate;
    char *color;
    void *fullimage;
    int fulllen;
    void *plateimage;
    int platelen;
    //printf("jpeg call back\r\n");
    if(strcmp(jpegInfo->chIp, pstrCamera_ip) == 0) {
        if (jpegInfo->nStatus == 0) {
            //printf("jpeg call back\r\n");
            CLIENT_LPRC_GetJpegStream(pstrCamera_ip, JPEGBuf, &JPEGBufLen);
            CLIENT_LPRC_GetVehicleInfoEx(pstrCamera_ip, plate, color, fullimage, &fulllen, plateimage, &platelen);
            if (platelen > 0){
                printf("plate:%s:%s\r\n", color, plate);
            }
        }
    }
}
#endif

/*
 *  void (*CLIENT_LPRC_AlarmCallback)(CLIENT_LPRC_DEVDATA_INFO *alarmInfo,LDWORD dwUser);
 */
void AlarmCallBackHandler(CLIENT_LPRC_DEVDATA_INFO *alarmInfo, LDWORD dwUser)
{
    printf("alarm callback\r\n");
}

void GPIOCallBackHandler(char *chWTYIP, CLIENT_LPRC_GPIO_In_Statue *pGpioState)
{
    if (strcmp(chWTYIP, pstrCamera_ip) == 0) {
        //gpio0
        if (pGpioState->gpio_in0) {
        
        } else {
        
        }
        //gpio1
        if (pGpioState->gpio_in0) {
        
        } else {
        
        } 
        //gpio2
        if (pGpioState->gpio_in0) {
        
        } else {
        
        } 
        //gpio3
        if (pGpioState->gpio_in0) {
        
        } else {
        
        }
    }
}

void *pthread_door(void *arg)
{
    int delay = 0;

    while(1) {
        if (door_flag == 1) {
            OPENDOOR();
            delay++;
            if(delay >= 6) {
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

HWND getJpegInfoHandler(void *arg)
{
#if 0
    char plate[20];
    char color[20];
    char *fullImage[10*1024*1024];
    int len;
    char *plateImage[10*1024*1024];
    int platelen;

    CLIENT_LPRC_GetVehicleInfoEx(pstrCamera_ip, 
                                plate,
                                color,
                                fullImage,
                                &len,
                                plateImage,
                                &platelen);
#endif
    printf("getJpegInfoHandler\r\n");
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

    printf("1111111111111111\r\n");
	// 注册获取识别结果数据的回调函数
	CLIENT_LPRC_RegDataEx2Event((CLIENT_LPRC_DataEx2Callback) LPRC_DataEx2CallBackHandler);
	// 注册链接状态的回调函数
	CLIENT_LPRC_RegCLIENTConnEvent ((CLIENT_LPRC_ConnectCallback) ConnectStatus);

#if 1
    //注册获取动态jpeg流图片的回掉函数
    CLIENT_LPRC_RegJpegEvent((CLIENT_LPRC_JpegCallback) JpegCallBackHandler);

    CLIENT_LPRC_SetJpegStreamPlayOrStop(pstrCamera_ip, 1);

    CLIENT_LPRC_JpegMessageInit((HWND)getJpegInfoHandler, 1, pstrCamera_ip);
#endif

    //注册设备报警回掉函数
    //CLIENT_LPRC_RegAlarmEvent((CLIENT_LPRC_AlarmCallback) AlarmCallBackHandler);

    // 设置图片保存的路径（设置路径后，接口库会自动将识别结果保存到指定目录下）
    CLIENT_LPRC_SetSavePath(chPath);
    
    CLIENT_LPRC_RegWTYGetGpioState((CLIENT_LPRC_GetGpioStateCallback) GPIOCallBackHandler);

    //设置图片上传
    //CLIENT_LPRC_SetTransContent(pstrCamera_ip, CAMERA_PORT, 0, 1);

    printf("goto cmd\r\n");

    //获取设备列表
    switch(cmd)
    {
        case 1:
            {
                CLIENT_LPRC_DeviceInfo cld[10];
                int iCldNum = 0;

                if ((iCldNum = CLIENT_LPRC_SearchDeviceList(cld)) > 0) {
                    for(int i=0; i < iCldNum; i++) {
                        printf("********** Device NO[%d] **********\r\n", i);
                        printf("Device Name:    %s\r\n", cld[i].chDevName);
                        printf("Soft Version:   %s\r\n", cld[i].chSoftVer);
                        printf("Hard Version:   %s\r\n", cld[i].chHardVer);
                        printf("System Version: %s\r\n", cld[i].chSysVer);
                        printf("Port:           %d\r\n", cld[i].nSdkPort);
                        printf("ip:             %s\r\n", cld[i].chIp);
                        printf("GateWay:        %s\r\n", cld[i].chGateway);
                        printf("NetMask:        %s\r\n", cld[i].chNetmask);
                        printf("Mac:            %s\r\n", cld[i].chMac);
                        printf("RoomID:         %s\r\n", cld[i].chRoomID);
                        printf("SN:             %s\r\n", cld[i].chSN);
                        printf("*********************************\r\n");
                    }
                }
            }break;
        case 2:
            {
                //修改系统时间
                CLIENT_LPRC_CAMERA_TIME clct;
                clct.Year = 2019;
                clct.Month = 1;
                clct.Day = 14;
                clct.Hour = 18;
                clct.Minute = 14;
                clct.Second = 0;
                clct.Millisecond = 0;
                CLIENT_LPRC_SetDevTimeParam(pstrCamera_ip, &clct);
            }
            break;
        default:
            break;
    }


#if 0
    //修改设备信息
    CLIENT_LPRC_DeviceInfo cldi;
    //修改结构体内容
    CLIENT_LPRC_AlterDeviceInfo(pstrCamera_ip, cldi);
#endif

    printf("init------\r\n");
    // 初始化。（多个相机的话，需要调用多次这个接口,输入不同的IP地址）
	ret =  CLIENT_LPRC_InitSDK(CAMERA_PORT, (HWND)getJpegInfoHandler, 1, pstrCamera_ip, 1);
	//ret =  CLIENT_LPRC_InitSDK(CAMERA_PORT, NULL, 0, pstrCamera_ip, 1);
	if (ret == 1)
	{
		printf("%s InitSDK fail\n\tthen quit\r\n", pstrCamera_ip);
        CLIENT_LPRC_QuitSDK();
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

    printf("init---22222---\r\n");
    char *JPEGBuf;
    char JPEGBufLen = 0;
    char *plate;
    char *color;
    void *fullimage;
    int fulllen;
    void *plateimage;
    int platelen;

    printf("goto while!\r\n");
    while(1)
	{
        //CLIENT_LPRC_SetTrigger(pstrCamera_ip, CAMERA_PORT);
//        CLIENT_LPRC_GetJpegStream(pstrCamera_ip, JPEGBuf, &JPEGBufLen);
//        CLIENT_LPRC_GetVehicleInfoEx(pstrCamera_ip, plate, color, fullimage, &fulllen, plateimage, &platelen);
//        if(platelen > 0) {
//            printf("result:%s\r\n", plate);
//        }
//        if (fulllen > 0) {
//            printf("full len:%d\r\n", fulllen);
//        }
        printf("paiyizhang!\r\n");
        //CLIENT_LPRC_SnapJpegFrame(pstrCamera_ip, pstrPhoto, NULL, 10*1024*1024);
        sleep(3);
	}

    sleep(3);
	
    // 释放资源
	CLIENT_LPRC_QuitSDK();

	return 0;
}
