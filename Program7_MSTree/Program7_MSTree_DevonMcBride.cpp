/* Minimum Spanning Tree using Kruskal's algorithm

   Devon McBride ported this code from Intro. to Algorithms, 2nd Ed. 
   -------------                       by Thomas H. Cormen, et al.

   to run: > ./a.out < input

   Description: 
   Implement Kruskal's algorithm, to build the minimum spanning tree 
   for the points. Print out the length of the minimum spanning tree, 
   followed by the edges used to construct the tree.

   The input will consist of the # of vertices, the # of edges, and 
   then a series of edges. As an example, we might have a 5 vertex 
   graph, with 4 edges. I'll name the vertices A, B, C, D, E, for 
   explanation, but they'll actually be numbered 0 through 4. A 
   connects to B with a length of 10, B connects to C with length 3, 
   A connects to D with length 5, A connects to E with length 2. The 
   input file looks like this:

   5 4
   0 1 10
   1 2 3
   0 3 5
   0 4 2

   For this assignment, you won't need to use the abstract data 
   structure from the connected components program; it can be much 
   simpler. All you'll really need is an array of edges, and a 
   disjoint set data structure.

   Sort the edges based on cost, do the make-set for the vertices, 
   and then run through edges in order. This should be an easy program. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
using namespace std;

typedef struct vertex_struct
{
  int parent;
  int rank;
  int distance;
  // Edges that connect to the vertex.
  struct edge_struct *edges;
};

vertex_struct *vertices; // global array pointer here (very friendly)

typedef struct edge_struct
{
  int u;
  int v;
  int length;
  struct edge_struct *next;
};

edge_struct *edges; // global array pointer here (very friendly)

edge_struct *MSTree; // holds the edges of the MSTree for given graph

/********************************************************************/
/************************* methods **********************************/
/********************************************************************/

void heapify(edge_struct  s[], int semiRootIndex, int lastLeafIndex){
	int left    = 2*semiRootIndex + 1;  // left leaf
	int right   = 2*semiRootIndex + 2;	// right leaf
	int largest = semiRootIndex;        // index of largest value

	if(left <= lastLeafIndex && s[left].length > s[semiRootIndex].length)
		largest = left;
	if(right <= lastLeafIndex && s[right].length > s[largest].length)
		largest = right;
          /* if value of given index isn't larger than one of its 
             leaves, swap the larger-valued leaf with the given node. */
	if(largest != semiRootIndex)
  {
	    edge_struct  temp = s[largest];
	    s[largest] = s[semiRootIndex];
	    s[semiRootIndex] = temp;
          /* percolating down (largest is now the leaf index 
             and it's parent now actually has a larger value, 
             so the name 'largest' has become temporarily misleading.
             On the other hand, this leaf at index 'largest' is now
             the semi-heap's root index to be heapified.*/
		  heapify(s, largest, lastLeafIndex);
	}
}

           //make heap of given array
void build_heap(edge_struct e[], int lastIndex){ 
  int index;
  for ( index = lastIndex/2; index >= 0; index-- )
    heapify(e, index, lastIndex);
}

void heapSort(edge_struct  e[], int lastIndex)
{
  int index;
  edge_struct  temp;

  build_heap(e, lastIndex);

        /*  1 temporarily store biggest element (from root)   
            2 put last element (a small one) into root position
            3 put largest element into heap's last position
            heapify "i-1"-sized heap, leaving alone that last 
            position because it now contains the largese element */
  for ( index = lastIndex; index >= 1; index-- )
  {
    temp     = e[0];
    e[0]     = e[index];
    e[index] = temp;
    heapify(e, 0, index-1);
  }
}


// called this way:
//   make_set(    i);
void make_set(vertex_struct *verticies, int x){
  verticies[x].parent = x;
  verticies[x].rank = 0; // Tree tallness; dist. to bottom leaf.
               // Singleton's rank is 0; leaf's rank is 0. 
}

void link(vertex_struct *vertices, int x, int y){
  if (vertices[x].rank > vertices[y].rank) // y is worst-case height of tree
  {
    vertices[y].parent = x;
  }else{
    vertices[x].parent = y;
    if (vertices[x].rank == vertices[y].rank) // 'rank' is height below that vertex
    {
      vertices[y].rank = vertices[y].rank+1;
    }
  }
}

