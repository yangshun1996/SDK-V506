// cts_sdk.h : main header file for the CTS_SDK DLL
//

#if !defined(__CTS_SDK_H__)
#define __CTS_SDK_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////       �������        ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// Term  : �ն�(Terminal)���ֻ�
// Dev   : �豸(��Termһ��)
// Req   : ����
// Asw   : Ӧ��
// Post  : ��������֪ͨ �� ������������
// Event : �¼����͵Ļص�֪ͨ
// Cnn   : ����
// User  : ����SDK��Ӧ�ó���


// �ر�˵�� : ����/Ӧ���������������첽�����ġ���������������ж�Ӧ��Ӧ�𷵻أ�
//            ���ӻص������з�����������
///////////////////////////////////////////////////////////////////////////////////



// SDK ϵͳ����˵��:
// ���֧�� 6000 ���ն��豸(Term)ͬʱ���߹���
#define MAX_TERM_COUNT     6000  // SDK��֧�ֵ�����ն�����
#define MAX_BROAD_GROUP    300   // 60    // ���㲥��������
#define MAX_BROAD_TERM     300   // 100   // ÿ���㲥���������ն�����

#define MAX_AUDVID_NUM     36   // ѹ����Ƶ��Ƶ���źŷ�Χ��1 =< AudVidNum =< 36
#define MAX_PCM_NUM        16   // PCM��Ƶ���źŷ�Χ��     1 =< PcmNum    =< 16

#define MAX_NVR_CHAN       40   // NVRͨ����Χ�� 1 =< nChanNum =< 40

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
	CB_Asw_OpenTermAudVid  = 101, // Ӧ����ն���Ƶ��Ƶ
	CB_Asw_CloseTermAudVid = 102, // Ӧ��ر��ն���Ƶ��Ƶ
	CB_Asw_OpenTermTalk    = 103, // Ӧ����ն˶Խ�
	CB_Asw_CloseTermTalk   = 104, // Ӧ��ر��ն˶Խ�
	CB_Asw_GetAudLevel     = 105, // Ӧ���ȡ�ն���������
	CB_Asw_SetAudLevel     = 106, // Ӧ�������ն���������
	CB_Asw_DismissTerm     = 107, // Ӧ�����ֻ�����
	CB_Asw_GetDbAlmCfg     = 108, // Ӧ���ȡ�ն�������������
	CB_Asw_SetDbAlmCfg     = 109, // Ӧ�������ն�������������
	CB_Asw_NvrSrchRecFile  = 110, // Ӧ��NVR����¼���ļ����
	CB_Asw_NvrPlayRecFile  = 111, // Ӧ��NVR�ط�¼���ļ����

	CB_Post_TermSos        = 201, // �����ն˺���
	CB_Post_TermState      = 202, // �����ն�״̬
	CB_Post_485PipeData    = 203, // ����485�ܵ�����(����)
	CB_Post_Mp3PlayFinish  = 204, // ����MP3�ļ����Ž���֪ͨ
	CB_Post_AlmNotify      = 205, // ���ͱ���֪ͨ
	CB_Post_NvrChanNotify  = 206, // ����NVRͨ��֪ͨ
	CB_Post_TermDbValL     = 207, // ��������ʰ������һ��������ֵ
	CB_Post_TermDbValR     = 208, // ��������ʰ�����ڶ���������ֵ
	CB_Post_NvrSrchRecFile = 209, // ����NVR����¼���ļ��б���Ϣ
	CB_Post_NvrPlayProg    = 210, // ����NVR¼���ļ��طŽ���

	CB_Event_TermRegister  = 301, // �ն�����ע��֪ͨ������-1����ע��ʧ�ܣ�����0����ע��ɹ�
	CB_Event_TermConnect   = 302, // �ն���������֪ͨ������-1�����������ӣ�����0������������
	CB_Event_TermCnnLost   = 303, // �ն˶Ͽ�����֪ͨ

	CB_Data_TermAudio      = 401, // �ն˵���Ƶ���Ѿ�ѹ�����ģ���Ҫ����SDK���Žӿڲ��ܲ���
	CB_Data_TermVideo      = 402, // �ն˵�H264��׼��Ƶ֡�����Ե���SDK���Žӿڲ���
	CB_Data_PcMicAudio     = 403, // ���Ե���Ƶ��ΪPCM֡��8K�����ʣ�16λ/��
	CB_Data_BypassAudio    = 404, // ��·��Ƶ��ΪPCM֡��8K�����ʣ�16λ/��
	CB_Data_TermMp3L       = 405, // ����ʰ������һ����MP3��Ƶ
	CB_Data_TermMp3R       = 406, // ����ʰ�����ڶ�����MP3��Ƶ
	CB_Data_TermPcmL       = 407, // ����ʰ������һ����PCM��Ƶ(16K������16λ)
	CB_Data_TermPcmR       = 408, // ����ʰ�����ڶ�����PCM��Ƶ(16K������16λ)
	CB_Data_NvrMp3         = 409, // ¼������MP3��Ƶ
	CB_Data_NvrPcm         = 410, // ¼������PCM��Ƶ(16K������16λ)
	CB_Data_NvrPlayMp3     = 411, // ¼�������ط�MP3��Ƶ

}enSdkCbType;

