/* Dijkstra's Algorithm using extended heap functions

   Program8_Dijkstras_DevonMcBride.cpp

   By Devon McBride (ported from Cormen book, page 595).

   Implement a programs to walk through a graph using Dijkstra's algorithm. 
   YOU MUST USE AN ABSTRACT DATA STRUCTURE which supports arbitrary numbers 
   of edges per vertex. DO NOT use a matrix. Some of the graphs will be too 
   large for a matrix.

   For the priority queue, use an EXTENDED HEAP. We've talked about this in 
   class a bunch of times -- you'll need to implement a normal binary heap, 
   but will need an extra array of integers that stores the location of each 
   vertex within the heap. When you update the distance to an element in the 
   heap, you'll need to find the location of the vertex, and then have it 
   "percolate up." When the vertex percolates up, you'll also need to update 
   the locations of things in this extra array. 

   The input will consist of the # of vertices, the # of edges, and then a 
   series of edges. As an example, we might have a 5 vertex graph, with 4 
   edges. I'll name the vertices A, B, C, D, E, for explanation, but they'll 
   actually be numbered 0 through 4. A connects to B with a length of 10, 
   B connects to C with length 3, A connects to D with length 5, 
   A connects to E with length 2. The input file looks like this:

   v e
   5 4
   0 1 10
   1 2 3
   0 3 5
   0 4 2

  Compute the distances from vertex 0 to every other vertex. If there are 
  less than 10 vertices, print the distance to each. If there are 10 or more 
  vertices, print the distance to the furthest vertex (and print which 
  vertex it is).
                                
 */

#include <stdio.h>
#include <malloc.h>
//#include <limits.h> // to use INT_MAX

/*****************************************************************************/
/*** Structures **************************************************************/
/*****************************************************************************/

typedef struct vertex_struct
{
  int parent; // pointer to the shortest path to root
  int index;
  int distance;
  // Edges that connect to the vertex.
  struct edge_struct *edges;
};

vertex_struct *vertices; // global array pointer here (very friendly)
vertex_struct *queue;
vertex_struct *shortest; 

typedef struct edge_struct
{
  int u;
  int v;
  int length;
  struct edge_struct *next;
};

edge_struct *edges; // global array pointer here (very friendly)

edge_struct *MSTree; // holds the edges of the MSTree for given graph


/*****************************************************************************/
/*** Functions  **************************************************************/
/*****************************************************************************/

void min_heapify(vertex_struct *v, int semiRootIndex, int lastLeafIndex)
{
  //printf("...semiRootIndex: %d, lastLeafIndex: %d\n", semiRootIndex, lastLeafIndex);
	int left  = 2*semiRootIndex + 1;  //left leaf
	int right = 2*semiRootIndex + 2;	//right leaf
	int smallest = semiRootIndex; // index containing the smallest value
                                // initialized to semiRootIndex
  //printf("...left: %d, right: %d, smallest: %d\n", left, right, smallest);

	if(left <= lastLeafIndex && v[left].distance < v[semiRootIndex].distance)
		smallest = left;
	if(right <= lastLeafIndex && v[right].distance < v[smallest].distance)
		smallest = right;
  /* if value of given index's value isn't smaller than one of its leaves', 
     swap the smaller-valued leaf with the given node.  */
	if(smallest != semiRootIndex)
  {
	    vertex_struct temp = v[smallest];
	    v[smallest] = v[semiRootIndex];
	    v[semiRootIndex] = temp;
      /* percolating down (smallest is now the leaf index and it's parent now
         actually has a larger value, so the name 'smallest' has become 
         temporarily misleading here.  On the other hand, this leaf at index
         'smallest' is now the semi-heap's root index to be heapified.*/
		  min_heapify(v, smallest, lastLeafIndex);
	}
}
  
      /* Prof. Madden calls this 'fast-build-heap' on tests, with O(n). */
