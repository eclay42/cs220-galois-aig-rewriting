
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
#define MAX_NODES 10000

struct Node{
	int id;
};

int main() {

	unordered_map <string, int> map;
	//unordered_map<string, int> map;
	int node_index = 0;

	typedef Galois::Graph::FirstGraph<Node,int,true> Graph;

	Graph g;
	Node nodes[MAX_NODES];
	Graph::GraphNode gnodes[MAX_NODES];

	vector<string> fields;

	ifstream fin;
	fin.open("/home/alex/workspace/galoisparse/test.v"); // open a file
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

	    if(fields[0].compare("input") == 0 || fields[0].compare("output") == 0||fields[0].compare("wire") == 0)
	    {
	    	for(unsigned i =1;i<fields.size();i++){
	    		//add to hash map to index to corresponding node in array
	    		if(fields[i].size()>1)
	    		{
	    			//remove last char b/c its ',' or ';'
	    			map[fields[i].substr(0,fields[i].length()-1)] = node_index;
	    			gnodes[node_index] = g.createNode(nodes[node_index]);

	    			node_index++;
	    		}

	    	}
	    }
	    if(fields[0].compare("assign") == 0)
	    {
	    	size_t invert = fields[3].find_first_of("~");
	    	if (invert != string::npos){
	    		//addEdge(src,dest)
	    		//remove the '~' from the start of string
	    		//map[string] returns node index to find which node in gnodes
	    		g.addEdge(gnodes[map[fields[3].substr(1,fields[3].length())]],gnodes[map[fields[1]]]);
	    		//TODO add edge weights
	    	}
	    	invert = fields[5].find_first_of("~");
	    	if (invert != string::npos){
	    		//remove first and last char b/c '~' to start and ';' at the end
	    		g.addEdge(gnodes[map[fields[5].substr(1,fields[3].length()-1)]],gnodes[map[fields[1]]]);
	    	}

	    }

	  }
/*
	  for(unsigned i = 0; i<map.bucket_count();i++){
		  std::cout << "bucket #" << i << " contains:";
		      for ( auto local_it = map.begin(i); local_it!= map.end(i); ++local_it )
		        std::cout << " " << local_it->first << ":" << local_it->second;
		      std::cout << std::endl;
	  }
	  */
	  //cout << map["n12,"]<<"\n"<<map["n11,"];

	return 0;
}
