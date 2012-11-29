/* Pgm4_MatrixChain.cpp  matrix chain to find least steps of multiplication
 *     gcc Pgm4_MatrixChain.c -o Pgm4_MatrixChain  
 *         (this isn't a c program, though, is it?)
 * or  g++ Pgm4_MatrixChain.cpp -o Pgm4_MatrixChain
 * and run it with something like:
 *     ./Pgm4_MatrixChain < datafile.txt
 */

// Devon McBride refactored this code from Intro. to Algorithms, 2nd Ed. 
// -------------                           by Thomas H. Cormen, et al.
//                                         circa page 336
// ...and with a lot of help from Prof. Madden, CS333

#include <stdio.h> // for printf, and scanf
#include <iostream> // for cout?
#include <limits.h> // to use INT_MAX for sentinal value
#define MAX_SIZE 100000
using namespace std; // for cout?


/***************************************************************************
 * set up structs to hold input and our working-matrix that we use to 
 * find the optimal chain of matrix multiplications. 
 ***************************************************************************/

typedef struct  // contains the matrix chain values that are to be input
{
  int p,q;
} input;

typedef struct  // contains the matrix chain values that are to be input
{
  int num_multiplies;
  int split_num;
} results;

input     theInput[100];
results theResults[100][100]; // a 2dimensional array of type 'matrix'

/***************************************************************************
 * functions
 ***************************************************************************/

// theResults is global, thank Jesus
void PrintOptimalParentheses(int i, int j)	
{
    if (i==j)
        printf("A%d", i);
    else
    {
        printf("(");
        PrintOptimalParentheses(i, (theResults[i][j].split_num));
        PrintOptimalParentheses(theResults[i][j].split_num+1, j);
        printf(")");
    }
}

void MatrixChainOrder(int n)
{ 
  /***************************************************************************
  * This block handles matrix chain order, for chains the size 'n'. 
  ***************************************************************************/
  // refactored from the book, with much guidance from Dr. Madden.
  int i, j, k, z, level;
  //if matrix is alone, it costs nothing (no multiplications)
  for (z=0; z<(n); z++)
  { 
    theResults[z][z].num_multiplies = 0;
	  //printf("theResults[%d][%d] is: %d\n",z,z,theResults[z][z].num_multiplies );
  }

  // step through the levels of the pyramid, initializing with 'infinity' values
  for (level=1; level<n; level++)
    for (i=0; i<(n-level); i++) // traversal gets narrower as pyramid is climbed
    {
      j = i+level;
      theResults[i][j].num_multiplies = INT_MAX; // initialize with huge number 
                                                 // so it will fail comparison
                                                 // with all other costs produced                                                // by the following code
      // code to calculate costs
      for (k=i; k<j; k++)
      {
        int q = theResults[i]  [k].num_multiplies + 
                theResults[k+1][j].num_multiplies + 
                theInput[i].p * theInput[k+1].p * theInput[j].q;

        if (q < theResults[i][j].num_multiplies)
        {
          theResults[i][j].num_multiplies = q;
          theResults[i][j].split_num      = k;
        }
      }
  
  }
/* TESTING TO SEE THE ENTIRE RESULTS MATRIX THINGY
  for (level=0; level<n; level++)
  {
    for (i=0; i<(n-level); i++) // traversal gets narrower as pyramid is climbed
    {
      j = i+level;
      printf("%d ", theResults[i][j].num_multiplies);

    }    
    printf("\n");
  }
*/
}

int main(int argc, char *argv[])
{

  // receive pairs of ints input to this program, save into struct array
  int a = 0;
  while (scanf("%ld %ld", &theInput[a].p, &theInput[a].q) == 2)
    a++;  

  MatrixChainOrder(a);
  printf("%d ", theResults[0][a-1].num_multiplies);
  printf("\n");
  PrintOptimalParentheses(0,a-1);
  printf("\n");

}
