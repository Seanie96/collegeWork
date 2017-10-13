import static org.junit.Assert.assertEquals;


import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import cs.tcd.ie.Stopwatch;

import java.io.File;
import java.util.Arrays;
//-------------------------------------------------------------------------
/**
 *  Test class for Collinear.java
 *
 *  @author  
 *  @version 03/10/16 17:10:35
 */
@RunWith(JUnit4.class)
public class CollinearTest
{
	
	
    //~ Constructor ........................................................
    @Test
    public void testConstructor()
    {
      new Collinear();
    }

    //~ Public Methods ........................................................

    // ----------------------------------------------------------
    /**
     * Check that the two methods work for empty arrays
     */
    @Test
    public void testEmpty()
    {
        int expectedResult = 0;

        assertEquals("countCollinear failed with 3 empty arrays",       expectedResult, Collinear.countCollinear(new int[0], new int[0], new int[0]));
        assertEquals("countCollinearFast failed with 3 empty arrays", expectedResult, Collinear.countCollinearFast(new int[0], new int[0], new int[0]));
    }

    // ----------------------------------------------------------
    /**
     * Check for no false positives in a single-element array
     */
    @Test
    public void testSingleFalse()
    {
        int[] a3 = { 15 };
        int[] a2 = { 5 };
        int[] a1 = { 10 };

        int expectedResult = 0;

        assertEquals("countCollinear({10}, {5}, {15})",       expectedResult, Collinear.countCollinear(a1, a2, a3) );
        assertEquals("countCollinearFast({10}, {5}, {15})", expectedResult, Collinear.countCollinearFast(a1, a2, a3) );
    }

    // ----------------------------------------------------------
    /**
     * Check for no false positives in a single-element array
     */
    @Test
    public void testSingleTrue()
    {
        int[] a3 = { 15, 5 };       int[] a2 = { 5 };       int[] a1 = { 10, 15, 5 };

        int expectedResult = 1;

        assertEquals("countCollinear(" + Arrays.toString(a1) + "," + Arrays.toString(a2) + "," + Arrays.toString(a3) + ")",     expectedResult, Collinear.countCollinear(a1, a2, a3));
        assertEquals("countCollinearFast(" + Arrays.toString(a1) + "," + Arrays.toString(a2) + "," + Arrays.toString(a3) + ")", expectedResult, Collinear.countCollinearFast(a1, a2, a3));
    }

    @Test
    public void testBinarySearch()
    {
    	int[] a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    	Collinear.binarySearch(a, 5);
    	Collinear.binarySearch(a, 1);
    	Collinear.binarySearch(a, 10);
    }
    
    
    
    @Test
    public void testSort()
    {
    	int[] a = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    	Collinear.sort(a);
    }

    // ----------------------------------------------------------
    /**
     *  Main Method.
     *  Use this main method to create the experiments needed to answer the experimental performance questions of this assignment.
     *
     *  You should read the lecture notes and/or book to understand how to correctly implement the main methods.
     *  You can use any of the provided classes to read from files, and time your code.
     *
     */
     public static void main(String[] args)
     {    	 
    	 File f1 = new File("r05000-1.txt");
    	 In in1 = new In(f1);
    	 int[] set1 = in1.readAllInts();
    	 File f2 = new File("r05000-2.txt");
    	 In in2 = new In(f2);
    	 int[] set2 = in2.readAllInts();
    	 File f3 = new File("r05000-3.txt");
    	 In in3 = new In(f3);
    	 int[] set3 = in3.readAllInts();
    	 
    	 Stopwatch sw = new Stopwatch();
    	 Collinear.countCollinearFast(set1, set2, set3);
    	 double timeTaken = sw.elapsedTime();
    	 System.out.println("timeTaken:" + timeTaken);
     }

}

