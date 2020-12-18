package com.fh.test;

import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermAudio;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermMp3L;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermMp3R;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermPcmL;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermPcmR;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Event_TermConnect;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Event_TermRegister;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Post_TermState;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Post_Mp3PlayFinish;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Asw_SetAudLevel;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Post_TermSos;



import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.ArrayList;
import java.util.concurrent.BlockingQueue;
import com.fh.utils.fh.CtsSdk;
import com.fh.utils.fh.CtsSdk.enSdkAudAmpLevel;
import com.fh.utils.fh.CtsSdk.enSdkAudCapLevel;
import com.fh.utils.fh.CtsSdk.enSdkAudCapTalkLevel;
import com.fh.utils.fh.ToolKits;
import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import lombok.extern.slf4j.Slf4j;

@Slf4j
public class Test {

    static CtsSdk netSinkLib = CtsSdk.COMMON_INSTANCE;
    private static int m_nTermCnt=0;
    private static ArrayList<CtsSdk.TSdkEventTermRegister>m_TermList =new ArrayList<CtsSdk.TSdkEventTermRegister>();
    private static ArrayList<Integer>m_TermState=new ArrayList<Integer>();
    private static int index=-1;


	public static void main(String[] args) throws InterruptedException {

		netSinkLib.TSDK_DeInit();
		OnCtsSdkCallBack onCtsSdkCallBack = new OnCtsSdkCallBack();
		boolean a = true;
		int b = 0x12345678;
		int tsdk_init = netSinkLib.TSDK_Init(onCtsSdkCallBack, a, b);
		if(tsdk_init==0) {
			System.out.println("SDK 初始化成功！");
			loadTermList();
		}else {
			System.out.println("SDK 初始化失败！错误代码:"+tsdk_init);
		}


        Thread.sleep(10000L);
		int i3 = netSinkLib.TSDK_Req_OpenTermAudVid(0x10001);
		System.out.println("打开音频==" + i3);

		CtsSdk.TSdkGroupTermList pTermList = new CtsSdk.TSdkGroupTermList();
		pTermList.TermList[0] = 0x10001;
		int i2 = netSinkLib.TSDK_SetupGroup(1, pTermList);//建立广播分组
		System.out.println("建立广播==" + i2);

		netSinkLib.TSDK_StartBroadMp3(1);//开启广播MP3sfsf

		//设置音量
		CtsSdk.TSdkAudioLevel tSdkAudioLevelEx = new CtsSdk.TSdkAudioLevel();
		tSdkAudioLevelEx.eAmpLevel = 1;
		int i4 = netSinkLib.TSDK_Req_SetAudLevel(0x10001, tSdkAudioLevelEx);
		System.out.println("设置音量==" + i4);


		Pointer p2 = new Memory(15);
		p2.setString(0, "E:\\a.mp3");
		netSinkLib.TSDK_BroadMp3File(1,p2);//广播MP3文件

//		//对讲
//		int i5 = netSinkLib.TSDK_Req_OpenTermTalk(0x10001);
//		System.out.println("对讲==" + i5);
//		int i2 = netSinkLib.TSDK_Player_PcMicOpen();
//		System.out.println("获取麦克风" + i2);


		while (tsdk_init == 0){
			int i1 = netSinkLib.TSDK_Req_GetAudLevel(0x10001);
			System.out.println("音量===" + i1);

			int i = netSinkLib.TSDK_GetTermGroupNum(65537);
			System.out.println("分组" + i);

			Thread.sleep(2000L);
		}
	}

	private static void loadTermList() {
		CtsSdk.TSdkEventTermRegister item =new CtsSdk.TSdkEventTermRegister();
		item.dwTermID=0x10001;
//		item.dwTermID=22;
		String strTermName="1";
		item.TermName=strTermName.getBytes();
		String strTermIp="192.168.10.156";
		item.TermIp=strTermIp.getBytes();
		m_TermList.add(item);


//		CtsSdk.TSdkEventTermRegister item1 =new CtsSdk.TSdkEventTermRegister();
//		item1.dwTermID=0x1002E;
//		String strTermName1="POE TEST";
//		item1.TermName=strTermName1.getBytes();
//		String strTermIp1="192.168.1.113";
//		item1.TermIp=strTermIp1.getBytes();
//		m_TermList.add(item1);

		m_nTermCnt=1;
	}



