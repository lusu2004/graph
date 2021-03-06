
# In_adjacency_list

Declared in `<graph/Adjacency_list.hpp>`:
```c++
class In_adjacency_list;
```

Satisfies the [`In_edge_graph`](In_edge_graph.md) concept using a classical adjacency list data structure that supports removals.

## Member functions

In addition to the members required by the [`In_edge_graph`](In_edge_graph.md) concept, `In_adjacency_list` provides functions to facilitate mutation.

| Member functions | | |
|------------------|-|-|
| `insert_vert()` | `Vert` | constructs a new vertex |
| `insert_edge(Vert s, Vert t)` | `Edge` | constructs a new edge with tail `s` and head `t` |
| `erase_vert(Vert v)` | | removes a vertex `v` with no outgoing edges to other vertices |
| `erase_edge(Edge e)` | | removes edge `e` |
| `clear()` | | removes all vertices and edges |
