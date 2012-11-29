/* Program 10, CS333 Algorithms, Spring 2009
   using....
   
   Devon McBride's implementation of "Brizuela's Algorithm"


The vertex cover for a graph is a subset of the vertices, such that for 
every edge, at least one endpoint is in the cover. We are trying to find 
the vertex cover with the minimum size. As a specific example -- suppose 
we have three vertices, connected in a line, as A-B-C. The smallest cover 
for the graph is vertex B, which covers the edges going A-B, and B-C. 
There are other covers (AC, ABC, AB, BC), but the cover with B is the 
smallest. The sets with only A or with only C are not covers. 

Your job is to find the smallest cover you can for datafile3.txt -- 
this is the small graph with 1000 vertices and 5000 edges. The edges 
are undirected, and you can ignore the length/cost. You will submit 
TWO files for this assignment: one will contain the vertices in your 
cover, and the second will contain your source code.

The problem is NP-Complete -- so I'm not expecting you to find the 
best answer possible. But, in the real world, you just have to do your 
best, so that's what you'll do. Go ahead and be creative. Be clever.

Scoring on this assignment is as follows: the five smallest covers 
(that are actually covers) will get 10/10 points. The next 10 will 
get 5/10 points. The rest get no points (yikes!) -- this is to avoid 
having everyone turn in a cover that contains all the vertices, and 
to get a little competition going.

You can use as much run time as you want.... which is why you need 
to send me the output of your program, which is just a list of vertices. 
I'll post a program that will check to see if your cover is correct shortly.


STL, vectors, priority_queues (min and max): 
-------------------------------------------
passing back and forth between min and max priority_queues is a relatively 
huge inefficiency here.  But I used them in order to insure error free 
heapifying -- my heap is not guaranteed to work right, and there comes a 
point when you JUST WANT IT TO WORK RIGHT. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;
                                                                   /*
###################################################################
## STRUCTURES #####################################################
###################################################################*/

typedef struct vertex {
	int index; // a clean way to reference this vertex' index
	int parent; // pointer to the shortest path to root
  int incidents;
  int dead;
	struct edge *edges; // Edges that connect to the vertex.
} vertex;

typedef struct edge {
	int index;
  int dead; 
  int capacity;
	int u;
	int v;
	struct edge *next; // Linked list for the edges
} edge;

                                                                   /*
###################################################################
## ARRAYs, VECTORs, PRIORITY_QUEUEs ###############################
###################################################################*/

edge   *E;      // 
vertex *V;      // order never changes, so we have random access
vector<int> safe;   // vector 
vector<int> covers;

//Overload operator< to determine priority:
bool operator<(const vertex &i1, const vertex &i2)
{
    return i1.incidents < i2.incidents;
}

