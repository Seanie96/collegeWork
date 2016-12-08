// -------------------------------------------------------------------------
/**
 *  This class contains the methods of Doubly Linked List.
 *
 *  @author  
 *  @version 13/10/16 18:15
 */


/**
 * Class DoublyLinkedList: implements a *generic* Doubly Linked List.
 * @param <T> This is a type parameter. T is used as a class name in the
 * definition of this class.
 *
 * When creating a new DoublyLinkedList, T should be instantiated with an
 * actual class name that extends the class Comparable.
 * Such classes include String and Integer.
 *
 * For example to create a new DoublyLinkedList class containing String data: 
 *    DoublyLinkedList<String> myStringList = new DoublyLinkedList<String>();
 *
 * The class offers a toString() method which returns a comma-separated string of
 * all elements in the data structure.
 * 
 * This is a bare minimum class you would need to completely implement.
 * You can add additional methods to support your code. Each method will need
 * to be tested by your jUnit tests -- for simplicity in jUnit testing
 * introduce only public methods.
 */
class DoublyLinkedList<T extends Comparable<T>>
{

    /**
     * private class DLLNode: implements a *generic* Doubly Linked List node.
     */
	
	private int doubleLinkedListSize;
	
    private class DLLNode
    {
        public final T data; // this field should never be updated. It gets its
                             // value once from the constructor DLLNode.
        public DLLNode next;
        public DLLNode prev;
    
        /**
         * Constructor
         * @param theData : data of type T, to be stored in the node
         * @param prevNode : the previous Node in the Doubly Linked List
         * @param nextNode : the next Node in the Doubly Linked List
         * @return DLLNode
         */
        public DLLNode(T theData, DLLNode prevNode, DLLNode nextNode)
        {
          data = theData;
          prev = prevNode;
          next = nextNode;
        }
        
        /*
         * These are Getter and Setter Methods for getting previous and next DLLNode's.(There is no setter method for the Data.)
         */
        
        public DLLNode getPrev() {
        	return prev;
        }
        
        public DLLNode getNext() {
        	return next;
        }
        
        public void setPrev(DLLNode newPrev) {
        	this.prev = newPrev;
        }
        
        public void setNext(DLLNode newNext) {
        	this.next = newNext;
        }
        
        public T getData() {
        	return data;
        }
    }

    // Fields head and tail point to the first and last nodes of the list.
    private DLLNode head, tail;
    
    /**
     * Constructor
     * @return DoublyLinkedList
     */
    
    public DoublyLinkedList() 
    {
      head = null;
      tail = null;
      doubleLinkedListSize = 0;
    }

    /**
     * Tests if the doubly linked list is empty
     * @return true if list is empty, and false otherwise
     *
     * Worst-case asymptotic runtime cost: Theta(1)
     *
     * Justification:
     *  The check of the size doubleLinkedListSize is constant time, and the cost is 1.
     *  
     */
    public boolean isEmpty()
    {
      if(doubleLinkedListSize == 0) return true;
      else return false;
    }

    /**
     * Inserts an element in the doubly linked list
     * @param pos : The integer location at which the new data should be
     *      inserted in the list. We assume that the first position in the list
     *      is 0 (zero). If pos is less than 0 then add to the head of the list.
     *      If pos is greater or equal to the size of the list then add the
     *      element at the end of the list.
     * @param data : The new data of class T that needs to be added to the list
     * @return none
     *
     * Worst-case asymptotic runtime cost: Theta(n)
     *
     * Justification: The worse case is when the pos is (LastPosInList - 1) and the length of the list is greater than 2.
     * In this case the program will have to run through the whole list - 1. Each Operation inside the for loop is Theta(1), and it's iterated n - 1 times. So the 
     * runtime cost then becomes (n - 1) * Theta(1). Each operation after the for loop is theta(1). Altogether we get Theta(n) + (Theta(1) x 5)
     * We can get rid of the smaller orders. Which leaves us with Theta(n).
     * 
     */
    public void insertBefore( int pos, T data ) 
    {
      DLLNode node;
      if(head == null) {
    	  node = new DLLNode(data, null, null);
    	  head = tail = node;
      }	else if(pos <= 0) {
    		  node = new DLLNode(data, null, head);
    		  head.setPrev(node);
    		  head = node;
      }
  	  else if(pos > doubleLinkedListSize - 1)	{
    		  node = new DLLNode(data, tail, null);
    		  tail.setNext(node);
    		  tail = node;
      }	else {
    	  node = head;
    	  for(int i = 0; i < pos; i++) {
    		  node = node.getNext();
    	  }
    	  DLLNode tmp = node.getPrev();
    	  DLLNode theNode = new DLLNode(data, tmp, node);
    	  node.setPrev(theNode);
    	  tmp.setNext(theNode);
      }
      doubleLinkedListSize++;
    }