void build_min_heap(vertex_struct *v, int lastIndex){ 
  // Start at last node (not leaf), heapify, 
  // then step back one node each loop, heapifying each time.
  //printf("::::::::::::::::::::::Building min-heap::::::::::::::::::::::\n");
  int index;
  for ( index = lastIndex/2; index >= 0; index-- ) 
  {
    //printf("index counts back to zero: %d\n", index);
    min_heapify(v, index, lastIndex);
  }
            //*
            printf("This is how the array looks after the min-heap is built:\n");
            int i;
            for (i=0; i<=lastIndex ; i++)
            {
              printf("%5ld ", v[i].distance);
            }
            printf("\n");
            //*/
}

// WARNING: HAVE TO MODIFY YOUR heapSize (OR WHATEVER IT'S NAMED) IN main
//          BECAUSE THIS FUNCTION DOESN'T TAKE CARE OF MODIFYING IT.
//                  (the array,     last leaf,    thing to insert)
void insert_min_heap(vertex_struct A[], int i, vertex_struct temp)
{
  A[i] = temp; // put this new node at last leaf

// percolate that node up if it needs to
//while(under root && parent val > new_one val)
  while ( i>0 && A[(i-1)/2].distance > A[i].distance)
  {
    A[i] = A[(i-1)/2]; //last leaf = parent's bigger value
    A[(i-1)/2] = temp; //parent = the new node's lower value
    i = (i-1)/2; // i now points to that one's parent    
  }
}


void min_heapSort(vertex_struct *v, int lastIndex)
{
  int index;
  vertex_struct temp;

	//build_max_heap(v, lastIndex);
  build_min_heap(v, lastIndex);

  // put smallest element (root) into last leaf's position, 
  // then min_heapify the heap ONE LEAF LESS IN SIZE --in other words, 
  // leave the leaf you just moved untouched by the next heapify.
  for ( index = lastIndex; index >= 1; index-- )
  {
    temp     = v[0]; // temporarily store biggest element (from root)
    v[0]     = v[index]; // put last element (smallest) into root position
    v[index] = temp; // put biggest element into last position
    min_heapify(v, 0, index-1); // min_heapify "i-1"-sized heap
  //max_heapify(v, 0, index-1); // max_heapify "i-1"-sized heap 
                                        // (leave out the leaf you just did)
  }
}

//
vertex_struct min_heap_minimum(vertex_struct A[])
{
  return A[0];
}

/* this function modifies the size of the heap globally */
         // call it with (v,        &lastIndex)
vertex_struct extract_min_heap(vertex_struct A[], int* lastIndex)
{////////////////////// lastIndex might be misleadingly named (too big by 1?)
  if ((*lastIndex)-1 < 0)
    printf("Hey!  Heap underflow!");

  vertex_struct minimum = A[0];
  A[0] = A[(*lastIndex)-1];
  //A[(*lastIndex)-1] = minimum; // 'hide' root node at "last" index
  (*lastIndex)--; /* cab10886: you need to dereference the pointer before 
                     modifying it otherwise you'll be modifying the pointer */
  min_heapify(A, 0, (*lastIndex)); 
  return minimum;
}


void relax (edge_struct e)
{
  int u = e.u;
  int v = e.v;
  int length = e.length;

  if ( vertices[v].distance > (vertices[u].distance + length) )
  {
    vertices[v].distance = vertices[u].distance + e.length;
    //vertices[v].parent = vertices[u]; // not necessary for us here
  }
}

