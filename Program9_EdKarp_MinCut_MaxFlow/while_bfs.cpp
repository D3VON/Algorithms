while (BFS(num_v, vertices, vertices[0]))
{
  stack<vertex> reverse;
  int least = 999999;

  // find path capacity
  vertex tempVert;
  while (!pathE.empty())
  {
    tempVert = pathE.top();
    reverse.push(tempVert); // temporarily store order
    pathE.pop();
    printf("%d, ", tempVert.index);

    //find least capacity of all edges in path
    if (least > tempVert.capacity)
    {
      least = tempVert.capacity;
    }
    printf("\n%d is the least capacity edge\n", least);
  }
}

