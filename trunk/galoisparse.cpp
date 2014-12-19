
#include <iostream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <Galois/Galois.h>
#include <Galois/Graph/Graph.h>

//#include <cstring>
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

typedef Galois::Graph::FirstGraph<Node,int,true> Graph;

Graph g;

int node_index = 0;
vector<Graph::GraphNode> gnodes;

vector<string> fields;

bool checkWorkability(Graph::GraphNode gnode){
	int count = 0;
	for (Graph::edge_iterator edge : g.out_edges(gnode)) {
		int edgedata = g.getEdgeData(edge);
	   	if(edgedata == 1|| edgedata ==2)
	   		count++;
	   	}
	if (count > 1)
		return false;
	else
		return true;
}

bool isEqualNodes(Graph::GraphNode gnode1, Graph::GraphNode gnode2){
	int count = 0;
	if(g.getData(gnode1).label_type==g.getData(gnode2).label_type){
		//cout<< "same node"<<endl;
		return true;
	}

	for (Graph::edge_iterator edge1 : g.out_edges(gnode1)){
		for (Graph::edge_iterator edge2 : g.out_edges(gnode2)){
			if(g.getEdgeData(edge1)== 3)
				if(g.getEdgeData(edge2)== 3)
					if(g.getEdgeDst(edge1)==g.getEdgeDst(edge2)){
						count++;
					}
		}
	}
	if(count == 2)
		return true;
	else
		return false;
}

bool find_cut(Graph::GraphNode &top,Graph::GraphNode &child_left,Graph::GraphNode &child_right)
{
	if ( g.getData(top).level != 0 && g.getData(top).level != 1) {
		for (Graph::edge_iterator edge : g.out_edges(top)){
			Graph::GraphNode dst = g.getEdgeDst(edge);
			if(g.getEdgeData(edge) == 3){	
				if(child_left==NULL){
					child_left=dst;
				}	
				else{
					child_right=dst;
				}
			}
		}
		return true;			
	}
	else 
		return false;
}

void parseFileintoGraph(string inFile, unordered_map <string, int> &map){
	ifstream fin;
	fin.open(inFile); // open a file
	if (!fin.good()){
		cout << "file not found\n";

	}
	else
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
		string f1,f3,f5;
		f1 = fields[1];
		f3 = fields[3];
		f5 = fields[5];
		if(fields[4].compare("|") == 0)
		{	//Edge weight: 2- Negative 1 - Positive
			f3 = f3.substr(0,f3.length());
			f5 = f5.substr(0,f5.length()-1);
			g.getEdgeData(g.addEdge(gnodes[map[f3]]//src
										   ,gnodes[map[f1]])) = 2;//dest

			g.getEdgeData(g.addEdge(gnodes[map[f5]]
										   ,gnodes[map[f1]])) = 2;

			level1 = g.getData(gnodes[map[f3]]).level;
			level2 = g.getData(gnodes[map[f5]]).level;
			g.getEdgeData(g.addEdge(gnodes[map[f1]],
					  gnodes[map[f3]])) = 3;//back edges to determine fanins -> wt. = 3
			g.getEdgeData(g.addEdge(gnodes[map[f1]],
					  gnodes[map[f5]])) = 3;//back edges to determins fanins -> wt. = 3
		}
	
		else
		{	size_t invert = fields[3].find_first_of("~");
			if (invert != string::npos){
				f3 = f3.substr(1,f3.length());
				//addEdge(src,dest)
				//remove the '~' from the start of string
				//map[string] returns node index to find which node
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 2;//
				level1 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
							  gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			else{
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 1;
				level1 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
						  gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3

			}
			invert = fields[5].find_first_of("~");
			if (invert != string::npos){
				f5 = f5.substr(1,f5.length()-2);
				//remove first and last char b/c '~' to start and ';' at th
				//cout <<fields[5] << " substr"<<fields[5].substr(1,fields[5].length()-1)<<" "<<map[fields[5].substr(1,fields[5].length()-1)]<< " "<< nodes[map[fields[5].substr(1,fields[5].length()-1)]].label_type <<endl;
				g.getEdgeData(g.addEdge(gnodes[map[f5]],gnodes[map[f1]])) = 2;
				level2 = g.getData(gnodes[map[f5]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
						  gnodes[map[f5]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			else{
				f5 = f5.substr(0,f5.length()-1);
				g.getEdgeData(g.addEdge(gnodes[map[f5]],gnodes[map[f1]])) = 1;
				level2 = g.getData(gnodes[map[f5]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
						  gnodes[map[f5]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
		}

	if(level1>=level2)
		g.getData(gnodes[map[fields[1]]]).level= level1+1;
	else
		g.getData(gnodes[map[fields[1]]]).level= level2+1;
	}

	}
}

bool checkxor(Graph::GraphNode node){
	Graph::GraphNode inode1=NULL,inode2=NULL;
	Graph::GraphNode input1=NULL,input2=NULL,input3=NULL,input4=NULL;


}

void getChildren(Graph::GraphNode parent, Graph::GraphNode &child1, Graph::GraphNode &child2){
	for (Graph::edge_iterator edge : g.out_edges(node)){
			if(g.getEdgeData(edge)==3){
				if (inode1==NULL)
					inode1 = g.getEdgeDst(edge);
				else
					inode2 = g.getEdgeDst(edge);
			}
		}
}

int main(int argc, char *argv[]) {
	unordered_map <string, int> map;
	if ( argc != 2 )
    		cout<<"usage: "<< argv[0] <<" <filename>\n";
  	else
  			parseFileintoGraph(argv[1],map);

	//cout << g.getData(gnodes[0]).label_type << endl;
	// Traverse graph
 	//for (Graph::iterator ii = g.begin(), ei = g.end(); ii != ei; ++ii) {
	for ( Graph::GraphNode src : g){
   		//Graph::GraphNode src = *ii;
		cout <<"src: "<< g.getData(src).label_type;
		cout <<" level: "<<g.getData(src).level;

		for (Graph::edge_iterator edge : g.out_edges(src)) {
   			Graph::GraphNode dst = g.getEdgeDst(edge);
     		cout <<" dest: "<< g.getData(dst).label_type;
     		int edgeData = g.getEdgeData(edge);
     		cout << " edge data " << edgeData;
     		//assert(edgeData == 5);
   		}
	   	cout <<endl;
 	}
 	if(checkWorkability(gnodes[map["n15"]]))
 		cout << "1 output"<<endl;
 	else
 		cout <<"more than 1 output"<<endl;

	bool out;
	Graph::GraphNode child_left=NULL,child_right=NULL;
	if(find_cut(gnodes[map["s0"]],child_left,child_right)){
		cout<<"Src node:"<<g.getData(gnodes[map["s0"]]).label_type << endl;
		cout<<"Child node 1:"<<g.getData(child_left).label_type << endl;
		cout<<"Child node 2:"<<g.getData(child_right).label_type << endl;
	}

	if(isEqualNodes(gnodes[map["n8"]],gnodes[map["n8"]]))
		cout << "equal nodes"<<endl;

	return 0;
}