//Overload operator> to determine priority:
bool operator>(const vertex &i1, const vertex &i2)
{
    return i1.incidents > i2.incidents;
}

                                                                   /*
###################################################################
## function PROTOTYPES                                           ##
###################################################################*/
void kill_min(priority_queue<vertex, vector<vertex>, less<vertex> > max);
void decrement_incidents(int jerk);
void kill_max(priority_queue<vertex, vector<vertex>, greater<vertex> > min);
void find_covers(int num_v);
void print_covers();



                                                                   /*
###################################################################
## decrement_incidents() kills the adjacent edges of a given vertex ##
###################################################################*/
void decrement_incidents(int jerk)
{
  edge *currentE = V[jerk].edges;
  // check adjacencies which aren't already dead
  while (currentE != NULL) 
  {
    E[currentE->index].dead = 1; // mark the edge as dead

    if (V[currentE->v].incidents != 0)
    {
      if (V[currentE->v].dead == 0)
      {
        // decrement the adjacent vertex's .incidents counts
        V[currentE->v].incidents--;
        //printf("%d's neighbor %d's incidents now %d, dead = %d\n",
        //                  currentE->u, currentE->v, V[currentE->v].incidents, V[currentE->v].dead);
      }
    }
   
    currentE = currentE->next;
  }
}


                                                                   /*
###################################################################
## kill_min that receives a priority queue (instead of a vector) ##
###################################################################*/
void kill_min(priority_queue<vertex, vector<vertex>, less<vertex> > max)
{
  vertex leaf, target;
  priority_queue<vertex, vector<vertex>, greater<vertex> > min;
  //printf("____________entering kill_min, filling MIN_priority_queue\n");

  int count = 0;
  // receive max_queue, populate min_queue
	while (!max.empty()) 
  { 
    // put only 'living' vertices into priority_queue -- that are connected
    if (V[max.top().index].dead == 0)
    {
      if (V[max.top().index].incidents == 0)
      {
        //printf("%d has %d incidents\n", max.top().index, V[max.top().index].incidents);
        //printf("putting it into safe vector\n");
        safe.push_back(max.top().index); // save so we can check count when done.
        V[max.top().index].dead = 1;
      }else{
        min.push(max.top());
        count++; 
      }
    }
    max.pop();
  }
  //printf("there are %d in the min_queue now\n", count);

  // if  more to check & is a leaf (or unattached) &  still isn't covered or safe
  while ( !min.empty() && V[min.top().index].incidents < 2)
  {
   if (V[min.top().index].dead != 0)
   {
    min.pop(); // if top of min-Q is already dead, pop it out.
   }
   else
   {
    //printf("min: not empty, top.incidents<2, V[min.top().index].dead != 0\n");

    // catch any new, stray unattached vertices; put them in safe[]
    if (V[min.top().index].incidents == 0)
    {
      //printf("%d has %d incidents\n", min.top().index, V[min.top().index].incidents);
      //printf("putting it into safe vector\n");
      safe.push_back(min.top().index); // save so we can check count when done.
      V[min.top().index].dead = 1;
      min.pop();
    }

    /*###################################
      # This is the real secret to the  #
      # algorithm: any leaf MUST have   #
      # it's "parent" covered.  Working # 
      # from leaves first should help   #
      # achieve a minimum cover.        #
      # THIS IS ENTIRELY THE IDEA OF    #
      # Carlos Brizuela, I'm just implementing it :^D
      ###################################*/   
 
    // remove leaf from future consideration (remove from priority_queue)
    while (V[min.top().index].incidents == 1)
    {
      //printf("vertex %d has %d incident\n", min.top().index, V[min.top().index].incidents);
      // Remove leaf forever from priority_queue, store it in safe vector.
      leaf = min.top();
      V[leaf.index].dead = 1;
      V[leaf.index].incidents--;
      min.pop(); 
      //printf("PUSHED LEAF %d TO safe[], dead = %d, incidents = %d\n", leaf.index, V[leaf.index].dead, V[leaf.index].incidents);
      safe.push_back(leaf.index); // save so we can check count when done.
      
      
      // KILL LEAF'S PARENT:
      // mark leaf's parent as "dead": since we cannot directly access
      // parent in the priority_queue, an alternative is to access it by index
      // in the global V[] array.  There, we can flag it as 'dead'.

      // LEAF could have two edges: the old one, pointing to dead vertex, 
      // and the new one, pointing to the parent we really want to kill.  
      //                                            ^^^^^^
      // TRAVERSE LEAF'S ADJACENCY LIST TO PICK THE LIVING PARENT.
      //                                            ^^^^^^
      edge *current = V[leaf.index].edges;
      while (current != NULL)
      {
        if (V[current->v].dead ==0)
        {
          //printf("Pushing vertex %d (%d's parent) TO covers[]", current->v, leaf.index);
          //printf(", marked it dead.\n");

          V[current->v].dead = 1;
          V[current->v].incidents = 0;// removed, so connected to nothing.
          covers.push_back(V[current->v].index);
          // shouldn't be any other vertices.incidents to decrement

          decrement_incidents(current->v);

          /*###################################################################
            # NOTICE: by removing leaves, then removing their parents, we can
            # create new leaves, but without re-loading non-dead vertices from
            # the global V array (where we are recording .incitents counts), 
            # we are not able to detect newly created leaves here.  So, newly
            # created leaves will have to wait until after we pass through the
            # kill_max() function again.  When kill_max() calls kill_min()
            # it is only at that time that the min_priority-queue is able to 
            # have the leaves created here at the top of the queue. 
            ###################################################################*/
        }
        current = current->next;
      }
      //* BIG INEFFICIENCY FIXED HERE:
      // This clunky block takes into account changes to the global .incidents
      // variables THAT ARE NOT REFLECTED IN THE LOCAL priority_queue.  I'm 
      // basically re-heapifying the priority queue by making it reference
      // the global V array, where actual changes have been made to .incidents.
      vector<int> vee;
      int woof = 0;
      while (min.size() > 0)
      {
        //printf("put %d into vee\n", min.top().index);
        vee.push_back(min.top().index); 
        min.pop();
      } 

      for (int meow = 0; meow < vee.size() ; meow++)
      {
        min.push(V[vee[meow]]);
      }
      //*/
    }
   }
  }
  //printf("No more 0 or 1 incident vertices. Vertex %d has %d incidents\n", min.top().index, V[min.top().index].incidents);


  if (min.size() > 0)
  {
    //printf("Done with min, going to check max_priority_queue again\n");
    kill_max(min);
  }
  //printf("no more to check\n");
}


                                                                   /*                                                                 /*
###################################################################
###################################################################
###################################################################*/
void kill_max(priority_queue<vertex, vector<vertex>, greater<vertex> > min)
{
  priority_queue<vertex, vector<vertex>, less<vertex> > max;
  //printf("____________entering kill_max, filling MAX_priority_queue\n");

  int count = 0;
  // receive min_queue, populate max_queue
	while (!min.empty()) 
  { 
    // store only 'living' vertices into priority_queue
    if (V[min.top().index].dead == 0)
    {
      max.push(min.top()); 
      count++;
    }    
    min.pop();
  }
  //printf("there are %d in the max_queue now\n", count);
                                                 /*
    #############################################
    # I started by killing one 'star' vertex    #
    # (which is one with a lot of adjacencies). #
    # Later, I made a LOOP and killed           #
    # several star verticies. Changing how many #
    # 'star' vertices to cover doesn't help     #
    # much.                                     #
    #############################################*/

  vertex kill;

  // looping introduces problems, depending on which magnitude of a loop.
  //for (int i=0; i<13; i++)
  //{
    kill = max.top();
    //printf("choosing to kill vertex %d, marking it DEAD!\n", kill.index);
    V[kill.index].dead = 1;
    max.pop();
    covers.push_back(kill.index);
    //printf("%d pushed to covers[]\n", kill.index);
    //printf(".....and these adjacent vertices' .incidents were decremented:\n");
    decrement_incidents(kill.index);  
  //}

  if (max.size() > 0)
  {
    //printf("Done with max, going to check min_priority_queue again\n");
    kill_min(max);
  }   
  //printf("no more to check\n");
}
  
                                                                   /*
###################################################################
## find_covers() puts the global vector's data into a min_queue  ##
###################################################################*/
void find_covers(int num_v)
{
  priority_queue<vertex, vector<vertex>, greater<vertex> > min;
  vertex leaf, target;
  //printf("____________entering find_covers; filling MIN_priority_queue\n");

  int count = 0;
  // load priority_queue with data from global vector V
	for (int x=0; x < num_v; ++x) 
  {
      min.push(V[x]); 
      count++;
  }
  //printf("there are %d in the min_queue now\n", count);

  // if   more to check &   is a leaf             &  isn't already covered or safe
  while ( !min.empty() && V[min.top().index].incidents < 2)
  {
   if (V[min.top().index].dead != 0)
   {
     min.pop(); 
   }
   else
   {
    // remove all unattached vertices from future consideration
    while (V[min.top().index].incidents == 0) 
    {
      if (V[min.top().index].dead == 0)
      {
        //printf("%d has %d incidents\n", min.top().index, V[min.top().index].incidents);
        safe.push_back(min.top().index); // save so we can check count when done.
        V[min.top().index].dead = 1;
      }     
      min.pop(); 
    }

    /*###################################
      # This is the real secret to the  #
      # algorithm: any leaf MUST have   #
      # it's "parent" covered.  Working # 
      # from leaves first should help   #
      # achieve a minimum cover.        #
      # THIS IS ENTIRELY THE IDEA OF    #
      # Carlos Brizuela, I'm just implementing it :^D
      ###################################*/   
 
    // remove leaf from future consideration (remove from priority_queue)
    while (V[min.top().index].incidents == 1)
    {
      // Remove leaf forever from priority_queue, store it in safe vector.
      leaf = min.top();
      min.pop();

      if (V[leaf.index].dead == 0)
      {
        //printf("vertex %d has %d incidents\n", leaf.index, V[leaf.index].incidents);
        V[leaf.index].incidents--;
        V[leaf.index].dead = 1;
        safe.push_back(leaf.index);
      } 
      

      // mark leaf's parent as "dead": since we cannot directly access
      // parent in the priority_queue, an alternative is to access it by index
      // in the global V[] array.  There, we can flag it as 'dead'.
      V[leaf.edges->v].dead = 1;
      V[leaf.edges->v].incidents--;
      //printf("PUSHED vertex %d TO covers[]\n", V[leaf.edges->v].index);
      covers.push_back(V[leaf.edges->v].index);
    }
   }
  }
  //printf("priority_queue indicated no more have less than %d incidents\n", V[min.top().index].incidents);
  //printf("if any left in priority_queue, go to kill_max\n");

  if (min.size() > 0)
  {
    kill_max(min);
  }
  //printf("no more to check\n");
}

                                                                   /*
###################################################################
## print_verification() prints out covers and safe arrays        ##
###################################################################*/
void print_verification()
{
  int c, s;
  //printf("covers are:\n");
  for (c = 0; c < covers.size(); c++)
  {
    printf("%d\n", covers[c]);
  }
  //printf("safes are:\n");
  //for (s = 0; s < safe.size(); s++)
  //{
  //  printf("%d, (has %d incidents)\n", safe[s], V[safe[s]].incidents);
  //}
  printf("%d\n", c);
  //printf(" safes: %4d\n", s);
  //printf("----------\n");
  //int total = c + s;
  //printf(" total: %4d\n", total);


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
	int num_v, num_e;
	/*****************************************************************************
   first two numbers signify: #vertices, #edges
   *****************************************************************************/
	scanf("%d %d", &num_v, &num_e);

	V = (vertex *) malloc(sizeof(vertex) * num_v    );
	E = (edge   *) malloc(sizeof(edge  ) * num_e * 2);
	// Twice as many edges, because we're creating/storing
	// pseudo-doubly-linked edges (u to v) and (v to u) for each edge entered
	// into input.

  // MAKE ALL VERTICES (no need to wait for edge information)
	int i; // initialize all vertices' data members
	for (i = 0; i < num_v; ++i) {
		V[i].edges       = NULL;
		V[i].index       = i;
    V[i].incidents   = 0;
    V[i].dead        = 0;
	}

  //RECIEVE EDGE INFO. 
  // Each line is: vertex, vertex, capacity
  // --create also a reverse-flow edge for each. 
	for (i = 0; i < num_e * 2; i += 2)
	{
		// Receive each edge (u to v) from input, put into E[]
		scanf("%d %d %d", &E[i].u, 
                      &E[i].v, 
                      &E[i].capacity);
                       // Also tell the edge its index number
                       // (this global array of edges won't change it's order).
		                   E[i].index = i;
                       E[i].dead  = 0;


		// Create 'backward' edge and put at the next index (always an odd number)
    // This edge is from v to u instead of the received u to v.
                  E[i + 1].u = E[i].v;
                  E[i + 1].v = E[i].u;
                  E[i + 1].capacity = 0; // initialize residual capacity to zero
                  E[i + 1].index    = i + 1;
                  E[i + 1].dead     = 0;

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
		V[E[i].u].edges = &E[i  ]; 
		V[E[i].v].edges = &E[i+1];   

    // Increment the count of incidents these two vertices have.
    V[E[i].u].incidents++;
    V[E[i].v].incidents++;

  }
  /*TESTING .incidents of each vertes:
  printf("__________________Vertices .incidents initilized in main():\n");
  for (int g = 0; g < num_v; g++)
  {
    printf("vertex %d's incidents: %d\n", V[g].index, V[g].incidents);
  }
  */
  
  
  
  
  
  
  find_covers(num_v);
  print_verification();

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





/* TESTING THAT KILL_MIN() AND KILL_MAX() WORK
  // have to make up data first.

  for (int x=0; x<5; x++)
  {
    V[x].index = x;
  }

  V[0].incidents = 6;
  V[1].incidents = 3;
  V[2].incidents = 99;
  V[3].incidents = 222;
  V[4].incidents = 4;

  find_covers(num_v);

*/

}
