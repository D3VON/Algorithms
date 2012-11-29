/* extra_Prov5_Dijkstras_DevonMcBride.cpp

   By Devon McBride
    
   Problem 5, extra credit (See text at end for description). 

   I solved this problem using:
   1) Dijkstra's algorithm;
   2) NICE TRICK: each vertex keeps track of how many hops it took to get its 
      distance value.  If a new edge wants to update the distance, but the
      hops to do it would exceed maxHops, it can't update the vertex. 
   3) STL priority_queue to act like a heap.
   4) The STL heap was cumbersome to use because my vertexes are bloated
      with values, so I have to keep reloading the heap.  This is because
      the values to the vertices are only updated in the original array. 
   5) INT_MAX also posed some trickiness, so I have to be careful an operation
      doesn't cause a massively negative value. 
 */

#include <stdio.h>
#include <malloc.h>
#include <limits.h> // to use INT_MAX for sentinal value
#include <vector>
#include <queue>
using namespace std;

/*****************************************************************************/
/*** Structures **************************************************************/
/*****************************************************************************/

typedef struct vertex
{
  int parent; // pointer to the shortest path to root
  int index;
  int removed;
  int hops;
  int distance;
  // Edges that connect to the vertex.
  struct edge *edges;
};


typedef struct edge
{
  int u;
  int v;
  int length;
  int index;
  struct edge *next;
};

vertex *V; // global array pointer here (very friendly)
edge *E; // global array pointer here (very friendly)
edge *edges; // global array pointer here (very friendly)
edge *MSTree; // holds the edges of the MSTree for given graph

//Overload operator< to determine priority:
bool operator<(const vertex &d1, const vertex &d2)
{
    return d1.distance < d2.distance;
}

//Overload operator> to determine priority:
bool operator>(const vertex &d1, const vertex &d2)
{
    return d1.distance > d2.distance;
}

// Since my heap was so buggy, I am using STL for reliability. 
                                                                   /*
###################################################################
## relax()                                                       ##
###################################################################*/
void relax (edge e, int maxHops)
{
  int u = e.u;
  int v = e.v;
  int length = e.length;
  /*######################################################################
    # If both: (v's distance) > (u's distance + length from u to v)
    #           AND (source's hop count + 1) not > maxHops
    ######################################################################*/
  if (V[u].distance + length > 0)// guard against becoming massively negative
  {                              // by adding a little to INT_MAX
    if ( V[v].distance > (V[u].distance + length) 
         && V[u].hops + 1 < maxHops)
    {
      V[v].distance = V[u].distance + e.length;
      V[v].hops     = V[u].hops + 1;
      V[v].parent   = V[u].index; 
    }
  }
}

                                                                           /*
#############################################################################
# Compute the distances from vertex 0 to every other vertex. If there are 
# less than 10 vertices, print the distance to each. If there are 10 or more 
# vertices, print the distance to the furthest vertex (and print which 
#############################################################################
#    dijkstra()                                                       
#############################################################################*/
void dijkstra (int source, int num_verts, int maxHops)
{
  vertex nextContestant;
  int cost = 0;
  //             type    container       compare           name
  priority_queue< vertex, vector<vertex>, greater<vertex> > min_queue;

  // initialize and put vertices into min_queue
  int i;
  for (i = 0; i < num_verts; ++i)
  {
    if (i != source)
    {
      V[i].distance = INT_MAX; // distance 'estimate' for relax situation. 
      V[i].hops = INT_MAX; // # hops maxed out to start
      min_queue.push(V[i]);
      //printf("V[%d] has distance %d\n", i, V[i].distance);
    }
    else
    {
      // initialize root's attributes
      V[source].distance = 0; // zero distance from itself
      V[source].hops     = 0; // no hops from self to self
      min_queue.push(V[source]);
      //printf("V[%d] has distance %d\n", source, V[source].distance);
    }
  }


  //initialize an 'empty set' "of vertices whose final shortest-path weights
  //from the source have already been determined." Cormen book pg. 595
  //shortest = (vertex *) malloc(sizeof(vertex) * num_verts);

                                                                       /*
   ######################################################################
   # Done initializing. 
   # First one extracted from heap is SOURCE vertex; it was initialized 
   # with distance 0, and all others were initialized with distance 'huge'. 
   # Next, source's adjacency list's distances-from-source are updated, 
   # then the lowest distance is popped from min_queue, and 
   # distances-from-source are updated again, and so on. 
   ######################################################################*/

  //int shortSize = 0;    
  while (!min_queue.empty())
  {   
    nextContestant = min_queue.top();
    min_queue.pop();
    V[nextContestant.index].removed = 1;
    //printf("POPPED %d  \n", nextContestant.index);
  
    // TRAVERSE THE POPPED VERTEX'S ADJACENCY & RELAX ADJACENT VERTICES
    //edge *current = shortest[shortSize].edges;
    edge *current = nextContestant.edges;
    while (current != NULL)
    {
      relax((*current), maxHops); // if adjacent exceeds masHops, skip it.
      //printf("Vertex %d is %d miles from %d\n", (*current).u, (*current).length, (*current).v); 
      current = current->next; // use -> when it's a pointer, for some wretchedly fussy reason
    }

    //Using STL priority_queue forces me to reload to accomplish reheapifying
    // god it sucks using priority_queue! min_queue.clear();
    int z = 0;
    while ( !min_queue.empty() ) 
    { 
      //printf("V[%d] has distance %d\n", min_queue.top().index, V[min_queue.top().index].distance);
      min_queue.pop(); 
    }
    for (int x=0; x<num_verts; x++)
    {
      if (V[x].removed == 0)// reheapify without vertexes already "removed"
      {
        min_queue.push(V[x]);
      }
    }
  }
}


