// cts_sdk.h : main header file for the CTS_SDK DLL
//

#if !defined(__CTS_SDK_H__)
#define __CTS_SDK_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////       术语解释        ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// Term  : 终端(Terminal)、分机
// Dev   : 设备(跟Term一样)
// Req   : 请求
// Asw   : 应答
// Post  : 主机主动通知 或 主机主动推送
// Event : 事件类型的回调通知
// Cnn   : 连接
// User  : 调用SDK的应用程序


// 特别说明 : 请求/应答是在网络两端异步操作的。所有请求命令都会有对应的应答返回，
//            并从回调函数中返回请求结果。
///////////////////////////////////////////////////////////////////////////////////



// SDK 系统容量说明:
// 最多支持 6000 个终端设备(Term)同时在线工作
#define MAX_TERM_COUNT     6000  // SDK所支持的最大终端数量
#define MAX_BROAD_GROUP    300   // 60    // 最大广播分组数量
#define MAX_BROAD_TERM     300   // 100   // 每个广播分组的最大终端数量

#define MAX_AUDVID_NUM     36   // 压缩音频视频播放号范围：1 =< AudVidNum =< 36
#define MAX_PCM_NUM        16   // PCM音频播放号范围：     1 =< PcmNum    =< 16

#define MAX_NVR_CHAN       40   // NVR通道范围： 1 =< nChanNum =< 40

#define MAX_FILE_CNT_PER_POST  50


typedef enum
{
	CERR_SUCCESS                =  0,
	CERR_UNKNOWN                = -1,

	CERR_NOT_INIT               = -801,
	CERR_INVALID_PARAMETER      = -802,
	CERR_CONNECT_TIMEOUT        = -803,
	CERR_SOCKET_RECV_ERROR      = -804,
	CERR_ALREADY_CONNECTED      = -805,
	CERR_NOT_CONNECTED          = -806,
	CERR_DEVICE_BUSY            = -807,
	CERR_HOST_REGISTER_ERROR    = -908,
	CERR_INVALID_DEV_STATE      = -909,
	CERR_LISTEN_PORT_ERROR      = -910,
	CERR_OPEN_FILE_ERROR        = -911,
	CERR_INVALID_FILE_PATH      = -912,
	CERR_INVALID_FILE_SIZE      = -913,
	CERR_INVALID_FILE_DATA      = -914,
	CERR_TERM_IS_OCCUPIED       = -915,
	CERR_GROUP_NOT_SETUP        = -916,
	CERR_TERM_LIST_FULL         = -917,
	CERR_DATA_NOT_SYNC          = -918,
	CERR_INVALID_BIOS_VERSION   = -919,
	CERR_INVALID_DEV_TYPE       = -920,
	CERR_CHAN_NOT_REGISTERED    = -921,

	CERR_HST_NUM_OCCUPIED       = -701,
	CERR_TRM_NUM_OCCUPIED       = -702,
	CERR_HST_NUM_NOT_EXISTENT   = -703,
	CERR_TRM_NUM_NOT_EXISTENT   = -704,
	CERR_HST_IS_IN_CALLING      = -705,
	CERR_TRM_NUM_NOT_FIT_HOST   = -706,
	CERR_HST_CNT_REACH_MAX      = -707,
	CERR_GROUP_NUM_NOT_EXISTENT = -708,
	CERR_DEV_NUM_OCCUPIED       = -709,
	CERR_DEV_NOT_REGISTERED     = -710,

}enSdkErrCode;

