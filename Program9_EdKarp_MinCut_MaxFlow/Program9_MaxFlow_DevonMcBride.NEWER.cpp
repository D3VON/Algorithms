/* Edmunds-Karp Algorithm using extended heap functions (min-priority queue)
   ...to find maximum flow / minimum cut of a directed graph.

   Program9_MaxFlow_DevonMcBride.cpp

   By Devon McBride (ported from Cormen book, page 660).

   NOTE TO TEACHER: I spend such a giant amount of time trying to get C++ to work. 
   I spend less time thinking about any given algorithm.  I think using 
   a more friendly language would allow me to actually pay attention to the algorithm. 
   This is true for every assignment this semester.  

   I need a summer class in C++ for the sadly retarded. 

   TEXT FROM THE GIVEN ASSIGNMENT:
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
#include <stack>
#include <queue>
using namespace std;

typedef struct vertex
{
  int index; // a clean way to reference this vertex' index
  int capacity; // set when parent is set: the 
  int parent; // pointer to the shortest path to root
  int distance;
  int stepsToRoot;
  int color; // 0=white, 1=gray, 2=black  
  struct edge *edges; // Edges that connect to the vertex.
} vertex;


typedef struct edge
{
  int index;
  int u;
  int v;
  int capacity;
  struct edge *next; // Linked list for the edges
} edge;

// I'm so tired of getting confused with scope

edge *edges; // global array pointer here (very friendly)
vertex *vertices; // pointer to vertices[] array (malloc in main)
stack<int> pathV; //path represented as list of vertices
stack<int> pathE; //path represented as list of edges

/*
 12 13 
 0 1  5 
 0 2  6 
 0 3  7 
 1 2  8
 1 3  9
 1 4  2 
 2 3  3 
 5 7  4
 6 7  11
 6 9  12
 7 8  13
 8 9  14
 10 11 15

*/


/* MIASMA OF USING MY OWN LINKED LIST DATA STRUCTURE
void do_path( vertex vertices[], int s, int v ){
  vertex *temp;
  if (s == v)
  {
    printf("%d ", vertices[s].index);
    pathV = vertices[s]; // establish head of path
  }else{
    if (vertices[v].parent < 0)
    {
      printf("no path from %d to %d exists\n", s, v);
    }else{
      do_path( vertices, s, vertices[v].parent );
      printf("%d ", v);
      temp = &path;
      path = vertices[v];
      path.next = temp;
    }
  }
  printf("\n");
}

*/






void do_path( vertex vertices[], int s, int v ){
  edge *temp;
  if (s == v)
  {
    printf("vertex %d ", vertices[s].index); // source vertex
    pathV.push(v); // start of path
  }else{
    if (vertices[v].parent < 0)
    {
      printf("no path from %d to %d exists\n", s, v);
    }else{
      do_path( vertices, s, vertices[v].parent );
      //printf("vertex %d's parent path:", v);
      pathV.push(v);

      temp = vertices[ vertices[v].parent ].edges;
      while (temp != NULL)
      {
        if (temp->v == v)
        {
          pathE.push(temp->index);
          printf("%d,%d, has capacity %d\n", temp->u, temp->v, temp->capacity);
        }
        temp = temp->next;
      }
    }
  }
  printf("\n");
}









int BFS( int num_v, vertex vertices[], vertex s )
{
  //int vertexCount = 0; // count of vertices we discover
  vertex tempVert;
  edge  *tempEdge;

  while (   !pathE.empty() )   pathE.pop();
  while (   !pathV.empty() )   pathV.pop();

  // color root vertex gray (discovered, means added to queue) 
  vertices[s.edges->u].color = 1;
  //Signifies root doesn't have parent.
  vertices[s.edges->u].parent = -1; 

  queue<vertex> Q;
  Q.push(s);

  while (!Q.empty())
  {
    tempVert = Q.front(); // grab the vertex at the head of the queue
    Q.pop(); // dequeue vertex at the head of the queue

    //printf("vertex %d is connected to: ", tempVert.index);

    tempEdge = tempVert.edges; // 1st edge in tempVert's adjacency list.
                                     // Traverse the linked list.
    // TRAVERSE tempVert's ADJACENCY LIST:
    // Enqueue vertices connected tempVert by edges in tempEdge adjacency list. 
    // Mark all of them gray (discovered; gray indicates they're in the queue).
    // After traversing adjacency list, mark tempVert black: don't visit again
    // (all its connected vertices discovered/enqueued).
    while (tempEdge != NULL)
    {
      if (tempEdge->capacity > 0)
      {
        //printf("%d, ", tempEdge->capacity);
        //printf("%d, ", tempEdge->v);
        //if vertex connected to tempVert hasn't been visited...
        if (vertices[tempEdge->v].color == 0) //==white
        {
          vertices[tempEdge->v].color = 1; //= gray
          vertices[tempEdge->v].stepsToRoot = vertices[tempEdge->u].stepsToRoot+1;
          vertices[tempEdge->v].parent = tempEdge->u;

          Q.push(vertices[tempEdge->v]); // enqueue this discovered vertex
        }
        //printf("%d is what's stored in vertices[%d].color \n", vertices[temp->u].color, temp->u);
      }
      tempEdge = tempEdge->next; 
    }
    //printf("\n");
    //black means all its connected vertices are discovered (added to queue)
    vertices[tempVert.index].color = 2; // black
    //printf("vertex %d changed to black\n", tempVert.index);
    //vertexCount++;
  }

  do_path( vertices, 0, num_v-1);

  return !pathE.empty(); // 0 if there's a path, something else if there isn't.
}

