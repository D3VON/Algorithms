/* fib.cpp
 *     gcc fib.c -o fib  (this isn't a c program, though, is it?)
 * or  g++ fib.cpp -o fib
 * and run it with something like
 *     ./fib < datafile.txt
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
  printf("fib(%d) is requested.\n",n);
  if (n == 0) return 0;
  if (n == 1) return 1;
  if (A[n] == -1) 
    A[n] = fib(n-1) + fib(n-2);
  return A[n];
}


int main(int argc, char *argv[])
{
  int number;

  //fill array with -1 to indicate untouched elements
  for (int z=0; z<MAX_SIZE; z++)
    A[z] = -1;

  printf("Enter an number to fibonacci-ize\n");
  scanf("%d",&number); 
  printf("Your number is %d\n",number);
  
  printf("It's fibonacci number is:%d\n", fib(number));
}
