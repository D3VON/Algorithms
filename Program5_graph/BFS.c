
void BFS( int num_v, int num_e, vertex vertices,  edges,  vertex s )
{
  vertex U;
  // color all vertices white (untouched)
  for (int x=0; x < num_v; x++)
    vertices[x].color = 0; 

  // color root vertex gray (discovered)
  vertices[s].color = 1; 

  queue<vertex> Q;
  Q.push(s);
  //assert(!Q.empty());
//while (Q.size() != 0)
  while (!Q.empty())
  {
    U=Q.pop(); // enque the given vertex
    edge *temp = U.edges; // grab the first edge in its adjacency list

    // add vertices from that vertex's adjacency list to the queue, 
    // mark all of them gray (discovered), then mark the original vertex black
    while (temp != NULL)
    {
      if (vertices[temp.u]==0) //==white
      {
        vertices[temp.v].color = 1; //= gray
        Q.push(vertices[temp.v]);
      }
      vertices[U.u].color = 2 //=black, meaning discovered and left for dead
      temp = temp->next;
    }
  }