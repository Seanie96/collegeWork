/**
 * Sean McDonagh, 15319517, StopNWait
 */
package cs.tcd.ie;

import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;
import java.net.URL;
import javax.imageio.ImageIO;

import tcdIO.*;

/**
 *
 * Client class
 * 
 * An instance accepts user input 
 *
 */
public class client extends node {
    static final int DEFAULT_SRC_PORT = 50000;
    static final int DEFAULT_DST_PORT = 50001;
    static final String DEFAULT_DST_NODE = "localhost";    
    
    Terminal terminal;
    InetSocketAddress dstAddress;
    int sequenceNumber, normalPacketSize;
    int ackNum, ackNumReturned, sizeOfArray;
    byte[] array;
    PacketNormal packetData;
    Packet packetSent;
    String fileName;

    /**
     * Constructor
     *      
     * Attempts to create socket at given port and create an InetSocketAddress for the destinations
     */
   client(Terminal terminal, String dstHost, int dstPort, int srcPort, int normalPacketSize) {
        try {
        	this.normalPacketSize = normalPacketSize;
        	ackNum = 0;
            this.terminal = terminal;
            dstAddress = new InetSocketAddress(dstHost, dstPort);
            socket = new DatagramSocket(srcPort);
            super.client();
            listener.go();
        }
        catch(java.lang.Exception e) {
        	e.printStackTrace();
        	}
    }

   public boolean isClient() {
	    return true;
   }
    
    /**
     * Receiver method for the client....
     * Receives a DatagramPacket and filters it into the Packet class.
     * if the current sequenceNumber i.e. the seqNum of the last packet sent.... is equals to 0, then sent the header Packet... the packet contain all of the information for the communication, then increment sequenceNumber
     * else if type of Packet is 'ack' then check to see if the seqNum of the packet received is equals to the seqNum of the last packet sent. If so, swap ackNum to either 0 ro 1.
     * if keepGoing == true i.e. client still receiving packets
     * 
     */
    public synchronized void onReceipt(DatagramPacket packet) {
    	Packet packetData2 = new Packet(packet);
        packetData = new PacketNormal(packet);
        Packet packetToSend;
        String type = packetData.getType();
        //terminal.println("Client Recieved: " + packetData2.getString().substring(0, packet.getLength()));
        if(sequenceNumber == 0) {
        	packetToSend = makePacket(ackNum);
	        packetSent = packetToSend;
			sendPacket(packetToSend);
			increment();
        }	else if(type.equals("ack")) {
            ackNumReturned = packetData.getAckNum();
            if((ackNumReturned == 1 && ackNum == 1) || ((ackNumReturned == 0) && (ackNum == 0))) {
            	ackNum = (ackNum + 1) % 2;
            	if(keepGoing == true) {
            		packetToSend = makePacket(ackNum);
            		packetSent = packetToSend;
            		sendPacket(packetToSend);
            	}
    			increment();
    		}	else if((ackNumReturned == 1 && ackNum == 0) || ((ackNumReturned == 0) && (ackNum == 1))) {
    			// resend last packet. Dont' increment.
    		}	else if(ackNumReturned == -1) {
    			setKeepGoing();
    		}
            
        }
        
    }
    
    
    /*
     * Construct a Packet, passing in a string and a byte array into the Packet.
     */
    
    public Packet makePacket(int ackNum) {
    	
    	int num = 0, length;
        length = normalPacketSize;
        num = (array.length - (normalPacketSize * sequenceNumber));
        if(num < normalPacketSize) {
        	length = num;
        }
        
        byte[] data = new byte[length];
    	
        System.arraycopy(array, sequenceNumber * normalPacketSize, data, 0, length);
        System.out.println("sequenceNumber: " + sequenceNumber);
        System.out.println("normalPacketSize: " + normalPacketSize);
        
        System.out.println("next!");
        
        String string = new String("type:ack,sequenceNum:" + sequenceNumber + ",ackNum:" + ackNum + ",lengthOfPayLoad:" + length + ",data:");
    	
        Packet packetToSend = new Packet(string, data);
        
        return packetToSend;
    	
    }
    
    /**
     * Sender method.
     * 	Sends a DatagramPacket
	 **/
    
    public synchronized void sendPacket(Packet packetData) {
    	
	    DatagramPacket packet = packetData.getDatagramPacket(dstAddress);
	    if(sequenceNumber != 0) {
	    	terminal.println(packetData.getString().substring(0, 100));
	    }
	    try {
	    	socket.send(packet);
		} catch (IOException e) {
			e.printStackTrace();
		}
    	

    }
    
    /*
     *Method used for extracting bytes from a BufferedImage 
     */
    
    public byte[] extractBytes(String imageName) throws IOException {
        
        byte[] imageInByte;
        URL url = getClass().getResource("/res/grass.png");
    	BufferedImage originalImage = null;
    	originalImage = ImageIO.read(url);

		// convert BufferedImage to byte array
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		ImageIO.write(originalImage, "png", baos);
		baos.flush();
		imageInByte = baos.toByteArray();
		return imageInByte;
		
    }
    
    /*
     * 	
     * Get the Packet of the last DatagramPacket sent.
     *	 
     */
    
	public Packet getPacketData() {
		return packetSent;
	}
    
    public void increment(){
        sequenceNumber++;
    }

    /**
     * Test method
     * 
     * Sends a packet to a given address
     */
    public static void main(String[] args) {
        try {                          
        	Terminal term = new Terminal("Client");
        	String fileName = "/res/grass.png";
        	
        	Packet packet = null;
        	
            client client = new client(term, DEFAULT_DST_NODE, DEFAULT_DST_PORT, DEFAULT_SRC_PORT, 10000);
            byte[] array = null;
        	try{
        		array = client.extractBytes(fileName);
        	}	catch(Exception e) {
        		e.printStackTrace();
        	}
        	
        	
        	
        	client.sizeOfArray = array.length;
        	client.array = array;
        	
        	
        	String string = new String("type:info,sizeOfArray:" + array.length + ",fileName:" + fileName + ",fileExt:png,packetNormalSize:" + 10000);
            term.println("size..... :" + array.length);
        	packet = new PacketInfo(string, null);
        	client.sendPacket(packet);
        	client.packetSent = packet;
            term.println("In static!");
        } catch(java.lang.Exception e) {
        	e.printStackTrace();
       	}
    }

	
}