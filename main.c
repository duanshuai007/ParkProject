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
        //�г�����ͣ����
        if (recResult->pPlateImage.nLen > 0) {

            printf("\n�ſ�ը��%s: ���ƺ��룺%s:%s\n", \
                    recResult->chCLIENTIP, \
                    recResult->chColor, \
                    recResult->chLicense);
            
            strcpy(msg.c4Head, MESSAGE_CAR_IN);

            goto OK;
        } 
    } else if (strcmp(recResult->chCLIENTIP, pstrCameraOUT_ip) == 0) {
        //�г��뿪ͣ����
        if (recResult->pPlateImage.nLen > 0) {

            printf("\n�����ϵ�%s: ���ƺ��룺%s:%s\n", 
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

// �������״̬�Ļص�����
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
        //�ӽ���
        printf("execlp park_mysql\r\n");
        execlp("./park_mysql", "park_mysql_thread_name", NULL);
    } else {

        siMsgID = createMsgQueue();
        if (siMsgID == -1) {
            printf("crate msgqueue failed\r\n");
            return -1;
        }
        // ע���ȡʶ�������ݵĻص�����
        CLIENT_LPRC_RegDataEx2Event((CLIENT_LPRC_DataEx2Callback) LPRC_DataEx2CallBackHandler);
        // ע������״̬�Ļص�����
        CLIENT_LPRC_RegCLIENTConnEvent ((CLIENT_LPRC_ConnectCallback) ConnectStatus);

        //ע���豸�����ص�����
        //CLIENT_LPRC_RegAlarmEvent((CLIENT_LPRC_AlarmCallback) AlarmCallBackHandler);

        // ����ͼƬ�����·��������·���󣬽ӿڿ���Զ���ʶ�������浽ָ��Ŀ¼�£�
        CLIENT_LPRC_SetSavePath(chPath);
        //����gpio�ص�����
        CLIENT_LPRC_RegWTYGetGpioState((CLIENT_LPRC_GetGpioStateCallback) GPIOCallBackHandler);

        // ��ʼ�������������Ļ�����Ҫ���ö������ӿ�,���벻ͬ��IP��ַ��
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

        // �ͷ���Դ
        CLIENT_LPRC_QuitSDK();
        destoryMsgQueue(siMsgID);

        return 0;
    }
}
