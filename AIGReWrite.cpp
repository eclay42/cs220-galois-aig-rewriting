/** My first Galois program -*- C++ -*-
 */
#include "Galois/Galois.h"
#include "Galois/Graph/Graph.h"
#include <boost/iterator/counting_iterator.hpp>
#include <iostream>
#include <string>
#include <sstream>

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
Graph g;

stringstream ss;

int main(int argc, char** argv) {

  /*
   * Subgraph 1 from Fig.2 in "DAG-Aware AIG Rewriting"
   *
   */

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
  g.getEdgeData(g.addEdge(n1, n4)) = 2; // a0->n0
  g.getEdgeData(g.addEdge(n4, n1)) = 3; // n0->a0 (back edge)
  g.getEdgeData(g.addEdge(n1, n5)) = 1; // a0->n1
  g.getEdgeData(g.addEdge(n5, n1)) = 3; // n1->a0 (back edge)

  g.getEdgeData(g.addEdge(n2, n4)) = 1; // a1->n0
  g.getEdgeData(g.addEdge(n4, n2)) = 3; // n0->a1 (back edge)
  
  g.getEdgeData(g.addEdge(n3, n5)) = 1; // a2->n1
  g.getEdgeData(g.addEdge(n5, n3)) = 3; // n1->a2 (back edge)

  g.getEdgeData(g.addEdge(n4, n6)) = 1; // n0->n2
  g.getEdgeData(g.addEdge(n6, n4)) = 3; // n2->n0 (back edge)
  
  g.getEdgeData(g.addEdge(n5, n6)) = 2; // n1->n2
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
      else if ( e == 3 )
        cout << "Back edge to ";
      else
    	  cout << "Edge error";
      
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

  cout << "Begin Algorithm:\n";

  for (Graph::iterator ii = g.begin(), ei = g.end(); ii != ei; ++ii) {
    Graph::GraphNode top = *ii; // node we are attempting to build the cut from (top of the pyramid)
    Graph::GraphNode left = NULL, right = NULL, input1 = NULL, input2 = NULL, input3 = NULL, input4 = NULL;
    Graph::GraphNode output = NULL;
    cout << "Checking node " << g.getData(top).label_type << endl;
    if ( g.getData(top).type != nd ) {
      cout << "Node is not nd type\n";
      nextcut:
      cout << "Moving to next node\n\n";
      continue; } // move to the next node if a node is a primary input or output
      
    cout << "Checking edges for top node\n";
    for (Graph::edge_iterator jj = g.edge_begin(top), ej = g.edge_end(top); jj != ej; ++jj) {
      // look for the back edges and follow them back to the input nodes to generate our cut
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      cout << "Found edge of weight " << ew << " to node " << g.getData(dst).label_type << endl;
      if ( ew == 3 ) {
        if ( g.getData(dst).type != nd ) {
          cout << "Left or right node is not nd type\n";
          goto nextcut; }// bail out if we find a primary input or output
        else if ( left == NULL ) {
          left = dst; cout << "left = " << g.getData(left).label_type << endl; }
        else if ( right == NULL ) {
          right = dst; cout << "right = " << g.getData(right).label_type << endl; }
        else {
          cerr << "Error: node " << g.getData(top).label_type << " has more than two inputs\n"; exit; }
      }
      else { // build list of outputs
        }
    }
    if ( left == NULL || right == NULL ) {
      cerr << "Error: node " << g.getData(top).label_type << " does not have two inputs\n"; exit; }
      
    //cout << "Found nodes " << g.getData(left).label_type << " and " << g.getData(right).label_type << endl;
      
    int left_edge, right_edge;
    // find the edges leading back to the top node and the input edges
    cout << "Checking edges for left node\n";
    for (Graph::edge_iterator jj = g.edge_begin(left), ej = g.edge_end(left); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      cout << "Found edge of weight " << ew << " to node " << g.getData(dst).label_type << endl;
      if ( dst == top ) {
        left_edge = ew; cout << "left_edge = " << left_edge << endl; }
      else if ( ew == 3 ) {
        if ( input1 == NULL ) {
          input1 = dst; cout << "input1 = " << g.getData(input1).label_type << endl; }
        else if ( input2 == NULL ) {
          input2 = dst; cout << "input2 = " << g.getData(input2).label_type << endl; }
        else {
          cerr << "Error: node " << g.getData(left).label_type << " has more than two inputs\n"; exit; }
      }
      else {
        goto nextcut; }
        
    }
    cout << "Checking edges for right node\n";
    for (Graph::edge_iterator jj = g.edge_begin(right), ej = g.edge_end(right); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      cout << "Found edge of weight " << ew << " to node " << g.getData(dst).label_type << endl;
      if ( dst == top ) {
        right_edge = ew; cout << "right_edge = " << left_edge << endl; }
      else if ( ew == 3 ) {
        if ( input3 == NULL ) {
          input3 = dst; cout << "input3 = " << g.getData(input3).label_type << endl; }
        else if ( input4 == NULL ) {
          input4 = dst; cout << "input4 = " << g.getData(input4).label_type << endl; }
        else {
          cerr << "Error: node " << g.getData(right).label_type << " has more than two inputs\n"; exit; }
      }
      else {
        goto nextcut; }
    }
    
    int edge1, edge2, edge3, edge4;
    
    cout << "Checking edges for input1\n";
    for (Graph::edge_iterator jj = g.edge_begin(input1), ej = g.edge_end(input1); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      if ( dst == left ) {
        edge1 = ew; cout << "edge1 = " << edge1 << endl; break;}
    }
    
    cout << "Checking edges for input2\n";
    for (Graph::edge_iterator jj = g.edge_begin(input2), ej = g.edge_end(input2); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      if ( dst == left ) {
        edge2 = ew; cout << "edge2 = " << edge2 << endl; break;}
    }
    
    cout << "Checking edges for input3\n";
    for (Graph::edge_iterator jj = g.edge_begin(input3), ej = g.edge_end(input3); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      if ( dst == right ) {
        edge3 = ew; cout << "edge3 = " << edge3 << endl; break;}
    }
    
    cout << "Checking edges for input4\n";
    for (Graph::edge_iterator jj = g.edge_begin(input4), ej = g.edge_end(input4); jj != ej; ++jj) {
      Graph::GraphNode dst = g.getEdgeDst(jj);
      int ew = g.getEdgeData(jj);
      if ( dst == right ) {
        edge4 = ew; cout << "edge4 = " << edge4 << endl; break;}
    }
    
    
    string bool_exp = "";
    string in1 = "", in2 = "", in3 = "", in4 = "";
    char lastin = 'a';
    
    in1 += lastin;
      
    if ( input1 == input2 )
      in2 = in1;
    else
      in2 += ++lastin;
    
    
    if ( input3 == input1 )
      in3 = in1;
    else if ( input3 == input2 )
      in3 = in2;
    else
      in3 += ++lastin;

    
    if ( input4 == input1 )
      in4 = in1;
    else if ( input4 == input2 )
      in4 = in2;
    else if ( input4 == input3 )
      in4 = in3;
    else
      in4 += ++lastin;
    
    if ( edge1 == 2 )
      in1 += "'";
    if ( edge2 == 2 )
      in2 += "'";
    if ( edge3 == 2 )
      in3 += "'";
    if ( edge4 == 2 )
      in4 += "'";
      
    string in_left = "", in_right = "";
    in_left = "(" + in1 + in2 + ")";
    if ( left_edge == 2 )
      in_left += "'";
    in_right = "(" + in3 + in4 + ")";
    if ( right_edge == 2 )
      in_right += "'";

    //ss << left_edge << right_edge << in1 << in2 << in3 << in4;
    bool_exp = in_left + in_right;
    
    // generate truth table
    bool x, y; // intermediate values
    string truth_table = "";
    if ( lastin == 1 ) {
      cerr << "All inputs to the cut are identical\n"; exit; }
    else if ( lastin == 4 ) {
      cout << "All inputs to the cut are unique, unable to simplify\n"; goto nextcut; }
    
    int n;
    if ( lastin == 'b' ) n = 4;
    else n = 8;
    
    for ( int i = 0; i < n; ++i )
    {
    }
    
    cout << "\nCut:\n";
    cout << "Boolean Expression = " << bool_exp << endl;
    cout << "Truth Table = " << truth_table << endl;
    cout << endl;
  }
  cout << "Done\n";
  return 0;
}
