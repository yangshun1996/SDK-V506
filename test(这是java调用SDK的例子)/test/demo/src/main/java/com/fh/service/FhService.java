package com.fh.service;

public interface FhService {

    int initSdk();//初始化 SDK

    int deInitSdk();//注销 SDK

    Integer openTermAudVid(int dwTermID); // 请求打开终端实时音频视频。

    int closeTermAudVid(int dwTermID); // 请求关闭终端实时音频视频。

    int previewMp3L(int dwTermID, boolean bEnable); // 预览第一声道MP3

    int previewMp3R(int dwTermID, boolean bEnable); // 预览第二声道MP3

    int previewPcmL(int dwTermID, boolean bEnable); // 预览第一声道PCM(16K采样率16位)

    int previewPcmR(int dwTermID, boolean bEnable); // 预览第二声道PCM(16K采样率16位)

    int dbAnalyze(int dwTermID, boolean bEnable); // 分析第一声道 DB 值

}
