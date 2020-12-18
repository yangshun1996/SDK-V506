package com.fh.controller;

import com.fh.service.FhService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/fh")
public class FhController {

    @Autowired
    private FhService fhService;


    @GetMapping(value = "/init")
    public void init() throws InterruptedException {
        fhService.deInitSdk();
        fhService.initSdk();
        System.out.println("init" + Thread.currentThread().getId() + ","+Thread.currentThread().getName() );
        Thread.sleep(5000L);
    }

    @GetMapping(value = "/previewMp3L")
    public void previewMp3L(){
        int a = 0x10001;
        boolean b = true;
        int i = fhService.previewMp3L(a, b);
        System.out.println("previewMp3L:" + i);
    }

    @GetMapping(value = "/deInit")
    public void deInit(){
        fhService.deInitSdk();
    }

    @GetMapping(value = "/openTermAudVid")
    public void openTermAudVid(){

       /* Runnable b = ()->{
            System.out.println(Thread.currentThread().getId() + ","+Thread.currentThread().getName() );
            int a = 0x10001;
            Integer i = fhService.openTermAudVid(a);
            while (true){}
        };
        Thread t = new Thread(b);
        t.start();

        System.out.println("main:"+Thread.currentThread().getId() + ","+Thread.currentThread().getName() );
*/
        int a = 0x10001;
        Integer i = fhService.openTermAudVid(a);
        System.out.println("openTermAudVid" + Thread.currentThread().getId() + ","+Thread.currentThread().getName() );

    }

    @GetMapping(value = "/previewPcmL")
    public void previewPcmL(){
        int a = 0x10001;
        int i = fhService.previewPcmL(a,true);
        System.out.println("previewPcmL:" + i);
    }

    @GetMapping(value = "/previewPcmR")
    public void previewPcmR(){
        int a = 0x10001;
        int i = fhService.previewPcmR(a,true);
        System.out.println("previewPcmR:" + i);
    }

    @GetMapping(value = "/dbAnalyze")
    public void dbAnalyze(){
        int a = 65537;
        int i = fhService.previewPcmR(a,true);
        System.out.println("dbAnalyze:" + i);
    }



}