typedef enum
{
	CB_Asw_OpenTermAudVid  = 101, // 应答打开终端音频视频
	CB_Asw_CloseTermAudVid = 102, // 应答关闭终端音频视频
	CB_Asw_OpenTermTalk    = 103, // 应答打开终端对讲
	CB_Asw_CloseTermTalk   = 104, // 应答关闭终端对讲
	CB_Asw_GetAudLevel     = 105, // 应答获取终端音量级别
	CB_Asw_SetAudLevel     = 106, // 应答设置终端音量级别
	CB_Asw_DismissTerm     = 107, // 应答解除分机呼叫
	CB_Asw_GetDbAlmCfg     = 108, // 应答获取终端喧哗报警配置
	CB_Asw_SetDbAlmCfg     = 109, // 应答设置终端喧哗报警配置
	CB_Asw_NvrSrchRecFile  = 110, // 应答NVR查找录音文件结果
	CB_Asw_NvrPlayRecFile  = 111, // 应答NVR回放录音文件结果

	CB_Post_TermSos        = 201, // 推送终端呼叫
	CB_Post_TermState      = 202, // 推送终端状态
	CB_Post_485PipeData    = 203, // 推送485管道数据(接收)
	CB_Post_Mp3PlayFinish  = 204, // 推送MP3文件播放结束通知
	CB_Post_AlmNotify      = 205, // 推送报警通知
	CB_Post_NvrChanNotify  = 206, // 推送NVR通道通知
	CB_Post_TermDbValL     = 207, // 推送网络拾音器第一声道音量值
	CB_Post_TermDbValR     = 208, // 推送网络拾音器第二声道音量值
	CB_Post_NvrSrchRecFile = 209, // 推送NVR查找录音文件列表信息
	CB_Post_NvrPlayProg    = 210, // 推送NVR录音文件回放进度

	CB_Event_TermRegister  = 301, // 终端请求注册通知，返回-1代表注册失败，返回0代表注册成功
	CB_Event_TermConnect   = 302, // 终端请求连接通知，返回-1代表不允许连接，返回0代表允许连接
	CB_Event_TermCnnLost   = 303, // 终端断开连接通知

	CB_Data_TermAudio      = 401, // 终端的音频，已经压缩过的，需要调用SDK播放接口才能播放
	CB_Data_TermVideo      = 402, // 终端的H264标准视频帧，可以调用SDK播放接口播放
	CB_Data_PcMicAudio     = 403, // 电脑的音频，为PCM帧，8K采样率，16位/样
	CB_Data_BypassAudio    = 404, // 旁路音频，为PCM帧，8K采样率，16位/样
	CB_Data_TermMp3L       = 405, // 网络拾音器第一声道MP3音频
	CB_Data_TermMp3R       = 406, // 网络拾音器第二声道MP3音频
	CB_Data_TermPcmL       = 407, // 网络拾音器第一声道PCM音频(16K采样率16位)
	CB_Data_TermPcmR       = 408, // 网络拾音器第二声道PCM音频(16K采样率16位)
	CB_Data_NvrMp3         = 409, // 录音主机MP3音频
	CB_Data_NvrPcm         = 410, // 录音主机PCM音频(16K采样率16位)
	CB_Data_NvrPlayMp3     = 411, // 录音主机回放MP3音频

}enSdkCbType;

typedef enum
{
	TSDK_DEV_STATE_OFFLINE   = 0,  // 离线
	TSDK_DEV_STATE_ONLINE    = 1,  // 在线
	TSDK_DEV_STATE_PLAYING   = 2,  // 正在播放mp3
	TSDK_DEV_STATE_TALKING   = 3,  // 正在对讲
	TSDK_DEV_STATE_TALK_PLAY = 4   // 正在对讲和播放mp3
}enSdkDevState;

typedef enum
{
	TSDK_VIDEO = 1,
	TSDK_AUDIO = 2
}enSdkAudVidType;

typedef enum
{
	SDK_ACAP_LEVEL_1, // 2米
	SDK_ACAP_LEVEL_2, // 4米
	SDK_ACAP_LEVEL_3, // 6米
	SDK_ACAP_LEVEL_4  // 8米
}enSdkAudCapLevel; // 终端监听输入音量级别

typedef enum
{
	SDK_AAMP_LEVEL_0, // 0级 : 静音
	SDK_AAMP_LEVEL_1, // 1级
	SDK_AAMP_LEVEL_2, // 2级
	SDK_AAMP_LEVEL_3, // 3级
	SDK_AAMP_LEVEL_4, // 4级
	SDK_AAMP_LEVEL_5, // 5级
	SDK_AAMP_LEVEL_6, // 6级
	SDK_AAMP_LEVEL_7, // 7级
	SDK_AAMP_LEVEL_8, // 8级
	SDK_AAMP_LEVEL_9, // 9级
	SDK_AAMP_LEVEL_10 // 10级 : 音量最大
}enSdkAudAmpLevel; // 终端功放输出音量级别