int findset(vertex_struct *vertices, int x){
  if (x != vertices[x].parent) // root points to itself
  {                            // recurse till root; return root 
                               // to all we've recursed through
    vertices[x].parent = findset(vertices, vertices[x].parent);
  }
  return vertices[x].parent;
}

    /* finds roots of both given nodes */
void Union(vertex_struct *vertices, int x, int y){
  link(vertices, findset(vertices, x), findset(vertices, y));
}

/*  CARLOS' LINK FUNCTION
void Link(vertex_struct x, vertex_struct y){ 
  v_list[x] = y;
}
*/

    /* ported from Carmen book, pg. 569 */
 void mst_kruskal(edge_struct edges[], int num_e, vertex_struct vertices[], int num_v)
{
  int i;
  edge_struct MSTree[num_e]; // madden said these were edges

  /* make each vertex be its own singleton tree */
  for (i=0; i<num_v; i++)
  {
    make_set(vertices, i);
  }

  // "nondescending order" does that mean it needs to be stable? no? 
  // nondescending is similar to ascending order. 
    /* sort the edges of Edges in nondescending order by length */
/*
  printf("These are the edges we're dealing with:\n");
  for (i=0; i<num_e ; i++)
  {
    printf("edge %d to %d is length %d \n", edges[i].u, edges[i].v, edges[i].length);
  }
  printf("\n");
*/

  build_heap(edges, num_e-1);

/*
  printf("This is the heap:\n");
  for (i=0; i<num_e ; i++)
  {
    printf("edge %d to %d is length %d \n", edges[i].u, edges[i].v, edges[i].length);
  }
  printf("\n");
*/
  heapSort(edges, num_e-1);
/*
  printf("This is how the edges array looks after sorting (weights showing here):\n");
  for (i=0; i<num_e ; i++)
  {
    printf("%5d ", edges[i].length);
  }
  printf("\n");
*/

  int x = 0;
  for (i=0; i<num_e ; i++)
  { 
    if (findset(vertices, edges[i].u) != findset(vertices, edges[i].v) )
    {
      MSTree[x] = edges[i];
      Union(vertices, edges[i].u, edges[i].v);
      x++;
    }
  }  

  // tally length of Minimum Spanning Tree
  int sum = 0;
  for (i=0; i<x ; i++)
  {
    sum += MSTree[i].length;
  }
  printf("%d\n", sum);

  // print all the edges of the minimum spanning tree
  for (i=0; i<x ; i++)
  {
    printf("%d %d\n", MSTree[i].u, MSTree[i].v);
  }
}

int main(int argc, char *argv[])
{
  int num_v, num_e;
  // vertices and edges were both declaired globally (see top)
  vertices = (vertex_struct *) malloc(sizeof(vertex_struct) * num_v);
  edges = (edge_struct *) malloc(sizeof(edge_struct) * num_e);

  // Grab 1st two numbers, make space to hold that many vertices and edges.
  scanf("%d %d", &num_v, &num_e);
  // Reserve space -- twice as many edges, because we're creating links for
  // both directions.

  int i;
  for (i = 0; i < num_v; ++i)
    {
      vertices[i].distance = -1; //??MAX_INT;
      vertices[i].edges = NULL;
    }

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
//  printf("This is how we received all the numbers:\n");
//  for (i=0; i<num_e ; i++)
//  {
//    printf("%d to %d is %d miles\n", edges[i].u, edges[i].v, edges[i].length);
//  }


  /***********************************************
   ***** I don't know what this does, ************
   ***** but if I don't have it,      ************
   ***** the program seg faults.      ************
   ***********************************************/
  for (i=0; i<num_v ; i++)
  { // I was merely testing, but now it's necessary. 
 //   printf("Vertex %d's adjacency list:\n", i); 
    edge_struct *current = vertices[i].edges;
 //   while (current != 0)
 //   {
 //     printf("Vertex %d is %d miles from %d\n", (*current).u, (*current).length, (*current).v); 
 //     current = current->next; // use -> when it's a pointer, for some wretchedly fussy reason
 //   }
  }
 // ***************************************************/


  mst_kruskal(edges, num_e, vertices, num_v);


}





