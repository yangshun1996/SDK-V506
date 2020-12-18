package com.fh.controller;

import com.fh.service.FhService;
import com.fh.utils.PlayMp3Utils;

import java.util.HashMap;
import java.util.Map;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
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

    //打开音频
    @GetMapping(value = "/openTermAudVid")
    public void openTermAudVid(){
        int a = 0x10001;
        Integer i = fhService.openTermAudVid(a);
        System.out.println("openTermAudVid" + Thread.currentThread().getId() + ","+Thread.currentThread().getName() );

    }

    //播放
    @GetMapping(value = "/play")
    public void play(){


        fhService.play();
    }

    @GetMapping(value = "/previewPcmL")
    public void previewPcmL(){
        int a = 1;
        int i = fhService.previewPcmL(a,true);
        System.out.println("previewPcmL:" + i);
    }

    @GetMapping(value = "/previewPcmR")
    public void previewPcmR(){
        int a = 1;
        int i = fhService.previewPcmR(a,true);
        System.out.println("previewPcmR:" + i);
    }

    @GetMapping(value = "/dbAnalyze")
    public void dbAnalyze(){
        int a = 1;
        int i = fhService.previewPcmR(a,true);
        System.out.println("dbAnalyze:" + i);
    }

    @RequestMapping(value = "/playMp3", method = {RequestMethod.POST,RequestMethod.GET})
	public Map<String,Object> playMp3(@RequestParam Map<String, Object> mapParam) {
    	Map<String,Object> map = new HashMap<String,Object>();
        try {
        	String termName="4G网络音柱测试";
//        	String strTermId = (String) mapParam.get("TERMID");
        	String strTermId = "0x10001";
//        	String termIp = (String) mapParam.get("TERMIP");
        	String termIp = "192.168.10.156";
//        	 termName = (String) mapParam.get("TERMNAME");
        	if("".equals(termIp) || termIp == null
        			|| "".equals(strTermId) || strTermId == null
        			){
        		map.put("success", false);
        		map.put("message", "TERMID,TERMIP不能为空!");
        		return map;
        	}
            int termId = Integer.parseInt(strTermId);
            termId = 0x10001;
        	PlayMp3Utils.Mp3Play(termId,termIp,termName);
        	map.put("success", true);
    		map.put("message", "Mp3播放成功!");
		} catch (Exception e) {
			e.printStackTrace();
		}
        return map;
    }

}