/*###########################################################################
  ###########################################################################
  ##################                                  #######################
  ##################              MAIN                #######################
  ##################                                  #######################
  ###########################################################################
  ###########################################################################*/

int main(int argc, char *argv[]) {
	int num_v, num_e, maxHops, source, sink, cost;

	/*****************************************************************************
	 Grab first two numbers and make space enough to hold that many vertices and
	 edges. Initialize each of their distances to be -1 and their edges to be NULL.
	 *****************************************************************************/
	scanf("%d %d %d", &num_v, &num_e, &maxHops);
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
    V[i].removed     = 0;
	}

  //RECIEVE EDGE INFO. 
  // Each line is: vertex, vertex, length
  // --create also a reverse-flow edge for each. 
	for (i = 0; i < num_e * 2; i += 2)
	{
    // Receive each edge (u to v) from input, put into E[]
    scanf("%d %d %d", &E[i].u, &E[i].v, &E[i].length);

    E[i].index = i;
    // Create 'backward' edge and put at the next index (always an odd number)
    // This edge is from v to u instead of the received u to v.
                  E[i + 1].u = E[i].v;
                  E[i + 1].v = E[i].u;
                  E[i + 1].length = E[i].length; // initialize residual length= 0; // initialize residual length to zero
                                  
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

    // NOTE: we will rely on .length to determine whether BFS can use 
    //       an edge in the path.  
	}

  scanf("%d %d", &source, &sink);
  //TESTING:
  //printf("SOURCE: %d, SINK: %d\n", source, sink);
  
 
  /*/TESTING:
  printf("____________________________THIS IS HOW EDGES WERE INITIALIZED IN main():\n");
  for (int g = 0; g < num_e*2; g++)
  {
    printf("index %d: %d - %d has length %d\n", g, E[g].u, E[g].v, E[g].length);
  }
  */

  /*/TESTING:
  printf("____________________________HERE IS EACH VERTEX'S ADJACENCY LIST main():\n");
  edge *currentE;
  for (int g = 0; g < num_v; g++)
  {
		currentE = V[g].edges; // 1st edge in tempV's adjacency list.
		while (currentE != NULL) 
    {
      printf("vertex %d has edge %d-%d (length %d)\n", g, currentE->u, 
                                                            currentE->v, 
                                                            currentE->length);
      currentE = currentE->next;
    }
  }
  */






  dijkstra (source, num_v, maxHops);
  
  if (V[sink].distance < INT_MAX)
  {
    printf("%d\n", V[sink].distance);
  }
  else
  {
    printf("Impossible.\n");
  }




}





/* TEXT OF THE PROBLEM:

Problem 5 - Zombie Crossing
The King of England has hired you as a consultant for his travel problems. 
His kingdom consists of cities labeled 0 to N-1, where N is the number of 
cities, and roads which connect two cities each. Due to taxes that he must 
pay to the King of France (for some reason, France rules England), each 
road has a toll that one must pay for travelling on it (all tolls are 
positive integers). The King would like to write a program that can help 
his citizens figuring out the path between two cities which will incur 
the minimal cost in tolls. You get a huge grin on your face and start 
to explain to your king hat this is a simple problem and you can solve 
it easily, but he then tells you one additional fact. The roads in his 
kingdoms are infested with zombies that can only be fought off with 
royal hand grenades. If a citizen has a royal hand grenade, he will 
be able to go across the road safely, but only that road. That is 
one grenade will only protect the citizen for one road. The King 
explains that he would like to be able to find the minimal cost path 
from city A to city B, subject to the fact that the citizen only has 
K grenades and thus can only travel on at most K roads.


Input:

The first line will contain three numbers N E K where N ( 2 <= N <= 1000) 
is the number of cities, E is the number of roads, and K ( 0 <= K <= 100000) 
is the maximum number of roads that the citizen can travel on.

The next E lines will contain three numbers A B X where A and B are 
cities connected by a road that has a toll of X. Each road is bidirectional, 
meaning that you can travel from A to B and from B to A.

The last line will contain two numbers A B where A is the city the citizen 
starts from and B is the city the citizen wants to go to.


Output:

Output the minimal cost that the citizen must pay in tolls to get from 
city A to City B given that the citizen can only travel on at most K roads.

If it is impossible to reach city B from city A, either because there 
is no path or it can't be done with K grenades, print "IMPOSSIBLE"
Example:
Input:
8 9 100
0 1 1
1 4 3
4 7 4
0 2 1
2 3 1
3 6 1
6 5 1
5 7 1
0 7 20
0 7

Output:
5
 
This example is from the map on right. Since we have 100 grenades and 
only 9 roads, we don't have to restrict the path at all.

Input:
8 9 4
0 1 1
1 4 3
4 7 4
0 2 1
2 3 1
3 6 1
6 5 1
5 7 1
0 7 20
0 7

Ouput:
8
Still representing the map on the right, this time we must restrict 
our path because we don't have enough grenades to travel  on the shortest path.
Input: 
7 6 5
0 1 5
0 2 4
1 2 8
3 6 7
4 5 15
5 6 1
1 6


Output:
IMPOSSIBLE
Note: This does not represent the map on the right.

*/