typedef enum
{
	SDK_ACAP_TALK_LEVEL_1, // 0.5米
	SDK_ACAP_TALK_LEVEL_2, // 0.8米
	SDK_ACAP_TALK_LEVEL_3, // 1.2米
	SDK_ACAP_TALK_LEVEL_4  // 1.8米
}enSdkAudCapTalkLevel; // 终端对讲输入音量级别

typedef enum
{
	SDK_ALMTYPE_ALARM_IN1 = 0x00000004, // 报警输入1
	SDK_ALMTYPE_ALARM_IN2 = 0x00000010, // 报警输入2
	SDK_ALMTYPE_DB_ALARM  = 0x00000020, // 喧哗报警
}enSdkAlarmType;

typedef enum
{
	TSDK_DEV_TYPE_UNDEF  = 0,  // 未知设备类型
	TSDK_DEV_TYPE_TALK   = 1,  // 对讲终端 或 对讲广播终端
	TSDK_DEV_TYPE_BROAD  = 2,  // 广播终端
	TSDK_DEV_TYPE_MP3    = 3,  // 网络拾音器
	TSDK_DEV_TYPE_NVR    = 4,  // 录音主机
}enSdkDevType;

typedef enum
{
	PCM_32000   = 1,  // 32K   采样率
	PCM_44100   = 2,  // 44.1K 采样率
	PCM_48000   = 3   // 48K   采样率
}enPcmSmpRate;

typedef enum
{
	NVR_CHAN_ADD,
	NVR_CHAN_DELETE,
	NVR_CHAN_MODIFY,
	NVR_CHAN_STATE_CHANGE
}enSdkNvrNotifyType;

typedef struct
{
	DWORD TermList[MAX_BROAD_TERM]; // 终端ID列表，空白区请用 0 填充
}TSdkGroupTermList;

typedef struct
{
	enSdkAudCapLevel	 eCapLevel;
	enSdkAudAmpLevel	 eAmpLevel; // 输出音量级别：0 ～ 10 (0静音,10最大)
	enSdkAudCapTalkLevel eCapTalkLevel;
}TSdkAudioLevel;

typedef struct
{
	enSdkAudCapLevel     eCapLevel;
	int                  nAmpValue; // 输出音量值：0 ～ 100 (0静音,100最大)
	enSdkAudCapTalkLevel eCapTalkLevel;
}TSdkAudioLevelEx;

typedef struct
{
	BOOL bDbAlmEnable;
	int nDbAlmVal;
}TSdkDbAlmCfg;

typedef struct
{
	char Name[64];
	int nSize;
}TSdkNvrFileInfo;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswOpenTermAudVid;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswCloseTermAudVid;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswOpenTermTalk;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswCloseTermTalk;

typedef struct
{
	DWORD dwTermID;
	TSdkAudioLevel Level;
	int            nAmpValue; // 输出音量值：0 ～ 100 (0静音,100最大)
}TSdkAswGetAudLevel;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
	enSdkAudAmpLevel eAmpLevel; // 应答设置分机的输出音量级别：0 ～ 10 (0静音,10最大)
	int              nAmpValue; // 应答设置分机的输出音量值：0 ～ 100 (0静音,100最大)
	BOOL             bIsEx;     // TRUE:应答调用TSDK_Req_SetAudLevelEx() , FALSE:应答调用TSDK_Req_SetAudLevel()
}TSdkAswSetAudLevel;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswDismissTerm;

typedef struct
{
	DWORD dwTermID;
	TSdkDbAlmCfg Cfg;
}TSdkAswGetDbAlmCfg;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
}TSdkAswSetDbAlmCfg;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	int nChanNum;
	DWORD dwTermID;
}TSdkAswNvrSrchRecFile;

typedef struct
{
	int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
	DWORD dwTermID;
	int nChanNum;
	int nTotalFileSize;  // 字节大小
	int nTotalRecTime;   // 录音文件时长:秒
}TSdkAswNvrPlayRecFile;

