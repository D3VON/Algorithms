/* SelectionSort.c
 *
 * Sample SelectionSort program for sorting numbers from biggest to smallest.
 *
 * Wikipedia says:
 * - An in-place comparison sort (destroys original order of original array).
 * - It has O(n2) time complexity, making it inefficient on large lists,
 *   and generally performs worse than the similar insertion sort.
 * - Selection sort is noted for its simplicity, and also has performance
 *   advantages over more complicated algorithms in certain situations,
 *   particularly where auxiliary memory is limited.
 *
 * Really nice animated gif on wikipedia here:
 * http://upload.wikimedia.org/wikipedia/commons/b/b0/Selection_sort_animation.gif
 *
 * You can compile this with something like:
 *     gcc SelectionSort.c -o SelectionSort (to compile as C)
 *     c++ SelectionSort.c -o SelectionSort (to compile as C++)
 * ...and run it with something like:
 *     ./SelectionSort < datafile.txt
 */
#include <stdio.h>

#define MAX_SIZE 1000000

int data[MAX_SIZE];

int main(int argc, char *argv[])
{
  int i, n;

  n = 0;
  // fill data array with elements in 'raw' order
  while (scanf("%d", &data[n]) == 1)
    ++n;

  int length = n; // preserve the value of n; it's needed later.

/*SELECTION SORT ALGORITHM:
  - outer loop begins as size of entire array
  - when inner loop finishes, smallest item has migrated to the end of the array
  - by comparing elements pair by pair, moving the smallest element to the end,
    outer loop gets one element shorter (after a full inner loop finishes)
    leaving the most recent smallest out of reach of the next pass of the
    next iteration of the inner loop.
  - outer loop repeats this process until the inner loop has nothing left

  NOTE: this is technically not the 'classic' Selection Sort in that it
  sorts 'backwards', putting smaller items at the end.
 */

  // decrement length of array after inner loop has found smallest 
  // and placed it at the end.
  for (; 0<length; length--)
  {
    int x, temp;
    // inner loop swaps smallest-of-a-pair (going through all pairs)
    // to the higher position in the array until it reaches the end.
    for (x = 0; x<length; x++)
    {
      if ( data[x+1] > data[x])
      {
        temp = data[x];
        data[x] = data[x+1];
        data[x+1] = temp;
        //printf("%d\n", data[x]);
      }

    }
  }


  /* Print out the data in the order we read. */
  for (i = 0; i < n; ++i)
    printf("%d\n", data[i]);
}
