package com.fh.utils.fh;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.io.*;
import java.math.BigInteger;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.security.MessageDigest;
import java.text.SimpleDateFormat;

public class ToolKits {
	static CtsSdk ctsSdk = CtsSdk.COMMON_INSTANCE;


	public ToolKits() {

	}

	public static void GetPointerData(Pointer pNativeData, Structure pJavaStu){
		GetPointerDataToStruct(pNativeData, 0, pJavaStu);
	}

	public static void GetPointerDataToStruct(Pointer pNativeData, long OffsetOfpNativeData, Structure pJavaStu) {
		pJavaStu.write();
		Pointer pJavaMem = pJavaStu.getPointer();
		pJavaMem.write(0, pNativeData.getByteArray(OffsetOfpNativeData, pJavaStu.size()), 0,
				pJavaStu.size());
		pJavaStu.read();
	}

	public static void GetPointerDataToStructArr(Pointer pNativeData, Structure[]pJavaStuArr) {
		long offset = 0;
		for (int i=0; i<pJavaStuArr.length; ++i){
			GetPointerDataToStruct(pNativeData, offset, pJavaStuArr[i]);
			offset += pJavaStuArr[i].size();
		}
	}

	/**
	 * 将结构体数组拷贝到内存
	 * @param pNativeData
	 * @param pJavaStuArr
	 */
	public static void SetStructArrToPointerData(Structure[]pJavaStuArr, Pointer pNativeData) {
		long offset = 0;
		for (int i = 0; i < pJavaStuArr.length; ++i) {
			SetStructDataToPointer(pJavaStuArr[i], pNativeData, offset);
			offset += pJavaStuArr[i].size();
		}
	}

	public static void SetStructDataToPointer(Structure pJavaStu, Pointer pNativeData, long OffsetOfpNativeData){
		pJavaStu.write();
		Pointer pJavaMem = pJavaStu.getPointer();
		pNativeData.write(OffsetOfpNativeData, pJavaMem.getByteArray(0, pJavaStu.size()), 0, pJavaStu.size());
	}

	public static void ByteArrToStructure(byte[] pNativeData, Structure pJavaStu) {
		pJavaStu.write();
		Pointer pJavaMem = pJavaStu.getPointer();
		pJavaMem.write(0, pNativeData, 0, pJavaStu.size());
		pJavaStu.read();
	}

	public static void ByteArrZero(byte[] dst) {
		// 清零
		for (int i = 0; i < dst.length; ++i) {
			dst[i] = 0;
		}
	}

	//byte转换为byte[2]数组
	public static byte[] getByteArray(byte b){
		byte[] array = new byte[8];
		for (int i = 0; i < 8; i++) {
			array[i] = (byte) ((b & (1 << i)) > 0 ? 1:0);
		}

		return array;
	}

    public static byte[] getByteArrayEx(byte b){
        byte[] array = new byte[8];
        for(int i = 7; i >= 0; i--) {
            array[i] = (byte)(b & 1);
            b = (byte)(b >> 1);
        }
        return array;
    }

	public static void StringToByteArr(String src, byte[] dst) {
		try {
			byte[] GBKBytes = src.getBytes("GBK");
			for (int i = 0; i < GBKBytes.length; i++) {
				dst[i] = (byte) GBKBytes[i];
			}
		} catch (Exception e1) {
			e1.printStackTrace();
		}
	}

	public static long GetFileSize(String filePath)
	{
		File f = new File(filePath);
		if (f.exists() && f.isFile()) {
			return f.length();
		}
		else
		{
			return 0;
		}
	}

	public static boolean ReadAllFileToMemory(String file, Memory mem)
	{
		if (mem != Memory.NULL)
		{
			long fileLen = GetFileSize(file);
			if (fileLen <= 0)
			{
				return false;
			}

			try {
				File infile = new File(file);
				if (infile.canRead())
				{
					FileInputStream in = new FileInputStream(infile);
					int buffLen = 1024;
					byte[] buffer = new byte[buffLen];
					long currFileLen = 0;
					int readLen = 0;
					while (currFileLen < fileLen)
					{
						readLen = in.read(buffer);
						mem.write(currFileLen, buffer, 0, readLen);
						currFileLen += readLen;
					}

					in.close();
					return true;
				}
		        else
		        {
		        	System.err.println("Failed to open file %s for read!!!\n");
		            return false;
		        }
			}catch (Exception e)
			{
				System.err.println("Failed to open file %s for read!!!\n");
				e.printStackTrace();
			}
		}

		return false;
	}

	public static void savePicture(byte[] pBuf, String sDstFile)
	{
        try
        {
          	FileOutputStream fos = new FileOutputStream(sDstFile);
        	fos.write(pBuf);
        	fos.close();
        } catch (Exception e){
        	e.printStackTrace();
        }
	}

