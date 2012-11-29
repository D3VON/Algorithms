#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Linked list for the edges
typedef struct edge_struct
{
  int u, v;
  int length;
  struct edge_struct *next;
} edge;

typedef struct
{
  int distance;
  // Edges that connect to the vertex.
  struct edge_struct *edges;
} vertex;


int main(int argc, char *argv[])
{
  int num_v, num_e;
  vertex *vertices;
  edge *edges;


  scanf("%d %d", &num_v, &num_e);
  // Reserve space -- twice as many edges, because we're creating links for
  // both directions.
  vertices = (vertex *) malloc(sizeof(vertex) * num_v);
  edges = (edge *) malloc(sizeof(edge) * num_e * 2);
  int i;

  for (i = 0; i < num_v; ++i)
    {
      vertices[i].distance = -1;
      vertices[i].edges = NULL;
    }

  for (i =  0; i < num_e * 2; i += 2)
    {
      scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].length);
      edges[i + 1].u = edges[i].v;
      edges[i + 1].v = edges[i].u;
      edges[i + 1].length = edges[i].length;

      // Insert into the list for both u and v
      edges[i].next = vertices[edges[i].u].edges;
      vertices[edges[i].u].edges = &edges[i];

      edges[i + 1].next = vertices[edges[i + 1].u].edges;
      vertices[edges[i + 1].u].edges = &edges[i + 1];
    }

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
}
