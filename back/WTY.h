#ifndef _WTY_H
#define _WTY_H

#if !defined(WIN32) && !defined(__stdcall)
#define __stdcall
#endif

#ifndef WIN32
typedef unsigned int UINT;
typedef void *HWND;
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define BIG_PICSTREAM_SIZE     200000-312///		//相机上传全景图占用内存的最大大小
#define SMALL_PICSTREAM_SIZE   10000////			//相机上传车牌截图占用内存的最大大小
#define BIG_PICSTREAM_SIZE_EX   1*800*1024-312		//相机上传全景图占用内存的最大大小
#define SMALL_PICSTREAM_SIZE_EX   10000			//相机上传车牌截图占用内存的最大大小

/* 相机时间 */
typedef struct 
{
	int Year;			/* 年 	*/
	int Month;			/* 月 	*/
	int Day;			/* 日 	*/
	int Hour;			/* 时 	*/
	int Minute;			/* 分 	*/
	int Second;			/* 秒 	*/
	int Millisecond;	/* 微妙 */
}camera_time;

/* 识别结果坐标 */
typedef struct 
{
	int 	Left;	/* 左 */
	int 	Top;	/* 上 */
	int 	Right;	/* 右 */
	int 	Bottom;	/* 下 */
}plate_location;

/* 识别结果 */
typedef struct 
{	
	char		chWTYIP[16];						/* 相机IP 				*/
	int			nFullLen;							/* 全景图像数据大小 	*/
	int			nPlateLen;							/* 车牌图像数据大小 	*/
	char		chFullImage[BIG_PICSTREAM_SIZE];	/* 全景图像数据 		*/
	char		chPlateImage[SMALL_PICSTREAM_SIZE];	/* 车牌图像数据 		*/
	char		chColor[8];							/* 车牌颜色 			*/
	char		chLicense[16];						/* 车牌号码 			*/
	plate_location 	pcLocation;						/* 车牌在图像中的坐标 	*/
	camera_time		shootTime;						/* 识别出车牌的时间 	*/
	int			nConfidence;						/* 车牌可信度			*/
	int			nTime;								/* 识别耗时				*/
	int			nDirection;							/*	车牌方向			*/
	char		reserved[256-12];
}plate_result;

/* 图像信息*/
typedef struct
{
	int				nWidth;							/* 宽度					*/
	int				nHeight;						/* 高度					*/
	int				nPitch;							/* 图像宽度的一行像素所占内存字节数*/
	int				nLen;							/* 图像的长度			*/
	char			reserved[16];					/* 预留     			*/
	unsigned char	*pBuffer;						/* 图像内存的首地址		*/
}LPRC_IMAGE_INFO;


/* 识别结果 */
typedef struct 
{	
	char				chWTYIP[16];						/* 相机IP 				*/
	char				chColor[8];							/* 车牌颜色 			*/
	char				chLicense[16];						/* 车牌号码 			*/
	plate_location 		pcLocation;							/* 车牌在图像中的坐标 	*/
	camera_time			shootTime;							/* 识别出车牌的时间 	*/
	int					nConfidence;						/* 车牌可信度			*/
	int					nTime;								/* 识别耗时				*/
	int					nDirection;							/* 车牌方向		    	*/
	char				reserved[256];						/* 预留     			*/
	LPRC_IMAGE_INFO		pFullImage;						/* 全景图像数据(注意：相机不传输，此处指针为空) */
	LPRC_IMAGE_INFO		pPlateImage;						/* 车牌图像数据(注意：相机不传输，此处指针为空) */
}plate_result_ex;

