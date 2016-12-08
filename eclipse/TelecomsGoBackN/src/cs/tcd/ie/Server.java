/**
 * Sean McDonagh, 15319517, Selective Repeat
 */
package cs.tcd.ie;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.DatagramPacket;
import javax.imageio.ImageIO;
import org.apache.commons.lang3.ArrayUtils;
import tcdIO.Terminal;

public class Server extends Node {

    static final int DEFAULT_PORT = 50001;
	
    DatagramPacket packet;
    int packetNumber;
    Byte[] image;
    int packetSize, imageSize, normalPacketSize, packetsPerSeq, ackRecieved, lastIndex, maxSeq, minSeq;
    PacketNormal packetDataNormal;
    PacketInfo packetDataInfo;
    String fileName, seq, info, type;
    boolean lastSet, lastPacket, infoPacketRecieved, isLastPacket;
    Packet[] packetsOfSlide;
    Thread timer;
    
    Terminal terminal;
    
    /*
     * 
     */
    Server(Terminal terminal, int port, int normalPacketSize) {
    	this.isLastPacket = false;								// variable that says whether this is the last packet ot be sent to the server or not.
    	this.isFinished = false;								// vairable that is used to stop methods from running, once last Packet is sent.
    	this.packetsOfSlide = new Packet[20000];				// packets of slide..... the max is set to 20000 packets.
    	this.image = new Byte[10000];							// create a new image array to store all of the bytes in. the size is arbitrary, as I reinitalise it.
    	this.normalPacketSize = normalPacketSize;
    	this.packetNumber = 0;
    	this.info = "info";
    	this.seq = "seq";
    	this.lastSet = false;
    	this.lastPacket = false;
        try {
            this.terminal= terminal;
            socket = new DatagramSocket(port);
            listener.go();
        }
        catch(java.lang.Exception e) {
        	Terminal term = new Terminal("server object");
        	term.println("caught exception");
        	e.printStackTrace();
        	}
    }
    
    /**
     * Receiever method.
     * if isFinished boolean variable hasn't been set to true, then turn the DatagramPacket into a PacketNormal and PacketInfo, by passing them into the constructor.
     * Both of these Classes are children of the parent class Packet.
     * Retrieve the type using the method getType(). 
     * If it returns the string seq... the packet is going to be processed as a Normal packet.
     * infoPacketRecieved is set to false because if it is true, then it will send an unnecessary ack packet to the client.
     * Each packet that is received, the sequenceNumber is taken from the packet and that specific packet is put into it's sequenceNumber part of the packetsOfSlide array.
     * Then when it comes to the send NAK method, it will only send packets to the client that correspond to the elements that are not present in the array, within the maxSeq and the minSeq.
     * 
     * If the type is info... then it's going to be processed as a header packet, or information packet on the normal packets that are going to be sent to the server.
     * A normal packet is sent back to the client, looking for the the packet with the same sequenceNumber as the one sent.
     * Once it has been sent, a thread is created to run a Timer that extends runnable.
     * When the timer reaches 5 seconds and interrupt hasn't been cause then the packets that havnt been sent, within the maxSeq and minSeq, the seqNum's of those packets are sent back to client, so that they can be sent back to the server.
     * 
     */
    public synchronized void onReceipt(DatagramPacket packet) {
    	if(isFinished == false) {
	    	this.packet = packet;
	      	packetDataNormal = new PacketNormal(packet);
		    packetDataInfo = new PacketInfo(packet);
			type = packetDataInfo.getType();
			if(type.equals(seq)) {
				infoPacketRecieved = false;
				terminal.println("infoPacketRecieved is now false");
				Byte[] data = packetDataNormal.getPayLoad();					// get payLoad
				int size = packetDataNormal.getSizeOfPayLoad();					// get size of pay load... i.e number of bytes sent.
				int seqNum = packetDataNormal.getSequenceNum();					// get the packets sequence number.
				terminal.println(packetDataNormal.getString().substring(0, 100));
				
				if(seqNum == maxSeq) {											// if the sequenceNumber returned is equivelent to the current maxSeq, then increment both maxSeq and minSeq by the usual packets per window slide.
					maxSeq += packetsPerSeq;
					minSeq += packetsPerSeq;
				}
				
				if(packetDataNormal.isLastPacket() == 1) {						// if the packet received is the last packet, then set the boolean variable to true.
					isLastPacket = true;
					lastIndex = seqNum;
					lastIndex -= minSeq;
				}
				
				terminal.println("seqNum:" + packetDataNormal.getSequenceNum() + ", size:" + size + ", normalPacketSize:" + normalPacketSize);
				if(packetsOfSlide[packetDataNormal.getSequenceNum()] == null) {
					System.arraycopy(data, 0, image, (normalPacketSize * (packetDataNormal.getSequenceNum())), size);
					packetsOfSlide[packetDataNormal.getSequenceNum()] = packetDataNormal;
				}	else	{
					
					// do nothing, because the packet is already contained in the array of packets.
					
				}
				
			}	else if(type.equals(info))	{
		    	packetsPerSeq = packetDataInfo.getSizeOfEachInterval();
		    	maxSeq = packetsPerSeq;
		    	minSeq = 0;
		    	imageSize = packetDataInfo.getSizeOfArray();
		    	
		    	String string = "type:seq,sequenceNum:" + 0 + ",LengthOfPayLoad:0,lastPacket:0,data:";
		    	PacketInfo packetData = new PacketInfo(string, null);
		    	packetDataInfo = packetData;
		    	sendPacket(packetData);
		    	infoPacketRecieved = true;
		    	terminal.println("infoPacketRecieved is now true");
		    	image = new Byte[imageSize];
				timer = new Thread(new Timer(5));
				timer.start();
			}
    	}

		terminal.println("finished onReceipt().........");
    }
    
