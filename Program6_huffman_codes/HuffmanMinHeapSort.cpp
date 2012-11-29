/* modifying heap functions to produce Huffman coding
                                       **************

   Devon McBride ported this code from Intro. to Algorithms, 2nd Ed. 
   -------------                       by Thomas H. Cormen, et al.

   to run: > ./a.out < input

   For this program, you will need to compute the optimal Huffman code for 
   a set of letters. The input will consist of a series of frequencies (and 
   these correspond to the number of A, B, C, ...). There will be at most 26 
   numbers in the input file.

   For example, if you have 5 As, 2 Bs, and 1 C, then the input is

   5
   2
   1

   Your program should determine the total number of bits to encode the 
   letters, and then print out the bit patterns used for each code work. 
   In this example, you would have a tree with A on one side (requiring 1 
   bit), and then B and C on the other side (requiring 2 bits each). The 
   total number of bits is 11 (5 bits, one for each A, then 4 bits for 
   the Bs, and 2 more bits for the C). The output would look like

   11
   A 0
   B 10
   C 11

   There are many different possible bit patterns that would work -- you 
   just need to print out one of them.

   You should use a heap data structure for the letters; extract two from 
   the heap (using the extract min function), add their frequencies together, 
   and then insert it back into the heap as a "new letter." You keep going, 
   until there's only one thing extracted from the heap. 
               
 */

#include <stdio.h>

typedef struct symbol
{
  int letter;
  int freq;
};


typedef struct tree
{
  int parent;
  int zero_or_one;
  int numBits; // number of bits in this letter's huffman code
  int freq; // frequency of this letter's usage in the given data
};

symbol s[100]; // heap to act as min-priority queue
tree   t[100]; // tree for huffman coding

/*
void max_heapify(symbol s[], int semiRootIndex, int lastLeafIndex)
{
  int ok, largest;
  symbol temp;

  ok = 0; // flag if perculation can stop (0 means don't stop, 1 means stop)

  //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
  //while there are children and leaves left to check...
  while ( (semiRootIndex*2 <= lastLeafIndex) && (!ok) )
  {
    if ( semiRootIndex*2 == lastLeafIndex )
      largest = semiRootIndex*2;
    else if ( s[semiRootIndex*2].freq > s[semiRootIndex*2 + 1].freq )
      largest = semiRootIndex*2;
    else
      largest = semiRootIndex*2 + 1;

    if ( s[semiRootIndex].freq < s[largest].freq )
    {
      temp               = s[semiRootIndex];
      s[semiRootIndex] = s[largest];
      s[largest] = temp;
      semiRootIndex      = largest;
      //max_heapify(s, semiRootIndex, lastLeafIndex);
      //COULDN'T GET RECURSIVE TO WORK!!!!DANG IT!!!! RECURSIVE IS ACCURSED! 
    }//////update: I got recursive to work; see the other max_heapify function
    else
      ok = 1;
  }
}
*/
/*
void max_heapify(symbol s[], int semiRootIndex, int lastLeafIndex){
	int left  = 2*semiRootIndex + 1;  //left leaf
	int right = 2*semiRootIndex + 2;	//right leaf
	int largest = semiRootIndex; // index of largest value

	if(left <= lastLeafIndex && s[left].freq > s[semiRootIndex].freq)
		largest = left;
	if(right <= lastLeafIndex && s[right].freq > s[largest].freq)
		largest = right;
  /* if value of given index isn't larger than one of its leaves, 
     swap the larger-valued leaf with the given node.  */
/*	if(largest != semiRootIndex)
  {
	    symbol temp = s[largest];
	    s[largest] = s[semiRootIndex];
	    s[semiRootIndex] = temp;
      /* percolating down (largest is now the leaf index and it's parent now
         actually has a larger value, so the name 'largest' has become 
         temporarily misleading here.  On the other hand, this leaf at index
         'largest' is now the semi-heap's root index to be heapified.*/
/*		  max_heapify(s, largest, lastLeafIndex);
	}
}
*/
void min_heapify(symbol s[], int semiRootIndex, int lastLeafIndex)
{
  //printf("...semiRootIndex: %d, lastLeafIndex: %d\n", semiRootIndex, lastLeafIndex);
	int left  = 2*semiRootIndex + 1;  //left leaf
	int right = 2*semiRootIndex + 2;	//right leaf
	int smallest = semiRootIndex; // index containing the smallest value
                                // initialized to semiRootIndex
  //printf("...left: %d, right: %d, smallest: %d\n", left, right, smallest);

	if(left <= lastLeafIndex && s[left].freq < s[semiRootIndex].freq)
		smallest = left;
	if(right <= lastLeafIndex && s[right].freq < s[smallest].freq)
		smallest = right;
  /* if value of given index's value isn't smaller than one of its leaves', 
     swap the smaller-valued leaf with the given node.  */
	if(smallest != semiRootIndex)
  {
	    symbol temp = s[smallest];
	    s[smallest] = s[semiRootIndex];
	    s[semiRootIndex] = temp;
      /* percolating down (smallest is now the leaf index and it's parent now
         actually has a larger value, so the name 'smallest' has become 
         temporarily misleading here.  On the other hand, this leaf at index
         'smallest' is now the semi-heap's root index to be heapified.*/
		  min_heapify(s, smallest, lastLeafIndex);
	}
}

  
      /* Prof. Madden calls this 'fast-build-heap' on tests, with O(n).
         "It starts at the bottom layer of the heap, 
         and then does a percolate-down at each spot along the way." */