/* Jpeg流回调返回每一帧jpeg数据结构体 */
typedef struct 
{
	char			chIp[16];				/*相机IP												*/
	char			*pchBuf;				/*每一帧jpeg数据缓冲区									*/
	unsigned int	nLen;					/*当前jpeg帧的数据长度									*/
	int				nStatus;				/* 当前jpeg帧接收状态： 0-正常, 非0-不正常 				*/
	char			reserved[128];			/* 保留		  											*/
}DevData_info;
/*接收串口数据的结构体*/
typedef struct
{
	unsigned char *pData;//[1024];////串口数据指针
	int nsize;///串口数据大小
	char reserved[128];			/* 保留	*/
}DevSerial_data;
/************************************************************************/
/* WTY_InitSDK: 连接相机												*/
/*		Parameters:														*/
/*			nPort[in]:		连接相机的端口，现默认为8080				*/
/*			hWndHandle[in]:	接收消息的窗体句柄，当为NULL时，表示无窗体  */
/*			uMsg[in]:		用户自定义消息，当hWndHandle不为NULL时，	*/
/*							检测到有新的车牌识别结果并准备好当前车牌	*/
/*							缓冲区信息后，用::PostMessage 给窗口		*/
/*							hWndHandle发送uMsg消息，其中WPARAM参数为0，	*/
/*							LPARAM参数为0								*/
/*			chServerIP[in]:	相机的IP地址								*/
/*		Return Value:   int												*/
/*							0	相机连接成功							*/
/*							1	相机连接失败							*/
/*		Notice:   														*/
/*				如果采用回调的方式获取数据时，hWndHandle句柄为NULL，	*/
/*				uMsg为0，并且注册回调函数，通知有新的数据；				*/
/*				反之，在主窗口收到消息时，调用WTY_GetVehicleInfo获取	*/
/*				数据。													*/
/************************************************************************/
int __stdcall WTY_InitSDK(UINT nPort,HWND hWndHandle,UINT uMsg,char *chServerIP);

/************************************************************************/
/* WTY_QuitSDK: 断开所有已经连接设备，释放资源							*/
/*		Parameters:														*/
/*		Return Value:   void											*/
/************************************************************************/
void  __stdcall WTY_QuitSDK();


/***********************************************************************************/
/* 回调函数:循环检测并通知相机设备通讯状态的回调函数						       */
/*		Parameters:														           */
/*			chWTYIP[out]:		返回设备IP								           */
/*			nStatus[out]:		设备状态：0表示网络异常或设备异常；			       */
/*										  1表示网络正常，设备已连接		    	   */
/*										  2表示网络正常，设备已连接，但相机有重启  */
/*		Return Value:   void											           */
/***********************************************************************************/
typedef void (*WTYConnectCallback)(char *chWTYIP,UINT nStatus);


/***********************************************************************************/
/* 回调函数:获取相机485发送的数据						       */
/*		Parameters:														           */
/*			chWTYIP[out]:		返回设备IP								           */
/*          serialData[out]          串口数据地址										*/
/*			nlen[out]				串口数据大小										*/
/*		Return Value:   void											           */
/***********************************************************************************/
typedef void (*WTYSerialDataCallback)(char *chWTYIP,DevSerial_data *pSerialData);


/************************************************************************/
/* WTY_RegWTYConnEvent: 注册相机通讯状态的回调函数						*/
/*		Parameters:														*/
/*			WTYConnect[in]:		ConnectCallback类型回调函数				*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall WTY_RegWTYConnEvent (WTYConnectCallback WTYConnect);


/************************************************************************/
/* WTY_CheckStatus: 主动检查相机设备的通讯状态							*/
/*		Parameters:														*/
/*			chWTYIP[in]:		要检查的相机的IP						*/
/*		Return Value:   int												*/
/*							0	正常									*/
/*							1	网络不通								*/
/************************************************************************/
int __stdcall WTY_CheckStatus (char *chWTYIP);


/************************************************************************/
/* 回调函数: 获取识别结果的回调函数										*/
/*		Parameters:														*/
/*			chWTYIP[out]:		返回识别结果的设备IP					*/
/*			chPlate[out]:		车牌号码								*/
/*			chColor[out]:		车牌颜色								*/
/*			chFullImage[out]:	全景图数据								*/
/*			nFullLen[out]:		全景图数据长度							*/
/*			chPlateImage[out]:	车牌图数据								*/
/*			nPlateLen[out]:		车牌图数据长度							*/
/*		Return Value:   void											*/
/************************************************************************/
typedef void (*WTYDataCallback)(char *chWTYIP, 
							    char *chPlate,
							    char *chColor,
								char *chFullImage,
								int nFullLen, 
								char *chPlateImage,
							    int nPlateLen);

								
/************************************************************************/
/* 回调函数: 注册接收识别数据回调函数									*/
/*		Parameters:														*/
/*			recResult[out]:		识别结果数据							*/
/*		Return Value:   void											*/
/************************************************************************/
typedef void (*WTYDataExCallback)(plate_result *recResult);


/************************************************************************/
/* 回调函数: 注册接收识别数据回调函数									*/
/*		Parameters:														*/
/*			recResult[out]:		识别结果数据							*/
/*		Return Value:   void											*/
/*	Note:																*/
/*		新扩展的回调函数，方便用户接收清晰度较高或分辨率较高的JPEG图像	*/
/************************************************************************/
typedef void (*WTYDataEx2Callback)(plate_result_ex *recResultEx);


