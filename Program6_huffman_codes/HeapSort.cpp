/* modifying HEAP SORT to produce Huffman coding
 * You can compile this with something like
 *     gcc HeapSort.c -o HeapSort
 * and run it with something like
 *     ./HeapSort      
 */

// Devon McBride refactored this code from Intro. to Algorithms, 2nd Ed. 
// -------------                           by Thomas H. Cormen, et al.

#include <stdio.h>

#define MAX_SIZE 1000000

typedef struct HuffNode
{
  int letter;
  int freq;
  int parent;
  int left;
  int right;
};

HuffNode nodeArray[MAX_SIZE];
/*
void max_heapify(HuffNode nodeArray[], int semiRootIndex, int lastLeafIndex)
{
  int ok, largest;
  HuffNode temp;

  ok = 0; // flag if perculation can stop (0 means don't stop, 1 means stop)

  //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
  //while there are children and leaves left to check...
  while ( (semiRootIndex*2 <= lastLeafIndex) && (!ok) )
  {
    if ( semiRootIndex*2 == lastLeafIndex )
      largest = semiRootIndex*2;
    else if ( nodeArray[semiRootIndex*2].freq > nodeArray[semiRootIndex*2 + 1].freq )
      largest = semiRootIndex*2;
    else
      largest = semiRootIndex*2 + 1;

    if ( nodeArray[semiRootIndex].freq < nodeArray[largest].freq )
    {
      temp               = nodeArray[semiRootIndex];
      nodeArray[semiRootIndex] = nodeArray[largest];
      nodeArray[largest] = temp;
      semiRootIndex      = largest;
      //max_heapify(nodeArray, semiRootIndex, lastLeafIndex);
      //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
    }//////update: I got recursive to work.
    else
      ok = 1;
  }
}
*/


void max_heapify(HuffNode nodeArray[], int semiRootIndex, int lastLeafIndex){
	int left  = 2*semiRootIndex + 1;  //left leaf
	int right = 2*semiRootIndex + 2;	//right leaf
	int largest = semiRootIndex; // index of largest value

	if(left <= lastLeafIndex && nodeArray[left].freq > nodeArray[semiRootIndex].freq){
		largest = left;
	}
	else
		largest = semiRootIndex;
	if(right <= lastLeafIndex && nodeArray[right].freq > nodeArray[largest].freq){
		largest = right;
	}
  /* if value of given index isn't larger than one of its leaves, 
     swap the larger-valued leaf with the given node.  */
	if(largest != semiRootIndex){
	    HuffNode temp = nodeArray[largest];
	    nodeArray[largest] = nodeArray[semiRootIndex];
	    nodeArray[semiRootIndex] = temp;
      /* percolating down (largest is now the leaf index and it's parent now
         actually has a larger value, so the name 'largest' has become 
         temporarily misleading here.  On the other hand, this leaf at index
         'largest' is now the semi-heap's root index to be heapified.*/
		  max_heapify(nodeArray, largest, lastLeafIndex);
	}
}

/* Prof. Madden calls this 'fast-build-heap' on tests, with O(n).
   "It starts at the bottom layer of the heap, 
   and then does a percolate-down at each spot along the way." */
void build_max_heap(HuffNode nodeArray[], int arraySize){ 
  //make heap of given array
  int index;
  for ( index = arraySize/2; index >= 0; index-- )
    max_heapify(nodeArray, index, arraySize);
}



void heapSort(HuffNode nodeArray[], int arraySize)
{
  int index;
  HuffNode temp;

	build_max_heap(nodeArray, arraySize);

  //put biggest element (root) into last leaf's position, 
  //then max_heapify the heap ONE LEAF LESS in size 
  //(In other words, leave the leaf you just did alone.)
  for ( index = arraySize-1; index >= 1; index-- )
  {
    temp       = nodeArray[0]; // temporarily store biggest element (from root)
    nodeArray[0]     = nodeArray[index]; // put last element (smallest) into root position
    nodeArray[index] = temp; // put biggest element into last position
    max_heapify(nodeArray, 0, index-1); // max_heapify "i-1"-sized heap 
                              // (protect the leaf you just did)
  }
}



int main(int argc, char *argv[])
{

  // receive pairs of ints input to this program, save into struct array
  int m = 0;
  while (scanf("%ld %ld", &nodeArray[m].freq, &nodeArray[m].letter) == 2)
    m++;

  heapSort(nodeArray, m);

  long i;
  for (i=0; i<m ; i++)
  {
    printf("%5ld ", nodeArray[i].freq);
    printf("%09ld\n", nodeArray[i].letter);
  }
}