typedef enum
{
	TSDK_DEV_STATE_OFFLINE   = 0,  // ����
	TSDK_DEV_STATE_ONLINE    = 1,  // ����
	TSDK_DEV_STATE_PLAYING   = 2,  // ���ڲ���mp3
	TSDK_DEV_STATE_TALKING   = 3,  // ���ڶԽ�
	TSDK_DEV_STATE_TALK_PLAY = 4   // ���ڶԽ��Ͳ���mp3
}enSdkDevState;

typedef enum
{
	TSDK_VIDEO = 1,
	TSDK_AUDIO = 2
}enSdkAudVidType;

typedef enum
{
	SDK_ACAP_LEVEL_1, // 2��
	SDK_ACAP_LEVEL_2, // 4��
	SDK_ACAP_LEVEL_3, // 6��
	SDK_ACAP_LEVEL_4  // 8��
}enSdkAudCapLevel; // �ն˼���������������

typedef enum
{
	SDK_AAMP_LEVEL_0, // 0�� : ����
	SDK_AAMP_LEVEL_1, // 1��
	SDK_AAMP_LEVEL_2, // 2��
	SDK_AAMP_LEVEL_3, // 3��
	SDK_AAMP_LEVEL_4, // 4��
	SDK_AAMP_LEVEL_5, // 5��
	SDK_AAMP_LEVEL_6, // 6��
	SDK_AAMP_LEVEL_7, // 7��
	SDK_AAMP_LEVEL_8, // 8��
	SDK_AAMP_LEVEL_9, // 9��
	SDK_AAMP_LEVEL_10 // 10�� : �������
}enSdkAudAmpLevel; // �ն˹��������������

typedef enum
{
	SDK_ACAP_TALK_LEVEL_1, // 0.5��
	SDK_ACAP_TALK_LEVEL_2, // 0.8��
	SDK_ACAP_TALK_LEVEL_3, // 1.2��
	SDK_ACAP_TALK_LEVEL_4  // 1.8��
}enSdkAudCapTalkLevel; // �ն˶Խ�������������

typedef enum
{
	SDK_ALMTYPE_ALARM_IN1 = 0x00000004, // ��������1
	SDK_ALMTYPE_ALARM_IN2 = 0x00000010, // ��������2
	SDK_ALMTYPE_DB_ALARM  = 0x00000020, // ��������
}enSdkAlarmType;

typedef enum
{
	TSDK_DEV_TYPE_UNDEF  = 0,  // δ֪�豸����
	TSDK_DEV_TYPE_TALK   = 1,  // �Խ��ն� �� �Խ��㲥�ն�
	TSDK_DEV_TYPE_BROAD  = 2,  // �㲥�ն�
	TSDK_DEV_TYPE_MP3    = 3,  // ����ʰ����
	TSDK_DEV_TYPE_NVR    = 4,  // ¼������
}enSdkDevType;

typedef enum
{
	PCM_32000   = 1,  // 32K   ������
	PCM_44100   = 2,  // 44.1K ������
	PCM_48000   = 3   // 48K   ������
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
	DWORD TermList[MAX_BROAD_TERM]; // �ն�ID�б��հ������� 0 ���
}TSdkGroupTermList;

typedef struct
{
	enSdkAudCapLevel	 eCapLevel;
	enSdkAudAmpLevel	 eAmpLevel; // �����������0 �� 10 (0����,10���)
	enSdkAudCapTalkLevel eCapTalkLevel;
}TSdkAudioLevel;

