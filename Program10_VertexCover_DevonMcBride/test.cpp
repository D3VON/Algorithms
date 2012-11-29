#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;

int main(int argc, char *argv[]) {

  int n = 829;
	/*****************************************************************************
   first two numbers signify: #vertices, #edges
   *****************************************************************************/
  int *V;
	V = (int *) malloc(sizeof(int) * n );

  int i, j, temp;
  for (i=0; i<n; i++)
  {
    scanf("%d", &V[i]);
  }

  /*outer loop 'splits' array into sorted and unsorted sections, 
    and considers element 0 the 1st sorted element.  Inner loop inserts
    next element from the unsorted part into sorted part of list. 
  */
  for (j=1; j<n; j++)
  {
    temp = V[j];
    i = j-1; 
    while ( i>=0 && V[i]<temp)
    {
      V[i+1] = V[i];
      i--;
    }
    V[i+1] = temp;
  }


  /* Print out the data in the order we read. */
  for (i = 0; i < n; ++i){
    if (V[i] == V[i+1])
    {
      printf("%d\n", V[i]);
    }
  }
  
}

