/**
 * Sean McDonagh, 15319517, StopNWait
 */

package cs.tcd.ie;

import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.util.concurrent.CountDownLatch;
import tcdIO.Terminal;

public abstract class node {
    static final int PACKETSIZE = 65536;

    DatagramSocket socket;
    Listener listener;
    CountDownLatch latch;
    boolean client = false, keepGoing = true;
    
    node() {
        latch= new CountDownLatch(1);
        listener= new Listener();
        listener.setDaemon(true);
        listener.start();
    }
    

    public void client() {
        this.client = true;
    }
    
    public void setKeepGoing() {
    	this.keepGoing = false;
    }
    
    public abstract void onReceipt(DatagramPacket packet);
    public abstract void sendPacket(Packet packetData);
    public abstract Packet getPacketData();
    public abstract boolean isClient();
    
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
                    		sendPacket(getPacketData());
                    	}
                    }
                }
            } catch (Exception e) {
            	if (!(e instanceof SocketException)) e.printStackTrace();
            }
        }
    }
}