	private static class OnCtsSdkCallBack implements CtsSdk.OnCtsSdkCallBack {
        private byte[] G_buffer = new byte[0];
        private BlockingQueue<byte[]> queue;
        public byte[]  byteMerger(byte[] a,byte[] b){
            byte[] temp = new  byte[a.length+b.length];
            System.arraycopy(a,0,temp,0,a.length);
            System.arraycopy(b,0,temp,a.length,b.length);
            return  temp;
        }

        @Override
        public int invoke(int eCbType, Pointer pParam, int dwSize, int usr_data) throws IOException, ClassNotFoundException {
            System.out.println(eCbType);
            switch (eCbType) {
            	case CB_Event_TermRegister:
            		CtsSdk.TSdkEventTermRegister tSdkTermRegister = new CtsSdk.TSdkEventTermRegister();
            		ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermRegister);
            		int i;
            		for(i=0;i<m_nTermCnt;i++) {
            			if(m_TermList.get(i).dwTermID == tSdkTermRegister.dwTermID)
        					break;
            		}
            		if(i < m_nTermCnt){
        				// 更新
        				m_TermList.add(tSdkTermRegister);
        			}else {
        				// 注册
        				if(m_nTermCnt >= CtsSdk.MAX_TERM_COUNT)
        					return -1; // 返回 0 代表注册成功，否则代表注册失败 !!!
        				m_TermList.add(tSdkTermRegister);
        				m_TermState.add(CtsSdk.enSdkDevState.TSDK_DEV_STATE_OFFLINE);
        				m_nTermCnt=1;
        			}
            		return 0;
                case CB_Event_TermConnect:
                	CtsSdk.TSdkEventTermConnect tSdkTermConnect= new CtsSdk.TSdkEventTermConnect();
                	ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermConnect);
                	
//                	System.out.println(">>> %s 请求连接：终端ID:%X , %s视频 , MAC地址:%02X-%02X-%02X-%02X-%02X-%02X , IP地址:%s , 名称:%s",
//                			tSdkTermConnect.eTermType, tSdkTermConnect.dwTermID, tSdkTermConnect.bHasVideo ? "有" : "无",
//                			"", tSdkTermConnect.TermIp, tSdkTermConnect.TermName);
                	
                	
                	System.out.println("终端ID:"+tSdkTermConnect.dwTermID);
                	System.out.println("名称:"+new String(tSdkTermConnect.TermName));
                	
//                	System.out.println("连接dwTermID:"+tSdkTermConnect.dwTermID);
                
                	index=0;
        			for(index=0; index<m_nTermCnt; index++){
        				if( (m_TermList.get(index).dwTermID == tSdkTermConnect.dwTermID)){
        					break;
        				}
        			}
        			
        			if(index < m_nTermCnt){
        				//bfmp3();
        				return 0; // 返回 0 代表允许连接，否则代表不允许 !!!
        			}
        			else{
        				// 未注册
        				return -1; // 返回 0 代表允许连接，否则代表不允许 !!!
        			}
        			
            
                case CB_Data_TermMp3L:
                    CtsSdk.TSdkDataTermMp3L tSdkDataTermMp3L = new CtsSdk.TSdkDataTermMp3L();
                    ToolKits.GetPointerDataToStruct(pParam, 0, tSdkDataTermMp3L);
                    Pointer pMp3Data = tSdkDataTermMp3L.pMp3Data;
                    if (pMp3Data == null){
                        return 0;
                    }

                    byte[] recive_buf = pMp3Data.getByteArray(0, tSdkDataTermMp3L.nDataSize);
                    //判断缓存数组是否超过2048字节，如果超出则写入队列，然后清空数组
                    int gBufferLen = G_buffer.length;
                    int reciveBufferLen = recive_buf.length;
                    int totalLen = gBufferLen + reciveBufferLen;