    // method for sending individual packets.
    
    public synchronized void sendPacket(Packet packet) {
    	
    	//terminal.println(packet.getString());
    	
    	DatagramPacket response = packet.getDatagramPacket();
    	response.setSocketAddress(this.packet.getSocketAddress());
    	try {
			socket.send(response);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
	   	terminal.println("finished sendPacket().........");
    	
    }
    
    // method for sending NAK.. explanation in Report.
    
    public synchronized void sendNAK() {
    	
    	if(isFinished == false) {
	    	if(infoPacketRecieved == false) {
				if(isLastPacket == true) {
					System.out.println("last index.........");
					maxSeq -= packetsPerSeq;
					maxSeq += lastIndex + 1;
				}
				
				int packetsNeeded = maxSeq - minSeq;
				
				int counter = 0;
				Packet needSequenceNumber = null;
				for(int i = minSeq; i < maxSeq; i++) {
					Packet packet = null;
						if(packetsOfSlide[i] == null) {
							packet = packetsOfSlide[i];
							int seqNum = packetDataNormal.getSequenceNum();
						    String string = "type:seq,sequenceNum:" + seqNum + ",LengthOfPayLoad:0,lastPacket:0,data:";
						    packet = new PacketInfo(string, null);
							sendPacket(packet);
						}	else	{
							counter++;
						}
						if(i == (maxSeq - 1)) {
							needSequenceNumber = packetsOfSlide[i];
						}
				}
				
				String string = null;
				if(counter == packetsNeeded ) {
					string = "type:seq,sequenceNum:" + (needSequenceNumber.getSequenceNum() + 1) + ",LengthOfPayLoad:0,lastPacket:0,data:";
					Packet packet = new PacketNormal(string, null);
					sendPacket(packet);
					if(isLastPacket == true) {
						finishedAndDisplay();
						timer.interrupt();
					}
				}	else	{
					
				}
	    	}
	    	
	    	timer = new Thread(new Timer(5));
	    	timer.start();
	    	
			
	    	if(isLastPacket == true) {
	    		maxSeq -= lastIndex + 1;
	    		maxSeq += packetsPerSeq;
	    	}
    	}
    	
    }
    
    public void finishedAndDisplay() {
		terminal.println("program completed......");
    	
    	Byte[] byteObjects = new Byte[imageSize];
    	
    	
    	int i = 0;
    	for(Byte b: image) {
    	   byteObjects[i++] = b;
    	   if(i % 10000 == 0) {
    		   System.out.println(i + ",");
    	   }	else	{
    		   System.out.print(i + ",");
    	   }
    	   if(b == null) {
    		   System.out.print(i + ":null" + ",");
    	   }
    	   
    	}
    	InputStream in = new ByteArrayInputStream(ArrayUtils.toPrimitive(byteObjects));
		BufferedImage bImageFromConvert = null;
		try {
			bImageFromConvert = ImageIO.read(in);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		try {
			ImageIO.write(bImageFromConvert, "png", new File(
					"image3.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
		terminal.println("completed program!");
		
        isFinished = true;
		
    }
    
    /*
     * This class 
     */
    public static void main(String[] args) {
        try {                    
            Terminal terminal= new Terminal("Server");
        	terminal.println("Server started...");
            new Server(terminal, DEFAULT_PORT, 10000);
        } catch(java.lang.Exception e) {
        	e.printStackTrace();
        }
    }
    
    /*
     * This class is used as a timer, to cause interrupts.
     */
    
    protected class Timer implements Runnable {
    	
    	int timeInMiliSec;
    	Packet packet;
    	
    	Timer(int time) {
    		//terminal.println("Thread created!");
    		this.timeInMiliSec = time;
    	}
    	
    	public void run() {
    		Stopwatch sw = new Stopwatch();
    		while(sw.elapsedTime() < (double)timeInMiliSec) {
    			// do nothing.........
    		}
    		if(!(Thread.currentThread().isInterrupted())) {
    			sendNAK();
    		}	else	{
    			// timer has been interrupted.
    		}
    	}
    	
    }

    // method does nothing
    
	@Override
	public void sendPackets() {
		// doesnt do anything........
	}

}