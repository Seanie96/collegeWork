/**
 * Sean McDonagh, 15319517, Selective Repeat
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
	
	public int getSizeOfPayLoad() {
		return Integer.parseInt(stringSplit[2].substring(16));
	}
	
	public Byte[] getPayLoad() {
		int payLoadL = getSizeOfPayLoad();
		int index = sizeOfPacket - payLoadL;
		Byte[] payload = new Byte[payLoadL];
		System.arraycopy(data, index, payload, 0, payLoadL);
		return payload;
	}

	public int isLastPacket() {
		// TODO Auto-generated method stub
		return Integer.parseInt(stringSplit[3].substring(11));
	}
	
}