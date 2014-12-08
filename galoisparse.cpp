
#include <iostream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <Galois/Galois.h>
#include <Galois/Graph/Graph.h>

#include <cstring>
#include <string>
#include <fstream>
using namespace std;
using namespace boost;

typedef enum{pi,po,nd} node_type;

struct Node {
 string label_type;//a0,b0...
 node_type type;// pi,po,nd
 int fanins; // Not required as of now
 bool fanout; //Not required as of now
 int level;
 };

int main() {

	unordered_map <string, int> map;
	//unordered_map<string, int> map;
	int node_index = 0;

	typedef Galois::Graph::FirstGraph<Node,int,true> Graph;

	Graph g;
	//vector<Node> nodes;
	vector<Graph::GraphNode> gnodes;

	vector<string> fields;

	ifstream fin;
	fin.open("/home/aditya/CS_220/alan-abc/examples/sk"); // open a file
	if (!fin.good()){
		cout << "file not found\n";
	    return 1; // exit if file not found
	}
	  // read each line of the file
	  while (!fin.eof())
	  {
	    // read an entire line into memory
	    string line;
	    getline(fin,line);
	    trim(line);
	    split(fields, line, is_any_of(" "));
	    Node n;
	    if(fields[0].compare("input") == 0 || fields[0].compare("output") == 0||fields[0].compare("wire") == 0)
	    {
	    	for(unsigned i =1;i<fields.size();i++){
	    		//add to hash map to index to corresponding node in array
	    		if(fields[i].size()>1)
	    		{
					//Add data to each node for level and type of input
					if(fields[0].compare("input") == 0)
					{
						n.type = pi;
						n.level = 0;
						n.label_type = fields[i].substr(0,fields[i].length()-1);

						//nodes[node_index].type = pi;
						//nodes[node_index].level = 0;
						//nodes[node_index].label_type = fields[i].substr(0,fields[i].length()-1);
					}
					if(fields[0].compare("output") == 0)
					{
						n.type = po;
						n.level = -1;
						n.label_type = fields[i].substr(0,fields[i].length()-1);
						//nodes[node_index].type = po;
						//nodes[node_index].label_type = fields[i].substr(0,fields[i].length()-1);
					}
					if(fields[0].compare("wire") == 0)
					{
						n.type = nd;
						n.level = -2;
						n.label_type = fields[i].substr(0,fields[i].length()-1);
						//nodes[node_index].type = nd;
						//nodes[node_index].label_type = fields[i].substr(0,fields[i].length()-1);
					}
					//remove last char b/c its ',' or ';'
					map[fields[i].substr(0,fields[i].length()-1)] = node_index;
					//gnodes[node_index] = g.createNode(nodes[node_index]);
					gnodes.push_back(g.createNode(n));

					g.addNode(gnodes[gnodes.size()-1]);
					//cout << "node index: "<< node_index<<endl;
					node_index++;
	    		}

	    	}
	    }
	    //cout <<"compare assign"<<endl;
	    if(fields[0].compare("assign") == 0)
	    {
		int level1,level2;		

			if(fields[4].compare("|") == 0)
			{	//Edge weight: 2- Negative 1 - Positive
				g.getEdgeData(g.addEdge(gnodes[map[fields[3].substr(0,fields[3].length())]]//src
											   ,gnodes[map[fields[1]]])) = 2;//dest
				g.getEdgeData(g.addEdge(gnodes[map[fields[5].substr(0,fields[5].length()-1)]]
											   ,gnodes[map[fields[1]]])) = 2;
	
				level1 = g.getData(gnodes[map[fields[3].substr(0,fields[3].length())]]).level;
				level2 = g.getData(gnodes[map[fields[5].substr(0,fields[5].length()-1)]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
					      gnodes[map[fields[3].substr(0,fields[3].length())]])) = 3;//back edges to determine fanins -> wt. = 3
				g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
					      gnodes[map[fields[5].substr(0,fields[5].length())]])) = 3;//back edges to determins fanins -> wt. = 3 
			}

			else
			{	size_t invert = fields[3].find_first_of("~");
				if (invert != string::npos){
					//addEdge(src,dest)
					//remove the '~' from the start of string
					//map[string] returns node index to find which node
					g.getEdgeData(g.addEdge(gnodes[map[fields[3].substr(1,fields[3].length())]],gnodes[map[fields[1]]])) = 2;//
					level1 = g.getData(gnodes[map[fields[3].substr(1,fields[3].length())]]).level;
					g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
					              gnodes[map[fields[3].substr(1,fields[3].length())]])) = 3;// Back edges to determine fanins -> wt. = 3
				}
				else{
					g.getEdgeData(g.addEdge(gnodes[map[fields[3].substr(0,fields[3].length())]],gnodes[map[fields[1]]])) = 1;
					level1 = g.getData(gnodes[map[fields[3].substr(0,fields[3].length())]]).level;
					g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
						      gnodes[map[fields[3].substr(0,fields[3].length())]])) = 3;// Back edges to determine fanins -> wt. = 3
					
				}
				invert = fields[5].find_first_of("~");
				if (invert != string::npos){
					//remove first and last char b/c '~' to start and ';' at th
					//cout <<fields[5] << " substr"<<fields[5].substr(1,fields[5].length()-1)<<" "<<map[fields[5].substr(1,fields[5].length()-1)]<< " "<< nodes[map[fields[5].substr(1,fields[5].length()-1)]].label_type <<endl;
					g.getEdgeData(g.addEdge(gnodes[map[fields[5].substr(1,fields[5].length()-2)]],gnodes[map[fields[1]]])) = 2;
					level2 = g.getData(gnodes[map[fields[5].substr(1,fields[5].length()-2)]]).level;
					g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
						      gnodes[map[fields[5].substr(1,fields[5].length()-2)]])) = 3;// Back edges to determine fanins -> wt. = 3
				}
				else{
					g.getEdgeData(g.addEdge(gnodes[map[fields[5].substr(0,fields[5].length()-1)]],gnodes[map[fields[1]]])) = 1;
					level2 = g.getData(gnodes[map[fields[5].substr(0,fields[5].length()-1)]]).level;
					g.getEdgeData(g.addEdge(gnodes[map[fields[1]]],
						      gnodes[map[fields[5].substr(0,fields[5].length()-1)]])) = 3;// Back edges to determine fanins -> wt. = 3
				}
			}
		
		if(level1>=level2)
			g.getData(gnodes[map[fields[1]]]).level= level1+1;
		else
			g.getData(gnodes[map[fields[1]]]).level= level2+1;		
	    }

	  }

	//cout << g.getData(gnodes[0]).label_type << endl;
	// Traverse graph
 	for (Graph::iterator ii = g.begin(), ei = g.end(); ii != ei; ++ii) {
   		Graph::GraphNode src = *ii;
		cout <<"src: "<< g.getData(src).label_type;
		cout <<"src: level"<<g.getData(src).level;
   		for (Graph::edge_iterator edge : g.out_edges(src)) {
   			Graph::GraphNode dst = g.getEdgeDst(edge);
     		cout <<" dest: "<< g.getData(dst).label_type;
     		int edgeData = g.getEdgeData(edge);
     		cout << " edge data " << edgeData;
     		//assert(edgeData == 5);
   		}
	   	cout <<endl;
 	}

	return 0;
}
