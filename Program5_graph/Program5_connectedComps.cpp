/* Program5_connectedComps.cpp to discover how many graphs there are in a given 
 *                                          bunch of pre-formated data
 * or  g++ Program5_connectedComps.cpp -o Program5_connectedComps
 * and run it with something like:
 *     ./Program5_connectedComps < datafile5.txt
 *
 * Devon McBride ported (and modified) this code from Intro. to Algorithms, 2nd Ed. 
 * -------------                           by Thomas H. Cormen, et al.
 *                                         BFS circa page 532
 *...and with help from Prof. Madden, CS333, Binghamton University

 Implement a programs to walk through a graph in depth first or breadth 
 first order. YOU MUST USE AN ABSTRACT DATA STRUCTURE which supports 
 arbitrary numbers of edges per vertex. DO NOT use a matrix. Some of 
 the graphs will be too large for a matrix.

The input will consist of the # of vertices, the # of edges, and then 
a series of edges. Each edge will connect vertex i to vertex j, and 
will have a length (ignore this for this assignment). As an example, 
we might have a 5 vertex graph, with 4 edges. I'll name the vertices 
A, B, C, D, E, for explanation, but they'll actually be numbered 0 
through 4. A connects to B with a length of 10, B connects to C with 
length 3, A connects to D with length 5, A connects to E with length 2. 
(Again, the lengths don't matter for this assignment). The input file 
looks like this:

5 4
0 1 10
1 2 3
0 3 5
0 4 2

The objective is to determine the number of connected components. You 
could do this easily by having a "visited" flag on each vertex. With 
a for loop, go through each vertex in order -- if it is not visited, 
mark it as visited, then run either DFS or BFS to mark all the vertices 
that are connected -- you have found one connected component. Continue 
on in your for loop to the next unmarked vertex, and do the same thing.

Your program should print out the total number of connected components 
in the input file (just a single number to print). 

There are 6 data files to try:

datafile1.txt 
datafile2.txt 
datafile3.txt 
datafile4.txt 
datafile5.txt 
datafile6.txt 

There's a simple C program to read in and print out a graph (with luck, 
there will be less weirdness in the graphs people use.). 
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <queue>
using namespace std;

typedef struct edge_struct
{
  int u, v;
  int length;
  struct edge_struct *next; // Linked list for the edges
} edge;

typedef struct
{
  int distance;
  int color; // 0=white, 1=gray, 2=black  
  struct edge_struct *edges; // Edges that connect to the vertex.
} vertex;


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

[01:07] aznprime817: in your while loop when DFSvisit is 5
[01:07] aznprime817: 5 goes to 7
[01:07] aznprime817: and 7 goes to 8
[01:07] aznprime817: 8 goes to 9
[01:07] aznprime817: then 9 goes to nothing
[01:07] aznprime817: so yu never landed on 6
[01:08] aznprime817: im really not sure how to solve this
[01:08] aznprime817: but i have to go downstairs for a while
[01:08] aznprime817: so i'll ttyl
[01:08] aznprime817: gl
*/


void DFSVisit(vertex vertices[], int x){
  vertices[x].color = 1;
  edge *temp = vertices[x].edges; 
  //printf("vertex = %d\n", x); //something nutty going on here
  //printf("goes to = %d\n", temp->v); 
  while (temp != NULL) 
  {
    if (vertices[temp->v].color == 0)
      DFSVisit(vertices, temp->v);
    temp = temp->next;
  }  
}


int DFS(int num_v, vertex vertices[])
{
  int count = 0;
  int x;
  for (x=0; x < num_v; x++)
    if (vertices[x].color == 0){
      // marks all components connected to that vertex
      // ...in other words, marks all 'connected components' clumps
      DFSVisit(vertices, x); 
      //printf("count = %d, index = %d, vertex = %d\n", count, x, vertices[x].edges->u);
      count++;
    } 
  return count;
}

