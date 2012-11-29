/* MERGE SORT
 * Sample MergeSort program for sorting numbers.
 * You can compile this with something like
 *     gcc MergeSort.c -o MergeSort
 * and run it with something like
 *     ./MergeSort < datafile.txt
 */

// Devon McBride refactored this code from Intro. to Algorithms, 2nd Ed. 
//                                         by Thomas H. Cormen, et al.
// hardest to deal with: converting indexes so as not to segfault

#include <stdio.h> // for printf, and scanf
#include <iostream> // for cout?
#include <limits.h> // to use INT_MAX for sentinal value
#define MAX_SIZE 1000000
using namespace std; // for cout?

int mainArray[MAX_SIZE];

void Merge(int mergeArray[], int p, int q, int r){
            cout << "start Merge \n";
  int n1 = q - p + 1; // total number of elements in the Left array (length)
  int n2 = r - q; // ............................in the Right array (length)

  n1++; // Lengths incremented to accomodate 'sentinel' values.
  n2++;
  
  int Left [n1]; // Create temporary arrays (they disappear when
  int Right[n2]; // this function's namespace does).
  
  int i, j;
  // split array into two new arrays: Left and Right
  for (i=0; i<n1-1; i++) 
  {
    Left [i] = mergeArray[p+i];
  }

  //Right's first element is 1 after q (q is last element in Left Array)
  for (j=1; j<n2-1; j++)
  {
    Right[j] = mergeArray[q+1+j];
  }


  // set 'sentinel' values; reliably higher than anything in the given array.
  Left [n1] = INT_MIN;   // this is like an environment variable or something
  Right[n2] = INT_MIN;   // and returns the largest possible integer value.

  i = 0;
  j = 0;
  int k;
  // iterate "n" times (length of Right array, which is 'r')
  for (k = p; k <= r; k++)
  {
    if (Left[i] >= Right[j])
    {
      mergeArray[k] = Left[i];
      i++;
    }else{
      mergeArray[k] = Right[j];
      j++;
    }
  }
}

void MergeSort(int sortArray[], int p, int r){
                        cout << "start MergeSort \n";
  if (p<r) // e.g. 0<1, so it considers only arrays of two or more elements
  {
    int q = (p+r)/2; // q is last index of Left array
                        cout << "p is " << p << "\n";
                        cout << "q is " << q << "\n";
                        cout << "r is " << r << "\n----\n";
                        //sleep(1);
    MergeSort(sortArray, p, q); // does left side first, recursing as it goes
                        cout << "second mergeSort\n";
                        cout << "q+1 is " << q+1 << "\n";
                        cout << "r is " << r << "\n----\n";
                        //sleep(1);
    MergeSort(sortArray, q+1, r);
    Merge(sortArray, p, q, r);
                        for (int z=0; z<=r; z++)
                        {
                          cout << sortArray[z] << ", ";
                        }
                          cout << "\n";
                        //sleep(1);
                    }else{
                      cout << "no more splitting to do here; so go to next mergesort function \n";
                            //sleep(1);
  }
}


int main(int argc, char *argv[])
{
  int n, i; 

  n = 0;
  while (scanf("%d", &mainArray[n]) == 1)
    ++n;
  // even though the array was declared huge,
  // 'n' will give a cap for the part we will use.

  MergeSort(mainArray, 0, n-1);

  /* Print out the data in the order we read. */
  for (i = 0; i < n; ++i)
    printf("%d\n", mainArray[i]);
}