/*
void build_max_heap(symbol s[], int lastIndex){ 
  //make heap of given array
  int index;
  for ( index = lastIndex/2; index >= 0; index-- )
    max_heapify(s, index, lastIndex);
}
*/
      /* Not much modification here to build a min-heap. 
         The real modification is in min_heapify. */
void build_min_heap(symbol s[], int lastIndex){ 
  // Start at last node (not leaf), heapify, 
  // then step back one node each loop, heapifying each time.
  //printf("::::::::::::::::::::::Building min-heap::::::::::::::::::::::\n");
  int index;
  for ( index = lastIndex/2; index >= 0; index-- ) 
  {
    //printf("index counts back to zero: %d\n", index);
    min_heapify(s, index, lastIndex);
  }

  /*printf("This is how the array looks after the min-heap is built:\n");
  int i;
  for (i=0; i<=lastIndex ; i++)
  {
    printf("%5ld ", s[i].freq);
    printf("%d\n", s[i].letter);
  }
  printf("\n");
  */
}

/*
void heapSort(symbol s[], int lastIndex)
{
  int index;
  symbol temp;

	build_max_heap(s, lastIndex);
  
  //put biggest element (root) into last leaf's position, 
  //then max_heapify the heap ONE LEAF LESS in size 
  //(In other words, leave the leaf you just did alone.)
  for ( index = lastIndex; index >= 1; index-- )
  {
    temp       = s[0]; // temporarily store biggest element (from root)
    s[0]     = s[index]; // put last element (smallest) into root position
    s[index] = temp; // put biggest element into last position
    max_heapify(s, 0, index-1); // min_heapify "i-1"-sized heap
  //max_heapify(s, 0, index-1); // max_heapify "i-1"-sized heap 
                                        // (leave out the leaf you just did)
  }
}
*/


void print (int x)
{
  if (t[x].parent != -1)
  {
    print(t[x].parent);
    printf("%d", t[x].zero_or_one);
  }
}



int getBits (int x)
{
  int numBits = 0;
  if (t[x].parent != -1)
  {
    numBits = getBits(t[x].parent);
    numBits++;
  }
  return numBits;
}



//                   the array, last leaf, thing to insert);
void min_heap_insert(symbol A[], int i, symbol temp)
{
  A[i] = temp; // put this new node at last leaf

// percolate that node up if it needs to
//while(under root && parent val > new_one val)
  while ( i>0 && A[(i-1)/2].freq > A[i].freq)
  {
    A[i] = A[(i-1)/2]; //last leaf = parent's bigger value
    A[(i-1)/2] = temp; //parent = the new node's lower value
    i = (i-1)/2; // i now points to that one's parent    
  }
}

void min_heapSort(symbol s[], int lastIndex)
{
  int index;
  symbol temp;

	//build_max_heap(s, lastIndex);
  build_min_heap(s, lastIndex);

  // put smallest element (root) into last leaf's position, 
  // then min_heapify the heap ONE LEAF LESS IN SIZE --in other words, 
  // leave the leaf you just moved untouched by the next heapify.
  for ( index = lastIndex; index >= 1; index-- )
  {
    temp     = s[0]; // temporarily store biggest element (from root)
    s[0]     = s[index]; // put last element (smallest) into root position
    s[index] = temp; // put biggest element into last position
    min_heapify(s, 0, index-1); // min_heapify "i-1"-sized heap
  //max_heapify(s, 0, index-1); // max_heapify "i-1"-sized heap 
                                        // (leave out the leaf you just did)
  }
}

//
symbol min_heap_minimum(symbol A[])
{
  return A[0];
}


/* this function modifies the size of the heap globally */
         // call it with (s,        &lastIndex)
symbol min_heap_extract(symbol A[], int* lastIndex)
{
  if ((*lastIndex)-1 < 0)
    printf("Hey!  Heap underflow!");

  symbol minimum = A[0];
  A[0] = A[(*lastIndex)-1];
  (*lastIndex)--; /* cab10886: you need to dereference the pointer before 
                     modifying it otherwise you'll be modifying the pointer */
  min_heapify(s, 0, (*lastIndex)); 
  return minimum;
}





