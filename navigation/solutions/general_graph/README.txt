Some solutions for an arbitrary graph.
There we cannot do a regular DFS, so instead we do BFS using a layered DFS that expands 1 layer each time.
The key is to record the distance to each node from the start modulo 3.
That way we can figure out the types of each edge.
These solutions do O(N^2) steps.