/*
  Compute the distances from vertex 0 to every other vertex. If there are 
  less than 10 vertices, print the distance to each. If there are 10 or more 
  vertices, print the distance to the furthest vertex (and print which 
  vertex it is).
*/                        // starting vertex always 0
void dijkstra (vertex_struct *v, int source, int num_verts)
{
  int vertArraySize = num_verts;
  int shortSize = 0;
  int heapSize = 0;
  queue    = (vertex_struct *) malloc(sizeof(vertex_struct) * num_verts);


  // initialize single-source (distance and parent data members)
  int i;
  for (i = 0; i < vertArraySize; ++i)
  {
    vertices[i].distance = 9999; //
    //vertices[i].edges = NULL;
    vertices[i].index = i; //redundant?
    vertices[i].parent = NULL; // the parent giving the shortest path to root
  }

  vertices[source].distance   = 0; // root is zero distance from itself
  vertices[source].index = 0;

  //initialize an 'empty set'... "of vertices whose final shortest-path weights
  //from the source have already been determined." Cormen book pg. 595
  shortest = (vertex_struct *) malloc(sizeof(vertex_struct) * num_verts);

  //load all vertices into the queue (sorted by distance data member)
  while (vertArraySize > 0)
  {                       //lastIndex of growing queue
    insert_min_heap (queue, heapSize, v[vertArraySize-1]);
    heapSize++;
    vertArraySize--;
  }
            // TEST: 
            printf("This is the queue (heap):\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("queue index %d, distance %d \n", i, queue[i].distance);
            }
            printf("\n");

  while (heapSize > 0)
  {                       // extract decrements heapSize
    shortest[shortSize] = extract_min_heap(queue, &heapSize);

    //printf("Vertex %d's adjacency list:\n", i); 
    edge_struct *current = shortest[shortSize].edges;
    while (current != 0)
    {
      relax((*current));
      //printf("Vertex %d is %d miles from %d\n", (*current).u, (*current).length, (*current).v); 
      current = current->next; // use -> when it's a pointer, for some wretchedly fussy reason
    }
    shortSize++;
  }

  for (i=0; i<shortSize; i++)
  {     
    printf("vertex %d is %d from root\n", shortest[i].index, vertices[shortest[i].index].distance);
  }
    printf("works this way too:\n");
  // works this way, too
  for (i=0; i<num_verts; i++)
  {     
    printf("vertex %d is %d from root\n", i, vertices[i].distance);
  }






}



/*
  int MSTPrims(int x)
  {
      //Initialize all distances to be very far; and everyone's an orphan
    while (vertex v)
    {
      v.distance = INT_MAX; // book calls this 'key'
      v.parent   = NULL;      
    }
    verticev[x].distance = 0; // root has zero distance from itself

    min-buildheap;
    min-heapsort;
    min-queue;
  
    vertex U;
    while (Q.next != NULL)
    {
      U = Q.extract;
      while (u.next != NULL)
      {
        if (that edge.length < edge.distance) ???? whats really going on here? ?????
        {
          something.parent = u;
          something.distance = edge.distance;
        }
      }
    }
  }

*/



/*###########################################################################
  ###########################################################################
  ##################                                  #######################
  ##################              MAIN                #######################
  ##################                                  #######################
  ###########################################################################
  ###########################################################################*/

int main(int argc, char *argv[])
{
  int num_v, num_e, grenades;

  // Grab 1st two numbers, make space to hold that many vertices and edges.
  scanf("%d %d %d", &num_v, &num_e, &grenades);
printf("number of vertices is %d\n", num_v);


  // Reserve space -- twice as many edges, because we're creating links for
  // both directions.
  // (vertices and edges were both declaired globally (see top) )
  vertices = (vertex_struct *) malloc(sizeof(vertex_struct) * num_v);
  edges = (edge_struct *) malloc(sizeof(edge_struct) * num_e*2);

  // these will be incremented/decremented at various times, 
  // but 'num_v' shall not be changed. 
  int originalSize = num_v;
  int heapSize     = num_v;
  int lastIndex = num_v - 1;
  int i;

  num_e = num_e*2; // capturing reverse direction of edges doubles num of edges
  for (i =  0; i < num_e; i += 2)
  {
    scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].length);
    edges[i + 1].u = edges[i].v;
    edges[i + 1].v = edges[i].u;
    edges[i + 1].length = edges[i].length;

    // add edge to the vertex's adjacency list. 
    edges[i].next = vertices[edges[i].u].edges;
    vertices[edges[i].u].edges = &edges[i]; 
    // Add to the other vertex, too.  EXPLAINED BETTER:
    // ... this edge points to the existing adjacency list of the vertex...
    // ...then, that adjacency list is clobbered by the edge whose 
    // next we just added to.  Basically, we're adding the new edge
    // to the head of the adjacency list.  COOL!  (and wierd)
    // So we don't have to keep track of the tail or traverse or anything. WOW.
    edges[i + 1].next = vertices[edges[i + 1].u].edges;
    vertices[edges[i + 1].u].edges = &edges[i + 1]; //add edge to ajacency list
  }