int main(int argc, char *argv[])
{
  // Receive pairs of ints input to this program, save into struct array
  int numSymbols = 0;
  while ( scanf("%d", &s[numSymbols].freq) == 1 )
  {
    s[numSymbols].letter = numSymbols;
    t[numSymbols].freq   = s[numSymbols].freq; // Store into hufftree, too.
    numSymbols++;
  }

  printf("\n");


  // these will be incremented/decremented at various times, 
  // but 'originalSize' shall not be changed. 
  int originalSize = numSymbols;
  int heapSize     = numSymbols;
/*
  printf("This is how we received all the numbers:\n");
  int i;
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5d ", s[i].freq);
    printf("is the freq of letter: %d\n", s[i].letter);
  }
*/


  build_min_heap(s, heapSize-1);
/*
  printf("This is the heap:\n");
  //int i;
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5ld ", s[i].freq);
    printf("%d\n", s[i].letter);
  }
*/
  // watch out: heapSize will shrink
  // ---------  numSymbols with grow
  // ...but they start out equal.
  //printf("heapSize: %d\n", heapSize);
  while (heapSize>=2)
  {
    symbol thing1 = min_heap_extract(s, &heapSize);
    //printf("AFTER EXTRACT heapSize: %d\n", heapSize);
    //printf("thing1.letter: %d\n", thing1.letter);
    //printf("thing1.freq: %d\n", thing1.freq);
    symbol thing2 = min_heap_extract(s, &heapSize);
    //printf("AFTER EXTRACT heapSize: %d\n", heapSize);
    //printf("thing2.letter: %d\n", thing2.letter);
    //printf("thing2.freq: %d\n", thing2.freq);
    symbol new_thing;
           new_thing.letter = numSymbols;
           new_thing.freq   = thing1.freq + thing2.freq;
    //printf("new_thing.letter: %d\n", new_thing.letter);
    //printf("new_thing.freq: %d\n", new_thing.freq);

    //printf("^^^^^^^^^^^^^^fidlin' with the t tree^^^^^^^^^^^^\n");
    t[thing1.letter].parent      = numSymbols;
    //printf("t[%d].parent: %d\n", thing1.letter, t[thing1.letter].parent);
    t[thing1.letter].zero_or_one = 0;
    //printf("t[%d].zero_or_one: %d\n", thing1.letter, t[thing1.letter].zero_or_one);
    t[thing2.letter].parent      = numSymbols;
    //printf("t[%d].parent: %d\n", thing2.letter, t[thing2.letter].parent);
    t[thing2.letter].zero_or_one = 1;
    //printf("t[%d].zero_or_one: %d\n", thing2.letter, t[thing2.letter].zero_or_one);
    t[numSymbols].parent         = -1;
    //printf("t[%d].parent: %d\n", numSymbols, t[numSymbols].parent);
    numSymbols++;
    heapSize++; // doesn't get modified by insert function, do it manually here
    min_heap_insert(s, heapSize-1, new_thing);
    //printf("AFTER INSERT heapSize: %d\n", heapSize);
/*
    printf("This is the heap:\n");
    //int i;
    for (i=0; i<heapSize ; i++)
    {
      printf("%5ld ", s[i].freq);
      printf("%d\n", s[i].letter);
    }
*/
  }
  
  /* If the given frequencies of letters were actually used, 
     print the number of bits needed to huffman-encode the whole thing. */
  int i;
  int tally = 0;
  for (i=0; i<originalSize; i++)
  {
    t[i].numBits = getBits(i);
    tally = tally + t[i].numBits * t[i].freq;
  }
  printf("%d\n", tally);


  // print the huffman codes of each given letter
  for (i=0; i<originalSize; i++)
  {
    printf("%c ", i+'A');
    print(i);
    printf("\n");
  }











/*
  symbol x = min_heap_extract(s, &numSymbols);
  printf("the min node has value: %d\n", x.freq);
  
  printf("numSymbols is now %d\n", numSymbols);
  printf("This is the heap after extracting:\n");
  //int i;
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5ld ", s[i].freq);
    printf("%d\n", s[i].letter);
  }

  x = min_heap_extract(s, &numSymbols);
  printf("the min node has value: %d\n", x.freq);
  
  printf("numSymbols is now %d\n", numSymbols);
  printf("This is the heap after extracting again:\n");
  //int i;
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5ld ", s[i].freq);
    printf("%d\n", s[i].letter);
  }



  //TESTING INSERT 
  symbol woof; 
  woof.freq = 17;
  woof.letter = 17;
  numSymbols++;
  min_heap_insert(s, numSymbols, woof);
  printf("===================does INSERT work?====================\n");
  x = min_heap_minimum(s);
  printf("woof node has value: %d\n", woof.freq);  
  printf("the min node has value: %d\n", x.freq);  
  printf("numSymbols is now %d\n", numSymbols);
  printf("This is the heap after inserting:\n");
  //int i;
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5ld ", s[i].freq);
    printf("%d\n", s[i].letter);
  }

*/

/*
  //  heapSort(s, numSymbols-1);
  min_heapSort(s, numSymbols-1);

  printf("This is how how it looks after sorting:\n");
  for (i=0; i<numSymbols ; i++)
  {
    printf("%5d ", s[i].freq);
    printf("for letter %d\n", s[i].letter);
  }

*/


  printf("\n");


}

