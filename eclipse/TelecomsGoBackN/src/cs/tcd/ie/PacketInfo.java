/**
 * Sean McDonagh, 15319517, Selective Repeat
 */
package cs.tcd.ie;

import java.net.DatagramPacket;

public class PacketInfo extends Packet {
	
	PacketInfo(DatagramPacket packet) {
		super(packet);
	}
	
	PacketInfo(String string, byte[] payLoadData) {
		super(string, payLoadData);
	}
	
	public int getSizeOfArray() {
		return Integer.parseInt(stringSplit[2].substring(12));
	}
	
	public String getFileName()	{
		return stringSplit[3].substring(9);
	}
	
	public String getFileExtension() {
		return stringSplit[4].substring(8);
	}
	
	public int getSizeOfEachInterval() {
		return Integer.parseInt(stringSplit[5].substring(19));
	}
	
}