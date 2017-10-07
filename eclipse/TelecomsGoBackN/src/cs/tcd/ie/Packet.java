/**
 * Sean McDonagh, 15319517, Selective Repeat
 */
package cs.tcd.ie;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;

import org.apache.commons.lang3.ArrayUtils;

public class Packet {
	
	DatagramPacket packet;
	Byte[] data;
	String string;
	String[] stringSplit;
	int sizeOfPacket;
	
	Packet(DatagramPacket packet) {
		this.packet = packet;
		data = ArrayUtils.toObject(packet.getData());
		try {
			this.string = new String(ArrayUtils.toPrimitive(data), 0, data.length, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		stringSplit = string.split(",");
		this.sizeOfPacket = packet.getLength();
	}
	
	Packet(String string, byte[] payLoadData) {
		String tmp = "";
		
		if(payLoadData != null) {
			try {
				tmp = new String(payLoadData, 0, payLoadData.length, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}	else	{
			tmp = "NULL";
		}
		
		this.string = string + tmp;
		
		stringSplit = string.split(",");		
		
		byte[] theData = null;
		try {
			theData = string.getBytes("UTF-8");
		} catch (UnsupportedEncodingException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if(payLoadData == null) {
			this.data = new Byte[theData.length + 4];
		}	else	{
			this.data = new Byte[theData.length + payLoadData.length];
		}
		System.arraycopy(ArrayUtils.toObject(theData), 0, this.data, 0, theData.length);
		if(!tmp.equals("NULL")) {
			System.arraycopy(ArrayUtils.toObject(payLoadData), 0, this.data, theData.length, payLoadData.length);
		}	else	{
			byte[] array = null;
			try {
				array = tmp.getBytes("UTF-8");
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.arraycopy(ArrayUtils.toObject(array), 0, this.data, theData.length, tmp.length());
		}
		
		packet = new DatagramPacket(ArrayUtils.toPrimitive(data), this.data.length);
	}
	
	public String getType() {
		return stringSplit[0].substring(5);
	}
	
	public String getString() {
		return string;
	}
	
	public int getSequenceNum() {
		return Integer.parseInt(stringSplit[1].substring(12));
	}
	
	public DatagramPacket getDatagramPacket() {
		return this.packet;
	}
	
	public DatagramPacket getDatagramPacket(InetSocketAddress dstAddress) {
		return new DatagramPacket(ArrayUtils.toPrimitive(data), data.length, dstAddress);
	}
	
}
