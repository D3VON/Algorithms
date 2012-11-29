/* fibDP.cpp  "fib Dynamic Programming"
 *     gcc fibDP.c -o fibDP  (this isn't a c program, though, is it?)
 * or  g++ fibDP.cpp -o fibDP
 * and run it with something like
 *     ./fibDP < datafile.txt
 */

// Devon McBride refactored this code from Intro. to Algorithms, 2nd Ed. 
//                                         by Thomas H. Cormen, et al.

#include <stdio.h> // for printf, and scanf
#include <iostream> // for cout?
#include <limits.h> // to use INT_MAX for sentinal value
#define MAX_SIZE 100000
using namespace std; // for cout?

int input[MAX_SIZE];
int A[MAX_SIZE];

int fib(int n){
  if (n == 0) return 0;
  if (n == 1) return 1;
  int previous = 0; // represents n-2
  int current = 1;  // represents n-1
  int z, nowy;
  for (z=1; z<n; z++)
  {
    nowy = previous + current;
    previous = current;
    current = nowy;
  }
  return current;
}

int main(int argc, char *argv[])
{
  int number;

  printf("Enter an number to fibonacci-ize\n");
  scanf("%d",&number); 
  printf("Your number is %d\n",number);
  
  printf("It's fibonacci number is:%d\n", fib(number));
}
