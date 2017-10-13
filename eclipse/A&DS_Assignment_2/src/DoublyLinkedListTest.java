import static org.junit.Assert.*;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

@RunWith(JUnit4.class)
public class DoublyLinkedListTest {

	@Test
	public void testConstructor() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
	}

	@Test
	public void testPush() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.push(new Integer(15));
		System.out.println("push: " + L.toString());
		L.push(new Integer(14));
		System.out.println("push: " + L.toString());
		L.push(new Integer(13));
		System.out.println("push: " + L.toString());
	}
	

	
	@Test
	public void testPop() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.push(new Integer(15));
		L.push(new Integer(14));
		L.push(new Integer(13));
		System.out.println("Before pop: " + L.toString());
		Integer num = L.pop();
		System.out.println("num popped: " + num);
		System.out.println("Before pop: " + L.toString());
		Integer num1 = L.pop();
		System.out.println("num popped: " + num1);
		System.out.println("Before pop: " + L.toString());
		Integer num2 = L.pop();
		System.out.println("num popped: " + num2);
		System.out.println("Before pop: " + L.toString());
		Integer num3 = L.pop();
		if(num3 == null) {
			System.out.println("num popped: " + num3);
		}
	}
	
	
	@Test
	public void testEnqueue() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.enqueue(new Integer(15));
		System.out.println("enqueue: " + L.toString());
		L.enqueue(new Integer(14));
		System.out.println("enqueue: " + L.toString());
		L.enqueue(new Integer(13));
		System.out.println("enqueue: " + L.toString());
	}
	
	@Test
	public void testDequeue() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.enqueue(new Integer(15));
		System.out.println("After enqueue: " + L.toString());		
		L.enqueue(new Integer(14));
		System.out.println("After enqueue: " + L.toString());
		L.enqueue(new Integer(13));
		System.out.println("After enqueue: " + L.toString());
		System.out.println("Before dequeue: " + L.toString());
		Integer num = L.dequeue();
		System.out.println("num dequeued: " + num);
		System.out.println("Before dequeue: " + L.toString());
		Integer num1 = L.dequeue();
		System.out.println("num dequeued: " + num1);
		System.out.println("Before dequeue: " + L.toString());
		Integer num2 = L.dequeue();
		System.out.println("num dequeued: " + num2);
		System.out.println("Before dequeue: " + L.toString());
		Integer num3 = L.dequeue();
		if(num3 == null) {
			System.out.println("num dequeued: " + num3);
		}
	}
	
	@Test
	public void testReverseAndIsEmpty() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.enqueue(new Integer(15));
		L.enqueue(new Integer(14));
		L.enqueue(new Integer(13));
		if(!(L.isEmpty())) {
			System.out.println("DLL is not empty!");
		}
		System.out.println("num sequence: " + L.toString());
		L.reverse();
		System.out.println("num sequence: " + L.toString());
		L.reverse();
		System.out.println("num sequence: " + L.toString());
		L.pop();
		System.out.println("num sequence: " + L.toString());
		L.reverse();
		System.out.println("num sequence: " + L.toString());
		L.reverse();
		System.out.println("num sequence: " + L.toString());
		L.pop();
		System.out.println("num sequence: " + L.toString());
		L.pop();
		System.out.println("num sequence: " + L.toString());
		if(L.isEmpty()) {
			System.out.println("It's now empty!");

		}
	}
	
	@Test
	public void testDeleteAt() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.enqueue(new Integer(15));
		L.enqueue(new Integer(14));
		L.enqueue(new Integer(13));
		L.enqueue(new Integer(12));
		L.enqueue(new Integer(11));
		L.enqueue(new Integer(10));
		L.enqueue(new Integer(9));
		L.enqueue(new Integer(8));
		L.enqueue(new Integer(7));
		L.enqueue(new Integer(6));
		L.enqueue(new Integer(5));
		L.enqueue(new Integer(4));
		L.enqueue(new Integer(3));
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(12);
		System.out.println("Delete, num sequence: " + L.toString());		
		L.deleteAt(-1);
		System.out.println("Delete, num sequence: " + L.toString());				
		L.deleteAt(15);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(8);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(6);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(9);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(0);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(7);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(4);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(4);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(8);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(8);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(8);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(3);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(0);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(2);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(0);
		System.out.println("Delete, num sequence: " + L.toString());		
		L.enqueue(new Integer(10));
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(1);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(1);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(0);
		System.out.println("Delete, num sequence: " + L.toString());
		L.deleteAt(0);
		System.out.println("Delete, num sequence: " + L.toString());
		L.reverse();
	}
	
	@Test
	public void testInsertBefore() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(0, 10);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(1, 11);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(1, 12);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(2, 13);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(4, 14);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(0, 15);
		System.out.println("Insert, num sequence: " + L.toString());
		L.insertBefore(1, 16);
		System.out.println("Insert, num sequence: " + L.toString());		
	}
	
	@Test
	public void testGet() {
		DoublyLinkedList<Integer> L = new DoublyLinkedList<Integer>();
		L.push(new Integer(15));
		L.push(new Integer(14));
		L.push(new Integer(13));
		System.out.println("Delete, num sequence: " + L.toString());
		Integer num1 = L.get(0);
		System.out.println("Get, num1: " + num1);		
		Integer num2 = L.get(1);
		System.out.println("Get, num2: " + num2);
		Integer num3 = L.get(-1);
		if(num3 == null) {
			System.out.println("Delete, num sequence: " + L.toString());
		}
		Integer num4 = L.get(10);
		if(num4 == null) {
			System.out.println("Delete, num sequence: " + L.toString());
		}
	}	
	
	public static void main(String[] args){
		// nothing
	}
	
}
