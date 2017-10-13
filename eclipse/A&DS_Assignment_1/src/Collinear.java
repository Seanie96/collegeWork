// -------------------------------------------------------------------------
/**
 *  This class contains only two static methods that search for points on the
 *  same line in three arrays of integers. 
 *
 *  @author  
 *  @version 03/10/16 17:10:35
 */
class Collinear
{

   // ----------------------------------------------------------
    /**
     * Counts for the number of non-horizontal lines that go through 3 points in arrays a1, a2, a3.
     * This method is static, thus it can be called as Collinear.countCollinear(a1,a2,a3)
     * @param a1: An UNSORTED array of integers. Each integer a1[i] represents the point (a1[i], 1) on the plain.
     * @param a2: An UNSORTED array of integers. Each integer a2[i] represents the point (a2[i], 2) on the plain.
     * @param a3: An UNSORTED array of integers. Each integer a3[i] represents the point (a3[i], 3) on the plain.
     * @return the number of points which are collinear and do not lie on a horizontal line.
     *
     * Array a1, a2 and a3 contain points on the horizontal line y=1, y=2 and y=3, respectively.
     * A non-horizontal line will have to cross all three of these lines. Thus
     * we are looking for 3 points, each in a1, a2, a3 which lie on the same
     * line.
     *
     * Three points (x1, y1), (x2, y2), (x3, y3) are collinear (i.e., they are on the same line) if
     * 
     * x1(y2−y3)+x2(y3−y1)+x3(y1−y2)=0 
     *
     * In our case y1=1, y2=2, y3=3.
     *
     * You should implement this using a BRUTE FORCE approach (check all possible combinations of numbers from a1, a2, a3)
     *
     * ----------------------------------------------------------
     *
     * Experimental Performance:
     * -------------------------
     *  Write the running time of the algorithm when run with the following input sizes
     *  
     *  Input Size N      Running Time (sec)
     *  ------------------------------------
     *  1000              0.425
     *  2000              3.007
     *  4000              22.481
     *
     *  Assuming that the running time of your algorithm is of the form aN^b,
     *  estimate 'b' and 'a' by fitting a line to the experimental points:
     *
     *  b = 2.822
     *  a = -29.358
     *
     *  What running time do you predict using your results for input size 5000?
     *  What is the actual running time you get with such an input?
     *  What is the error in percentage?
     *
     *  Error = ( (Actual time) - (Predicted time) ) * 100 / (Predicted time)
     *
     *  Input Size N      Predicted Running Time (sec)        Actual Running Time (sec)       Error (%)
     *  ------------------------------------------------------------------------------------------------
     *  5000                      39.8877                        43.194                          8.29%
     * 
     *  Approximate Mathematical Performance:
     *  -------------------------
     *
     *  Using an appropriate cost model, give the performance of your algorithm.
     *  Explain your answer.
     *
     *  Performance: ~1/2(n^3)
     *
     *  Explanation:Using tilde notation. Three nested for loops. (n)(n-1)(n-2)/(3!). Three different array accesses, therefore 3x(n)(n-1)(n-2)/3!. 
     */
    static int countCollinear(int[] a1, int[] a2, int[] a3)
    {
    	int count = 0;
    	for(int i = 0; i < a1.length; i++) {
    		for(int j = 0; j < a2.length; j++) {
    			for(int k = 0; k < a3.length; k++) {
    				if((a1[i])*(2-3) + (a2[j])*(3-1) + (a3[k])*(1-2) == 0 ) {
    					count++;
    				}
    			}
    		}
    	}
      return count;
    }