/************************************************************************/
/* WTY_RegDataEvent: 注册获取识别结果的回调函数							*/
/*		Parameters:														*/
/*			WTYData[in]:		处理识别结果的回调函数的指针			*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall WTY_RegDataEvent(WTYDataCallback WTYData);

/************************************************************************/
/* WTY_RegSerialDataEvent: 注册获取串口数据的回调函数							*/
/*		Parameters:														*/
/*			WTYSerialData[in]:		处理接收串口数据的回调函数的指针			*/
/*		Return Value:   void											*/
/************************************************************************/
void __stdcall WTY_RegSerialDataEvent(WTYSerialDataCallback WTYSerialData);



/************************************************************************/
/* WTY_RegDataExEvent: 注册获取识别结果的回调函数						*/
/*		Parameters:														*/
/*			WTYData[in]:		处理识别结果的回调函数的指针			*/
/*		Return Value:   void											*/
/*	Note:																*/
/*		接收清晰度较低，或分辨率较低的JPEG图像							*/
/************************************************************************/
void __stdcall WTY_RegDataExEvent(WTYDataExCallback WTYDataEx);


/************************************************************************/
/* WTY_RegDataEx2Event: 注册获取识别结果的回调函数						*/
/*		Parameters:														*/
/*			WTYData[in]:		处理识别结果的回调函数的指针			*/
/*		Return Value:   void											*/
/*	Note:																*/
/*		接收清晰度较高，或分辨率较高的JPEG图像							*/
/************************************************************************/
void __stdcall WTY_RegDataEx2Event(WTYDataEx2Callback WTYDataEx2);


/************************************************************************/
/* 	函数: 消息方式获取指定IP相机识别结果。								*/
/*		  当WTY_initSDK函数中设置了窗体句柄和消息时，					*/
/*		  需要在消息处理函数中调用此函数来主动获取识别结果。			*/
/*		Parameters:														*/
/*			chWTYIP[in]:		根据消息，获取指定IP设备识别数据		*/
/*			chPlate[in]:		车牌号码								*/
/*			chColor[in]:		车牌颜色								*/
/*			chFullImage[in]:	全景图数据								*/
/*			nFullLen[in]:		全景图数据长度							*/
/*			chPlateImage[in]:	车牌图数据								*/
/*			nPlateLen[in]:		车牌图数据长度							*/
/*		Return Value:   int												*/
/*							0	获取成功								*/
/*							1	获取失败								*/
/*		Notice:   														*/
/*			当设置了传输内容不传时，各自对应的数据为NULL，长度为-1；	*/
/*			当没有形成数据时，各自对应数据为NULL，长度为0				*/
/************************************************************************/
int __stdcall WTY_GetVehicleInfo(char *chWTYIP,
					   char *chPlate,	
					   char *chColor,
					   void *chFullImage ,	
					   int *nFullLen, 
					   void *chPlateImage,
					   int *nPlateLen);


/************************************************************************/
/* 	函数: 消息方式获取指定IP的相机识别结果。							*/
/*		  当WTY_initSDK函数中设置了窗体句柄和消息时，					*/
/*		  需要在消息处理函数中调用此函数来主动获取识别结果。			*/
/*		Parameters:														*/
/*			chWTYIP[in]:		根据消息，获取指定IP设备识别数据		*/
/*			chPlate[in]:		车牌号码								*/
/*			chColor[in]:		车牌颜色								*/
/*			chFullImage[in]:	全景图数据								*/
/*			nFullLen[in]:		全景图数据长度							*/
/*			chPlateImage[in]:	车牌图数据								*/
/*			nPlateLen[in]:		车牌图数据长度							*/
/*			chTwoImage[in]:		二值化数据								*/
/*			nTwoLen[in]:		二值化数据长度							*/
/*		Return Value:   int												*/
/*							0	获取成功								*/
/*							1	获取失败								*/
/*		Notice:   														*/
/*			当设置了传输内容不传时，各自对应的数据为NULL，长度为-1；	*/
/*			当没有形成数据时，各自对应数据为NULL，长度为0				*/
/************************************************************************/
int __stdcall WTY_GetVehicleInfoEx(char *chWTYIP,
								 char *chPlate,	
								 char *chColor,
								 void *chFullImage ,
								 int *nFullLen,
								 void *chPlateImage,
								 int *nPlateLen,
								 void *chTwoImage,
								 int *nTwoLen);

