/**
 * Sean McDonagh, 15319517, StopNWait
 */

package cs.tcd.ie;

import java.net.DatagramPacket;

public class PacketNormal extends Packet {
	

	PacketNormal(DatagramPacket packet)	{
		super(packet);
	}
	
	PacketNormal(String string, byte[] payLoadData)	{
		super(string, payLoadData);
	}
	
	public int getSequenceNum() {
		return Integer.parseInt(stringSplit[1].substring(12));
	}
	
	public int getAckNum() {
		return Integer.parseInt(stringSplit[2].substring(7));
	}
	
	public int getSizeOfPayLoad() {
		return Integer.parseInt(stringSplit[3].substring(16));
	}
	
	public byte[] getPayLoad() {
		int payLoadL = getSizeOfPayLoad();
		int index = sizeOfPacket - payLoadL;
		byte[] payload = new byte[payLoadL];
		System.arraycopy(data, index, payload, 0, payLoadL);
		return payload;
	}
	
}