void Edmunds_Karp(int num_v){
  while (BFS(num_v, vertices, vertices[0]))
  {
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    // find path forward capacity
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    stack<int> reverse;
    int least = 999999;
    int tempEdge;
    while (!pathE.empty())
    {
      tempEdge = pathE.top();
      reverse.push(tempEdge); // temporarily store order
      pathE.pop();

      //find least capacity of all edges in path
      if (least > edges[tempEdge].capacity)
      {
        least = edges[tempEdge].capacity;
      }
      printf("%d is the least capacity edge\n", least);
    }
    //repair pathE stack (yes, clumbsy, I know)
    while (!reverse.empty())
    {
      pathE.push( reverse.top() );
      reverse.pop();
    }
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    // done finding path forward capacity
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////



    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    // augmenting the path
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
    while (!pathE.empty())
    {
      tempEdge = pathE.top();
      reverse.push(tempEdge); // temporarily store order of path
      pathE.pop();

      edges[tempEdge  ].capacity -= least;
      printf("%d ", edges[tempEdge  ].capacity);

      edges[tempEdge+1].capacity += least;//backwards
      printf("%d \n", edges[tempEdge+1].capacity);
    }
    //repair pathE stack (yes, clumbsy, I know)
    while (!reverse.empty())
    {
      pathE.push( reverse.top() );
      reverse.pop();
    }
    /////////////////////////////////////////////////////////
    // done augmenting //////////////////////////////////////
    /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////
  }
}


/*****************************************************************************
 **MAIN MAIN MAIN MAIN *******************************************************
 *****************************************************************************/

int main(int argc, char *argv[])
{
  int num_v, num_e;

/*****************************************************************************
 Grab first two numbers and make space enough to hold that many vertices and
 edges. Initialize each of their distances to be -1 and their edges to be NULL. 
 *****************************************************************************/
  scanf("%d %d", &num_v, &num_e);
  // Make arrays
  vertices = (vertex *) malloc(sizeof(vertex) * num_v);
  // Twice as many edges, because we're creating/storing
  // pseudo-doubly-linked edges (u to v) and (v to u) for each edge entered 
  // into input.
  edges = (edge *) malloc(sizeof(edge) * num_e * 2);

  int i; // initialize all vertices' data members
  for (i = 0; i < num_v; ++i)
  {
    vertices[i].distance = -1;
    vertices[i].edges = NULL;
    vertices[i].stepsToRoot = 0;
    vertices[i].color = 0; // color all vertices white (untouched)
    vertices[i].index = i; 
    vertices[i].parent = -1; // initialize
  }

/*****************************************************************************
 Grab the rest of the input; each line is an edge: vertex, vertex, capacity
 ...but doubly link the vertices, for greater potential future usefulness. 
 *****************************************************************************/
  for (i =  0; i < num_e * 2; i+=2)
//for (i =  0; i < num_e    ; i++ )
  {
    // grab the edge (u to v) from input, put into edges[]
    scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].capacity);
    edges[i].index = i;

    // create 'backward' edge, too at the next index (always an odd number)
    edges[i + 1].u = edges[i].v;
    edges[i + 1].v = edges[i].u;
    edges[i + 1].capacity = 0; // initialize residual capacity to zero
    edges[i + 1].index = i + 1;

    //printf("%d is i\n", i);
    //printf("%d is edges[%d].u\n", edges[i].u, i);
    edges[i].next = vertices[edges[i].u].edges; // attach existing adj list to .next pointer
    //printf("%d is edge[%d]'s next\n", vertices[edges[i].u].edges, i);
    //printf("%d is vertices[%d]'s edges\n", vertices[edges[i].u].edges, edges[i].u);

    // "This is an edge that this vertex has."
    vertices[edges[i].u].edges = &edges[i]; // Clobber old head pointer
                                            // (already saved in this vertex as .next)
                                            // with new edge we just got from input.
                                            // This doesn't wipe out what we just
                                            // put into .next.

    //printf("%d is &edges[%d]\n", &edges[i], i);
    //printf("%d is vertices[edges[%d].u].edges\n", &edges[i], i);
  } 
 /*******************************************
  *******************************************
  ******* TEST THE ADJACENCY LIST ***********
  ******* OF EACH VERTEX          ***********
  *******************************************
  *******************************************
  edge *tempE;
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
  */

  /* TEST BFS works with count returned (have to modify BFS to test)
                        // BFS returns count of vertices
                        // BFS hardcoded to start at vertex 0
  int num_vertices = BFS(num_v, vertices, vertices[0]);
  printf("the count is %d \n", num_vertices);
  */

  // must use num_v, not num_vertices b.c. directed path could easily 
  // have fewer connected components than there are components!
  //do_path( vertices, 0, num_v-1);

  //BFS(num_v, vertices, vertices[0]);
  //do_path( vertices, 0, num_v-1);

  //*TEST: path queue works 
  // (after 3 hours mucking around with my own linked list that never worked).

  Edmunds_Karp(num_v);


  printf("printing path list in main: \n");
  int tempVert;
  while (!pathV.empty())
  {
    tempVert = pathV.top(); 
    pathV.pop();
    printf("%d, ", vertices[tempVert].index); 
  }
  printf("\n");
  //*/

}