    // ----------------------------------------------------------
    /**
     * Counts for the number of non-horizontal lines that go through 3 points in arrays a1, a2, a3.
     * This method is static, thus it can be called as Collinear.countCollinearFast(a1,a2,a3)
     * @param a1: An UNSORTED array of integers. Each integer a1[i] represents the point (a1[i], 1) on the plain.
     * @param a2: An UNSORTED array of integers. Each integer a2[i] represents the point (a2[i], 2) on the plain.
     * @param a3: An UNSORTED array of integers. Each integer a3[i] represents the point (a3[i], 3) on the plain.
     * @return the number of points which are collinear and do not lie on a horizontal line.
     *
     * In this implementation you should make non-trivial use of InsertionSort and Binary Search.
     * The performance of this method should be much better than that of the above method.
     *
     * Experimental Performance:
     * -------------------------
     *  Measure the running time of the algorithm when run with the following input sizes
     *  
     *  Input Size N      Running Time (sec)
     *  ------------------------------------
     *  1000              0.099
     *  2000              0.302
     *  4000              1.209
     *  5000              1.791
     *
     *
     *  Compare Implementations:
     *  ------------------------
     *  Show the speedup achieved by this method, using the times you got from your experiments.
     *
     *  Input Size N      Speedup = (time of countCollinear)/(time of countCollinearFast)
     *  ---------------------------------------------------------------------------------
     *  1000              4.29
     *  2000              9.957
     *  4000              18.59
     *  5000              24.11
     *
     *
     *  Approximate Mathematical Performance:
     *  -------------------------------------
     *
     *  Using an appropriate cost model, give the performance of your algorithm.
     *  Explain your answer.
     *
     *  Performance: ~2(n^2)(log(n))
     *
     *  Explanation: Using Tilde notation. Sort function takes 3(N^2) approx, the two for loops will iterate over the array accesses (n)(n-1)/2 times, but there are two array accesses,
     *  so it becomes (n^2),then the final binary search will iterate (n^2)x2(log(n)) approx.
     *  Therefore giving 3(n^2) + (n^2) + (n^2)x(2(log(n))). Take highest order. 2(n^2)(log(n)).
     *
     *
     */
    static int countCollinearFast(int[] a1, int[] a2, int[] a3)
    {
    	sort(a3);
    	int count = 0;
    	for(int i = 0; i < a1.length; i++) {
    		for(int j = 0; j < a2.length; j++) {
    			int num = (a1[i])*(2-3) + (a2[j])*(3-1);
    			if(binarySearch(a3, num)) {
    				count++;
    			}
    		}
    	}
    	return count;
    }

    // ----------------------------------------------------------
    /**
     * Sorts an array of integers according to InsertionSort.
     * This method is static, thus it can be called as Collinear.sort(a)
     * @param a: An UNSORTED array of integers. 
     * @return after the method returns, the array must be in ascending sorted order.
     *
     * ----------------------------------------------------------
     *
     * Approximate Mathematical Performance:
     * -------------------------------------
     *  Using an appropriate cost model, give the performance of your algorithm.
     *  Explain your answer.
     *
     *  Performance: ~3x(N^2)
     *
     *  Explanation: Using tilde notation. There are Two nested loops.
     *  Using the binomial theorem we get (n)(n-1)/2!. There are 6 array accesses => 6x(n)(n-1)/2 => 3n^2. Get rid of lower term because of tilde notation.
     *
     */
    static void sort(int[] a)
    {
    	for(int unsorted = 0; unsorted < a.length; unsorted++) {
    		int sort = unsorted;
    		while(sort > 0 && a[sort] < a[sort - 1]) {
    				int temp = a[sort];
    				a[sort] = a[sort-1];
    				a[sort-1] = temp;
    				sort--;
    		}
    	}
    }

    // ----------------------------------------------------------
    /**
     * Searches for an integer inside an array of integers.
     * This method is static, thus it can be called as Collinear.binarySearch(a,x)
     * @param a: A array of integers SORTED in ascending order.
     * @param x: An integer.
     * @return true if 'x' is contained in 'a'; false otherwise.
     *
     * ----------------------------------------------------------
     *
     * Approximate Mathematical Performance:
     * -------------------------------------
     *  Using an appropriate cost model, give the performance of your algorithm.
     *  Explain your answer.
     *
     *  Performance: ~2(log(n))
     *
     *  Explanation: Using tilde notation.
     *  the cost function of this algorithm is log(n) + 1.
     *  Then it becomes 2x(log(n)) for the two array accesses. You can get rid of the 1, because it's an unnecessary lower term in tilde notation.
     *
     */
    static boolean binarySearch(int[] a, int x)
    {
    	int lo = 0;
        int hi = a.length - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if      (x < a[mid]) hi = mid - 1;
            else if (x > a[mid]) lo = mid + 1;
            else return true;
        }
        return false;
    }
    
}