    /**
     * Returns the data stored at a particular position
     * @param pos : the position
     * @return the data at pos, if pos is within the bounds of the list, and null otherwise.
     *
     * Worst-case asymptotic runtime cost: Theta(n)
     *
     * Justification:
     *  Worst situation is when the pos is equals to (n - 1), where n is the size of the list. Each operation in the for loop is constant time, at Theta(1).
     *  The for loop will iterate n - 1 times. Therefore giving (n - 1) * Theta(1). Drop the -1, because it will give a meaningless term.
     *  This then gives n*Theta(1) => Theta(n).
     */
    public T get(int pos) 
    {
      DLLNode node;
      if((pos >= 0) && (pos <= doubleLinkedListSize - 1)) { // if pos is within the last a first element.
    	  node = head;
	      for(int i = 0; i < pos; i++) {
	    	  node = node.getNext();
	      }
	      return node.getData();
      }	else	{
    	  return null;
      }
    }

    /**
     * Deletes the element of the list at position pos.
     * First element in the list has position 0. If pos points outside the
     * elements of the list then no modification happens to the list.
     * @param pos : the position to delete in the list.
     * @return true : on successful deletion, false : list has not been modified. 
     *
     * Worst-case asymptotic runtime cost: Theta(n)
     *
     * Justification:
     *  Worst case is that the size of the list is greater than 2 and that the pos is not 0 and not (sizeOfList - 1). Worst case is when it goes through
     *  the last else statement of the last else statement. In this case, each operation is constant time. The worst case is when pos is n - 1. This becomes 
     *  (n - 1) * Theta(1) => Theta(n). Get rid of the -1 value.
     */
    public boolean deleteAt(int pos)
    {
    	DLLNode tmp, node;
    	if(head == null || pos < 0 || pos > (doubleLinkedListSize - 1)) { // Check to see if head and tail are equals to null.
    		return false;
    	}	else if(head == tail)	{ // Check to see if there is 1 element is in the list.
    		head = tail = null;
    		doubleLinkedListSize--;
    		return true;
    	}	else if(doubleLinkedListSize == 2) { // Check to see if there is 2 elements in the list.
    		if(pos == 0) {
    			tail.setPrev(null);
    			head = tail;
    		}	else if(pos == 1)	{
    			head.setNext(null);
    			tail = head;
    		}
    		doubleLinkedListSize--;
    		return true;
    	}	else {	// else, if there is greater than 2 elements in the list.
    		if(pos == 0) {
    			tmp = head.getNext();
    			tmp.setPrev(null);
    			head = tmp;
    		}	else if(pos == doubleLinkedListSize - 1)	{
    			tmp = tail.getPrev();
    			tmp.setNext(null);
    			tail = tmp;
    		}	else	{
    			node = head;
    			for(int i = 0; i < pos; i++) {
    				node = node.getNext();
    			}
    			tmp = node.getNext();
    			DLLNode tmp2 = node.getPrev();
    			tmp.setPrev(tmp2);
    			tmp2.setNext(tmp);
    		}
    		doubleLinkedListSize--;
    		return true;
    	}
    }

    /**
     * Reverses the list.
     * If the list contains "A", "B", "C", "D" before the method is called
     * Then it should contain "D", "C", "B", "A" after it returns.
     *
     * Worst-case asymptotic runtime cost: Theta(n)
     *
     * Justification:
     *  Worst case is that the size of the list is not greater than 2.
     *  Each operation is Theta(1), the for loop runs through the whole list of size n. Therefore it's going to be n*Theta(1). Therefore Theta(n).
     */
    public void reverse()
    {
      if(head == null || head == tail) { // if theres nothing in the list or theres 1 element in the list, do thing.
    	  // do nothing..
      }	else	{
	      DLLNode prev, next;
	      DLLNode node = head;
	      for(int i = doubleLinkedListSize; i > 0; i--) 
	      {
	    	  next = node.getNext();
	    	  prev = node.getPrev();
	    	  node.setNext(prev);
	    	  node.setPrev(next);
	    	  node = node.getPrev();
	      }
	      DLLNode tmp = head;
	      head = tail;
	      tail = tmp;
      }
    }


