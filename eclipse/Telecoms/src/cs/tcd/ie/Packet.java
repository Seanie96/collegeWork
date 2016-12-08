/**
 * Sean McDonagh, 15319517, StopNWait
 */

package cs.tcd.ie;

import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;

public class Packet {
	
	DatagramPacket packet;
	byte[] data;
	String string;
	String[] stringSplit;
	int sizeOfPacket;
	
	Packet(DatagramPacket packet) {
		this.packet = packet;
		this.data = packet.getData();
		try {
			this.string = new String(data, 0, data.length, "UTF-8");
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
			this.data = new byte[theData.length + 4];
		}	else	{
			this.data = new byte[theData.length + payLoadData.length];
		}
		System.arraycopy(theData, 0, this.data, 0, theData.length);
		if(!tmp.equals("NULL")) {
			System.arraycopy(payLoadData, 0, this.data, theData.length, payLoadData.length);
		}	else	{
			byte[] array = null;
			try {
				array = tmp.getBytes("UTF-8");
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.arraycopy(array, 0, this.data, theData.length, tmp.length());
		}
		
		packet = new DatagramPacket(this.data, this.data.length);
	}
	
	public String getType() {
		return stringSplit[0].substring(5);
	}
	
	public String getString() {
		return string;
	}
	
	public DatagramPacket getDatagramPacket() {
		return this.packet;
	}
	
	public DatagramPacket getDatagramPacket(InetSocketAddress dstAddress) {
		return new DatagramPacket(data, data.length, dstAddress);
	}
	
}