//*****************************************************
//  validate that the input is received and stored well
// *****************************************************
/*printf("This is how we received all the numbers:\n");
  for (i=0; i<num_e ; i++)
  {
    printf("%d to %d is %2d miles\n", edges[i].u, edges[i].v, edges[i].length);
  }
*/

            //* TEST: fill in some dummy value for distance field
            for (i=0; i<num_v ; i++)
            {
              vertices[i].distance = 100 - i;
            }

            //* TEST:
            printf("This is how the array looks before the min-heap is built:\n");
            for (i=0; i<num_v ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");
            //*/

  // TEST (printf) turned on in this function
  build_min_heap(vertices, heapSize-1); 

  /* TEST: insert stuff and see how heap works
  vertex_struct temp;
  temp.distance = 15;
  heapSize++;
  //   (the array,lastleaf(not occupied yet),thing to insert)
  insert_min_heap (vertices, heapSize-1, temp);
            printf("This is the heap after inserting 15:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: insert stuff and see how heap works
  temp.distance = 999;
  heapSize++;
  //              (the array,last leaf,thing to insert)
  insert_min_heap (vertices, heapSize-1, temp);
            printf("This is the heap after inserting 999:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  */



      // source will always be 0 here
  dijkstra (vertices, 0, num_v);






  /* TEST: extracting
   ##################################################################
   ##################################################################
   ###################  huge test  ##################################
   ##################################################################
   ##################################################################

  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");

  //* TEST: extracting
  temp = extract_min_heap(vertices, &heapSize);
  printf("Top o' the heap has distance: %d\n", temp.distance);
            printf("This is the heap after extracting:\n");
            for (i=0; i<heapSize ; i++)
            {
              printf("%4d ", vertices[i].distance);
            }
            printf("\n");
##################################################################
##################################################################
##################################################################
###  End huge test            ####################################
##################################################################
##################################################################
##################################################################*/

  // watch out: heapSize will shrink
  // ---------  numSymbols with grow
  // ...but they start out equal.
  //printf("heapSize: %d\n", heapSize);
/*
  while (heapSize>=2)
  {
    symbol thing1 = extract_min_heap(s, &heapSize);
    //printf("AFTER EXTRACT heapSize: %d\n", heapSize);
    //printf("thing1.letter: %d\n", thing1.letter);
    //printf("thing1.freq: %d\n", thing1.freq);
    symbol thing2 = extract_min_heap(s, &heapSize);
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
    insert_min_heap(s, heapSize-1, new_thing);
*/

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
//  }
  
  /* If the given frequencies of letters were actually used, 
     print the number of bits needed to huffman-encode the whole thing. */
/*
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
*/
          /*
            symbol x = extract_min_heap(s, &numSymbols);
            printf("the min node has value: %d\n", x.freq);
            
            printf("numSymbols is now %d\n", numSymbols);
            printf("This is the heap after extracting:\n");
            //int i;
            for (i=0; i<numSymbols ; i++)
            {
              printf("%5ld ", s[i].freq);
              printf("%d\n", s[i].letter);
            }

            x = extract_min_heap(s, &numSymbols);
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
            insert_min_heap(s, numSymbols, woof);
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

	return 0;
}



