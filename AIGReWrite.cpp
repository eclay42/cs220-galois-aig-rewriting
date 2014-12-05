/** My first Galois program -*- C++ -*-
 */
#include "Galois/Galois.h"
#include "Galois/Graph/Graph.h"
#include <boost/iterator/counting_iterator.hpp>
#include <iostream>
#include <string>

using namespace std;

typedef Galois::Graph::FirstGraph<std::string,std::string,true> Graph;

int main(int argc, char** argv) {

  /*
   * Subgraph 1 from Fig.2 in "DAG-Aware AIG Rewriting"
   *
   */
  Graph g;
  std::string vn1, vn2, vn3, vn4, vn5, vn6, vn7, vn8;
  Graph::GraphNode n1, n2, n3, n4, n5, n6, n7, n8;

  /*
   * Label vertices
   */
  vn1 = "a";
  vn2 = "b";
  vn3 = "a";
  vn4 = "c";

  /*
   * Create vertices
   */
  n1 = g.createNode(vn1);
  n2 = g.createNode(vn2);
  n3 = g.createNode(vn3);
  n4 = g.createNode(vn4);
  n5 = g.createNode(vn5);
  n6 = g.createNode(vn6);
  n7 = g.createNode(vn7);
  n8 = g.createNode(vn8);

  g.addNode(n1);
  g.addNode(n2);
  g.addNode(n3);
  g.addNode(n4);
  g.addNode(n5);
  g.addNode(n6);
  g.addNode(n7);
  g.addNode(n8);

  /*
   * Create edges
   */
  g.getEdgeData(g.addEdge(n1, n5)) = "x";
  g.getEdgeData(g.addEdge(n2, n5)) = "x";

  g.getEdgeData(g.addEdge(n3, n6)) = "x";
  g.getEdgeData(g.addEdge(n4, n6)) = "x";

  g.getEdgeData(g.addEdge(n5, n7)) = "x";
  g.getEdgeData(g.addEdge(n6, n7)) = "x";

  g.getEdgeData(g.addEdge(n7, n8)) = "x";
  
  for (Graph::iterator ii = g.begin(), ei = g.end(); ii != ei; ++ii) {
    Graph::GraphNode src = *ii;
    cout << g.getData(src);
    cout << endl;
    for (Graph::edge_iterator jj = g.edge_begin(src), ej = g.edge_end(src); ++jj) {
      Graph::GraphNode dst = graph.getEdgeDst(jj);
      cout << g.getEdgeData(jj);
    }
  }
  cout << endl;

  return 0;
}

