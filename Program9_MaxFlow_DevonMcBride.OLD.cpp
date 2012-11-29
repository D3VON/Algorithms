/* Edmunds-Karp Algorithm using extended heap functions (min-priority queue)
   ...to find maximum flow / minimum cut of a directed graph.

   Program9_MaxFlow_DevonMcBride.cpp

   By Devon McBride (ported from Cormen book, page 660).

   In this assignment, you will implement the Edmonds-Karp maximum flow 
   algorithm, to compute the maximum flow between vertex 0, and the highest 
   numbered vertex in the graph.

   The input will be the same format as with the prior graph assignments, 
   with the edge capacity meaning the edge capacity, AND THE GRAPH IS 
   DIRECTED. If there is an edge from A to B, then it only goes from A to B, 
   and not back.

   You will probably want to re-use the data structures from the connected 
   components assignment. When you create a pair of edges (one for each 
   direction), you can link them together, and use the "backwards" edge 
   to hold residual capacity.

   The algorithm is fairly simple -- use BFS to find a path from 0 to the 
   highest vertex. Back-trace the path to find the capacity of the path, 
   then trace it again to remove forward capacity, and put it on to the 
   reverse direction. Keep doing this until you have the flow....

   Print out the flow of the graph when you're done (don't print anything 
   else out, please!).


   The input will consist of the # of vertices, the # of edges, and then a 
   series of edges. As an example, we might have a 5 vertex graph, with 4 
   edges. I'll name the vertices A, B, C, D, E, for explanation, but they'll 
   actually be numbered 0 through 4. A connects to B with a capacity of 10, 
   B connects to C with capacity 3, A connects to D with capacity 5, 
   A connects to E with capacity 2. The input file looks like this:

   v e
   5 4
   0 1 10
   1 2 3
   0 3 5
   0 4 2
                               
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <queue>
// wow, this really screwed up my algorighm!
//#include <limits.h> // to use INT_MAX 

/*****************************************************************************/
/*** Structures **************************************************************/
/*****************************************************************************/

typedef struct vertex_struct
{
  int parent; // pointer to the shortest path to root
  int index;
  int stepsToRoot;
  int color;
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
  int capacity;
  int back;
  int forward;
  struct edge_struct *next;
};

edge_struct *edges; // global array pointer here (very friendly)

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

	if(left <= lastLeafIndex && v[left].capacity < v[semiRootIndex].capacity)
		smallest = left;
	if(right <= lastLeafIndex && v[right].capacity < v[smallest].capacity)
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
              printf("%5ld ", v[i].stepsToRoot);
            }
            printf("\n");
            //*/
}

