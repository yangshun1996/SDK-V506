package com.fh.utils.fh;

import com.sun.jna.*;
import com.sun.jna.win32.StdCallLibrary.StdCallCallback;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public interface CtsSdk extends Library {

    CtsSdk COMMON_INSTANCE = (CtsSdk) Native
            .loadLibrary(".\\libs\\CtsSdk", CtsSdk.class);


    // SDK 系统容量说明:
    // 最多支持 6000 个终端设备(Term)同时在线工作
    public static final int MAX_TERM_COUNT = 6000;  // SDK所支持的最大终端数量
    public static final int MAX_BROAD_GROUP = 60;    // 最大广播分组数量
    public static final int MAX_BROAD_TERM = 100;   // 每个广播分组的最大终端数量

    public static final int MAX_AUDVID_NUM = 36;   // 压缩音频视频播放号范围：1 =< AudVidNum =< 36
    public static final int MAX_PCM_NUM = 16;   // PCM音频播放号范围：     1 =< PcmNum    =< 16
    public static final int MAX_NVR_CHAN = 40;   // NVR通道范围： 1 =< nChanNum =< 40
    public static final int MAX_FILE_CNT_PER_POST = 50;


    public static class enSdkErrCode extends Structure {

        public static final int CERR_SUCCESS = 0;
        public static final int CERR_UNKNOWN = -1;
        public static final int CERR_NOT_INIT = -801;
        public static final int CERR_INVALID_PARAMETER = -802;
        public static final int CERR_CONNECT_TIMEOUT = -803;
        public static final int CERR_SOCKET_RECV_ERROR = -804;
        public static final int CERR_ALREADY_CONNECTED = -805;
        public static final int CERR_NOT_CONNECTED = -806;
        public static final int CERR_DEVICE_BUSY = -807;
        public static final int CERR_HOST_REGISTER_ERROR = -908;
        public static final int CERR_INVALID_DEV_STATE = -909;
        public static final int CERR_LISTEN_PORT_ERROR = -910;
        public static final int CERR_OPEN_FILE_ERROR = -911;
        public static final int CERR_INVALID_FILE_PATH = -912;
        public static final int CERR_INVALID_FILE_SIZE = -913;
        public static final int CERR_INVALID_FILE_DATA = -914;
        public static final int CERR_TERM_IS_OCCUPIED = -915;
        public static final int CERR_GROUP_NOT_SETUP = -916;
        public static final int CERR_TERM_LIST_FULL = -917;
        public static final int CERR_DATA_NOT_SYNC = -918;
        public static final int CERR_INVALID_BIOS_VERSION = -919;
        public static final int CERR_INVALID_DEV_TYPE = -920;
        public static final int CERR_CHAN_NOT_REGISTERED = -921;

        public static final int CERR_HST_NUM_OCCUPIED = -701;
        public static final int CERR_TRM_NUM_OCCUPIED = -702;
        public static final int CERR_HST_NUM_NOT_EXISTENT = -703;
        public static final int CERR_TRM_NUM_NOT_EXISTENT = -704;
        public static final int CERR_HST_IS_IN_CALLING = -705;
        public static final int CERR_TRM_NUM_NOT_FIT_HOST = -706;
        public static final int CERR_HST_CNT_REACH_MAX = -707;
        public static final int CERR_GROUP_NUM_NOT_EXISTENT = -708;
        public static final int CERR_DEV_NUM_OCCUPIED = -709;
        public static final int CERR_DEV_NOT_REGISTERED = -710;

    }

    public static class enSdkCbType extends Structure {
        public static final int CB_Asw_OpenTermAudVid = 101;// 应答打开终端音频视频
        public static final int CB_Asw_CloseTermAudVid = 102; // 应答关闭终端音频视频
        public static final int CB_Asw_OpenTermTalk = 103; // 应答打开终端对讲
        public static final int CB_Asw_CloseTermTalk = 104; // 应答关闭终端对讲
        public static final int CB_Asw_GetAudLevel = 105; // 应答获取终端音量级别
        public static final int CB_Asw_SetAudLevel = 106; // 应答设置终端音量级别
        public static final int CB_Asw_DismissTerm = 107; // 应答解除分机呼叫
        public static final int CB_Asw_GetDbAlmCfg = 108; // 应答获取终端喧哗报警配置
        public static final int CB_Asw_SetDbAlmCfg = 109; // 应答设置终端喧哗报警配置
        public static final int CB_Asw_NvrSrchRecFile = 110; // 应答NVR查找录音文件结果
        public static final int CB_Asw_NvrPlayRecFile = 111; // 应答NVR回放录音文件结果

        public static final int CB_Post_TermSos = 201; // 推送终端呼叫
        public static final int CB_Post_TermState = 202; // 推送终端状态
        public static final int CB_Post_485PipeData = 203; // 推送485管道数据(接收)
        public static final int CB_Post_Mp3PlayFinish = 204; // 推送MP3文件播放结束通知
        public static final int CB_Post_AlmNotify = 205; // 推送报警通知
        public static final int CB_Post_NvrChanNotify = 206; // 推送NVR通道通知
        public static final int CB_Post_TermDbValL = 207; // 推送网络拾音器第一声道音量值
        public static final int CB_Post_TermDbValR = 208; // 推送网络拾音器第二声道音量值
        public static final int CB_Post_NvrSrchRecFile = 209; // 推送NVR查找录音文件列表信息
        public static final int CB_Post_NvrPlayProg = 210; // 推送NVR录音文件回放进度

        public static final int CB_Event_TermRegister = 301; // 终端请求注册通知，返回-1代表注册失败，返回0代表注册成功
        public static final int CB_Event_TermConnect = 302; // 终端请求连接通知，返回-1代表不允许连接，返回0代表允许连接
        public static final int CB_Event_TermCnnLost = 303; // 终端断开连接通知

        public static final int CB_Data_TermAudio = 401; // 终端的音频，已经压缩过的，需要调用SDK播放接口才能播放
        public static final int CB_Data_TermVideo = 402; // 终端的H264标准视频帧，可以调用SDK播放接口播放
        public static final int CB_Data_PcMicAudio = 403; // 电脑的音频，为PCM帧，8K采样率，16位/样
        public static final int CB_Data_BypassAudio = 404; // 旁路音频，为PCM帧，8K采样率，16位/样
        public static final int CB_Data_TermMp3L = 405; // 网络拾音器第一声道MP3音频
        public static final int CB_Data_TermMp3R = 406; // 网络拾音器第二声道MP3音频
        public static final int CB_Data_TermPcmL = 407; // 网络拾音器第一声道PCM音频(16K采样率16位)
        public static final int CB_Data_TermPcmR = 408; // 网络拾音器第二声道PCM音频(16K采样率16位)
        public static final int CB_Data_NvrMp3 = 409; // 录音主机MP3音频
        public static final int CB_Data_NvrPcm = 410; // 录音主机PCM音频(16K采样率16位)
        public static final int CB_Data_NvrPlayMp3 = 411; // 录音主机回放MP3音频
    }

    public static class enSdkDevState extends Structure {
        public static final int TSDK_DEV_STATE_OFFLINE = 0;  // 离线
        public static final int TSDK_DEV_STATE_ONLINE = 1;  // 在线
        public static final int TSDK_DEV_STATE_PLAYING = 2;  // 正在播放mp3
        public static final int TSDK_DEV_STATE_TALKING = 3;  // 正在对讲
        public static final int TSDK_DEV_STATE_TALK_PLAY = 4;  // 正在对讲和播放mp3

    }

    public static class enSdkAudVidType extends Structure {
        public static final int TSDK_VIDEO = 1;
        public static final int TSDK_AUDIO = 2;
    }

    // 终端监听输入音量级别
    public static class enSdkAudCapLevel extends Structure {
        public static final int SDK_ACAP_LEVEL_1 = 2; // 2米
        public static final int SDK_ACAP_LEVEL_2 = 4;// 4米
        public static final int SDK_ACAP_LEVEL_3 = 6; // 6米
        public static final int SDK_ACAP_LEVEL_4 = 8; // 8米
    }

    // 终端功放输出音量级别
    public static class enSdkAudAmpLevel extends Structure {
        public static final int SDK_AAMP_LEVEL_0 = 0;// 0级 : 静音
        public static final int SDK_AAMP_LEVEL_1 = 1;// 1级
        public static final int SDK_AAMP_LEVEL_2 = 2;// 2级
        public static final int SDK_AAMP_LEVEL_3 = 3; // 3级
        public static final int SDK_AAMP_LEVEL_4 = 4; // 4级
        public static final int SDK_AAMP_LEVEL_5 = 5; // 5级
        public static final int SDK_AAMP_LEVEL_6 = 6; // 6级
        public static final int SDK_AAMP_LEVEL_7 = 7; // 7级
        public static final int SDK_AAMP_LEVEL_8 = 8; // 8级
        public static final int SDK_AAMP_LEVEL_9 = 9; // 9级
        public static final int SDK_AAMP_LEVEL_10 = 10;// 10级 : 音量最大
    }

    // 终端对讲输入音量级别
    public static class enSdkAudCapTalkLevel extends Structure {
        public static final int SDK_ACAP_TALK_LEVEL_1 = 1; // 0.5米
        public static final int SDK_ACAP_TALK_LEVEL_2 = 2;// 0.8米
        public static final int SDK_ACAP_TALK_LEVEL_3 = 3; // 1.2米
        public static final int SDK_ACAP_TALK_LEVEL_4 = 4; // 1.8米
    }

    public static class enSdkAlarmType extends Structure {
        public static final int SDK_ALMTYPE_ALARM_IN1 = 0x00000004; // 报警输入1
        public static final int SDK_ALMTYPE_ALARM_IN2 = 0x00000010; // 报警输入2
        public static final int SDK_ALMTYPE_DB_ALARM = 0x00000020; // 喧哗报警
    }

    public static class enSdkDevType extends Structure {
        public static final int TSDK_DEV_TYPE_UNDEF = 0;  // 未知设备类型
        public static final int TSDK_DEV_TYPE_TALK = 1;  // 对讲终端 或 对讲广播终端
        public static final int TSDK_DEV_TYPE_BROAD = 2;  // 广播终端
        public static final int TSDK_DEV_TYPE_MP3 = 3;  // 网络拾音器
        public static final int TSDK_DEV_TYPE_NVR = 4;  // 录音主机
    }

    public static class enPcmSmpRate extends Structure {
        public static final int PCM_32000 = 1;  // 32K   采样率
        public static final int PCM_44100 = 2;  // 44.1K 采样率
        public static final int PCM_48000 = 3;  // 48K   采样率
    }

    public static class enSdkNvrNotifyType extends Structure {
        public static final int NVR_CHAN_ADD = 1;
        public static final int NVR_CHAN_DELETE = 2;
        public static final int NVR_CHAN_MODIFY = 3;
        public static final int NVR_CHAN_STATE_CHANGE = 4;
    }

    public static class TSdkGroupTermList extends Structure {
        public int[] TermList = new int[MAX_BROAD_TERM]; // 终端ID列表，空白区请用 0 填充
    }

    public static class TSdkAudioLevel extends Structure {
        public enSdkAudCapLevel eCapLevel;
        public enSdkAudAmpLevel eAmpLevel; // 输出音量级别：0 ～ 10 (0静音;10最大)
        public enSdkAudCapTalkLevel eCapTalkLevel;
    }

    public static class TSdkAudioLevelEx extends Structure {
        public enSdkAudCapLevel eCapLevel;
        public int nAmpValue; // 输出音量值：0 ～ 100 (0静音;100最大)
        public enSdkAudCapTalkLevel eCapTalkLevel;
    }

    public static class TSdkDbAlmCfg extends Structure {
        public boolean bDbAlmEnable;
        public int nDbAlmVal;
    }


    public static class TSdkNvrFileInfo extends Structure {
        public byte[] Name = new byte[64];
        public int nSize;
    }

    public static class TSdkAswOpenTermAudVid extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswCloseTermAudVid extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswOpenTermTalk extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswCloseTermTalk extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswGetAudLevel extends Structure {
        public int dwTermID;
        public TSdkAudioLevel Level;
        public int nAmpValue; // 输出音量值：0 ～ 100 (0静音;100最大)
    }

    public static class TSdkAswSetAudLevel extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
        public enSdkAudAmpLevel eAmpLevel; // 应答设置分机的输出音量级别：0 ～ 10 (0静音;10最大)
        public int nAmpValue; // 应答设置分机的输出音量值：0 ～ 100 (0静音;100最大)
        public boolean bIsEx;     // TRUE:应答调用TSDK_Req_SetAudLevelEx() ; FALSE:应答调用TSDK_Req_SetAudLevel()
    }

    public static class TSdkAswDismissTerm extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswGetDbAlmCfg extends Structure {
        public int dwTermID;
        public TSdkDbAlmCfg Cfg;
    }

    public static class TSdkAswSetDbAlmCfg extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
    }

    public static class TSdkAswNvrSrchRecFile extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int nChanNum;
        public int dwTermID;
    }

    public static class TSdkAswNvrPlayRecFile extends Structure {
        public int nResult; // 请求结果，成功为0，否则为错误代码:enSdkErrCode
        public int dwTermID;
        public int nChanNum;
        public int nTotalFileSize;  // 字节大小
        public int nTotalRecTime;   // 录音文件时长:秒
    }

    public static class TSdkPostTermSos extends Structure {
        public int dwTermID;
        public int nSosType; // 0:咨询呼叫 ; 1:紧急呼叫
    }

    public static class TSdkPostTermState extends Structure {
        public int dwTermID;
        public enSdkDevState eTermState;
        public byte AlmInMask;   // 报警输入通道有效指示码，每位对应一路报警: 0(通道无效); 1(通道有效)。暂时只支持2路报警输入，第1位为通道1，第2位为通道2。
        public byte AlmInState;  // 报警输入通道状态指示码，每位对应一路报警: 0(无报警); 1(有报警)。
        public byte AlmOutMask;  // 报警输出通道有效指示码，每位对应一路报警: 0(通道无效); 1(通道有效)。暂时只支持2路报警输出，第1位为通道1，第2位为通道2。
        public byte AlmOutState; // 报警输出通道状态指示码，每位对应一路报警: 0(无报警); 1(有报警)。
    }

    public static class TSdkPost485PipeData extends Structure {
        public int dwTermID;
        public int nFrmLen;
        public byte[] FrmData = new byte[256];// Max 485 Frame Len : 255
    }

    public static class TSdkPostMp3PlayFinish extends Structure {
        public int nBroadNum;
        public int nResult; // 播放结果，成功为0，否则为错误代码:enSdkErrCode
    }

    public static class TSdkPostAlmNotify extends Structure {
        public int dwTermID;
        public int nChanNum;
        public enSdkAlarmType eAlmType;
    }

    public static class TSdkPostNvrChanNotify extends Structure {
        public int dwTermID;
        public enSdkNvrNotifyType eType;
        public int nChanNum;
    }

    public static class TSdkPostTermDbValL extends Structure {
        public int dwTermID;
        public int nDbVal;
    }


    public static class TSdkPostTermDbValR extends Structure {
        public int dwTermID;
        public int nDbVal;
    }


    public static class TSdkPostNvrSrchRecFile extends Structure {
        public int dwTermID;
        public int nChanNum;
        public int nFileCnt;
        public TSdkNvrFileInfo[] FileList = new TSdkNvrFileInfo[MAX_FILE_CNT_PER_POST];
    }

    public static class TSdkPostNvrPlayProg extends Structure {
        public int dwTermID;
        public int nChanNum;
        public int nPlayingFilePos; // 当前回放的文件字节位置
    }

    public static class TSdkEventTermRegister extends Structure {
        public enSdkDevType eTermType;
        public int dwTermID;    // 终端设备ID号，高16位为“配置工具”注册设备的“区域号”(1～10)，低16位为“配置工具”注册设备的“设备号”(1～600)
        public byte[] TermMac = new byte[6];   // 终端MAC地址
        public byte[] TermIp = new byte[16];   // 终端IP地址
        public byte[] TermName = new byte[32]; // 终端名称
    }

    public static class TSdkEventTermConnect extends Structure {
        public static class ByReference extends TSdkEventTermConnect implements Structure.ByReference {}
        public static class ByValue extends TSdkEventTermConnect implements Structure.ByValue {}

        public enSdkDevType eTermType;
        public int dwTermID;    // 终端设备ID号，高16位为“配置工具”注册设备的“区域号”(1～10)，低16位为“配置工具”注册设备的“设备号”(1～600)
        public byte[] TermMac = new byte[6];   // 终端MAC地址
        public byte[] TermIp = new byte[16];   // 终端IP地址
        public byte[] TermName = new byte[32]; // 终端名称
        public boolean bHasVideo;    // TRUE:有视频和音频 ; FALSE:无视频只有音频
        public boolean bTalkbackEnable; // 只针对网络拾音器，TRUE:开启对讲模式，FALSE:关闭对讲模式

        @Override
        protected List getFieldOrder() {
            return Arrays.asList(new String[]{"dwTermID", "TermMac", "TermIp", "TermName", "bHasVideo", "bTalkbackEnable"});
        }
    }

    public static class TSdkEventTermCnnLost extends Structure {
        public int dwTermID;
    }

    public static class TSdkDataTermAudio extends Structure {
        public int dwTermID;
        public Pointer pAudData;
        public int nDataSize;
    }

    public static class TSdkDataTermVideo extends Structure {
        public int dwTermID;
        public Pointer pVidFrm;      // H264视频帧
        public int nFrmSize;     // 帧大小
        public boolean bIsKeyFrm;    // TRUE:为I帧 ; FALSE:为P帧
    }

    public static class TSdkDataPcMicAudio extends Structure {
        public Pointer pPcmData;
        public int nDataSize;
    }

    public static class TSdkDataBypassAudio extends Structure {
        public int dwTermID;
        public Pointer pPcmData;
        public int nDataSize;
    }

    public static class TSdkDataTermMp3L extends Structure {
        public int dwTermID;
        public Pointer pMp3Data;
        public int nDataSize;
    }

    public static class TSdkDataTermMp3R extends Structure {
        public int dwTermID;
        public Pointer pMp3Data;
        public int nDataSize;
    }

    public static class TSdkDataTermPcmL extends Structure {
        public int dwTermID;
        public Pointer pPcmData;
        public int nDataSize;
    }

    public static class TSdkDataTermPcmR extends Structure {
        public int dwTermID;
        public Pointer pPcmData;
        public int nDataSize;
    }

    public static class TSdkTermUserData extends Structure {
        public byte[] UserData = new byte[32 + 1];
        public byte[] WriteDate = new byte[8 + 1];
    }

    public static class TSdkNvrChan extends Structure {
        public int nChanNum;      // 1～MAX_NVR_CHAN
        public boolean bIsLocal;      // 1:本地模拟拾音器 ; 0:网络拾音器
        public boolean bValid;        // 1:有效(有设备注册) ; 0:无效(无设备注册)
        public byte[] ChanName = new byte[32];  // 通道名称
        public boolean bIsOnline;     // 1:在线 ; 0:不在线
        public boolean bIsRecording;  // 1:正在录像 ; 0:停止录像
    }

    public static class TSdkNvrChanList extends Structure {
        public TSdkNvrChan[] ChanList = new TSdkNvrChan[MAX_NVR_CHAN];
    }


    public static class TSdkTime extends Structure {
        public short Year;   // 年 : 2018 ~ 2999
        public byte Month;  // 月 : 1 ~ 12
        public byte Day;    // 日 : 1 ~ 31
        public short Hour;   // 时 : 0 ~ 23
        public byte Minute; // 分 : 0 ~ 59
        public byte Second; // 秒 : 0 ~ 59
        public int MillSecond; // 毫秒 : 0 ~ 999
    }

    public static class TSdkDataNvrMp3 extends Structure {
        public int dwTermID;
        public int nChanNum;
        public Pointer pMp3Data;
        int nDataSize;
        public TSdkTime Time; // 时钟戳
    }

    public static class TSdkDataNvrPcm extends Structure {
        public int dwTermID;
        public int nChanNum;
        public Pointer pPcmData;
        public int nDataSize;
        public TSdkTime Time; // 时钟戳
    }

    public static class TSdkDataNvrPlayMp3 extends Structure {
        public int dwTermID;
        public int nChanNum;
        public Pointer pMp3Data;
        public int nDataSize;
        public TSdkTime Time; // 时钟戳
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

    // 此回调函数相当重要，所有设备注册、设备连接、音频视频数据、操作应答、状态事件 等都由回调函数返回，
// 所以要注意：回调函数不能阻塞 !!!!!!
    public interface OnCtsSdkCallBack extends StdCallCallback {
        public int invoke(int eCbType, Pointer pParam, int dwSize, int usr_data) throws IOException, ClassNotFoundException;
    }

    // 对讲
    public int TSDK_Init(OnCtsSdkCallBack CbFunc, boolean bServerMode, int usr_data); // SDK 初始化。

    public int TSDK_DeInit(); // SDK 注销。

    public int TSDK_Req_OpenTermAudVid(int dwTermID); // 请求打开终端实时音频视频。

    public int TSDK_Req_CloseTermAudVid(int dwTermID); // 请求关闭终端实时音频视频。

    public int TSDK_Req_OpenTermTalk(int dwTermID);   // 请求与终端打开对讲。

    public int TSDK_Req_OpenTermTalk_R(int dwTermID); // 请求与终端打开对讲（使用右声道，只支持网络拾音器）。

    public int TSDK_Req_CloseTermTalk(int dwTermID);  // 请求与终端关闭对讲。

    public int TSDK_Send_485PipeData(int dwTermID, Pointer pFrmData, int nFrmLen); // 发送485管道数据

    public int TSDK_Send_TalkAudio(int dwTermID, Pointer pPcmData, int nDataLen); // 发送对讲音频数据，音频格式为：PCM音频，8K采样率，16位/样,  nDataLen:必须为320倍数，建议640或960

    public int TSDK_Req_GetAudLevel(int dwTermID); // 请求获取终端音量级别

    public int TSDK_Req_SetAudLevel(int dwTermID, TSdkAudioLevel pLevel); // 请求设置终端音量级别

    public int TSDK_Req_SetAudLevelEx(int dwTermID, TSdkAudioLevelEx pLevel); // 请求设置终端音量值和级别。

    public int TSDK_Req_DismissTerm(int dwTermID);  // 请求解除分机呼叫

    public int TSDK_Req_GetDbAlmCfg(int dwTermID); // 请求获取终端喧哗报警配置

    public int TSDK_Req_SetDbAlmCfg(int dwTermID, TSdkDbAlmCfg pCfg); // 请求设置终端喧哗报警配置

    public int TSDK_Req_IFrame(int dwTermID); // 请求I帧

    public int TSDK_Req_SetAlmOut(int dwTermID, byte AlmOutMask, byte AlmOutState); // 请求设置终端报警输出状态

    public int TSDK_Req_SetTermName(int dwTermID, Pointer pTermName); // 请求设置终端名称

    public int TSDK_Req_OpenTermAudVid_Sub(int dwTermID); // 请求打开终端实时音频视频（子通道）。

    public int TSDK_Req_CloseTermAudVid_Sub(int dwTermID); // 请求关闭终端实时音频视频（子通道）。

    public int TSDK_Req_IFrame_Sub(int dwTermID); // 请求I帧（子通道）

    public int TSDK_Req_OpenTermPcm(int dwTermID); // 请求打开终端实时PCM音频(16K采样率16位)。

    public int TSDK_Req_CloseTermPcm(int dwTermID); // 请求关闭终端实时PCM音频(16K采样率16位)。

    public int TSDK_Req_SetWaitTime(int dwTermID, int nWaitTime); // 请求设置终端呼叫铃声播放时间(单位:秒)。

    // 广播
    public int TSDK_SetupGroup(int nBroadNum, TSdkGroupTermList pTermList);   // 建立广播分组, 支持空分组(TermList[]全为0),  1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_GroupAddTerm(int nBroadNum, TSdkGroupTermList pTermList); // 往分组添加广播终端，可以一次添加多个,  1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_GroupDelTerm(int nBroadNum, TSdkGroupTermList pTermList); // 从分组删除广播终端，可以一次删除多个,  1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_GetTermGroupNum(int dwTermID); // 查询终端属于哪个分组，返回0代表未分组，>0代表分组号，<0为错误代码

    public int TSDK_StartBroadVoice(int nBroadNum); // 1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_BroadPcmVoice(int nBroadNum, Pointer pPcmData, int nDataLen); // 发送广播音频数据，音频格式为：PCM音频，8K采样率，16位/样,  nDataLen:必须为320倍数，建议640或960

    public int TSDK_StopBroadVoice(int nBroadNum);

    public int TSDK_StartBroadMp3(int nBroadNum);   // 1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_BroadMp3File(int nBroadNum, Pointer pFilePath);

    public int TSDK_PauseMp3Broad(int nBroadNum, boolean bPause); // bPause=0:暂停 , bPause=1:继续

    public int TSDK_StopBroadMp3(int nBroadNum);

    public int TSDK_StartBroadPcm(int dwTermID, enPcmSmpRate eRate);   // 1 <= nBroadNum <= MAX_BROAD_GROUP

    public int TSDK_BroadPcmL(int dwTermID, Pointer pPcmData, int nDataLen); // 播放左声道PCM

    public int TSDK_BroadPcmR(int dwTermID, Pointer pPcmData, int nDataLen); // 播放右声道PCM

    public int TSDK_StopBroadPcm(int dwTermID);

    // 播放压缩音频视频
    public int TSDK_Player_AudVidOpen(int nAudVidNum, Pointer hDisplayWnd); // nAudVidNum 范围:1～MAX_AUDVID_NUM

    public int TSDK_Player_AudVidAdd(int nAudVidNum, enSdkAudVidType eDataType, Pointer pAudVidData, int nDataSize); // nAudVidNum 范围:1～MAX_AUDVID_NUM

    public int TSDK_Player_AudVidClose(int nAudVidNum); // nAudVidNum 范围:1～MAX_AUDVID_NUM

    // 获取电脑麦克风音频
    public int TSDK_Player_PcMicOpen();

    public int TSDK_Player_PcMicClose();

    // 播放PCM音频(未压缩)(8K采样率16位)
    public int TSDK_Player_PcmOpen(int nPcmNum); // nPcmNum 范围:1～MAX_PCM_NUM

    public int TSDK_Player_PcmAdd(int nPcmNum, Pointer pPcmData, int nDataSize); // nPcmNum 范围:1～MAX_PCM_NUM

    public int TSDK_Player_PcmClose(int nPcmNum); // nPcmNum 范围:1～MAX_PCM_NUM

    // 预览网络拾音器MP3音频，最多支持一个设备两个声道同时预览 !!!!!!
    public int TSDK_Req_PreviewMp3L(int dwTermID, boolean bEnable); // 预览第一声道MP3

    public int TSDK_Req_PreviewMp3R(int dwTermID, boolean bEnable); // 预览第二声道MP3

    // 预览网络拾音器PCM音频(16K采样率16位)，最多支持一个设备两个声道同时预览 !!!!!!
    public int TSDK_Req_PreviewPcmL(int dwTermID, boolean bEnable); // 预览第一声道PCM(16K采样率16位)

    public int TSDK_Req_PreviewPcmR(int dwTermID, boolean bEnable); // 预览第二声道PCM(16K采样率16位)

    // 分析网络拾音器当前 DB 值
    public int TSDK_Req_DbAnalyze(int dwTermID, boolean bEnable); // 分析第一声道 DB 值

    // 获取终端客户数据
    public int TSDK_GetTermUserData(int dwTermID, TSdkTermUserData pData);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
// NVR API 接口
    public int TSDK_NvrGetChanList(int dwTermID, TSdkNvrChanList pChanList); // 获取NVR通道列表信息

    public int TSDK_NvrOpenChanMp3(int dwTermID, int nChanNum);  // nChanNum 范围:1～MAX_NVR_CHAN

    public int TSDK_NvrCloseChanMp3(int dwTermID, int nChanNum); // nChanNum 范围:1～MAX_NVR_CHAN

    public int TSDK_NvrOpenChanPcm(int dwTermID, int nChanNum);  // nChanNum 范围:1～MAX_NVR_CHAN

    public int TSDK_NvrCloseChanPcm(int dwTermID, int nChanNum); // nChanNum 范围:1～MAX_NVR_CHAN

    public int TSDK_NvrGetChan(int dwTermID, int nChanNum, TSdkNvrChan pNvrChan); // nChanNum 范围:1～MAX_NVR_CHAN

    public int TSDK_Req_NvrSrchRecFile(int dwTermID, int nChanNum, TSdkTime pDate);

    // nChanNum 范围:1～MAX_NVR_CHAN, pDate:日期(年月日)
    public int TSDK_Req_NvrPlayRecFile(int dwTermID, int nChanNum, TSdkTime pDate, Pointer pFileName, boolean bSdkPlaySound);

    // nChanNum 范围:1～MAX_NVR_CHAN, pDate:日期(年月日), pFileName:录音文件名称
    // 暂时只支持1路录音回放
    public int TSDK_NvrSetPlayFilePos(int dwFilePos); // dwFilePos:文件字节位置

    public int TSDK_NvrPausePlay(boolean bPause); // TRUE:暂停播放, FALSE:继续播放

    public int TSDK_NvrStopPlay(); // 结束回放*/

}