void BFS( int num_v, vertex vertices[], vertex s )
{
  vertex U;

  //root will soon enough be colored black within the vertices array
  // color root vertex gray (discovered) 
  //vertices[s->edges.u].color = 1; 

  queue<vertex> Q;
  Q.push(s);
  while (!Q.empty())
  {
    U = Q.front(); // grab the vertex at the head of the queue
        Q.pop();   // dequeue vertex at the head of the queue
    edge *temp = U.edges; // grab the first edge in its adjacency list

    // use vertex U's adjacency list to identify connected vertices to enqueue, 
    // mark all of them gray (discovered), 
    // then mark the vertex U black (all its connected vertices discovered)
    while (temp != NULL)
    {
      printf("-------------entering---------------\n");
      if (vertices[temp->u].color == 0) //==white
      {
        printf("%d is what's stored in vertices[%d].color \n", vertices[temp->u].color, temp->u);
        vertices[temp->u].color = 1; //= gray
        Q.push(vertices[temp->u]); // enqueue this discovered vertex
      }
      vertices[temp->u].color = 2; //=black (all its connected vertices discovered)
      printf("%d is what's stored in vertices[%d].color \n", vertices[temp->u].color, temp->u);
      temp = temp->next; // note to self: temp is a pointer to a struct, 
    }                    // so use -> instead of .
  }
}


/*****************************************************************************
 **MAIN MAIN MAIN MAIN *******************************************************
 *****************************************************************************/

int main(int argc, char *argv[])
{
  int num_v, num_e;
  vertex *vertices; // 'head' pointer to vertices[] array
  edge *edges;      // 'head' pointer to    edges[] array

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

  int i;
  for (i = 0; i < num_v; ++i)
  {
    vertices[i].distance = -1;
    vertices[i].edges = NULL;
    vertices[i].color = 0; // color all vertices white (untouched)
  }

/*****************************************************************************
 Grab the rest of the input; each line is: vertex, vertex, distance
 ...but doubly link the vertices, for greater potential future usefulness. 
 *****************************************************************************/
  //for (i =  0; i < num_e * 2; i += 2)
  for (i =  0; i < num_e; i++)
  {
    // grab the edge (u to v) from input, put into edges[]
    scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].length);

    /*************do I need this for this assignment?************
    // next edge in edges[] is the reverse image if the one just input
    edges[i + 1].u = edges[i].v;
    edges[i + 1].v = edges[i].u;
    edges[i + 1].length = edges[i].length;
    *************************************************************/
    //printf("%d is i\n", i);
    //printf("%d is edges[%d].u\n", edges[i].u, i);
    edges[i].next = vertices[edges[i].u].edges; // all were initialized to NULL
    //printf("%d is edge[%d]'s next\n", vertices[edges[i].u].edges, i);
    //printf("%d is vertices[%d]'s edges\n", vertices[edges[i].u].edges, edges[i].u);

    // "This is an edge that this vertex has."
    vertices[edges[i].u].edges = &edges[i];
    //printf("%d is &edges[%d]\n", &edges[i], i);
    //printf("%d is vertices[edges[%d].u].edges\n", &edges[i], i);

    /*************do I need this for this assignment?************
    edges[i + 1].next = vertices[edges[i + 1].u].edges;
    vertices[edges[i + 1].u].edges = &edges[i + 1];
    *************************************************************/
  } 

  int m, count;
  count = 0;
  for (m=0; m<num_v; m++)
  {
    printf("vertices[%d]'s color is %d\n", m, vertices[m].color);
    if (vertices[m].color == 0)
    {
      // should detect all connected vertices.
      BFS(num_v, vertices, vertices[m]);
      count++;
    }
  }
  //count = DFS(num_v, vertices);
  printf("%d is the count\n", count);

  ///////////////
  ///////////////  DFS works, 
  ///////////////
  ///////////////  BFS, not so much...
  ///////////////


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
      printf(" connect %d %d, length %d\n",
       tmp->u, tmp->v, tmp->length);
      tmp = tmp->next;
    }
  }
*/

}

