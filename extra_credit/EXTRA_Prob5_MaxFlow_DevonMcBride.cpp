/* Edmunds-Karp Algorithm using extended heap functions (min-priority queue)
 ...to find maximum flow / minimum cut of a directed graph.

 Program9_minCost_DevonMcBride.cpp

 By Devon McBride (ported from Cormen book, page 660).

 (see full description at end)
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

typedef struct vertex {
	int index; // a clean way to reference this vertex' index
	//int capacity; // set when parent is set: the
	int parent; // pointer to the shortest path to root
	int distance;
	int stepsToRoot;
	int color; // 0=white, 1=gray, 2=black
	struct edge *edges; // Edges that connect to the vertex.
} vertex;

typedef struct edge {
	int index;
  int inPath; // 0 = not marked as "in the path"; 1 = marked as "in the path"
	int u;
	int v;
	int capacity;
	struct edge *next; // Linked list for the edges
} edge;

edge *E; // global array pointer here (very friendly)
vertex *V; // pointer to V[] array (malloc in main)
vector<int> vpath; //path represented as indexes to global vertices array V)
vector<int> epath; //path represented as indexes to global edges array E)
vector<int>::iterator it;  



/*###########################################################################
  ###########################################################################
  ###########################################################################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################             FUNCTIONS              ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###########################################################################
  ###########################################################################
  ###########################################################################
*/

// DISCOVER PATH:
// recursively following parent variable, starting at the last vertex
// until recursion hits root index (that's the base case)
bool do_path(vertex V[], int s, int v, int sink) 
{
  // vectors to hold path as:  indexes to vertices in global V array, 
  //                       and indexes to edges in global E array.
  vpath.clear();
  epath.clear();
  bool pathOK = false; // return true of path is complete

  edge *tempE;
	if (s == v) {
		//printf("Path from source: %d\n", V[s].index); // source vertex
    vpath.push_back(v); // used if we want to print path as vertices
    pathOK = true;
	} 
  else 
  {
		if (V[v].parent < 0) 
    {
			//printf("no path from %d to %d exists\n", s, v);
		} 
    else 
    {
			do_path(V, s, V[v].parent, sink);
			//printf("                    -> %d  ", v);
      vpath.push_back(v); // store this vertex index into vpath list
                          // (useful if we need to print it out)


      // find V[v].parent's edge that connects to v,
			tempE = V[V[v].parent].edges;

      // Traverse V[v]'s adjacency list to find the edge in the path, 
      // then get it's capacity 
			while (tempE != NULL) 
      {

        /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
           the second test is for when several edges connect the same two 
           vertices: pick the edge that BFS identified as in the path.
          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				if (tempE->v == v && tempE->inPath == 1) 
        {
          // store that edge's global array index (index in E[])
          // if we need to print out the path ever...
          epath.push_back(tempE->index); 
					//printf("(%d-%d, has capacity %d)\n", tempE->u, tempE->v, tempE->capacity);
				}
				tempE = tempE->next;

        // Is path finished (has it reached the sink)?
        pathOK = (v == sink)? true : false;
			}
		}
	}
  /* doesn't work nicely because of the RECURSIVE situation
  if (pathOK)
  {
    printf("____________________________done with building path\n");
  }
  else
  {
    printf("____________________________that path was incomplete\n");
  }
  */
  
  return pathOK;
}













//while ( BFS(num_v, sink, num_e, V, V[source]) )