/************************************************************************/
/* WTY_SetSavePath: 如果用户需要动态库自动保存图片，可以通过该函数设置保*/
/*					存图片的路径。										*/
/*		Parameters:														*/
/*			chSavePath[in]:	文件存储路径，以"\\"结束，如："D:\\Image\\"	*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:   														*/
/*			全景图：指定目录\\设备IP\\年月日（YYYYMMDD）\\FullImage\\	*/
/*									时分秒-毫秒__颜色_车牌号码__.jpg	*/
/*			车牌图：指定目录\\设备IP\\年月日（YYYYMMDD）\\PlatelImage\\	*/
/*									时分秒-毫秒__颜色_车牌号码__.jpg	*/
/************************************************************************/
void __stdcall WTY_SetSavePath (char *chSavePath);


/************************************************************************/
/* WTY_SetTrigger: 触发识别												*/
/*		Parameters:														*/
/*			pCameraIP[in]:			要触发的相机设备的IP				*/
/*			nCameraPort[in]:		端口,默认8080						*/
/*		Return Value:													*/
/*					0	触发成功返回									*/
/*				  非0	失败											*/
/************************************************************************/	
int __stdcall WTY_SetTrigger(char *pCameraIP, int nCameraPort);


/************************************************************************/
/* WTY_SetTransContent: 控制相机设备上传内容					        */
/*		Parameters:														*/
/*			pCameraIP[in]:		要设置的设备IP							*/
/*			nCameraPort[in]:	端口,默认8080							*/
/*			nFullImg[in]:		全景图，0表示不传，1表示传				*/
/*			nPlateImg[in]:		车牌图，0表示不传，1表示传				*/
/*		Return Value:   int												*/
/*							0	成功									*/
/*						  非0	失败									*/
/************************************************************************/
int __stdcall WTY_SetTransContent (char *pCameraIP, int nCameraPort, int nFullImg, int nPlateImg);


/************************************************************************/
/* 函数说明: 控制继电器的闭合											*/
/*		Parameters:														*/
/*			pCameraIP[in]:			相机IP								*/
/*			nCameraPort[in]:		端口,默认8080						*/
/*		Return Value:   int												*/
/*							0	设置成功								*/
/*						  非0	失败									*/
/*		Notice:   														*/
/*				通过此功能，可以在PC端通过一体机设备，来控制道闸的抬起	*/
/*				设备继电器输出信号为：开关量信号。						*/
/************************************************************************/
int __stdcall WTY_SetRelayClose(char *pCameraIP, int nCameraPort);

int __stdcall WTY_DropRod(char *pCameraIP, int nCameraPort);


/************************************************************************/
/* 回调函数: 获取Jpeg流的回调函数										*/
/*		Parameters:														*/
/*			JpegInfo[out]:		JPEG流数据信息							*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:一台PC连接多台设备时，此函数仅需实现一次。当区分不同		*/
/*			设备的JPEG流时，可以通过输出参数中DevData_info中的chIp来	*/
/*			区分.														*/
/*			2:此功能目前适用于V5.5.3.0、V6.0.0.0及以上版本,				*/
/*			  V5.2.1.0、V5.2.2.0、V5.2.6.0等版本不能使用此功能			*/
/************************************************************************/
typedef void (*WTYJpegCallback)(DevData_info *JpegInfo);


/************************************************************************/
/* WTY_RegJpegEvent: 注册获取Jpeg流的回调函数							*/
/*		Parameters:														*/
/*			JpegInfo[in]:		WTYJpegCallback类型回调函数				*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:一台PC连接多台设备时，此函数仅需实现一次。当区分不同		*/
/*			设备的JPEG流时，可以通过输出参数中DevData_info中的chIp来	*/
/*			区分.														*/
/*			2:此功能目前适用于V5.5.3.0、V6.0.0.0及以上版本,				*/
/*			  V5.2.1.0、V5.2.2.0、V5.2.6.0等版本不能使用此功能			*/
/************************************************************************/
void __stdcall WTY_RegJpegEvent(WTYJpegCallback JpegInfo);


/************************************************************************/
/* 回调函数: 获取报警信息的回调函数										*/
/*		Parameters:														*/
/*			alarmInfo[out]:		报警信息								*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			一台PC连接多台设备时，此函数仅需实现一次。当区分不同设备	*/
/*			的Alarm时，可以通过输出参数中DevData_info中的chIp来区分		*/
/*																		*/
/*		Notice:															*/
/*			1:一台PC连接多台设备时，此函数仅需实现一次。当区分不同		*/
/*			设备的JPEG流时，可以通过输出参数中DevData_info中的chIp来	*/
/*			区分.														*/
/*			2:此功能目前适用于V5.5.3.0、V6.0.0.0及以上版本,				*/
/*			  V5.2.1.0、V5.2.2.0、V5.2.6.0等版本不能使用此功能			*/
/************************************************************************/
typedef void (*WTYAlarmCallback)(DevData_info *alarmInfo);


