/** My first Galois program -*- C++ -*-
 */
#include "Galois/Galois.h"
#include "Galois/Graph/Graph.h"
#include <boost/iterator/counting_iterator.hpp>
#include <iostream>
#include <string>

using namespace std;

//typedef Galois::Graph::FirstGraph<std::string,std::string,true> Graph;


typedef enum{pi,po,nd} node_type;

struct Node {
 string label_type;//a0,b0...
 node_type type;// pi,po,nd
 int fanins;
 bool fanout; 
 int level;
 };
 
typedef Galois::Graph::FirstGraph<Node,int,true> Graph;

int main(int argc, char** argv) {

  /*
   * Subgraph 1 from Fig.2 in "DAG-Aware AIG Rewriting"
   *
   */
  Graph g;
  Node vn1, vn2, vn3, vn4, vn5, vn6, vn7, vn8;
  Graph::GraphNode n1, n2, n3, n4, n5, n6, n7, n8;

  /*
   * Label vertices
   */
   
  // primary inputs
  vn1.type = pi; vn2.type = pi; vn3.type = pi; 
  vn1.level = 0; vn2.level = 0; vn3.level = 0;
  vn1.label_type = "a0"; vn2.label_type = "a1"; vn3.label_type = "a2";
  
  // intermediate nodes
  vn4.type = nd; vn5.type = nd; vn6.type = nd;
  vn4.level = 1; vn5.level = 1; vn6.level = 2;
  vn4.label_type = "n0"; vn5.label_type = "n1"; vn6.label_type = "n2"; 
  
  // primary output
  vn7.type = po; vn7.level = 3; vn7.label_type = "s0";

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

  g.addNode(n1);
  g.addNode(n2);
  g.addNode(n3);
  g.addNode(n4);
  g.addNode(n5);
  g.addNode(n6);
  g.addNode(n7);

  /*
   * Create edges
   */
  g.getEdgeData(g.addEdge(n1, n4)) = 1; // a0->n0
  g.getEdgeData(g.addEdge(n4, n1)) = 3; // n0->a0 (back edge)
  g.getEdgeData(g.addEdge(n1, n5)) = 1; // a0->n1
  g.getEdgeData(g.addEdge(n5, n1)) = 3; // n1->a0 (back edge)

  g.getEdgeData(g.addEdge(n2, n4)) = 1; // a1->n0
  g.getEdgeData(g.addEdge(n4, n2)) = 3; // n0->a1 (back edge)
  
  g.getEdgeData(g.addEdge(n3, n5)) = 1; // a2->n1
  g.getEdgeData(g.addEdge(n5, n3)) = 3; // n1->a2 (back edge)

  g.getEdgeData(g.addEdge(n4, n6)) = 1; // n0->n2
  g.getEdgeData(g.addEdge(n6, n4)) = 3; // n2->n0 (back edge)
  
  g.getEdgeData(g.addEdge(n5, n6)) = 1; // n1->n2
  g.getEdgeData(g.addEdge(n6, n5)) = 3; // n2->n1 (back edge)

  g.getEdgeData(g.addEdge(n6, n7)) = 1; // n2->s0
  g.getEdgeData(g.addEdge(n7, n6)) = 3; // s0->n2 (back edge)
  
  // print graph
  for (Graph::iterator ii = g.begin(), ei = g.end(); ii != ei; ++ii) {
    Graph::GraphNode src = *ii;
    cout << "node: " << g.getData(src).label_type;
    cout << endl;
    for (Graph::edge_iterator jj = g.edge_begin(src), ej = g.edge_end(src); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int e = g.getEdgeData(jj);
      if ( e == 1 )
        cout << "Forward noninverted edge to ";
      else if ( e == 2 )
        cout << "Forward inverted edge to ";
      else
        cout << "Back edge to ";
      
      cout << g.getData(dst).label_type << endl;
      //cout << "edge weight: " << g.getEdgeData(jj);
      //cout << endl;
    }
    cout << endl;
  }
  cout << endl;
  
  /*
   *  Algorithm
   */

  for (Graph::iterator ii = g.begin(), ei = g.end(); ++ii) {
    Graph::GraphNode top = **ii; // node we are attempting to build the cut from (top of the pyramid)
    Graph::GraphNode left = NULL, right = NULL, input1 = NULL, input2 = NULL, input3 = NULL, input4 = NULL;
    Graph::GraphNode output = NULL;
    if ( g.getData(src).type != np ) {
      nextcut:
      continue; } // move to the next node if a node is a primary input or output
    
    for (Graph::edge_iterator jj = g.edge_begin(top), ej = g.edge_end(top); jj != ej; ++jj) {
      // look for the back edges and follow them back to the input nodes to generate our cut
      Graph::GraphNode dst = g.getEdgeDst(jj);
      if ( g.getData(dst).type != nd )
        goto nextcut; // bail out if we find a primary input or output
      
      int e = g.getEdgeData(jj);
      if ( e == 3 ) {
        if ( left == NULL )
          left = dst;
        else if ( right == NULL )
          right = dst;
        else {
          cerr << "Error: node " << g.getData(top).label_type << " has more than two inputs\n"; exit; }
      }
    }
    if ( left == NULL || right == NULL ) {
      cerr << "Error: node " << g.getData(top).label_type << " does not have two inputs\n"; exit; }
      
    int left_edge, right_edge;
    // find the edges leading back to the top node and the input edges
    for (Graph::edge_iterator jj = g.edge_begin(left), ej = g.edge_end(left); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int e = g.getEdgeData(jj);
      if ( dst == top )
        left_edge = e;
      else if ( e == 3 ) {
        if ( input1 == NULL )
          input1 = dst;
        else if ( input2 == NULL )
          input2 = dst;
        else {
          cerr << "Error: node " << g.getData(left).label_type << " has more than two inputs\n"; exit; }
      } 
    }
    
    for (Graph::edge_iterator jj = g.edge_begin(right), ej = g.edge_end(right); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int e = g.getEdgeData(jj);
      if ( dst == top )
        right_edge = e;
      else if ( e == 3 ) {
        if ( input3 == NULL )
          input1 = dst;
        else if ( input2 == NULL )
          input4 = dst;
        else {
          cerr << "Error: node " << g.getData(left).label_type << " has more than two inputs\n"; exit; }
      } 
    }
    
    
    
  return 0;
}