typedef struct
{
	DWORD dwTermID;
	int nSosType; // 0:咨询呼叫 , 1:紧急呼叫
}TSdkPostTermSos;

typedef struct
{
	DWORD dwTermID;
	enSdkDevState eTermState;
	BYTE AlmInMask;   // 报警输入通道有效指示码，每位对应一路报警: 0(通道无效), 1(通道有效)。暂时只支持2路报警输入，第1位为通道1，第2位为通道2。
	BYTE AlmInState;  // 报警输入通道状态指示码，每位对应一路报警: 0(无报警), 1(有报警)。
	BYTE AlmOutMask;  // 报警输出通道有效指示码，每位对应一路报警: 0(通道无效), 1(通道有效)。暂时只支持2路报警输出，第1位为通道1，第2位为通道2。
	BYTE AlmOutState; // 报警输出通道状态指示码，每位对应一路报警: 0(无报警), 1(有报警)。
}TSdkPostTermState;

typedef struct
{
	DWORD dwTermID;
	int nFrmLen;
	BYTE FrmData[256]; // Max 485 Frame Len : 255
}TSdkPost485PipeData;

typedef struct
{
	int nBroadNum;
	int nResult; // 播放结果，成功为0，否则为错误代码:enSdkErrCode 
}TSdkPostMp3PlayFinish;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum; 
	enSdkAlarmType eAlmType;
}TSdkPostAlmNotify;

typedef struct
{
	DWORD dwTermID;
	enSdkNvrNotifyType eType;
	int  nChanNum;
}TSdkPostNvrChanNotify;

typedef struct
{
	DWORD dwTermID;
	int    nDbVal;
}TSdkPostTermDbValL;

typedef struct
{
	DWORD dwTermID;
	int    nDbVal;
}TSdkPostTermDbValR;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	int   nFileCnt;
	TSdkNvrFileInfo FileList[MAX_FILE_CNT_PER_POST];
}TSdkPostNvrSrchRecFile;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	int   nPlayingFilePos; // 当前回放的文件字节位置
}TSdkPostNvrPlayProg;

typedef struct
{
	enSdkDevType eTermType;
	DWORD dwTermID;    // 终端设备ID号，高16位为“配置工具”注册设备的“区域号”(1～10)，低16位为“配置工具”注册设备的“设备号”(1～600)
	BYTE TermMac[6];   // 终端MAC地址
	char TermIp[16];   // 终端IP地址
	char TermName[32]; // 终端名称
}TSdkEventTermRegister;

typedef struct
{
	enSdkDevType eTermType;
	DWORD dwTermID;    // 终端设备ID号，高16位为“配置工具”注册设备的“区域号”(1～10)，低16位为“配置工具”注册设备的“设备号”(1～600)
	BYTE TermMac[6];   // 终端MAC地址
	char TermIp[16];   // 终端IP地址
	char TermName[32]; // 终端名称
	BOOL bHasVideo;    // TRUE:有视频和音频 , FALSE:无视频只有音频
	BOOL bTalkbackEnable; // 只针对网络拾音器，TRUE:开启对讲模式，FALSE:关闭对讲模式
}TSdkEventTermConnect;

typedef struct
{
	DWORD dwTermID;
}TSdkEventTermCnnLost;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pAudData;
	int    nDataSize;
}TSdkDataTermAudio;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pVidFrm;      // H264视频帧
	int    nFrmSize;     // 帧大小
	BOOL   bIsKeyFrm;    // TRUE:为I帧 , FALSE:为P帧
}TSdkDataTermVideo;

typedef struct
{
	LPBYTE pPcmData;
	int    nDataSize;
}TSdkDataPcMicAudio;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pPcmData;
	int    nDataSize;
}TSdkDataBypassAudio;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pMp3Data;
	int    nDataSize;
}TSdkDataTermMp3L;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pMp3Data;
	int    nDataSize;
}TSdkDataTermMp3R;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pPcmData;
	int    nDataSize;
}TSdkDataTermPcmL;

typedef struct
{
	DWORD dwTermID;
	LPBYTE pPcmData;
	int    nDataSize;
}TSdkDataTermPcmR;

