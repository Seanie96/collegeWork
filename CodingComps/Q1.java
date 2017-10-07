import java.util.Scanner;

public class Q1{

  public static void main(String[] args) {

    Scanner ss = new Scanner(System.in);

    //ss.useDelimiter("\n")

    System.out.println("here");

    int num = Integer.parseInt(ss.nextLine());

    System.out.println("------" + num);

    for(int n = 0; n < num; n++) {


      System.out.println("------" + num);
      String line = ss.nextLine();
      //line = ss.nextLine();
      System.out.println("------" + line);
      String[] args2 = line.split(",");
      System.out.println(args2[0] + " ----------- " + args[1]);
      int num1 = Integer.parseInt(args2[0]);
      int num2 = Integer.parseInt(args2[1]);

      int timeOfOnes = 0;
      while(num1 >= 1 && num2 >= 1) {
        int num1has1 = num1%2;
        int num2has1 = num2%2;
        if(num1has1 == 1 && num2has1 == 1) {
          timeOfOnes++;
        }
        num1 /=2;
        num2 /=2;
      }

      System.out.println(timeOfOnes);
    }
    }

  }