/************************************************************************/
/* WTY_RegAlarmEvent: 注册获取报警信息的回调函数						*/
/*		Parameters:														*/
/*			AlarmInfo[in]:		WTYAlarmCallback类型回调函数			*/
/*		Return Value:   void											*/
/*																		*/
/*		Notice:															*/
/*			1:一台PC连接多台设备时，此函数仅需实现一次。当区分不同		*/
/*			设备的JPEG流时，可以通过输出参数中DevData_info中的chIp来	*/
/*			区分.														*/
/*			2:此功能目前适用于V5.5.3.0、V6.0.0.0及以上版本,				*/
/*			  V5.2.1.0、V5.2.2.0、V5.2.6.0等版本不能使用此功能			*/
/************************************************************************/

void __stdcall WTY_RegAlarmEvent(WTYAlarmCallback AlarmInfo);


/************************************************************************/
/* WTY_RS485Send: RS485透明传输											*/
/*		Parameters:														*/
/*			pCameraIP[in]				相机设备IP地址					*/
/*			nPort[in]					端口,默认8080					*/
/*			chData[in]					将要传输的数据块的首地址		*/
/*			nSendLen[in]				将要传输的数据块的字节数		*/
/*		Return Value:   int												*/
/*							0	成功									*/
/*						  非0	失败									*/
/*		notice：														*/
/*				1：用户通过此接口，往相机发送数据，相机设备会原样将数据	*/
/*				通过RS485接口转发出去，到客户所接的外部设备上。			*/
/*				2：使用此功能前，需要在演示DEMO的设置界面上，设置相机不 */
/*				能传输识别结果(默认S485传输识别结果)。					*/
/************************************************************************/
int __stdcall WTY_RS485Send(char *pCameraIP, int nCameraPort, char *chData, int nSendLen);


/************************************************************************/
/* 函数: Jpeg流消息处理初始化											*/
/*		Parameters:														*/
/*			hWndHandle[in]:	接收消息的窗体句柄							*/
/*			uMsg[in]:		用户自定义消息								*/
/*							检测到有数据并准备好缓冲区数据后，			*/
/*							用::PostMessage 给窗口hWndHandle发送uMsg	*/
/*							消息，其中WPARAM参数为0，LPARAM参数为0		*/
/*			chIp[in]:		相机IP地址								*/
/*		Return Value:   int												*/
/*							0	获取成功								*/
/*							1	获取失败								*/
/************************************************************************/
int __stdcall WTY_JpegMessageInit(HWND hWndHandle,UINT uMsg,char *chIp);


/************************************************************************/
/* 	函数: 消息方式获取指定IP的相机的Jpeg流数据							*/
/*		Parameters:														*/
/*			chIp[in]:			相机IP地址								*/
/*			chJpegBuf[in]:		存储JPEG的buffer						*/
/*			nJpegBufLen[in]:	获取到的JPEG数据长度					*/
/*		Return Value:   int												*/
/*							0	获取成功								*/
/*							1	获取失败								*/
/*		Notice:   														*/
/*			使用此函数前需先调用WTY_JpegMessageInit函数设置自定义消息	*/
/************************************************************************/
int __stdcall WTY_GetJpegStream(char *chIp, char *chJpegBuf, char *nJpegBufLen);


/************************************************************************/
/* 	函数: 根据IP地址，断开指定设备链接									*/
/*		Parameters:														*/
/*			pCameraIP[in]:			相机IP地址							*/
/*		Return Value:   int												*/
/*							0	获取成功								*/
/*							1	获取失败								*/
/************************************************************************/
int __stdcall WTY_QuitDevice(char *pCameraIP);


/************************************************************************/
/* WTY_SetNetworkCardBind: 手动绑定指定网卡IP							*/
/*		Parameters:														*/
/*			pCameraIP[in]		要绑定的网卡IP地址						*/
/*		Return Value:   int												*/
/*							0	成功									*/
/*						  非0	失败									*/
/*		notice:当PC机存在多网卡的情况时，又不想禁用为单网卡时，可通过该	*/
/*				函数绑定与相机通讯的网卡IP								*/
/************************************************************************/
int __stdcall WTY_SetNetworkCardBind(char *pCameraIP);


#ifdef __cplusplus
}
#endif

#endif
