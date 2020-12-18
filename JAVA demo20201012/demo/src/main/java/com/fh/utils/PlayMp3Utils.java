package com.fh.utils;

import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermAudio;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermMp3L;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermMp3R;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermPcmL;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Data_TermPcmR;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Event_TermConnect;
import static com.fh.utils.fh.CtsSdk.enSdkCbType.CB_Event_TermRegister;

import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.BlockingQueue;

import com.fh.utils.fh.CtsSdk;
import com.fh.utils.fh.ToolKits;
import com.sun.jna.Memory;
import com.sun.jna.Pointer;

public class PlayMp3Utils {

	static CtsSdk netSinkLib = CtsSdk.COMMON_INSTANCE;
    private static int m_nTermCnt = 0;
    private static ArrayList<CtsSdk.TSdkEventTermRegister> m_TermList = new ArrayList<CtsSdk.TSdkEventTermRegister>();
    private static ArrayList<Integer> m_TermState = new ArrayList<Integer>();
    
    public static void Mp3Play(int TermID,String termIp,String TermName) throws InterruptedException {
		netSinkLib.TSDK_DeInit();
        OnCtsSdkCallBack onCtsSdkCallBack = new OnCtsSdkCallBack();
        boolean a = true;
        int b = 0x12345678;
        int tsdk_init = netSinkLib.TSDK_Init(onCtsSdkCallBack, a, b);
        if(tsdk_init==0) {
        	System.out.println("SDK 初始化成功！");
        	loadTermList(TermID,termIp,TermName);
        }else {
        	System.out.println("SDK 初始化失败！错误代码:"+tsdk_init);
        }
    }
    
    private static void loadTermList(int TermID,String termIp,String TermName) {
    	CtsSdk.TSdkEventTermRegister item =new CtsSdk.TSdkEventTermRegister();
    	item.dwTermID=TermID;
    	item.TermName=TermName.getBytes();
    	item.TermIp=termIp.getBytes();
    	m_TermList.add(item);
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
                
                	int b=0;
//        			for(b=0; b<m_nTermCnt; b++){
//        				if( (m_TermList.get(b).dwTermID == tSdkTermConnect.dwTermID) 
//        						&&m_TermList.get(b).TermMac==tSdkTermConnect.TermMac){
//        					break;
//        				}
//        			}
        			
        			if(b < m_nTermCnt){
        				// 已注册过
        				if(m_TermList.get(b).TermIp.equals(tSdkTermConnect.TermIp)) {

//            				m_TermList.get(b).eTermType = tSdkTermConnect.eTermType;
            				m_TermState.add(b, CtsSdk.enSdkDevState.TSDK_DEV_STATE_ONLINE);
        				}

        				
        				CtsSdk.TSdkGroupTermList pTermList=new CtsSdk.TSdkGroupTermList();
        				pTermList.TermList[b] = m_TermList.get(b).dwTermID;
        				int nResult = netSinkLib.TSDK_SetupGroup(1, pTermList);
        				if(nResult != 0){
        					System.out.println("创建分区失败！错误代码:"+nResult);
        					return nResult;
        				}else {
        					System.out.println("创建分区成功");
        					int nMp3=netSinkLib.TSDK_StartBroadMp3(1);//开启广播MP3
        					if(nMp3==0) {
        						System.out.println("开启广播MP3成功");
        						
        						Pointer porMp3File=new Memory(15);
        						porMp3File.setString(0, "a.mp3");
        						
        						int nMp3Filet= netSinkLib.TSDK_BroadMp3File(1, porMp3File);
        						if(nMp3Filet != 0) {
        							System.out.println("广播MP3文件失败！错误代码:"+nMp3Filet);
        							return nMp3Filet;
        						}
        					
        					}else {
        						System.out.println("开启广播MP3失败！错误代码:"+nResult);
        						return nResult;
        					}
        				}
        				
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
            }
            return 0;
        }

       
    }
	
}
