/**
 * Sean McDonagh, 15319517, StopNWait
 */

package cs.tcd.ie;

import java.net.DatagramSocket;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.DatagramPacket;
import javax.imageio.ImageIO;

import tcdIO.Terminal;

public class server extends node {
    static final int DEFAULT_PORT = 50001;
    DatagramPacket packet;
    int packetNumber;
    byte[] image;
    int ackNum, ackRecieved;
    int packetSize, imageSize, normalPacketSize;
    PacketNormal packetDataNormal;
    PacketInfo packetDataInfo;
    String fileName, ack, info, type;
    Terminal terminal;
    
    /*
     * 
     */
    server(Terminal terminal, int port, int normalPacketSize) {
    	this.normalPacketSize = normalPacketSize;
    	ackNum = 0;
    	packetNumber = 0;
    	info = "info";
    	ack = "ack";
        try {
            this.terminal = terminal;
            socket = new DatagramSocket(port);
            listener.go();
        }
        catch(java.lang.Exception e) {
        	Terminal term = new Terminal("server object");
        	term.println("caught exception");
        	e.printStackTrace();
        	}
    }
    
    public boolean isClient() {
    	return false;
    }
    
    /**
     *  Takes a DatagramPacket and dissects it into a PacketNormal and PacketInfo.
     *  if the packet's type is ack, it will print out the first 100 character of the packet and 
     *  if the ackNum (ackNumberExpected) is equals to the ackNumRecieved, then copy the payLoad from the packetDataNormal
     *  to the image array.
     *  Else if the packets type is info, then callSizeOfArray, initalise image and call sendPacket() method, which send the confirmation (ACK), of the packet.
     *  
     */
    public synchronized void onReceipt(DatagramPacket packet) {
    	this.packet = packet;
      	packetDataNormal = new PacketNormal(packet);
	    packetDataInfo = new PacketInfo(packet);
      	
		type = packetDataInfo.getType();
		
		if(type.equals(ack)) {
			terminal.println(packetDataNormal.getString().substring(0, 100));
			ackRecieved = packetDataNormal.getAckNum();
			if(ackNum == ackRecieved) {
				System.out.println("ackNum = ackRecieved.");
				byte[] data = packetDataNormal.getPayLoad();
				packetSize = packetDataNormal.getSizeOfPayLoad();
				System.out.println("Server recieved: " + packetDataNormal.getString().substring(0, packet.getLength()));
				System.out.println("byte index starting at: " + packetNumber * normalPacketSize + ", final byte index: " + (((packetNumber * normalPacketSize) + packetSize)) + ", ");
				
				System.arraycopy(data, 0, image, packetNumber * normalPacketSize, packetSize);
				
				Byte theLastByte = image[(packetNumber * normalPacketSize) + packetSize - 1];
				System.out.println("first byte: " + image[(packetNumber * normalPacketSize)] + ",last filled:" + theLastByte.intValue() + ", at pos:" + ((packetNumber * normalPacketSize) + packetSize));
				packetNumber++;
				sendPacket();
				ackNum = (ackRecieved + 1) % 2;
			}	else if(ackRecieved == -1)	{
				
			}
		}	else if(type.equals(info))	{
			imageSize = packetDataInfo.getSizeOfArray();
			image = new byte[imageSize];
			fileName = packetDataInfo.getFileName();
			sendPacket();
		}

    }
    
    /*
     * Sender method.
     * if the type of the last packet received is info, then send back a DatagramInfo packet.
     * else if the type of the last packet received is ack, and if packetSize of packet received is not the same as normalPacketSize, then send send a string with ackNum:-1
     * else send a normal string.
     * if packet is not equals to the normalPacketSize then, call the finishedAndDisplay method, which converts the byte array to a BufferedImage.
     */
    
    public synchronized void sendPacket() {
    	DatagramPacket response = null;
    	
    	if(type.equals(info)) {
        	String stringToSend = "type:info,sizeOfArray:0,fileName:null,fileExt:png,packetNormalSize:NULL";
    		Packet newPacketData = new Packet(stringToSend, null);
    		System.out.println("Server Sending: " + newPacketData.getString());
    		response = newPacketData.getDatagramPacket();
    	}	else	{
		   	String stringToSend;
		   	if(packetSize != normalPacketSize) {
		   		stringToSend = "type:ack,sequenceNum:NULL,ackNum:-1,lengthOfPayLoad:0,data:";
		   	}	else	{
		   		stringToSend = "type:ack,sequenceNum:NULL,ackNum:" + ackNum + ",lengthOfPayLoad:0,data:";
		   	}
		   	
		   	Packet newPacketData = new Packet(stringToSend, null);
		   	System.out.println("Server sending: " + newPacketData.getString());
		   	response = newPacketData.getDatagramPacket();
		   	
		   	if(packetSize != normalPacketSize) {
		   		finishedAndDisplay();
		   	}	else	{
		   		// do nothing!
		   	}
    	}
    	response.setSocketAddress(packet.getSocketAddress());
	    try {
			socket.send(response);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    }
    
    /*
     * Method used for converting the byte array into a BufferedImage and then writing the image to a file using ImageIO.write
     */
    
    public void finishedAndDisplay() {
    	InputStream in = new ByteArrayInputStream(image);
		BufferedImage bImageFromConvert = null;
		try {
			bImageFromConvert = ImageIO.read(in);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		try {
			ImageIO.write(bImageFromConvert, "png", new File(
					"image.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
    }
    
    /*
     * The following methods are not used by the server..............
     */
    
	@Override
	public void sendPacket(Packet packetData) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Packet getPacketData() {
		return null;
	}
    
    /*
     * static main is used to initalised a server object.
     */
    public static void main(String[] args) {
        try {                    
            Terminal terminal= new Terminal("Server");
        	terminal.println("Server started...");
            new server(terminal, DEFAULT_PORT, 10000);			// 10000 is the byte size per packet sent.
        } catch(java.lang.Exception e) {
        	e.printStackTrace();
        	}
    }


}