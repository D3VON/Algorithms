/* INSERTION SORT
 * Sample InsertionSort program for reading in numbers.
 * You can compile this with something like
 *     gcc InsertionSort.c -o InsertionSort
 * and run it with something like
 *     ./InsertionSort < datafile.txt
 */
#include <stdio.h>

#define MAX_SIZE 1000000

int data[MAX_SIZE];

int main(int argc, char *argv[])
{
  int i, j, n, temp;

  n = 0;
  while (scanf("%d", &data[n]) == 1)
    ++n;

  // Now you have an array with n elements in it.
  
  /*outer loop 'splits' array into sorted and unsorted sections, 
    and considers element 0 the 1st sorted element.  Inner loop inserts
    next element from the unsorted part into sorted part of list. 
  */
  for (j=1; j<n; j++)
  {
    temp = data[j];
    i = j-1; 
    while ( i>=0 && data[i]<temp)
    {
      data[i+1] = data[i];
      i--;
    }
    data[i+1] = temp;
  }


  /* Print out the data in the order we read. */
  for (i = 0; i < n; ++i)
    printf("%d\n", data[i]);
}
