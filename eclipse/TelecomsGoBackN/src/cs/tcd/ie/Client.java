/**
 * Sean McDonagh, 15319517, Selective Repeat
 */
package cs.tcd.ie;



import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;
import java.net.URL;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import tcdIO.*;

/**
 *
 * Client class
 * 
 * An instance accepts user input 
 *
 */
public class Client extends Node {

    static final int DEFAULT_SRC_PORT = 50000;
    static final int DEFAULT_DST_PORT = 50001;
    static final String DEFAULT_DST_NODE = "localhost";
	
    
    Terminal terminal;
    InetSocketAddress dstAddress;
    int ackNumReturned, sizeOfArray, baseSeq, maxSeq, normalPacketSize;
    byte[] image;
    boolean lastPacket;

    Packet packetData;
    String fileName;
    

    /**
     * Constructor
     *      
     * Attempts to create socket at given port and create an InetSocketAddress for the destinations, and initalize all of the necessary variables.
     */
  Client(Terminal terminal, String dstHost, int dstPort, int srcPort, String fileName, int normalPacketSize, int sizeOfEachInterval) {
        try {
        	this.keepGoing = true;							
        	this.infoPacketSent = false;
        	this.allPackets = new ArrayList<Packet>();				// an arrayList containing all of the of the Packets being sent.
        	this.fileName = fileName;
        	this.isFinished = false;
        	this.sizeOfEachInterval = sizeOfEachInterval;			// size of each window of packets being sent.
        	this.baseSeq = 0;										// base index for starting index of window
        	this.maxSeq = sizeOfEachInterval;						// max indew for last index of window
        	this.normalPacketSize = normalPacketSize;				// the normal packetSize.... number of bytes from the image to send per packet.
        	byte[] bytes = extractBytes(fileName);					// extract all of the bytes from the image.
        	this.image = bytes;
        	this.sizeOfArray = bytes.length;
        	super.client();											// tell the parent node that this class is the client..... used for when to set the SetSoTimeout() method for a child class or not
        	
        	this.lastPacket = false;								// 
            this.terminal = terminal;
            this.sequenceNumber = 0;
            this.dstAddress = new InetSocketAddress(dstHost, dstPort);
            this.socket = new DatagramSocket(srcPort);
            this.listener.go();
        }
        catch(java.lang.Exception e) {
        	Terminal term = new Terminal("client object");
        	term.println("caught exception");
        	e.printStackTrace();
        	}
    }

    
    /**
     *  Receives all packets from parent Node.Listener.run() while loop.
     *  Creates a new Packet data type. Then it calls the getSequence() method to find out the sequence number of the packet.
     *  If the Packet received has a sequence number greater than or equals to the last one sent from the client, and, last
     *  If the sequence number of the packet is equals to 0, and the boolean variable is false, then the next window of packets is sent and infoPacketSent is made true, so that it cant be sent again.
     *  else if sequence number(seq) returned is not equals to sequenceNumber.... i.e the last packetSent.... then the sequenceNumbe of the packet that is returned is put into a get method of the array list.
     *  In order to send back that specific packet. This is obviously the NAK of the Sliding Window protocol.
     */
    public synchronized void onReceipt(DatagramPacket packet) {
    	//terminal.println("start of recieving");
        packetData = new Packet(packet);
        PacketNormal packetDataNormal = new PacketNormal(packet);
        int seqNum = packetDataNormal.getSequenceNum();
        terminal.println(packetData.getString().substring(packet.getLength()));
        if(lastPacket == true && seqNum >= sequenceNumber) {
	    	isFinished = true;
	    	keepGoing = false;
        }	else	{
        	terminal.println("seqNum:" + seqNum + ", sequenceNumber:" + sequenceNumber);
        	
        	if(seqNum == sequenceNumber) {
        		if(sequenceNumber == 0 && infoPacketSent == false) {
        			infoPacketSent = true;
        			sendPackets();
	        	}	else	{
	        		baseSeq += sizeOfEachInterval;
	    			maxSeq += sizeOfEachInterval;
	    			sendPackets();
	        	}
        	}	else	{
        		sendPacket(allPackets.get(seqNum));
        	}
        }
    }
    
    
    /**
     * Sending multiple Packets Method
     * If sequenceNumber is 0 and infoPacketSent is false i.e packetInfo is still to be sent.
     * Then a PacketInfo is created and the necessary header is attached to the packet to be sent. The local variable packetInfo is assigned it's value.
     * This is in case the packet is not received by the server and it has to be sent again.
     * 
     * Else if sequenceNumber is greater than or eauals to 0 and infoPacktSent is false i.e packetInfo is still to be sent...
     * then a PacketNormal is sent... which are packets that arent headers. i.e packets containing the image data(bytes).
     * 
     * 
     * 
	 **/
    
