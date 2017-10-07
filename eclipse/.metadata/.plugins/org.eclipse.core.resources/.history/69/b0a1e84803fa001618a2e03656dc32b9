import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

public class HashCode {
	

	
	public static void main(String[] args){
		
		//Scanner in = new Scanner(new FileReader("filename.txt"));
		getInput();

	}
	
	private static void getInput(){
		
		try {
			
			Scanner in = new Scanner(new FileReader("kittens.in"));
			int v, e,r,c,x;
			
			v= in.nextInt();
			e= in.nextInt();
			r= in.nextInt();
			c= in.nextInt();
			x= in.nextInt();
			
			System.out.println(v);
			int[] videos = new int[v];
			for(int i=0;i< v;i++){
				videos[i] = in.nextInt();
				
			}
			

			
			Endpoint[] endpoints = new Endpoint[e]; 
			for(int i=0;i< e;i++){
				int l, k;
				endpoints[i] = new Endpoint(in.nextInt(), in.nextInt());
				for(int j=0; i<endpoints[i].getk();j++){
					
				}
			}
			

			int cEndpointId, cVideoId, cNumRequests;
			for(int i=0;i< r;i++){
				cNumRequests = in.nextInt();
				cVideoId = in.nextInt();
				cEndpointId = in.nextInt();
				endpoints[cEndpointId].addVideoRequest(cVideoId, cNumRequests);
			}
//			while(in.hasNext()){
//				
//				System.out.println(v + x);
//			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
}