	public static void savePicture(Pointer pBuf, int dwBufSize, String sDstFile)
	{
        try
        {
        	DataOutputStream out = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(sDstFile)));
        	out.write(pBuf.getByteArray(0, dwBufSize), 0, dwBufSize);
        	out.close();
        } catch (Exception e){
        	e.printStackTrace();
        }
	}

	public static void savePicture(Pointer pBuf, int dwBufOffset, int dwBufSize, String sDstFile)
	{
        try
        {
        	DataOutputStream out = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(sDstFile)));
        	out.write(pBuf.getByteArray(dwBufOffset, dwBufSize), 0, dwBufSize);
        	out.close();
        } catch (Exception e){
        	e.printStackTrace();
        }
	}

	// 读取本地图片到byte[]
	public static byte[] readPictureToByteArray(String filename) {
		File file = new File(filename);
		if(!file.exists()) {
			System.err.println("picture is not exist!");
			return null;
		}

		ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream((int)file.length());
		BufferedInputStream byteInStream = null;
		try {
			byteInStream = new BufferedInputStream(new FileInputStream(file));
			byte[] buf = new byte[1024];
			int len = 0;
			while((len = byteInStream.read(buf)) != -1) {
				byteOutStream.write(buf, 0, len);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				byteInStream.close();
				byteOutStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return byteOutStream.toByteArray();
	}

	// 将一位数组转为二维数组
	public static byte[][] ByteArrToByteArrArr(byte[] byteArr, int count, int length) {
		if(count * length != byteArr.length) {
			System.err.println(count * length + " != " + byteArr.length);
			return null ;
		}
		byte[][] byteArrArr = new byte[count][length];

		for(int i = 0; i < count; i++) {
			System.arraycopy(byteArr, i * length, byteArrArr[i], 0, length);
		}

		return byteArrArr;
	}


    // Win下，将GBK String类型的转为Pointer
    public static Pointer GetGBKStringToPointer(String src) {
    	Pointer pointer = null;
    	try {
			byte[] b = src.getBytes("GBK");

			pointer = new Memory(b.length);
			pointer.clear(b.length);

			pointer.write(0, b, 0, b.length);
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
    	return pointer;
    }

	// win下，将Pointer值转为 GBK String
	public static String GetPointerDataToGBKString(Pointer pointer) {
		String str = "";
		if(pointer == null) {
			return str;
		}

		int length = 0;
		byte[] bufferPlace = new byte[1];

		for(int i = 0; i < 2048; i++) {
			pointer.read(i, bufferPlace, 0, 1);
			if(bufferPlace[0] == '\0') {
				length = i;
				break;
			}
		}

		if(length > 0) {
			byte[] buffer = new byte[length];
			pointer.read(0, buffer, 0, length);
			try {
				str = new String(buffer, "GBK").trim();
			} catch (UnsupportedEncodingException e) {
				return str;
			}
		}

		return str;
	}

	// win下，将Pointer值转为 GBK String
	public static String GetPointerDataToGBKString(Pointer pointer, int length) {
		String str = "";
		if(pointer == null) {
			return str;
		}

		if(length > 0) {
			byte[] buffer = new byte[length];
			pointer.read(0, buffer, 0, length);
			try {
				str = new String(buffer, "GBK").trim();
			} catch (UnsupportedEncodingException e) {
				return str;
			}
		}

		return str;
	}

	public static void StringToByteArray(String src, byte[] dst) {
		for(int i = 0; i < dst.length; i++) {
			dst[i] = 0;
		}

		System.arraycopy(src.getBytes(), 0, dst, 0, src.getBytes().length);
	}

	/**
	 * 生成MD5
	 * @param path 图片路径
	 * @return MD5
	 * @throws FileNotFoundException
	 */
	public static String GetStringMD5(String path) {
		File file = new File(path);

		String value = "";
		FileInputStream inputStream = null;

		try {
			inputStream = new FileInputStream(file);
			MappedByteBuffer byteBuffer = inputStream.getChannel().map(FileChannel.MapMode.READ_ONLY, 0, file.length());
			MessageDigest md5 = MessageDigest.getInstance("MD5");
			md5.update(byteBuffer);
			BigInteger bi = new BigInteger(1, md5.digest());
			value = bi.toString(16);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if(null != inputStream) {
				try {
					inputStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return value;
	}

	// 获取当前时间
	public static String getDate() {
		SimpleDateFormat simpleDate = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String date = simpleDate.format(new java.util.Date()).replace(" ", "_").replace(":", "-");

		return date;
	}

	// 获取当前时间
	public static String getDay() {
		SimpleDateFormat simpleDate = new SimpleDateFormat("yyyy-MM-dd");
		String date = simpleDate.format(new java.util.Date());
		return date;
	}
}