    /*----------------------- STACK */
    /**
     * This method should behave like the usual push method of a Stack ADT.
     * If only the push and pop methods are called the data structure should behave like a stack.
     * How exactly this will be represented in the Doubly Linked List is up to the programmer.
     * @param item : the item to push on the stack
     *
     * Worst-case asymptotic runtime cost: Theta(1)
     *
     * Justification:
     *  All of the operations are constant time. It does'nt matter how long DLL is, when we push we merely add an element to the end of the DLL.
     *  This calls on the insertBefore method, which when called, takes Theta(1), when the pos is equals to the size of the list. Therefore it takes Theta(1).
     */
    public void push(T item) 
    {
    	insertBefore(doubleLinkedListSize, item);
    }

    /**
     * This method should behave like the usual pop method of a Stack ADT.
     * If only the push and pop methods are called the data structure should behave like a stack.
     * How exactly this will be represented in the Doubly Linked List is up to the programmer.
     * @return the last item inserted with a push; or null when the list is empty.
     *
     * Worst-case asymptotic runtime cost: Theta(1)
     *
     * Justification:
     *  It's constant time because it does'nt matter how long the DLL is, when we pop we merely take the last element from the DLL.
     *  This calls on the get and deleteAt methods. The get method is constant time Theta(1). The deleteAt method with pos = sizeOfList, takes Theta(1). Therefore,
     *  when we add both of them we get Theta(1). Therefore overall, we get Theta(1).
     */
    public T pop() 
    {
      T data = get(doubleLinkedListSize - 1);
      deleteAt(doubleLinkedListSize - 1);
      return data;
    }

    /*----------------------- QUEUE */
 
    /**
     * This method should behave like the usual enqueue method of a Queue ADT.
     * If only the enqueue and dequeue methods are called the data structure should behave like a FIFO queue.
     * How exactly this will be represented in the Doubly Linked List is up to the programmer.
     * @param item : the item to be enqueued to the stack
     *
     * Worst-case asymptotic runtime cost: Theta(1) - constant time
     *
     * Justification: 
 	 *	All of the operations are constant time. It does'nt matter how long DLL is, when we enqueue we merely add an element to the end of the DLL. 
     *  This calls on the insertBefore method, which when called, takes Theta(1), when the pos is equals to the size of the list. Therefore it takes Theta(1).
     */
    public void enqueue(T item) 
    {
      insertBefore(doubleLinkedListSize, item);
    }

     /**
     * This method should behave like the usual dequeue method of a Queue ADT.
     * If only the enqueue and dequeue methods are called the data structure should behave like a FIFO queue.
     * How exactly this will be represented in the Doubly Linked List is up to the programmer.
     * @return the earliest item inserted with a push; or null when the list is empty.
     *
     * Worst-case asymptotic runtime cost: Theta(1) - constant time.
     *
     * Justification:
     *  It's constant time because it does'nt matter how long the DLL is, when we dequeue we merely get the first element from the DLL.
     *  This calls on the get and deleteAt methods. The get method is constant time Theta(1). The deleteAt method with pos = 0, takes Theta(1). Therefore,
     *  when we add both of them we get Theta(1). Therefore overall, we get Theta(1).
     */
    public T dequeue()
    {
      T data = get(0);
      deleteAt(0);
      return data;
    }
 

    /**
     * @return a string with the elements of the list as a comma-separated
     * list, from beginning to end
     *
     * Worst-case asymptotic runtime cost:   Theta(n)
     *
     * Justification:
     *  We know from the Java documentation that StringBuilder's append() method runs in Theta(1) asymptotic time.
     *  We assume all other method calls here (e.g., the iterator methods above, and the toString method) will execute in Theta(1) time.
     *  Thus, every one iteration of the for-loop will have cost Theta(1).
     *  Suppose the doubly-linked list has 'n' elements.
     *  The for-loop will always iterate over all n elements of the list, and therefore the total cost of this method will be n*Theta(1) = Theta(n).
     */
    public String toString() 
    {
      StringBuilder s = new StringBuilder();
      boolean isFirst = true; 

      // iterate over the list, starting from the head
      for (DLLNode iter = head; iter != null; iter = iter.next)
      {
        if (!isFirst)
        {
          s.append(",");
        } else {
          isFirst = false;
        }
        s.append(iter.data.toString());
      }

      return s.toString();
    }


}



