/**
 * Sean McDonagh, 15319517, Selective Repeat
 */
package cs.tcd.ie;


import java.net.DatagramPacket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import tcdIO.Terminal;

public abstract class Node {
    static final int PACKETSIZE = 65536;

    DatagramSocket socket;
    Listener listener;
    CountDownLatch latch;
    boolean client = false, keepGoing;
    List<Packet> allPackets;
    int sizeOfEachInterval, sequenceNumber;
    boolean isFinished, infoPacketSent;
    Packet packetInfo;
    
    Node() {
    	
        latch = new CountDownLatch(1);
        
        listener = new Listener();
        listener.setDaemon(true);
        listener.start();
        
    }
    
    public void client() {
        this.client = true;
    }
    
    public abstract void onReceipt(DatagramPacket packet);
    public abstract void sendPacket(Packet packet);
    public abstract void sendPackets();
    
    /**
     *
     * Listener thread
     * 
     * Listens for incoming packets on a datagram socket and informs registered receivers about incoming packets.
     */
    class Listener extends Thread {
        
    	Terminal terminal;
    	
    	Listener() {
    	}
    	
        /*
         *  Telling the listener that the socket has been initialized 
         */
        public void go() {
            latch.countDown();
        }
        
        /*
         * Listen for incoming packets and inform receivers
         */
        public void run() {

        	try {
        		latch.await();
	                
	            if(client == true){
	            	socket.setSoTimeout(1000);
	            }
	                
	            // Endless loop: attempt to receive packet, notify receivers, etc
	            while(true) {
		          	try{
		             		DatagramPacket packet = new DatagramPacket(new byte[PACKETSIZE], PACKETSIZE);
		             		socket.receive(packet);
		               		onReceipt(packet);
		               		
		               	}	catch(SocketTimeoutException e) {
			               		if(client == true && keepGoing == true) {
			               			if(infoPacketSent == false && sequenceNumber == 0) {
				               			sendPacket(packetInfo);
			               			}	else	{
			               				int index = sequenceNumber - sizeOfEachInterval;
				               			Packet packet = null;
				               			while(index < sequenceNumber) {
				               				packet = allPackets.get(index);
				               				sendPacket(packet);
				               				index++;
				               			}
			               			}
			               		}
		                   	
		               	}
		        	}
	            }
	             catch (Exception e) {
	            	if (!(e instanceof SocketException)) e.printStackTrace();
	            }
        }
    }
}