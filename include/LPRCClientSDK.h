#ifndef _CLIENT_H
#define _CLIENT_H

#if !defined(WIN32) && !defined(__stdcall)
#define __stdcall
#endif

#ifndef WIN32
typedef unsigned int UINT;
typedef void *HWND;
#endif
typedef unsigned long LDWORD; 
#ifdef __cplusplus
extern "C" {
#endif
#define CLIENT_LPRC_BIG_PICSTREAM_SIZE     200000-312		/*����ϴ�jpeg��ÿ֡ռ�õ��ڴ������С*/
#define CLIENT_LPRC_BIG_PICSTREAM_SIZE_EX   1*800*1024-312	/*����ϴ�ȫ��ͼռ���ڴ������С		*/
#define CLIENT_LPRC_SMALL_PICSTREAM_SIZE_EX   10000			/*����ϴ����ƽ�ͼռ���ڴ������С	*/

/* ���ʱ�� */
typedef struct 
{
	int Year;			/* �� 	*/
	int Month;			/* �� 	*/
	int Day;			/* �� 	*/
	int Hour;			/* ʱ 	*/
	int Minute;			/* �� 	*/
	int Second;			/* �� 	*/
	int Millisecond;	/* ΢�� */
}CLIENT_LPRC_CAMERA_TIME;

/* ʶ�������� */
typedef struct 
{
	int 	Left;	/* �� */
	int 	Top;	/* �� */
	int 	Right;	/* �� */
	int 	Bottom;	/* �� */
}CLIENT_LPRC_PLATE_LOCATION;
/* ͼ����Ϣ*/
typedef struct
{
	int				nWidth;							/* ���					*/
	int				nHeight;						/* �߶�					*/
	int				nPitch;							/* ͼ���ȵ�һ��������ռ�ڴ��ֽ���*/
	int				nLen;							/* ͼ��ĳ���			*/
	char			reserved[16];					/* Ԥ��     			*/
	unsigned char	*pBuffer;						/* ͼ���ڴ���׵�ַ		*/
}CLIENT_LPRC_IMAGE_INFO;

/* ʶ���� */
typedef struct 
{	
	char				chCLIENTIP[16];						/* ���IP 				*/
	char				chColor[8];							/* ������ɫ 			*/
	char				chLicense[16];						/* ���ƺ��� 			*/
	CLIENT_LPRC_PLATE_LOCATION 		pcLocation;							/* ������ͼ���е����� 	*/
	CLIENT_LPRC_CAMERA_TIME			shootTime;							/* ʶ������Ƶ�ʱ�� 	*/
	int					nConfidence;						/* ���ƿ��Ŷ�			*/
	int					nTime;								/* ʶ���ʱ				*/
	int					nDirection;							/* ���Ʒ���		    	*/
	char				reserved[256];						/* Ԥ��     			*/
	CLIENT_LPRC_IMAGE_INFO		pFullImage;						/* ȫ��ͼ������(ע�⣺��������䣬�˴�ָ��Ϊ��) */
	CLIENT_LPRC_IMAGE_INFO		pPlateImage;						/* ����ͼ������(ע�⣺��������䣬�˴�ָ��Ϊ��) */
}CLIENT_LPRC_PLATE_RESULTEX;

/* Jpeg���ص�����ÿһ֡jpeg���ݽṹ�� */
typedef struct 
{
	char			chIp[16];				/*���IP												*/
	char			*pchBuf;				/*ÿһ֡jpeg���ݻ�����									*/
	unsigned int	nLen;					/*��ǰjpeg֡�����ݳ���									*/
	int				nStatus;				/* ��ǰjpeg֡����״̬�� 0-����, ��0-������ 				*/
	char			reserved[128];			/* ����		  											*/
}CLIENT_LPRC_DEVDATA_INFO;
/*���մ������ݵĽṹ��*/
typedef struct
{
	unsigned char *pData;					/*��������ָ��											*/
	int nsize;								/*�������ݴ�С											*/
	char reserved[128];						/* ����													*/
}CLIENT_LPRC_DEVSERIAL_DATA;
/* �����豸֮�󱣴��豸��Ϣ�Ľṹ�壬ע:Ҫ���޸�ip��Ϣ�뱣֤������Ϣ�����ı�*/
typedef struct 
{
	char 			chDevName[256];			/* �豸��		*/
	char			chSoftVer[20];			/* ����汾��	*/
	char 			chHardVer[20];			/* Ӳ���汾��	*/
	char 			chSysVer[20];			/* ϵͳ�汾	*/
	int				nSdkPort;				/* sdk�˿ں�			*/
	char 			chIp[16];				/* ip��ַ		*/
	char 			chGateway[16];			/* ����	*/
	char 			chNetmask[16];			/* ��������	*/
	char 			chMac[18];				/* Mac��ַ		*/
	char            chRoomID[20];				/*RooMID            */
	char            chSN[20];					/*SN*/
	char			reserved[256];			/* ����		  */
}CLIENT_LPRC_DeviceInfo;

/*�°汾Rs485͸�����ݽṹ��*/   
typedef struct{
	short           TimeDelay;	/*��ʱ��Χ:10~200֮��
						        ע:1�����õ��Ǳ������ݺ���һ������֮�����ʱʱ��
						           2�����һ�����ݲ���Ҫ������ʱ*/		
    unsigned char   *data;      /*�洢�������ݵĻ�����ָ�룬��Ҫ�ͻ��Լ����뻺����*/
	short           datalen;    /*��������󳤶�2*1024*/
	char            reserved[10];/*����*/
 } CLIENT_LPRC_RS485_Data_t;

/*�°汾485͸���ṹ��*/
typedef struct{
	CLIENT_LPRC_RS485_Data_t  rS485_data[5];  /*ʵ�ʷ������ݵĽṹ��*/
	int                       datanum;		  /*ÿ�η������֧�ַ���5������*/
} CLIENT_LPRC_RS485_Transparent_t;
/*GPIO �����״̬ �ṹ�� */
typedef struct{ 	
	unsigned  char gpio_in0;//GPIO IN0 0�͵�ƽ 1 �ߵ�ƽ 	
	unsigned  char gpio_in1;//GPIO IN1 0�͵�ƽ 1 �ߵ�ƽ
	unsigned  char gpio_in2;//GPIO IN2 0�͵�ƽ 1 �ߵ�ƽ
	unsigned  char gpio_in3;//GPIO IN3 0�͵�ƽ 1 �ߵ�ƽ 
}CLIENT_LPRC_GPIO_In_Statue;


/************************************************************************/
/* CLIENT_LPRC_InitSDK: �������												*/
/*		Parameters:														*/
/*			nPort[in]:		��������Ķ˿ڣ���Ĭ��Ϊ8080				*/
/*			hWndHandle[in]:	������Ϣ�Ĵ���������ΪNULLʱ����ʾ�޴���  */
/*			uMsg[in]:		�û��Զ�����Ϣ����hWndHandle��ΪNULLʱ��	*/
/*							��⵽���µĳ���ʶ������׼���õ�ǰ����	*/
/*							��������Ϣ����::PostMessage ������		*/
/*							hWndHandle����uMsg��Ϣ������WPARAM����Ϊ0��	*/
/*							LPARAM����Ϊ0								*/
/*			chServerIP[in]:	�����IP��ַ								*/
/*          dwUser[in]:     �û��Զ����ֶΣ���Ҫ�����ش����ص�������    */
/*		Return Value:   int												*/
/*							0	������ӳɹ�							*/
/*							1	�������ʧ��							*/
/*		Notice:   														*/
/*				������ûص��ķ�ʽ��ȡ����ʱ��hWndHandle���ΪNULL��	*/
/*				uMsgΪ0������ע��ص�������֪ͨ���µ����ݣ�				*/
/*				��֮�����������յ���Ϣʱ������CLIENT_LPRC_GetVehicleInfoEx��ȡ	*/
/*				���ݡ�													*/
/************************************************************************/
int __stdcall CLIENT_LPRC_InitSDK(UINT nPort,HWND hWndHandle,UINT uMsg,char *chServerIP,LDWORD dwUser);

/************************************************************************/
/* CLIENT_LPRC_QuitSDK: �Ͽ������Ѿ������豸���ͷ���Դ							*/
/*		Parameters:														*/
/*		Return Value:   void											*/
/************************************************************************/
void  __stdcall CLIENT_LPRC_QuitSDK();


/***********************************************************************************/
/* �ص�����:ѭ����Ⲣ֪ͨ����豸ͨѶ״̬�Ļص�����						       */
/*		Parameters:														           */
/*			chCLIENTIP[out]:		�����豸IP								           */
/*			nStatus[out]:		�豸״̬��0��ʾ�����쳣���豸�쳣��			       */
/*										  1��ʾ�����������豸������		    	   */
/*										  2��ʾ�����������豸�����ӣ������������  */
/*          dwUser[out]         CLIENT_InitSDK����sdk���û��Զ����ֶ�              */
/*		Return Value:   void											           */
/***********************************************************************************/
typedef void (*CLIENT_LPRC_ConnectCallback)(char *chCLIENTIP,UINT nStatus,LDWORD dwUser);


/***********************************************************************************/
/* �ص�����:��ȡ���485���͵�����						       */
/*		Parameters:														           */
/*			chWTYIP[out]:		�����豸IP								           */
/*          serialData[out]          �������ݵ�ַ										*/
/*			nlen[out]				�������ݴ�С										*/
/*		Return Value:   void											           */
/***********************************************************************************/
typedef void (*CLIENT_LPRC_SerialDataCallback)(char *chCLIENTIP,CLIENT_LPRC_DEVSERIAL_DATA *pSerialData,LDWORD dwUser);

/***********************************************************************************/
/* �ص�����:��ȡ���GPIO״̬						       */
/*		Parameters:														           */
/*			chWTYIP[out]:		�����豸IP								           */
/*          pGpioState[out]          �������ݵ�ַ										*/
/*		Return Value:   void											           */
/***********************************************************************************/
typedef void (*CLIENT_LPRC_GetGpioStateCallback)(char *chWTYIP,CLIENT_LPRC_GPIO_In_Statue *pGpioState);

/************************************************************************/
/* CLIENT_LPRC_RegWTYGetGpioState: ע���ȡ���GPIO״̬�Ļص�����						*/
/*		Parameters:														*/
/*			CLIENTGpioState[in]:		CLIENT_LPRC_GetGpioStateCallback���ͻص�����				*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall CLIENT_LPRC_RegWTYGetGpioState (CLIENT_LPRC_GetGpioStateCallback CLIENTGpioState);

/************************************************************************/
/* CLIENT_LPRC_RegCLIENTConnEvent: ע�����ͨѶ״̬�Ļص�����						*/
/*		Parameters:														*/
/*			CLIENTConnect[in]:		CLIENT_LPRC_ConnectCallback���ͻص�����				*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall CLIENT_LPRC_RegCLIENTConnEvent (CLIENT_LPRC_ConnectCallback CLIENTConnect);


/************************************************************************/
/* CLIENT_LPRC_CheckStatus: �����������豸��ͨѶ״̬							*/
/*		Parameters:														*/
/*			chCLIENTIP[in]:		Ҫ���������IP						*/
/*		Return Value:   int												*/
/*							0	����									*/
/*							1	���粻ͨ								*/
/************************************************************************/
int __stdcall CLIENT_LPRC_CheckStatus (char *chCLIENTIP);

/************************************************************************/
/* �ص�����: ע�����ʶ�����ݻص�����									*/
/*		Parameters:														*/
/*			recResult[out]:		ʶ��������							*/
/*          dwUser[out]            CLIENT_LPRC_InitSDK����sdk���û��Զ����ֶ�              */

/*		Return Value:   void											*/
/*	Note:																*/
/*		����չ�Ļص������������û����������Ƚϸ߻�ֱ��ʽϸߵ�JPEGͼ��	*/
/************************************************************************/
typedef void (*CLIENT_LPRC_DataEx2Callback)(CLIENT_LPRC_PLATE_RESULTEX *recResultEx,LDWORD dwUser);

/************************************************************************/
/* CLIENT_LPRC_RegSerialDataEvent: ע���ȡ�������ݵĻص�����							*/
/*		Parameters:														*/
/*			CLIENTSerialData[in]:		������մ������ݵĻص�������ָ��			*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall CLIENT_LPRC_RegSerialDataEvent(CLIENT_LPRC_SerialDataCallback CLIENTSerialData);

/************************************************************************/
/* CLIENT_LPRC_RegDataEx2Event: ע���ȡʶ�����Ļص�����						*/
/*		Parameters:														*/
/*			CLIENTData[in]:		����ʶ�����Ļص�������ָ��			*/
/*		Return Value:   void											*/
/*	Note:																*/
/*		���������Ƚϸߣ���ֱ��ʽϸߵ�JPEGͼ��							*/
/************************************************************************/
void __stdcall CLIENT_LPRC_RegDataEx2Event(CLIENT_LPRC_DataEx2Callback CLIENTDataEx2);

/************************************************************************/
/* 	����: ��Ϣ��ʽ��ȡָ��IP�����ʶ������							*/
/*		  ��CLIENT_LPRC_initSDK�����������˴���������Ϣʱ��					*/
/*		  ��Ҫ����Ϣ�������е��ô˺�����������ȡʶ������			*/
/*		Parameters:														*/
/*			chCLIENTIP[in]:		������Ϣ����ȡָ��IP�豸ʶ������		*/
/*			chPlate[in]:		���ƺ���								*/
/*			chColor[in]:		������ɫ								*/
/*			chFullImage[in]:	ȫ��ͼ����								*/
/*			nFullLen[in]:		ȫ��ͼ���ݳ���							*/
/*			chPlateImage[in]:	����ͼ����								*/
/*			nPlateLen[in]:		����ͼ���ݳ���							*/
/*		Return Value:   int												*/
/*							0	��ȡ�ɹ�								*/
/*							1	��ȡʧ��								*/
/*		Notice:   														*/
/*			�������˴������ݲ���ʱ�����Զ�Ӧ������ΪNULL������Ϊ-1��	*/
/*			��û���γ�����ʱ�����Զ�Ӧ����ΪNULL������Ϊ0				*/
/************************************************************************/
int __stdcall CLIENT_LPRC_GetVehicleInfoEx(char *chCLIENTIP,
								 char *chPlate,	
								 char *chColor,
								 void *chFullImage ,
								 int *nFullLen,
								 void *chPlateImage,
								 int *nPlateLen);

/************************************************************************/
/* CLIENT_LPRC_SetSavePath: ����û���Ҫ��̬���Զ�����ͼƬ������ͨ���ú������ñ�*/
/*					��ͼƬ��·����										*/
/*		Parameters:														*/
/*			chSavePath[in]:	�ļ��洢·������"\\"�������磺"D:\\Image\\"	*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:   														*/
/*			ȫ��ͼ��ָ��Ŀ¼\\�豸IP\\�����գ�YYYYMMDD��\\FullImage\\	*/
/*									ʱ����-����__��ɫ_���ƺ���__.jpg	*/
/*			����ͼ��ָ��Ŀ¼\\�豸IP\\�����գ�YYYYMMDD��\\PlatelImage\\	*/
/*									ʱ����-����__��ɫ_���ƺ���__.jpg	*/
/************************************************************************/
void __stdcall CLIENT_LPRC_SetSavePath (char *chSavePath);


/************************************************************************/
/* CLIENT_LPRC_SetTrigger: ����ʶ��												*/
/*		Parameters:														*/
/*			pCameraIP[in]:			Ҫ����������豸��IP				*/
/*			nCameraPort[in]:		�˿�,Ĭ��8080						*/
/*		Return Value:													*/
/*					0	�����ɹ�����									*/
/*				  ��0	ʧ��											*/
/************************************************************************/	
int __stdcall CLIENT_LPRC_SetTrigger(char *pCameraIP, int nCameraPort);


/************************************************************************/
/* CLIENT_LPRC_SetTransContent: ��������豸�ϴ�����					        */
/*		Parameters:														*/
/*			pCameraIP[in]:		Ҫ���õ��豸IP							*/
/*			nCameraPort[in]:	�˿�,Ĭ��8080							*/
/*			nFullImg[in]:		ȫ��ͼ��0��ʾ������1��ʾ��				*/
/*			nPlateImg[in]:		����ͼ��0��ʾ������1��ʾ��				*/
/*		Return Value:   int												*/
/*							0	�ɹ�									*/
/*						  ��0	ʧ��									*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SetTransContent (char *pCameraIP, int nCameraPort, int nFullImg, int nPlateImg);


/************************************************************************/
/* ����˵��: ���Ƽ̵����ıպ�											*/
/*		Parameters:														*/
/*			pCameraIP[in]:			���IP								*/
/*			nCameraPort[in]:		�˿�,Ĭ��8080						*/
/*		Return Value:   int												*/
/*							0	���óɹ�								*/
/*						  ��0	ʧ��									*/
/*		Notice:   														*/
/*				ͨ���˹��ܣ�������PC��ͨ��һ����豸�������Ƶ�բ��̧��	*/
/*				�豸�̵�������ź�Ϊ���������źš�						*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SetRelayClose(char *pCameraIP, int nCameraPort);
/************************************************************************/
/* ����˵��: ���Ƶ�բ����											*/
/*		Parameters:														*/
/*			pCameraIP[in]:			���IP								*/
/*			nCameraPort[in]:		�˿�,Ĭ��8080						*/
/*		Return Value:   int												*/
/*							0	���óɹ�								*/
/*						  ��0	ʧ��									*/
/*		Notice:   														*/
/*				ͨ���˹��ܣ�������PC��ͨ��һ����豸�������Ƶ�բ������	*/
/*				�豸�̵�������ź�Ϊ���������źš�						*/
/************************************************************************/
int __stdcall CLIENT_LPRC_DropRod(char *pCameraIP, int nCameraPort);



/************************************************************************/
/* �ص�����: ��ȡJpeg���Ļص�����										*/
/*		Parameters:														*/
/*			JpegInfo[out]:		JPEG��������Ϣ							*/
/*          dwUser[out]            CLIENT_LPRC_InitSDK����sdk���û��Զ����ֶ�              */

/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:һ̨PC���Ӷ�̨�豸ʱ���˺�������ʵ��һ�Ρ������ֲ�ͬ		*/
/*			�豸��JPEG��ʱ������ͨ�����������CLIENT_LPRC_DEVDATA_INFO�е�chIp��	*/
/*			����.														*/
/*			2:�˹���Ŀǰ������V5.5.3.0��V6.0.0.0�����ϰ汾,				*/
/*			  V5.2.1.0��V5.2.2.0��V5.2.6.0�Ȱ汾����ʹ�ô˹���			*/
/************************************************************************/
typedef void (*CLIENT_LPRC_JpegCallback)(CLIENT_LPRC_DEVDATA_INFO *JpegInfo,LDWORD dwUser);


/************************************************************************/
/* CLIENT_LPRC_RegJpegEvent: ע���ȡJpeg���Ļص�����							*/
/*		Parameters:														*/
/*			JpegInfo[in]:		CLIENT_LPRC_JpegCallback���ͻص�����				*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:һ̨PC���Ӷ�̨�豸ʱ���˺�������ʵ��һ�Ρ������ֲ�ͬ		*/
/*			�豸��JPEG��ʱ������ͨ�����������CLIENT_LPRC_DEVDATA_INFO�е�chIp��	*/
/*			����.														*/
/*			2:�˹���Ŀǰ������V5.5.3.0��V6.0.0.0�����ϰ汾,				*/
/*			  V5.2.1.0��V5.2.2.0��V5.2.6.0�Ȱ汾����ʹ�ô˹���			*/
/************************************************************************/
void __stdcall CLIENT_LPRC_RegJpegEvent(CLIENT_LPRC_JpegCallback JpegInfo);


/************************************************************************/
/* �ص�����: ��ȡ������Ϣ�Ļص�����										*/
/*		Parameters:														*/
/*			alarmInfo[out]:		������Ϣ								*/
/*          dwUser[out]            CLIENT_LPRC_InitSDK����sdk���û��Զ����ֶ�              */

/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			һ̨PC���Ӷ�̨�豸ʱ���˺�������ʵ��һ�Ρ������ֲ�ͬ�豸	*/
/*			��Alarmʱ������ͨ�����������CLIENT_LPRC_DEVDATA_INFO�е�chIp������		*/
/*																		*/
/*		Notice:															*/
/*			1:һ̨PC���Ӷ�̨�豸ʱ���˺�������ʵ��һ�Ρ������ֲ�ͬ		*/
/*			�豸��JPEG��ʱ������ͨ�����������LPRC_CLIENT_DEVDATA_INFO�е�chIp��	*/
/*			����.														*/
/*			2:�˹���Ŀǰ������V5.5.3.0��V6.0.0.0�����ϰ汾,				*/
/*			  V5.2.1.0��V5.2.2.0��V5.2.6.0�Ȱ汾����ʹ�ô˹���			*/
/************************************************************************/
typedef void (*CLIENT_LPRC_AlarmCallback)(CLIENT_LPRC_DEVDATA_INFO *alarmInfo,LDWORD dwUser);


/************************************************************************/
/* CLIENT_LPRC_RegAlarmEvent: ע���ȡ������Ϣ�Ļص�����						*/
/*		Parameters:														*/
/*			AlarmInfo[in]:		CLIENT_LPRC_AlarmCallback���ͻص�����			*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:һ̨PC���Ӷ�̨�豸ʱ���˺�������ʵ��һ�Ρ������ֲ�ͬ		*/
/*			�豸��JPEG��ʱ������ͨ�����������LPRC_CLIENT_DEVDATA_INFO�е�chIp��	*/
/*			����.														*/
/*			2:�˹���Ŀǰ������V5.5.3.0��V6.0.0.0�����ϰ汾,				*/
/*			  V5.2.1.0��V5.2.2.0��V5.2.6.0�Ȱ汾����ʹ�ô˹���			*/
/************************************************************************/

void __stdcall CLIENT_LPRC_RegAlarmEvent(CLIENT_LPRC_AlarmCallback AlarmInfo);


/************************************************************************/
/* CLIENT_LPRC_RS485Send: RS485͸������											*/
/*		Parameters:														*/
/*			pCameraIP[in]				����豸IP��ַ					*/
/*			nPort[in]					�˿�,Ĭ��8080					*/
/*			chData[in]					��Ҫ��������ݿ���׵�ַ		*/
/*			nSendLen[in]				��Ҫ��������ݿ���ֽ���		*/
/*		Return Value:   int												*/
/*							0	�ɹ�									*/
/*						  ��0	ʧ��									*/
/*		notice��														*/
/*				1���û�ͨ���˽ӿڣ�������������ݣ�����豸��ԭ��������	*/
/*				ͨ��RS485�ӿ�ת����ȥ�����ͻ����ӵ��ⲿ�豸�ϡ�			*/
/*				2��ʹ�ô˹���ǰ����Ҫ����ʾDEMO�����ý����ϣ���������� */
/*				�ܴ���ʶ����(Ĭ��S485����ʶ����)��					*/
/************************************************************************/
int __stdcall CLIENT_LPRC_RS485Send(char *pCameraIP, int nCameraPort, char *chData, int nSendLen);

/************************************************************************/
/* CLIENT_LPRC_RS485SendEx: �°汾RS485͸������											*/
/*		Parameters:														*/
/*			pCameraIP[in]				����豸IP��ַ					*/
/*			nPort[in]					�˿�,Ĭ��9110					*/
/*			nRs485[in]	����485���ݵĽṹ��		*/
/*		Return Value:   int												*/
/*							0	�ɹ�									*/
/*						  ��0	ʧ��									*/
/*		notice��														*/
/*				1���û�ͨ���˽ӿڣ�������������ݣ�����豸��ԭ��������	*/
/*				ͨ��RS485�ӿ�ת����ȥ�����ͻ����ӵ��ⲿ�豸�ϡ�			*/
/*				2��ʹ�ô˹���ǰ����Ҫ����ʾDEMO�����ý����ϣ���������� */
/*				�ܴ���ʶ����(Ĭ��S485����ʶ����)��					*/
/*				3:�°汾����һ���Ը�����������5�����ݣ���������ÿ������*/
/*				͸����ʱ������									      */
/************************************************************************/
int __stdcall CLIENT_LPRC_RS485SendEx(char *pCameraIP, int nCameraPort, CLIENT_LPRC_RS485_Transparent_t nRs485);


/************************************************************************/
/* ����: Jpeg����Ϣ�����ʼ��											*/
/*		Parameters:														*/
/*			hWndHandle[in]:	������Ϣ�Ĵ�����							*/
/*			uMsg[in]:		�û��Զ�����Ϣ								*/
/*							��⵽�����ݲ�׼���û��������ݺ�			*/
/*							��::PostMessage ������hWndHandle����uMsg	*/
/*							��Ϣ������WPARAM����Ϊ0��LPARAM����Ϊ0		*/
/*			chIp[in]:		���IP��ַ								*/
/*		Return Value:   int												*/
/*							0	��ȡ�ɹ�								*/
/*							1	��ȡʧ��								*/
/************************************************************************/
int __stdcall CLIENT_LPRC_JpegMessageInit(HWND hWndHandle,UINT uMsg,char *chIp);


/************************************************************************/
/* 	����: ��Ϣ��ʽ��ȡָ��IP�������Jpeg������							*/
/*		Parameters:														*/
/*			chIp[in]:			���IP��ַ								*/
/*			chJpegBuf[in]:		�洢JPEG��buffer						*/
/*			nJpegBufLen[in]:	��ȡ����JPEG���ݳ���					*/
/*		Return Value:   int												*/
/*							0	��ȡ�ɹ�								*/
/*							1	��ȡʧ��								*/
/*		Notice:   														*/
/*			ʹ�ô˺���ǰ���ȵ���CLIENT_JpegMessageInit���������Զ�����Ϣ	*/
/************************************************************************/
int __stdcall CLIENT_LPRC_GetJpegStream(char *chIp, char *chJpegBuf, char *nJpegBufLen);


/************************************************************************/
/* 	����: ����IP��ַ���Ͽ�ָ���豸����									*/
/*		Parameters:														*/
/*			pCameraIP[in]:			���IP��ַ							*/
/*		Return Value:   int												*/
/*							0	��ȡ�ɹ�								*/
/*							1	��ȡʧ��								*/
/************************************************************************/
int __stdcall CLIENT_LPRC_QuitDevice(char *pCameraIP);


/************************************************************************/
/* CLIENT_LPRC_SetNetworkCardBind: �ֶ���ָ������IP							*/
/*		Parameters:														*/
/*			pCameraIP[in]		Ҫ�󶨵�����IP��ַ						*/
/*		Return Value:   int												*/
/*							0	�ɹ�									*/
/*						  ��0	ʧ��									*/
/*		notice:��PC�����ڶ����������ʱ���ֲ������Ϊ������ʱ����ͨ����	*/
/*				�����������ͨѶ������IP								*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SetNetworkCardBind(char *pCameraIP);
/*************************************************************************/
/*CLIENT_LPRC_SnapJpegFrame ����ץ��һ֡�����ֱ��淽ʽ��ֱ�ӱ��浽�̶�Ŀ¼���߱��浽�ض��ڴ�,Ҫ�Ǳ����ض��ڴ�ģʽ��Ҫ�����ڴ����ֵ,���ַ�ʽ��ѡ*/
/*		Parameters:														*/
/*			chIp[in]		   �����IP��ַ						*/
/*			pSaveFileName[in]  ·���ʹ�JPEG��׺�����ļ��������ڰѵ�ǰץ�ĵ���֡����Ϊ�ض��ļ�  Ĭ����ƥ���ļ���	*/
/*          pSaveBuf[in]       ���ڱ��浱ǰ֡���ض��ڴ��,������Ҫ�����ڴ�ɴ洢�����ֵ�����ļ���Ϊ�յ�ʱ������Ż���Ч��*/  
/*          Maxlen[in]         ���浱ǰ֡�ض��ڴ�����ֵ*/
/*		Return Value:   int												*/
/*						   0	���浽�ض�Ŀ¼�ɹ�									*/
/*                         >0   ���浽�ض��ڴ�����ݵ�ʵ�ʴ�С                                            */
/*						  -1	ʧ��									*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SnapJpegFrame(char *chIp,char *pSaveFileName,unsigned char *pSaveBuf,int Maxlen);

/************************************************************************/
/* CLIENT_LPRC_SetJpegStreamPlayOrStop: ����jpeg���Ŀ���							    */
/*		Parameters:														*/
/*		pCameraIP[in]		��Ҫ���õ�����豸��ip��ַ				    */
/*		onoff[in]			jpeg�������0��ʾ�ر�����1��ʾ����		*/
/*		Return Value:   	int											*/
/*							0	�ɹ�									*/
/*						  	��0	ʧ��									*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SetJpegStreamPlayOrStop(char *pCameraIP,int onoff);

/************************************************************************/
/* CLIENT_LPRC_SearchDeviceList:    �����豸IP�б�							    */
/*		Parameters:														*/
/*		pBuf[out]			�洢������������б���Ϣ�ṹ������		*/
/*		Return Value:   	int											*/
/*							����0	�ɹ����������豸��									*/
/*						  	-1	ʧ��									*/
/************************************************************************/
int __stdcall CLIENT_LPRC_SearchDeviceList(CLIENT_LPRC_DeviceInfo *pBuf);

/************************************************************************/
/* CLIENT_LPRC_AlterDeviceInfo:    �޸�ָ�����豸���豸��Ϣ							    */
/*		Parameters:														*/
/*		pCameraIP[in]		��Ҫ�޸ĵ�����豸��ip��ַ				    */
/*		pBuf[out]			�洢��Ҫ�޸ĵ��豸��Ϣ�ṹ��		*/
/*		Return Value:   	int											*/
/*							==0	�ɹ�									*/
/*						  	��0	ʧ��									*/
/************************************************************************/
int __stdcall CLIENT_LPRC_AlterDeviceInfo(char *pCameraIP,CLIENT_LPRC_DeviceInfo pBuf);

/************************************************************************/
/* CLIENT_LPRC_SetDevTimeParam:    �޸��豸ϵͳʱ��							    */
/*		Parameters:														*/
/*		pCameraIP[in]		��Ҫ�޸ĵ�����豸��ip��ַ				    */
/*		sysTime[in]			����ʱ��ṹ��		*/
/*		Return Value:   	int											*/
/*							==0	�ɹ�									*/
/*						  	��0	ʧ��									*/
/************************************************************************/

int __stdcall CLIENT_LPRC_SetDevTimeParam(char *pCameraIP, CLIENT_LPRC_CAMERA_TIME *sysTime);


#ifdef __cplusplus
}
#endif

#endif