typedef struct
{
	char UserData[32+1];
	char WriteDate[8+1];
}TSdkTermUserData;

typedef struct
{
	int  nChanNum;      // 1～MAX_NVR_CHAN
	BOOL bIsLocal;      // 1:本地模拟拾音器 , 0:网络拾音器
	BOOL bValid;        // 1:有效(有设备注册) , 0:无效(无设备注册)
	char ChanName[32];  // 通道名称
	BOOL bIsOnline;     // 1:在线 , 0:不在线
	BOOL bIsRecording;  // 1:正在录像 , 0:停止录像
}TSdkNvrChan;

typedef struct
{
	TSdkNvrChan ChanList[MAX_NVR_CHAN];
}TSdkNvrChanList;

typedef struct
{
	WORD Year;   // 年 : 2018 ~ 2999
	BYTE Month;  // 月 : 1 ~ 12
	BYTE Day;    // 日 : 1 ~ 31
	WORD Hour;   // 时 : 0 ~ 23
	BYTE Minute; // 分 : 0 ~ 59
	BYTE Second; // 秒 : 0 ~ 59
	DWORD MillSecond; // 毫秒 : 0 ~ 999
}TSdkTime;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pMp3Data;
	int    nDataSize;
	TSdkTime Time; // 时钟戳
}TSdkDataNvrMp3;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pPcmData;
	int    nDataSize;
	TSdkTime Time; // 时钟戳
}TSdkDataNvrPcm;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pMp3Data;
	int    nDataSize;
	TSdkTime Time; // 时钟戳
}TSdkDataNvrPlayMp3;




///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// 此回调函数相当重要，所有设备注册、设备连接、音频视频数据、操作应答、状态事件 等都由回调函数返回，
// 所以要注意：回调函数不能阻塞 !!!!!!
typedef int (CALLBACK* ON_CTS_SDK_CALL_BACK)(enSdkCbType eCbType, LPVOID pParam, DWORD dwSize, int usr_data);


// 对讲
int WINAPI TSDK_Init(ON_CTS_SDK_CALL_BACK CbFunc, BOOL bServerMode, int usr_data); // SDK 初始化。
int WINAPI TSDK_DeInit(); // SDK 注销。

int WINAPI TSDK_Req_OpenTermAudVid(DWORD dwTermID); // 请求打开终端实时音频视频。
int WINAPI TSDK_Req_CloseTermAudVid(DWORD dwTermID); // 请求关闭终端实时音频视频。

int WINAPI TSDK_Req_OpenTermTalk(DWORD dwTermID);   // 请求与终端打开对讲。
int WINAPI TSDK_Req_OpenTermTalk_R(DWORD dwTermID); // 请求与终端打开对讲（使用右声道，只支持网络拾音器）。
int WINAPI TSDK_Req_CloseTermTalk(DWORD dwTermID);  // 请求与终端关闭对讲。

