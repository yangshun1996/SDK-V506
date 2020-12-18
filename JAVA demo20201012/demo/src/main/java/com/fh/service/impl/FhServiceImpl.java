package com.fh.service.impl;

import com.fh.callback.OnCtsSdkCallBack;
import com.fh.service.FhService;
import com.fh.utils.fh.CtsSdk;
import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.annotation.Order;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Component;
import org.springframework.stereotype.Service;

@Slf4j
@Service
public class FhServiceImpl implements FhService {

    static CtsSdk netSinkLib = CtsSdk.COMMON_INSTANCE;

    static OnCtsSdkCallBack onCtsSdkCallBack = new OnCtsSdkCallBack();

    /*@Component
    @Order()
    public class MyRunner implements CommandLineRunner {
        @Override
        public void run(String... strings) throws Exception {
            netSinkLib.TSDK_DeInit();
            OnCtsSdkCallBack onCtsSdkCallBack = new OnCtsSdkCallBack();
            boolean a = true;
            int b = 0x12345678;
            int tsdk_init = netSinkLib.TSDK_Init(onCtsSdkCallBack, a, b);
            netSinkLib.TSDK_Req_OpenTermAudVid(0x10001);
            while (tsdk_init == 0){

            }
        }
    }*/


    @Override
    public int initSdk() {

        boolean a = true;
        int b = 0x12345678;
        int tsdkInit = netSinkLib.TSDK_Init(onCtsSdkCallBack, a, b);
        if (tsdkInit == 0) {
            System.out.println("ok");
            //log.info("烽火sdk初始化成功");
        } else {
            //log.info("烽火sdk初始化失败");
        }
        return tsdkInit;
    }

    @Override
    public int deInitSdk() {

        int tsdkDeInit = netSinkLib.TSDK_DeInit();
        if (tsdkDeInit == 0) {
            //log.info("烽火sdk注销成功");
        } else {
            //log.info("烽火sdk注销失败");
        }

        return tsdkDeInit;
    }

    @Override
    public Integer openTermAudVid(int dwTermID) {
        return netSinkLib.TSDK_Req_OpenTermAudVid(dwTermID);
    }

    @Override
    public int closeTermAudVid(int dwTermID) {
        return netSinkLib.TSDK_Req_CloseTermAudVid(dwTermID);
    }

    @Override
    public int previewMp3L(int dwTermID, boolean bEnable) {
        return netSinkLib.TSDK_Req_PreviewMp3L(dwTermID, bEnable);
    }

    @Override
    public int previewMp3R(int dwTermID, boolean bEnable) {

        return netSinkLib.TSDK_Req_PreviewMp3R(dwTermID, bEnable);
    }

    @Override
    public int previewPcmL(int dwTermID, boolean bEnable) {
        return netSinkLib.TSDK_Req_PreviewPcmL(dwTermID,bEnable);
    }

    @Override
    public int previewPcmR(int dwTermID, boolean bEnable) {
        return netSinkLib.TSDK_Req_PreviewPcmR(dwTermID,bEnable);
    }

    @Override
    public int dbAnalyze(int dwTermID, boolean bEnable) {
        return netSinkLib.TSDK_Req_DbAnalyze(dwTermID,bEnable);
    }

    @Override
    public int play() {
        CtsSdk.TSdkAudioLevel tSdkAudioLevelEx = new CtsSdk.TSdkAudioLevel();
        tSdkAudioLevelEx.eAmpLevel = 1;
        int i4 = netSinkLib.TSDK_Req_SetAudLevel(0x10001, tSdkAudioLevelEx);
        System.out.println("设置音量==" + i4);

        CtsSdk.TSdkGroupTermList pTermList = new CtsSdk.TSdkGroupTermList();
        pTermList.TermList[0] = 0x10001;
        int i2 = netSinkLib.TSDK_SetupGroup(1, pTermList);//建立广播分组
        System.out.println("建立广播==" + i2);

        netSinkLib.TSDK_StartBroadMp3(1);//开启广播MP3sfsf

        Pointer p2 = new Memory(15);
        p2.setString(0, "E:\\a.mp3");
        netSinkLib.TSDK_BroadMp3File(1,p2);//广播MP3文件
        return 0;
    }


}