int BFS(int num_v, int sink, int num_e, vertex V[], vertex s) 
{
  //printf("____________________________EDMONDS-KARP DOING BFS-WHILE NOW:\n");
	//int vertexCount = 0; // count of vertices we discover
	vertex tempV;
	edge *tempE;

	int i; // reset vertices' data members to be altered during BFS
	for (i = 0; i < num_v; ++i) {
		V[i].distance = -1;
		V[i].stepsToRoot = 0;
		V[i].color = 0; // white = 'not visited'
    V[i].parent = -1; // initialize
	}
  for (i=0; i < num_e *2; i+=2)
  {
    E[i].inPath = 0;  // initialize/mark all as not "in the path"
  }

	V[s.edges->u].color = 1;// color root 'discovered'
	V[s.edges->u].parent = -1;// root doesn't have parent.

	queue<vertex> Q;
	Q.push(s);

	while (!Q.empty()) 
  {
		tempV = Q.front(); // grab the vertex at the head of the queue
		Q.pop(); // dequeue vertex at the head of the queue

    //printf("--->searching %d's adjacency list\n", tempV.index);
		tempE = tempV.edges; // 1st edge in tempV's adjacency list.
		// Traverse the linked list.
		// TRAVERSE tempV's ADJACENCY LIST:
		// Enqueue vertices connected tempV by edges in tempE adjacency list.
		// Mark all of them gray (discovered; gray indicates they're in the queue).
		// After traversing adjacency list, mark tempV black: don't visit again
		// (all its connected vertices discovered/enqueued).
    //printf("skip edges if capacity is zero.\n");
		while (tempE != NULL) 
    {
      //  if  forward capacity            ... && never been visited
			if ( tempE->capacity > 0 && V[tempE->v].color < 2 ) 
      {           
          //printf("edge: %d - %d, capacity %d\n", tempV.index, tempE->v, tempE->capacity);
					V[tempE->v].stepsToRoot = V[tempE->u].stepsToRoot + 1;

          /*/ if already has parent, keep parent with shortest dist. to root.
          if (V[V[tempE->v].parent].stepsToRoot > V[tempE->u].stepsToRoot )
          {
            V[tempE->v].parent = tempE->u;
          }
          */

          V[tempE->v].parent = tempE->u;


          E[tempE->index].inPath = 1;// mark edge as being "in the path"
          /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
           * the path that reaches last vertex FIRST will prevent 
           * any other vertex from becoming the last vertex' parent. 
           *^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
         //###########################################
         //###########################################
         //###########################################
         //###########################################
         //         1  ==   1
          if (sink == tempE->v)
          {
            //printf("___________________Sink found; doing do_path() to capture the path \n");
            //printf("___________________signified by back-tracing from sink's .parent \n");
            return do_path(V, 0, sink, sink); 
          }

          // check to see if already in queue, if yes, don't enqueue again
          if (V[tempE->v].color == 0)
          {
            Q.push(V[tempE->v]); // enqueue this discovered vertex
            //printf("vertex %d was pushed to Q \n", tempE->v);
          }
          V[tempE->v].color = 1; // mark visited
			}
			tempE = tempE->next;

		}
		//printf("---------Done with that adjacency list.\n");
		//black means all its connected vertices are discovered (added to queue)
		V[tempV.index].color = 2; // black
		//printf("vertex %d changed to black\n", tempV.index);
		//vertexCount++;
	}


  // I'm not sure if it ever gets this far........
  //printf("____________________________BFS DOING do_path (end of BFS):\n");
  // build path, if possible
  return do_path(V, s.index, sink, sink); 



}














bool Edmonds_Karp(int num_v, int num_e, int source, int sink) 
{
  if (V[source].edges != NULL)
  {
    //printf("Vertex zero: %d\n", V[0].index);
    while ( BFS(num_v, sink, num_e, V, V[source]) ) 
    {
      //#######################################################################
      //# find path forward capacity (the bottleneck edge in the path)
      //#######################################################################
      int least = 999999;

      //printf("finished building path, finding bottleneck\n");
      // check all edges BFS captured as part of the path
      // to detect which has the least forward capacity
      //printf("size of epath is %d\n", epath.size());
      for(int i=0;i < epath.size(); i++)
      {        
        //printf("%d is capacity of E[%d]\n", least, epath[i]);
        //find least capacity of all edges in path
        if (least > E[epath[i]].capacity) 
        {
          least = E[epath[i]].capacity;
        }
      }
      //printf("\n%d is the capacity of the least capacity edge in the path\n", least);


      //#######################################################################
      //# augment the path (adjust forward and backward with bottleneck amount)
      //#######################################################################
      for(int i=0;i < epath.size(); i++)
      {
        //printf("DEALING WITH PATH EDGE %d-%d\n", E[epath[i]].u, E[epath[i]].v);
        //printf("augmenting path: %d-%d, was %d, is now ", E[epath[i]].u, E[epath[i]].v, E[epath[i]].capacity);
        E[epath[i]    ].capacity -= least;
              //printf("%d\n", E[epath[i]].capacity);

              //printf("backward path: %d-%d, was %d, is now ", E[epath[i] + 1].u, E[epath[i] + 1].v, E[epath[i] + 1].capacity);
        E[epath[i] + 1].capacity += least;//backwards
              //printf("%d\n", E[epath[i] + 1].capacity);
      }
    }
    return true;
  }else{
    return false;
  }
}









int min_cost_path(edge *E, int num_e, int sink)
{
  int minCost = 0;
  int hops = 0;
  for(int i = 1; i < num_e*2; i+=2)
  {
    // Odd indexes in E[] represent reverse flow
    // and u & v are defined reverse-wise because the edge represents
    // reverse flow!  That wasted me some time to detect its behavior here.
    if(E[i].u == sink)
    {
      minCost += E[i].capacity;
    }
  }

  //*
  for(int i = 1; i < num_e*2; i+=2)
  {
    // Odd indexes in E[] represent reverse flow
    printf("%d - %d capacity is %d\n", E[i].u, E[i].v, E[i].capacity);
  }
  //*/

  //printf("max flow is %d\n", minCost);
  return minCost;
}