typedef struct
{
	enSdkAudCapLevel     eCapLevel;
	int                  nAmpValue; // �������ֵ��0 �� 100 (0����,100���)
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
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswOpenTermAudVid;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswCloseTermAudVid;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswOpenTermTalk;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswCloseTermTalk;

typedef struct
{
	DWORD dwTermID;
	TSdkAudioLevel Level;
	int            nAmpValue; // �������ֵ��0 �� 100 (0����,100���)
}TSdkAswGetAudLevel;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
	enSdkAudAmpLevel eAmpLevel; // Ӧ�����÷ֻ��������������0 �� 10 (0����,10���)
	int              nAmpValue; // Ӧ�����÷ֻ����������ֵ��0 �� 100 (0����,100���)
	BOOL             bIsEx;     // TRUE:Ӧ�����TSDK_Req_SetAudLevelEx() , FALSE:Ӧ�����TSDK_Req_SetAudLevel()
}TSdkAswSetAudLevel;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswDismissTerm;

typedef struct
{
	DWORD dwTermID;
	TSdkDbAlmCfg Cfg;
}TSdkAswGetDbAlmCfg;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
}TSdkAswSetDbAlmCfg;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	int nChanNum;
	DWORD dwTermID;
}TSdkAswNvrSrchRecFile;

typedef struct
{
	int nResult; // ���������ɹ�Ϊ0������Ϊ�������:enSdkErrCode
	DWORD dwTermID;
	int nChanNum;
	int nTotalFileSize;  // �ֽڴ�С
	int nTotalRecTime;   // ¼���ļ�ʱ��:��
}TSdkAswNvrPlayRecFile;

typedef struct
{
	DWORD dwTermID;
	int nSosType; // 0:��ѯ���� , 1:��������
}TSdkPostTermSos;

typedef struct
{
	DWORD dwTermID;
	enSdkDevState eTermState;
	BYTE AlmInMask;   // ��������ͨ����Чָʾ�룬ÿλ��Ӧһ·����: 0(ͨ����Ч), 1(ͨ����Ч)����ʱֻ֧��2·�������룬��1λΪͨ��1����2λΪͨ��2��
	BYTE AlmInState;  // ��������ͨ��״ָ̬ʾ�룬ÿλ��Ӧһ·����: 0(�ޱ���), 1(�б���)��
	BYTE AlmOutMask;  // �������ͨ����Чָʾ�룬ÿλ��Ӧһ·����: 0(ͨ����Ч), 1(ͨ����Ч)����ʱֻ֧��2·�����������1λΪͨ��1����2λΪͨ��2��
	BYTE AlmOutState; // �������ͨ��״ָ̬ʾ�룬ÿλ��Ӧһ·����: 0(�ޱ���), 1(�б���)��
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
	int nResult; // ���Ž�����ɹ�Ϊ0������Ϊ�������:enSdkErrCode 
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
	int   nPlayingFilePos; // ��ǰ�طŵ��ļ��ֽ�λ��
}TSdkPostNvrPlayProg;

typedef struct
{
	enSdkDevType eTermType;
	DWORD dwTermID;    // �ն��豸ID�ţ���16λΪ�����ù��ߡ�ע���豸�ġ�����š�(1��10)����16λΪ�����ù��ߡ�ע���豸�ġ��豸�š�(1��600)
	BYTE TermMac[6];   // �ն�MAC��ַ
	char TermIp[16];   // �ն�IP��ַ
	char TermName[32]; // �ն�����
}TSdkEventTermRegister;

typedef struct
{
	enSdkDevType eTermType;
	DWORD dwTermID;    // �ն��豸ID�ţ���16λΪ�����ù��ߡ�ע���豸�ġ�����š�(1��10)����16λΪ�����ù��ߡ�ע���豸�ġ��豸�š�(1��600)
	BYTE TermMac[6];   // �ն�MAC��ַ
	char TermIp[16];   // �ն�IP��ַ
	char TermName[32]; // �ն�����
	BOOL bHasVideo;    // TRUE:����Ƶ����Ƶ , FALSE:����Ƶֻ����Ƶ
	BOOL bTalkbackEnable; // ֻ�������ʰ������TRUE:�����Խ�ģʽ��FALSE:�رնԽ�ģʽ
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
	LPBYTE pVidFrm;      // H264��Ƶ֡
	int    nFrmSize;     // ֡��С
	BOOL   bIsKeyFrm;    // TRUE:ΪI֡ , FALSE:ΪP֡
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
	int  nChanNum;      // 1��MAX_NVR_CHAN
	BOOL bIsLocal;      // 1:����ģ��ʰ���� , 0:����ʰ����
	BOOL bValid;        // 1:��Ч(���豸ע��) , 0:��Ч(���豸ע��)
	char ChanName[32];  // ͨ������
	BOOL bIsOnline;     // 1:���� , 0:������
	BOOL bIsRecording;  // 1:����¼�� , 0:ֹͣ¼��
}TSdkNvrChan;

