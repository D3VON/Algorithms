/* HEAP SORT
 * You can compile this with something like
 *     gcc HeapSort.c -o HeapSort
 * and run it with something like
 *     ./HeapSort                    /////????< datafile.txt
 */

// Devon McBride refactored this code from Intro. to Algorithms, 2nd Ed. 
// -------------                           by Thomas H. Cormen, et al.

#include <stdio.h>

#define MAX_SIZE 1000000

typedef struct StudentIdentifiers
{
  long sID;
  long ssn;
};

StudentIdentifiers bunch_of_sIDs[MAX_SIZE];

/*
void heapify(StudentIdentifiers IDs[], int semiRootIndex, int lastLeafIndex)
{
  int ok, largest;
  StudentIdentifiers temp;

  ok = 0; // flag if perculation can stop (0 means don't stop, 1 means stop)

  //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
  //while there are children and leaves left to check...
  while ( (semiRootIndex*2 <= lastLeafIndex) && (!ok) )
  {
    if ( semiRootIndex*2 == lastLeafIndex )
      largest = semiRootIndex*2;
    else if ( IDs[semiRootIndex*2].sID > IDs[semiRootIndex*2 + 1].sID )
      largest = semiRootIndex*2;
    else
      largest = semiRootIndex*2 + 1;

    if ( IDs[semiRootIndex].sID < IDs[largest].sID )
    {
      temp               = IDs[semiRootIndex];
      IDs[semiRootIndex] = IDs[largest];
      IDs[largest] = temp;
      semiRootIndex      = largest;
      //heapify(IDs, semiRootIndex, lastLeafIndex);
      //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
    }
    else
      ok = 1;
  }
}
*/


void heapify(StudentIdentifiers IDs[], int semiRootIndex, int lastLeafIndex){
	int left  = 2*semiRootIndex + 1;  //left leaf
	int right = 2*semiRootIndex + 2;	//right leaf
	int largest = semiRootIndex; // index of largest value

	if(left <= lastLeafIndex && IDs[left].sID > IDs[semiRootIndex].sID){
		largest = left;
	}
	else
		largest = semiRootIndex;
	if(right <= lastLeafIndex && IDs[right].sID > IDs[largest].sID){
		largest = right;
	}

  /* if value of given index isn't larger than one of its leaves, 
     swap the larger-valued leaf with the given node.  */
	if(largest != semiRootIndex){ 
	    StudentIdentifiers temp = IDs[largest];
	    IDs[largest] = IDs[semiRootIndex];
	    IDs[semiRootIndex] = temp;
      /* percolating down (largest is now the leaf index and it's parent now
         actually has a larger value, so the name 'largest' has become 
         temporarily misleading here.  On the other hand, this leaf at index
         'largest' is now the semi-heap's root index to be heapified.*/
		  heapify(IDs, largest, lastLeafIndex);
	}
}


/* Prof. Madden calls this 'fast-build-heap' on tests, with O(n).
   "It starts at the bottom layer of the heap, 
   and then does a percolate-down at each spot along the way." */
void build_max_heap(StudentIdentifiers IDs[], int arraySize){ 
  //make heap of given array
  int index;
  for ( index = arraySize/2; index >= 0; index-- )
    heapify(IDs, index, arraySize);
}



void heapSort(StudentIdentifiers IDs[], int arraySize)
{
  int index;
  StudentIdentifiers temp;

	build_max_heap(IDs, arraySize);

  //put biggest element (root) into last leaf's position, 
  //then heapify the heap ONE LEAF LESS in size 
  //(In other words, leave the leaf you just did alone.)
  for ( index = arraySize-1; index >= 1; index-- )
  {
    temp       = IDs[0]; // temporarily store biggest element (from root)
    IDs[0]     = IDs[index]; // put last element (which is small) into root position
    IDs[index] = temp; // put biggest element into last position
    heapify(IDs, 0, index-1); // heapify "i-1"-sized heap 
                              // (protect the leaf you just did)
  }
}



int main(int argc, char *argv[])
{

  // receive pairs of ints input to this program, save into struct array
  int m = 0;
  while (scanf("%ld %ld", &bunch_of_sIDs[m].sID, &bunch_of_sIDs[m].ssn) == 2)
    m++; // Prof. Madden says 'scanf' is coolest thing ever invented. 

  heapSort(bunch_of_sIDs, m);

  long i;
  for (i=0; i<m ; i++)
  {
    printf("%5ld ", bunch_of_sIDs[i].sID);
    printf("%09ld\n", bunch_of_sIDs[i].ssn);
  }
}