                    if(gBufferLen+reciveBufferLen >= 2048){
                        byte[] temBuffer01 = new byte[2048-gBufferLen];
                        System.arraycopy(recive_buf,0,temBuffer01,0,2048-gBufferLen);
                        G_buffer = byteMerger(G_buffer,temBuffer01);
                        //写入队列
                        System.out.println("bufferTemp2048:"+ G_buffer.length);

                        //释放数组
                        G_buffer = new byte[0];
                        byte[] temBuffer02 = new byte[totalLen-2048];
                        System.arraycopy(recive_buf,2048-gBufferLen,temBuffer02,0,totalLen-2048);
                        G_buffer = byteMerger(G_buffer,temBuffer02);
                        System.out.println("bufferTemp:"+ G_buffer.length);
                    }
                    else {
                        G_buffer = byteMerger(G_buffer,recive_buf);
                    }
                    queue.add(G_buffer);
                    return 0;
                case CB_Data_TermMp3R:
                    System.out.println("____________________");
                    break;
                case CB_Data_TermPcmL:

                    break;
                case CB_Data_TermPcmR:
                    break;
                case CB_Data_TermAudio://播放音频
                    break;
                case CB_Asw_SetAudLevel://播放音频
                	
                    break;
            	case CB_Post_TermState://202 // 推送终端状态
            		CtsSdk.TSdkPostTermState tSdkTermState= new CtsSdk.TSdkPostTermState();
            		ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermState);
            		System.out.println("tSdkTermState.dwTermID:"+ tSdkTermState.dwTermID);
            		System.out.println("tSdkTermState.eTermState:"+ tSdkTermState.eTermState);
            		System.out.println("tSdkTermState.AlmOutMask:"+ tSdkTermState.AlmOutMask);
            		System.out.println("tSdkTermState.AlmOutState:"+ tSdkTermState.AlmOutState);
                	break;	
            	case CB_Post_TermSos://202 // 推送终端状态
            		CtsSdk.TSdkPostTermSos tSdkTermSos=new CtsSdk.TSdkPostTermSos();
            		ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermSos);
            		System.out.println("tSdkTermSos.dwTermID:"+ tSdkTermSos.dwTermID);
            		System.out.println("tSdkTermSos.nSosType:"+ tSdkTermSos.nSosType);
            		break;
            	 case CB_Post_Mp3PlayFinish://204; 推送MP3文件播放结束通知
            		 if(index>=0) {
            			 netSinkLib.TSDK_StopBroadMp3(index);
            		 }
            		 break;
                	
            }
            return 0;
        }
    }
    
    private static void bfmp3() {
    	if(index<0) {
    		return;
    	}
		CtsSdk.TSdkGroupTermList pTermList=new CtsSdk.TSdkGroupTermList();
		pTermList.TermList[index] = m_TermList.get(index).dwTermID;
		int nResult = netSinkLib.TSDK_SetupGroup(index+1, pTermList);
		if(nResult != 0){
			System.out.println("创建分区失败！错误代码:"+nResult);
		}else {
			System.out.println("创建分区成功");
			int nMp3=netSinkLib.TSDK_StartBroadMp3(index+1);//开启广播MP3
			if(nMp3==0) {
				System.out.println("开启广播MP3成功");
				
				Pointer porMp3File=new Memory(15);
				porMp3File.setString(0, "E:\\a.mp3");
			
				int nMp3Filet= netSinkLib.TSDK_BroadMp3File(index+1, porMp3File);
				if(nMp3Filet != 0) {
					System.out.println("广播MP3文件失败！错误代码:"+nMp3Filet);
				}
			
			}else {
				
				System.out.println("开启广播MP3失败！错误代码: "+nMp3);
				if(nMp3 == -807)
				netSinkLib.TSDK_StopBroadMp3(index+1);
				
			}
		}
    }

}