typedef struct
{
	TSdkNvrChan ChanList[MAX_NVR_CHAN];
}TSdkNvrChanList;

typedef struct
{
	WORD Year;   // �� : 2018 ~ 2999
	BYTE Month;  // �� : 1 ~ 12
	BYTE Day;    // �� : 1 ~ 31
	WORD Hour;   // ʱ : 0 ~ 23
	BYTE Minute; // �� : 0 ~ 59
	BYTE Second; // �� : 0 ~ 59
	DWORD MillSecond; // ���� : 0 ~ 999
}TSdkTime;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pMp3Data;
	int    nDataSize;
	TSdkTime Time; // ʱ�Ӵ�
}TSdkDataNvrMp3;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pPcmData;
	int    nDataSize;
	TSdkTime Time; // ʱ�Ӵ�
}TSdkDataNvrPcm;

typedef struct
{
	DWORD dwTermID;
	int   nChanNum;
	LPBYTE pMp3Data;
	int    nDataSize;
	TSdkTime Time; // ʱ�Ӵ�
}TSdkDataNvrPlayMp3;




///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// �˻ص������൱��Ҫ�������豸ע�ᡢ�豸���ӡ���Ƶ��Ƶ���ݡ�����Ӧ��״̬�¼� �ȶ��ɻص��������أ�
// ����Ҫע�⣺�ص������������� !!!!!!
typedef int (CALLBACK* ON_CTS_SDK_CALL_BACK)(enSdkCbType eCbType, LPVOID pParam, DWORD dwSize, int usr_data);


// �Խ�
int WINAPI TSDK_Init(ON_CTS_SDK_CALL_BACK CbFunc, BOOL bServerMode, int usr_data); // SDK ��ʼ����
int WINAPI TSDK_DeInit(); // SDK ע����

int WINAPI TSDK_Req_OpenTermAudVid(DWORD dwTermID); // ������ն�ʵʱ��Ƶ��Ƶ��
int WINAPI TSDK_Req_CloseTermAudVid(DWORD dwTermID); // ����ر��ն�ʵʱ��Ƶ��Ƶ��

int WINAPI TSDK_Req_OpenTermTalk(DWORD dwTermID);   // �������ն˴򿪶Խ���
int WINAPI TSDK_Req_OpenTermTalk_R(DWORD dwTermID); // �������ն˴򿪶Խ���ʹ����������ֻ֧������ʰ��������
int WINAPI TSDK_Req_CloseTermTalk(DWORD dwTermID);  // �������ն˹رնԽ���

