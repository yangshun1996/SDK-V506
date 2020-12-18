package com.fh.callback;

import com.fh.utils.fh.CtsSdk;
import com.fh.utils.fh.ToolKits;
import com.google.common.primitives.Bytes;
import com.sun.jna.Pointer;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.print.Book;
import java.io.*;
import java.util.ArrayList;
import java.util.concurrent.BlockingQueue;

import static com.fh.utils.fh.CtsSdk.enSdkCbType.*;

public class OnCtsSdkCallBack implements CtsSdk.OnCtsSdkCallBack {

    private static OnCtsSdkCallBack instance = new OnCtsSdkCallBack();

    private static byte[] G_buffer = new byte[0];

    private static BlockingQueue<byte[]> queue;

    public OnCtsSdkCallBack() {

    }

    public static OnCtsSdkCallBack getInstance() {
        return instance;
    }

    public  static  byte[]  byteMerger(byte[] a,byte[] b){
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
        		CtsSdk.TSdkEventTermRegister tSdkTermRegister = null;
        		ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermRegister);
        		break;
            case CB_Event_TermConnect:
            	CtsSdk.TSdkEventTermConnect tSdkTermConnect=new CtsSdk.TSdkEventTermConnect();
            	ToolKits.GetPointerDataToStruct(pParam, 0, tSdkTermConnect);
            	byte[] bTermMac= tSdkTermConnect.TermMac;
            	String s = new String(bTermMac);
                System.out.println("bTermMac : " + s);
                return 0;
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
            	CtsSdk netSinkLib = CtsSdk.COMMON_INSTANCE;
            	Pointer mPointer=ToolKits.GetGBKStringToPointer("C:\\Users\\lyy\\Desktop\\巡检\\对讲广播SDK__V506\\对讲广播SDK__V506\\test\\demo\\a.mp3");
            	netSinkLib.TSDK_BroadMp3File(1, mPointer);
//            	netSinkLib.TSDK_StartBroadMp3(1);
                break;
        }
        return 0;
    }

    /**
     * byte转对象
     *
     * @param bytes
     * @return
     */
    private Object ByteToObject(byte[] bytes) {
        Object obj = null;
        try {
            // bytearray to object
            ByteArrayInputStream bi = new ByteArrayInputStream(bytes);
            ObjectInputStream oi = new ObjectInputStream(bi);

            obj = oi.readObject();
            bi.close();
            oi.close();
        } catch (Exception e) {
            System.out.println("translation" + e.getMessage());
            e.printStackTrace();
        }
        return obj;
    }
}