int WINAPI TSDK_Send_485PipeData(DWORD dwTermID, LPBYTE pFrmData, int nFrmLen); // 发送485管道数据
int WINAPI TSDK_Send_TalkAudio(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // 发送对讲音频数据，音频格式为：PCM音频，8K采样率，16位/样,  nDataLen:必须为320倍数，建议640或960

int WINAPI TSDK_Req_GetAudLevel(DWORD dwTermID); // 请求获取终端音量级别
int WINAPI TSDK_Req_SetAudLevel(DWORD dwTermID, TSdkAudioLevel * pLevel); // 请求设置终端音量级别
int WINAPI TSDK_Req_SetAudLevelEx(DWORD dwTermID, TSdkAudioLevelEx * pLevel); // 请求设置终端音量值和级别。

int WINAPI TSDK_Req_DismissTerm(DWORD dwTermID);  // 请求解除分机呼叫

int WINAPI TSDK_Req_GetDbAlmCfg(DWORD dwTermID); // 请求获取终端喧哗报警配置
int WINAPI TSDK_Req_SetDbAlmCfg(DWORD dwTermID, TSdkDbAlmCfg * pCfg); // 请求设置终端喧哗报警配置

int WINAPI TSDK_Req_IFrame(DWORD dwTermID); // 请求I帧
int WINAPI TSDK_Req_SetAlmOut(DWORD dwTermID, BYTE AlmOutMask, BYTE AlmOutState); // 请求设置终端报警输出状态
int WINAPI TSDK_Req_SetTermName(DWORD dwTermID, LPCSTR pTermName); // 请求设置终端名称

int WINAPI TSDK_Req_OpenTermAudVid_Sub(DWORD dwTermID); // 请求打开终端实时音频视频（子通道）。
int WINAPI TSDK_Req_CloseTermAudVid_Sub(DWORD dwTermID); // 请求关闭终端实时音频视频（子通道）。
int WINAPI TSDK_Req_IFrame_Sub(DWORD dwTermID); // 请求I帧（子通道）

int WINAPI TSDK_Req_OpenTermPcm(DWORD dwTermID); // 请求打开终端实时PCM音频(16K采样率16位)。
int WINAPI TSDK_Req_CloseTermPcm(DWORD dwTermID); // 请求关闭终端实时PCM音频(16K采样率16位)。

int WINAPI TSDK_Req_SetWaitTime(DWORD dwTermID, int nWaitTime); // 请求设置终端呼叫铃声播放时间(单位:秒)。

// 广播
int WINAPI TSDK_SetupGroup(int nBroadNum, TSdkGroupTermList * pTermList);   // 建立广播分组, 支持空分组(TermList[]全为0),  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupAddTerm(int nBroadNum, TSdkGroupTermList * pTermList); // 往分组添加广播终端，可以一次添加多个,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupAddTermEx(int nBroadNum, TSdkGroupTermList * pTermList); // 往分组添加广播终端，可以一次添加多个,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupDelTerm(int nBroadNum, TSdkGroupTermList * pTermList); // 从分组删除广播终端，可以一次删除多个,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupDelTermEx(int nBroadNum, TSdkGroupTermList * pTermList); // 从分组删除广播终端，可以一次删除多个,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GetTermGroupNum(DWORD dwTermID); // 查询终端属于哪个分组，返回0代表未分组，>0代表分组号，<0为错误代码

int WINAPI TSDK_StartBroadVoice(int nBroadNum); // 1 <= nBroadNum <= MAX_BROAD_GROUP   /****  8K 采样率 ****/
int WINAPI TSDK_StartBroadVoiceEx(int nBroadNum); // 1 <= nBroadNum <= MAX_BROAD_GROUP /**** 16K 采样率 ****/
int WINAPI TSDK_BroadPcmVoice(int nBroadNum, LPBYTE pPcmData, int nDataLen);   // 发送广播音频数据，音频格式为：PCM音频， 8K采样率，16位/样,  nDataLen:必须为320倍数，建议640或960
int WINAPI TSDK_BroadPcmVoiceEx(int nBroadNum, LPBYTE pPcmData, int nDataLen); // 发送广播音频数据，音频格式为：PCM音频，16K采样率，16位/样,  nDataLen:必须为640倍数，建议1280或1920
int WINAPI TSDK_StopBroadVoice(int nBroadNum);

int WINAPI TSDK_StartBroadMp3(int nBroadNum);   // 1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_BroadMp3File(int nBroadNum, LPCSTR pFilePath);
int WINAPI TSDK_PauseMp3Broad(int nBroadNum, BOOL bPause); // bPause=0:暂停 , bPause=1:继续
int WINAPI TSDK_StopBroadMp3(int nBroadNum);

int WINAPI TSDK_StartBroadPcm(DWORD dwTermID, enPcmSmpRate eRate);   // 1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_BroadPcmL(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // 播放左声道PCM
int WINAPI TSDK_BroadPcmR(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // 播放右声道PCM
int WINAPI TSDK_StopBroadPcm(DWORD dwTermID);

// 播放压缩音频视频
int WINAPI TSDK_Player_AudVidOpen(int nAudVidNum, HWND hDisplayWnd); // nAudVidNum 范围:1～MAX_AUDVID_NUM
int WINAPI TSDK_Player_AudVidAdd(int nAudVidNum, enSdkAudVidType eDataType, LPBYTE pAudVidData, int nDataSize); // nAudVidNum 范围:1～MAX_AUDVID_NUM
int WINAPI TSDK_Player_AudVidClose(int nAudVidNum); // nAudVidNum 范围:1～MAX_AUDVID_NUM

// 获取电脑麦克风音频
int WINAPI TSDK_Player_PcMicOpen();
int WINAPI TSDK_Player_PcMicClose();

// 播放PCM音频(未压缩)(8K采样率16位)
int WINAPI TSDK_Player_PcmOpen(int nPcmNum); // nPcmNum 范围:1～MAX_PCM_NUM
int WINAPI TSDK_Player_PcmAdd(int nPcmNum, LPBYTE pPcmData, int nDataSize); // nPcmNum 范围:1～MAX_PCM_NUM
int WINAPI TSDK_Player_PcmClose(int nPcmNum); // nPcmNum 范围:1～MAX_PCM_NUM

// 预览网络拾音器MP3音频，最多支持一个设备两个声道同时预览 !!!!!!
int WINAPI TSDK_Req_PreviewMp3L(DWORD dwTermID, BOOL bEnable); // 预览第一声道MP3
int WINAPI TSDK_Req_PreviewMp3R(DWORD dwTermID, BOOL bEnable); // 预览第二声道MP3

// 预览网络拾音器PCM音频(16K采样率16位)，最多支持一个设备两个声道同时预览 !!!!!!
int WINAPI TSDK_Req_PreviewPcmL(DWORD dwTermID, BOOL bEnable); // 预览第一声道PCM(16K采样率16位)
int WINAPI TSDK_Req_PreviewPcmR(DWORD dwTermID, BOOL bEnable); // 预览第二声道PCM(16K采样率16位)

// 分析网络拾音器当前 DB 值
int WINAPI TSDK_Req_DbAnalyze(DWORD dwTermID, BOOL bEnable); // 分析第一声道 DB 值

// 获取终端客户数据
int WINAPI TSDK_GetTermUserData(DWORD dwTermID, TSdkTermUserData * pData);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// NVR API 接口
int WINAPI TSDK_NvrGetChanList(DWORD dwTermID, TSdkNvrChanList * pChanList); // 获取NVR通道列表信息
int WINAPI TSDK_NvrOpenChanMp3(DWORD dwTermID, int nChanNum);  // nChanNum 范围:1～MAX_NVR_CHAN
int WINAPI TSDK_NvrCloseChanMp3(DWORD dwTermID, int nChanNum); // nChanNum 范围:1～MAX_NVR_CHAN
int WINAPI TSDK_NvrOpenChanPcm(DWORD dwTermID, int nChanNum);  // nChanNum 范围:1～MAX_NVR_CHAN
int WINAPI TSDK_NvrCloseChanPcm(DWORD dwTermID, int nChanNum); // nChanNum 范围:1～MAX_NVR_CHAN
int WINAPI TSDK_NvrGetChan(DWORD dwTermID, int nChanNum, TSdkNvrChan * pNvrChan); // nChanNum 范围:1～MAX_NVR_CHAN

int WINAPI TSDK_Req_NvrSrchRecFile(DWORD dwTermID, int nChanNum, TSdkTime * pDate);
				// nChanNum 范围:1～MAX_NVR_CHAN, pDate:日期(年月日)
int WINAPI TSDK_Req_NvrPlayRecFile(DWORD dwTermID, int nChanNum, TSdkTime * pDate, LPCSTR pFileName, BOOL bSdkPlaySound);
				// nChanNum 范围:1～MAX_NVR_CHAN, pDate:日期(年月日), pFileName:录音文件名称
				// 暂时只支持1路录音回放
int WINAPI TSDK_NvrSetPlayFilePos(DWORD dwFilePos); // dwFilePos:文件字节位置
int WINAPI TSDK_NvrPausePlay(BOOL bPause); // TRUE:暂停播放, FALSE:继续播放
int WINAPI TSDK_NvrStopPlay(); // 结束回放

#ifdef __cplusplus
} // End of extern "C" block
#endif // __cplusplus

#endif // !defined(__CTS_SDK_H__)