// WARNING: HAVE TO MODIFY YOUR heapSize (OR WHATEVER IT'S NAMED) IN main
//          BECAUSE THIS FUNCTION DOESN'T TAKE CARE OF MODIFYING IT.
//                  (the array, new last leaf,    thing to insert)
void insert_min_heap(vertex_struct A[], int i, vertex_struct temp)
{
  A[i] = temp; // put this new node at last leaf

// percolate that node up if it needs to
//while(under root && parent val > new_one val)
  while ( i>0 && A[(i-1)/2].capacity > A[i].capacity)
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
vertex_struct extract_min_heap(vertex_struct A[], int* heapSize)
{
  if ((*heapSize)-1 < 0)
    printf("Hey!  Heap underflow!\n");

  vertex_struct minimum = A[0];
  A[0] = A[(*heapSize)-1];
  (*heapSize)--; /* cab10886: "you need to dereference the pointer before 
                     modifying it otherwise you'll be modifying the pointer" */
  min_heapify(A, 0, (*heapSize)); 
  return minimum;
}









/* Original "connected components" assignment was merely to:
   "...print out the total number of connected components
   in the input file (just a single number to print)."
*/

int BFS( int num_v, vertex_struct vertices[], vertex_struct s )
{
  int vertexCount = 0; // count of vertices we discover
  vertex_struct tempVert;
  edge_struct *tempEdge;
  int heapSize = 0;
  //printf("root vertex is %d\n", s.edges->u);

  queue = (vertex_struct *) malloc(sizeof(vertex_struct) * num_v);

  // color root vertex gray (discovered, means added to queue) 
  vertices[s.edges->u].color = 1; 
  vertices[s.edges->u].parent = -1; //Signifies root doesn't have parent.
  //            (the array, new last index, thing to insert)
  insert_min_heap(vertices, heapSize, s); //add 1st thing to queue
  heapSize++; // function doesn't change this variable, so do it manually

  while (heapSize > 0)
  {

    /* this function modifies the heapSize variable globally */
    tempVert = extract_min_heap(vertices, &heapSize);  
    //TEST
    //if (tempVert.color != 2)
    //{
    //  printf("vertex %d is connected to: ", tempVert.index);
    //}
    tempEdge = tempVert.edges; // Get 1st edge in tempVert's adjacency list
                               // & traverse the linked list.
    // TRAVERSE tempVert's ADJACENCY LIST:
    // Enqueue vertices connected tempVert by edges in tempEdge adjacency list. 
    // Mark all of them gray (discovered; gray indicates they're in the queue).
    // After traversing adjacency list, mark tempVert black: don't visit again
    // (all its connected vertices discovered/enqueued).
    while (tempEdge != NULL)
    {
    //  printf("%d, ", tempEdge->v);
      //if vertex connected to tempVert hasn't been visited...
      if (vertices[tempEdge->v].color == 0) //==white
      {                              // This gray business is superfluous.
        vertices[tempEdge->v].color = 1; //= gray = added to queue
        vertices[tempEdge->v].stepsToRoot = vertices[tempEdge->u].stepsToRoot+1;
        vertices[tempEdge->v].parent = tempEdge->u;

        // WARNING: HAVE TO MODIFY YOUR heapSize (OR WHATEVER IT'S NAMED) IN main
        //          BECAUSE THIS FUNCTION DOESN'T TAKE CARE OF MODIFYING IT.        
        //            (the array, last leaf, thing to insert)
        insert_min_heap(vertices, heapSize, vertices[tempEdge->v]);
        heapSize++;
      }
      vertices[tempEdge->u].color = 2; //=black (all adjacent vertices found)
      vertexCount++;
      tempEdge = tempEdge->next;

    } 
    //if (heapSize==0)
    //{
    //  printf("\n");
    //}
  }
  
    printf("there are %d vertices\n", num_v);
    int x;
    for (x=0; x<num_v; x++)
    {
      printf("%d's parent is %d\n", x, vertices[x].parent);
    }
  return vertexCount;
}


/*###########################################################################
  ###########################################################################
  ##################                                  #######################
  ##################              MAIN                #######################
  ##################                                  #######################
  ###########################################################################
  ###########################################################################*/

int main(int argc, char *argv[])
{
  int num_v, num_e;

  // Grab 1st two numbers, make space to hold that many vertices and edges.
  scanf("%d %d", &num_v, &num_e);

  // Reserve space -- twice as many edges, because we're creating links for
  // both directions.
  // (vertices and edges were both declaired globally (see top) )
  vertices = (vertex_struct *) malloc(sizeof(vertex_struct) * num_v);
  edges = (edge_struct *) malloc(sizeof(edge_struct) * num_e*2);

  int lastIndex = num_v - 1;
  int i;

  num_e = num_e*2; // capturing reverse direction of edges doubles num of edges
  for (i =  0; i < num_e; i += 2)
  {
    //1. grab a line from input, put it into edges array (at next arbitrary index)
    scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].capacity);
    //TEST: printf("%d - %d is %d long\n", edges[i].u, edges[i].v, edges[i].capacity);

    //2. create 'backward' edge, too at the next index (always an odd number)
    edges[i + 1].u = edges[i].v;
    edges[i + 1].v = edges[i].u;
    edges[i + 1].capacity = 0; // initialize residual capacity to zero

    // add the grabbed edge to the vertex's adjacency list. 
    edges[i].next = vertices[edges[i].u].edges; // attach existing adj list to .next pointer
    vertices[edges[i].u].index = edges[i].u; // a handy way to know index in vertices[]
    vertices[edges[i].u].stepsToRoot = 0; // initialize 
    vertices[edges[i].u].edges = &edges[i]; // Clobber old head pointer
                                            // (already saved in this vertex as .next)
                                            // with new edge we just got from input.
                                            // This doesn't wipe out what we just
                                            // put into .next.
    vertices[edges[i].u].color = 0; // color the newly received vertex white (untouched)
    vertices[edges[i].u].parent = -1; // initialize

  }

  
  edge_struct *tempE;
  int x;
  for (x=0; x<num_v; x++)
  {
    printf("vertex %d's adjacency list: ", x);
    tempE = vertices[x].edges;
    while (tempE != NULL)
    {
      printf("%d ", tempE->v);
      tempE = tempE->next;
    }
    printf("\n");
  }











                        // BFS returns count of vertices
                        // BFS hardcoded to start at vertex 0
  printf("%d is the count\n", BFS(num_v, vertices, vertices[0]));

/*
  for (m=0; m<num_v; m++)
  {
    printf("vertices[%d]'s color is %d\n", m, vertices[m].color);
  }
*/

/* more testing
  for (m=0; m < num_v; m++)
  {
    printf("vertices[%d].color is %d\n", m, vertices[m].color);
  }
*/

/*
  // Print out the graph
  printf("%d vertices %d edges\n", num_v, num_e);
  for (i = 0; i < num_v; ++i)
  {
    printf("Vertex %d:\n", i);
    edge *tmp = vertices[i].edges;
    while (tmp != NULL)
    {
      printf(" connect %d %d, capacity %d\n",
       tmp->u, tmp->v, tmp->capacity);
      tmp = tmp->next;
    }
  }
*/

  printf("\n");

	return 0;
}



