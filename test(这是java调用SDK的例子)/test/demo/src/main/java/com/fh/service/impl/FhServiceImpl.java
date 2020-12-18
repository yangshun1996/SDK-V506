package com.fh.service.impl;

import com.fh.callback.OnCtsSdkCallBack;
import com.fh.service.FhService;
import com.fh.utils.fh.CtsSdk;
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
            log.info("烽火sdk初始化成功");
        } else {
            log.info("烽火sdk初始化失败");
        }
        return tsdkInit;
    }

    @Override
    public int deInitSdk() {

        int tsdkDeInit = netSinkLib.TSDK_DeInit();
        if (tsdkDeInit == 0) {
            log.info("烽火sdk注销成功");
        } else {
            log.info("烽火sdk注销失败");
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


}
