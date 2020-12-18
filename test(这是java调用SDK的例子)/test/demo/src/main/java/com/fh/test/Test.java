package com.fh.test;

import com.fh.callback.OnCtsSdkCallBack;
import com.fh.utils.fh.CtsSdk;
import lombok.extern.slf4j.Slf4j;

@Slf4j
public class Test {

    static CtsSdk netSinkLib = CtsSdk.COMMON_INSTANCE;


    public static void main(String[] args) throws InterruptedException {
        netSinkLib.TSDK_DeInit();
        OnCtsSdkCallBack onCtsSdkCallBack = new OnCtsSdkCallBack();
        boolean a = true;
        int b = 0x12345678;
        int tsdk_init = netSinkLib.TSDK_Init(onCtsSdkCallBack, a, b);
        Thread.sleep(5000L);
        netSinkLib.TSDK_Req_OpenTermAudVid(0x10001);

        while (tsdk_init == 0){
            Thread.sleep(2000L);
            log.info("FHTEST");
        }
    }
}