/*###########################################################################
  ###########################################################################
  ###########################################################################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################                MAIN                ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###########################################################################
  ###########################################################################
  ###########################################################################
*/
int main(int argc, char *argv[]) {
	int num_v, num_e, zombies, source, sink;

	/*****************************************************************************
	 Grab first two numbers and make space enough to hold that many vertices and
	 edges. Initialize each of their distances to be -1 and their edges to be NULL.
	 *****************************************************************************/
	scanf("%d %d %d", &num_v, &num_e, &zombies);
	// Make arrays
	V = (vertex *) malloc(sizeof(vertex) * num_v);
	// Twice as many edges, because we're creating/storing
	// pseudo-doubly-linked edges (u to v) and (v to u) for each edge entered
	// into input.
	E = (edge *) malloc(sizeof(edge) * num_e * 2);


  // MAKE ALL VERTICES (no need to wait for edge information)
	int i; // initialize all vertices' data members
	for (i = 0; i < num_v; ++i) {
		V[i].edges       = NULL;
		V[i].index       = i;
	}

  //RECIEVE EDGE INFO. 
  // Each line is: vertex, vertex, capacity
  // --create also a reverse-flow edge for each. 
	for (i = 0; i < num_e * 2; i += 2)
	{
    // Receive each edge (u to v) from input, put into E[]
    scanf("%d %d %d", &E[i].u, &E[i].v, &E[i].capacity);





    E[i].index = i;
    // Create 'backward' edge and put at the next index (always an odd number)
    // This edge is from v to u instead of the received u to v.
                  E[i + 1].u = E[i].v;
                  E[i + 1].v = E[i].u;
                  E[i + 1].capacity = E[i].capacity; // initialize residual capacity= 0; // initialize residual capacity to zero
                                  
                  E[i + 1].index = i + 1;

    // ADD THE EDGE TO THE APPROPRIATE VERTEX'S ADJACENCY LIST
    // 2-STEP PROCESS:
    //
    // STEP 1:
    // preserve old adjacency list if it exists, 
    // by storing it in .next of the edge we just received (and just created)
    E[i  ].next = V[E[i  ].u].edges; // attach existing adj list to .next pointer
    E[i+1].next = V[E[i+1].u].edges; // attach existing adj list to .next pointer
                    //printf("%d is edge[%d]'s next\n", V[E[i].u].edges, i);
                    //printf("%d is V[%d]'s edges\n", V[E[i].u].edges, E[i].u);

    // STEP 2:
    // Whatever the edge's .u is,
    // make that vertex's .edge point to the new edge (set up in Step 1). 
    V[E[i  ].u].edges = &E[i  ];  
    V[E[i+1].u].edges = &E[i+1]; 

    // NOTE: we will rely on .capacity to determine whether BFS can use 
    //       an edge in the path.  
	}

  scanf("%d %d", &source, &sink);
  /*TESTING:
  printf("SOURCE: %d, SINK: %d\n", source, sink);
  */
 
  /*TESTING:
  printf("____________________________THIS IS HOW EDGES WERE INITIALIZED IN main():\n");
  for (int g = 0; g < num_e*2; g++)
  {
    printf("index %d: %d - %d has capacity %d\n", g, E[g].u, E[g].v, E[g].capacity);
  }
  */

  /*TESTING:
  printf("____________________________HERE IS EACH VERTEX'S ADJACENCY LIST main():\n");
  edge *currentE;
  for (int g = 0; g < num_v; g++)
  {
		currentE = V[g].edges; // 1st edge in tempV's adjacency list.
		while (currentE != NULL) 
    {
      printf("vertex %d has edge %d-%d (capacity %d)\n", g, currentE->u, 
                                                            currentE->v, 
                                                            currentE->capacity);
      currentE = currentE->next;
    }
  }
  */


	if ( Edmonds_Karp(num_v, num_e, source, sink) )
  {
    int minCost = min_cost_path(E, num_e, sink);
    printf("%d\n", minCost);
  }else{
    printf("%d\n", 0);
  }





}




/*###########################################################################
  ###########################################################################
  ###########################################################################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################         end of the program         ####################
  ###################                                    ####################
  ###################                                    ####################
  ###################                                    ####################
  ###########################################################################
  ###########################################################################
  ###########################################################################
*/



/*
In this assignment, you will implement the Edmonds-Karp maximum flow 
algorithm, to compute the maximum flow between vertex 0, and the highest 
numbered vertex in the graph.

The input will be the same format as with the prior graph assignments, 
with the edge length meaning the edge capacity, AND THE GRAPH IS DIRECTED. 
If there is an edge from A to B, then it only goes from A to B, and not back.

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

Here are some simple examples:

2 1
0 1 100

This first example is simply an edge from vertex 0 to vertex 1, 
with a capacity of 100. Your program should say "100."

3 2
0 1 10
1 2 5

The second example has a connection from vertex 0 to 1 with a capacity 
of 10, and then from 1 to 2 with a capacity of 5. The flow you can 
get is 5, so print that.

2 2
0 1 5
0 1 6

Two edges from 0 to 1, with a total capacity of 11

2 2
1 0 10
1 0 3

Two edges, but they go from 1 to 0, so no flow, print "0"

8 9
0 1 1
1 2 1
2 3 1
0 4 1
4 3 1
4 5 1
5 6 1
6 7 1
3 7 1

If you use BFS (and you should), the first path you find should be 
0->4->3->7. This will cause the 4->3 edge to have capacity in the 
reverse direction. The second path should be 0->1->2->3->4->5->6->7, 
for a total capacity of 2.


*/