    public synchronized void sendPackets() {
    		if(sequenceNumber == 0 && infoPacketSent == false) {
    	    	String string = "type:info,sequenceNum:" + 0 + ",sizeOfArray:" + sizeOfArray + ",fileName:" + fileName + ",fileExt:png,sizeOfEachInterval:" + sizeOfEachInterval + ",packetNormalSize:" + normalPacketSize;
    	    	Packet packet = new PacketInfo(string, null);
    	    	packetInfo = packet;
    	    	sendPacket(packet);
        	}	else	{
			   	int lastBytePos = sequenceNumber * normalPacketSize;
			   	int i = 0;
			   	while((lastBytePos <= sizeOfArray) && (sequenceNumber < maxSeq) && isFinished == false) {				// if the last byte index accessed is less than or equals to the size of the array & current sequenceNumber is less than maxSeq
				    byte[] data;
				    Packet packet = null;
				    int length = normalPacketSize;
				    String string = null;
				    
				    if(lastBytePos + normalPacketSize >= sizeOfArray) {			// basically, if the last byte index + the normal packets size is greater thena the size of the image.... then basically modify the size of the data being sent
				    	length = sizeOfArray - lastBytePos;
				    	data = new byte[length];
				    	System.arraycopy(image, lastBytePos, data, 0, length);
				    	string = "type:seq,sequenceNum:" + sequenceNumber + ",LengthOfPayLoad:" + length + ",lastPacket:1,data:";
				    	lastPacket = true;
				    }	else	{
				    	data = new byte[normalPacketSize];
				    	System.arraycopy(image, lastBytePos, data, 0, normalPacketSize);
				    	string = "type:seq,sequenceNum:" + sequenceNumber + ",LengthOfPayLoad:" + normalPacketSize + ",lastPacket:0,data:";
				    }
				    
					packet = new PacketNormal(string, data);
				    allPackets.add(sequenceNumber, packet);
				    terminal.println(packet.getString().substring(0, 100));
				    sendPacket(packet);
				    i++;
				    lastBytePos = (sequenceNumber * normalPacketSize) + length;
				    increment();													// increment sequenceNumber.... which is the index of the last packet sent
				    
			   	}
			   	maxSeq -= sizeOfEachInterval;					//
			   	maxSeq += i;									// These two lines set maxSeq to it's actual value. There is a case when maxSeq is going to be needed to be lesser than maxSeq += sizeOfEachInterval
        	}
    }
    
    /**
     * Send Individual Packet Method
     * Method used for sending individual Packet data type objects.
     * Takes a Packet and sets it's InetSocketAddress.
     * It attempts to send the packet through a DatagramSocket.
     * If the this is the lastPacket being sent. Then the local boolean variable isFinished is set to true.
     * This variable will then restrict the use of onRecipet and sendPackets.
     * 
     */
    
    public synchronized void sendPacket(Packet packetData) {
	    	DatagramPacket packet = packetData.getDatagramPacket(dstAddress);
	    	
	    	try {
	    		socket.send(packet);
	    		if(lastPacket == true) {
	    			isFinished = true;
	    		}
			} catch (IOException e) {
				e.printStackTrace();
			}
    }

    /**
     * extractBytes
     * method used for extracting all the bytes from an image.
     * 
     */
    
    public byte[] extractBytes(String imageName) throws IOException {
    	
    	byte[] imageInByte;
        URL url = getClass().getResource("/res/" + imageName +"");
    	BufferedImage originalImage = null;
    	originalImage = ImageIO.read(url);

		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		ImageIO.write(originalImage, "png", baos);
		baos.flush();
		imageInByte = baos.toByteArray();
		return imageInByte;
		
    }
    
    public void increment(){
        sequenceNumber++;
    }
    
    /**
     * Test method
     * 
     * Enter a desired file name, enter the size at which you desire to send packets at,
     *  enter the desired window size for the packets i.e. 11 packets per window.
     * 
     */
    public static void main(String[] args) {
        try {                          
        	Terminal term = new Terminal("Client");
        	String fileName = "grass.png";
            Client client = new Client(term, DEFAULT_DST_NODE, DEFAULT_DST_PORT, DEFAULT_SRC_PORT, fileName, 10000, 11);
            client.sendPackets();
            term.println("In static!");
        } catch(java.lang.Exception e) {
        	Terminal term = new Terminal("static main, client");
        	term.println("caught exception");
        	e.printStackTrace();
       	}
    }

}