int WINAPI TSDK_Send_485PipeData(DWORD dwTermID, LPBYTE pFrmData, int nFrmLen); // ����485�ܵ�����
int WINAPI TSDK_Send_TalkAudio(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // ���ͶԽ���Ƶ���ݣ���Ƶ��ʽΪ��PCM��Ƶ��8K�����ʣ�16λ/��,  nDataLen:����Ϊ320����������640��960

int WINAPI TSDK_Req_GetAudLevel(DWORD dwTermID); // �����ȡ�ն���������
int WINAPI TSDK_Req_SetAudLevel(DWORD dwTermID, TSdkAudioLevel * pLevel); // ���������ն���������
int WINAPI TSDK_Req_SetAudLevelEx(DWORD dwTermID, TSdkAudioLevelEx * pLevel); // ���������ն�����ֵ�ͼ���

int WINAPI TSDK_Req_DismissTerm(DWORD dwTermID);  // �������ֻ�����

int WINAPI TSDK_Req_GetDbAlmCfg(DWORD dwTermID); // �����ȡ�ն�������������
int WINAPI TSDK_Req_SetDbAlmCfg(DWORD dwTermID, TSdkDbAlmCfg * pCfg); // ���������ն�������������

int WINAPI TSDK_Req_IFrame(DWORD dwTermID); // ����I֡
int WINAPI TSDK_Req_SetAlmOut(DWORD dwTermID, BYTE AlmOutMask, BYTE AlmOutState); // ���������ն˱������״̬
int WINAPI TSDK_Req_SetTermName(DWORD dwTermID, LPCSTR pTermName); // ���������ն�����

int WINAPI TSDK_Req_OpenTermAudVid_Sub(DWORD dwTermID); // ������ն�ʵʱ��Ƶ��Ƶ����ͨ������
int WINAPI TSDK_Req_CloseTermAudVid_Sub(DWORD dwTermID); // ����ر��ն�ʵʱ��Ƶ��Ƶ����ͨ������
int WINAPI TSDK_Req_IFrame_Sub(DWORD dwTermID); // ����I֡����ͨ����

int WINAPI TSDK_Req_OpenTermPcm(DWORD dwTermID); // ������ն�ʵʱPCM��Ƶ(16K������16λ)��
int WINAPI TSDK_Req_CloseTermPcm(DWORD dwTermID); // ����ر��ն�ʵʱPCM��Ƶ(16K������16λ)��

int WINAPI TSDK_Req_SetWaitTime(DWORD dwTermID, int nWaitTime); // ���������ն˺�����������ʱ��(��λ:��)��

// �㲥
int WINAPI TSDK_SetupGroup(int nBroadNum, TSdkGroupTermList * pTermList);   // �����㲥����, ֧�ֿշ���(TermList[]ȫΪ0),  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupAddTerm(int nBroadNum, TSdkGroupTermList * pTermList); // ��������ӹ㲥�նˣ�����һ����Ӷ��,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupAddTermEx(int nBroadNum, TSdkGroupTermList * pTermList); // ��������ӹ㲥�նˣ�����һ����Ӷ��,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupDelTerm(int nBroadNum, TSdkGroupTermList * pTermList); // �ӷ���ɾ���㲥�նˣ�����һ��ɾ�����,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GroupDelTermEx(int nBroadNum, TSdkGroupTermList * pTermList); // �ӷ���ɾ���㲥�նˣ�����һ��ɾ�����,  1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_GetTermGroupNum(DWORD dwTermID); // ��ѯ�ն������ĸ����飬����0����δ���飬>0�������ţ�<0Ϊ�������

int WINAPI TSDK_StartBroadVoice(int nBroadNum); // 1 <= nBroadNum <= MAX_BROAD_GROUP   /****  8K ������ ****/
int WINAPI TSDK_StartBroadVoiceEx(int nBroadNum); // 1 <= nBroadNum <= MAX_BROAD_GROUP /**** 16K ������ ****/
int WINAPI TSDK_BroadPcmVoice(int nBroadNum, LPBYTE pPcmData, int nDataLen);   // ���͹㲥��Ƶ���ݣ���Ƶ��ʽΪ��PCM��Ƶ�� 8K�����ʣ�16λ/��,  nDataLen:����Ϊ320����������640��960
int WINAPI TSDK_BroadPcmVoiceEx(int nBroadNum, LPBYTE pPcmData, int nDataLen); // ���͹㲥��Ƶ���ݣ���Ƶ��ʽΪ��PCM��Ƶ��16K�����ʣ�16λ/��,  nDataLen:����Ϊ640����������1280��1920
int WINAPI TSDK_StopBroadVoice(int nBroadNum);

int WINAPI TSDK_StartBroadMp3(int nBroadNum);   // 1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_BroadMp3File(int nBroadNum, LPCSTR pFilePath);
int WINAPI TSDK_PauseMp3Broad(int nBroadNum, BOOL bPause); // bPause=0:��ͣ , bPause=1:����
int WINAPI TSDK_StopBroadMp3(int nBroadNum);

int WINAPI TSDK_StartBroadPcm(DWORD dwTermID, enPcmSmpRate eRate);   // 1 <= nBroadNum <= MAX_BROAD_GROUP
int WINAPI TSDK_BroadPcmL(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // ����������PCM
int WINAPI TSDK_BroadPcmR(DWORD dwTermID, LPBYTE pPcmData, int nDataLen); // ����������PCM
int WINAPI TSDK_StopBroadPcm(DWORD dwTermID);

// ����ѹ����Ƶ��Ƶ
int WINAPI TSDK_Player_AudVidOpen(int nAudVidNum, HWND hDisplayWnd); // nAudVidNum ��Χ:1��MAX_AUDVID_NUM
int WINAPI TSDK_Player_AudVidAdd(int nAudVidNum, enSdkAudVidType eDataType, LPBYTE pAudVidData, int nDataSize); // nAudVidNum ��Χ:1��MAX_AUDVID_NUM
int WINAPI TSDK_Player_AudVidClose(int nAudVidNum); // nAudVidNum ��Χ:1��MAX_AUDVID_NUM

// ��ȡ������˷���Ƶ
int WINAPI TSDK_Player_PcMicOpen();
int WINAPI TSDK_Player_PcMicClose();

// ����PCM��Ƶ(δѹ��)(8K������16λ)
int WINAPI TSDK_Player_PcmOpen(int nPcmNum); // nPcmNum ��Χ:1��MAX_PCM_NUM
int WINAPI TSDK_Player_PcmAdd(int nPcmNum, LPBYTE pPcmData, int nDataSize); // nPcmNum ��Χ:1��MAX_PCM_NUM
int WINAPI TSDK_Player_PcmClose(int nPcmNum); // nPcmNum ��Χ:1��MAX_PCM_NUM

// Ԥ������ʰ����MP3��Ƶ�����֧��һ���豸��������ͬʱԤ�� !!!!!!
int WINAPI TSDK_Req_PreviewMp3L(DWORD dwTermID, BOOL bEnable); // Ԥ����һ����MP3
int WINAPI TSDK_Req_PreviewMp3R(DWORD dwTermID, BOOL bEnable); // Ԥ���ڶ�����MP3

// Ԥ������ʰ����PCM��Ƶ(16K������16λ)�����֧��һ���豸��������ͬʱԤ�� !!!!!!
int WINAPI TSDK_Req_PreviewPcmL(DWORD dwTermID, BOOL bEnable); // Ԥ����һ����PCM(16K������16λ)
int WINAPI TSDK_Req_PreviewPcmR(DWORD dwTermID, BOOL bEnable); // Ԥ���ڶ�����PCM(16K������16λ)

// ��������ʰ������ǰ DB ֵ
int WINAPI TSDK_Req_DbAnalyze(DWORD dwTermID, BOOL bEnable); // ������һ���� DB ֵ

// ��ȡ�ն˿ͻ�����
int WINAPI TSDK_GetTermUserData(DWORD dwTermID, TSdkTermUserData * pData);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// NVR API �ӿ�
int WINAPI TSDK_NvrGetChanList(DWORD dwTermID, TSdkNvrChanList * pChanList); // ��ȡNVRͨ���б���Ϣ
int WINAPI TSDK_NvrOpenChanMp3(DWORD dwTermID, int nChanNum);  // nChanNum ��Χ:1��MAX_NVR_CHAN
int WINAPI TSDK_NvrCloseChanMp3(DWORD dwTermID, int nChanNum); // nChanNum ��Χ:1��MAX_NVR_CHAN
int WINAPI TSDK_NvrOpenChanPcm(DWORD dwTermID, int nChanNum);  // nChanNum ��Χ:1��MAX_NVR_CHAN
int WINAPI TSDK_NvrCloseChanPcm(DWORD dwTermID, int nChanNum); // nChanNum ��Χ:1��MAX_NVR_CHAN
int WINAPI TSDK_NvrGetChan(DWORD dwTermID, int nChanNum, TSdkNvrChan * pNvrChan); // nChanNum ��Χ:1��MAX_NVR_CHAN

int WINAPI TSDK_Req_NvrSrchRecFile(DWORD dwTermID, int nChanNum, TSdkTime * pDate);
				// nChanNum ��Χ:1��MAX_NVR_CHAN, pDate:����(������)
int WINAPI TSDK_Req_NvrPlayRecFile(DWORD dwTermID, int nChanNum, TSdkTime * pDate, LPCSTR pFileName, BOOL bSdkPlaySound);
				// nChanNum ��Χ:1��MAX_NVR_CHAN, pDate:����(������), pFileName:¼���ļ�����
				// ��ʱֻ֧��1·¼���ط�
int WINAPI TSDK_NvrSetPlayFilePos(DWORD dwFilePos); // dwFilePos:�ļ��ֽ�λ��
int WINAPI TSDK_NvrPausePlay(BOOL bPause); // TRUE:��ͣ����, FALSE:��������
int WINAPI TSDK_NvrStopPlay(); // �����ط�

#ifdef __cplusplus
} // End of extern "C" block
#endif // __cplusplus

#endif // !defined(__CTS_SDK_H__)
