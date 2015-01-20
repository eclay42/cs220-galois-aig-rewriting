
#include <iostream>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <Galois/Galois.h>
#include <Galois/Graph/Graph.h>
#include "Galois/Statistic.h"
#include <string>
#include <fstream>
#include "Galois/Runtime/ll/PaddedLock.h"
#include <time.h>
using namespace std;
using namespace boost;

typedef enum{pi,po,nd,op} node_type;
typedef Galois::Runtime::LL::PaddedLock<true> Lock_ty;

//Lock_ty mutex;
int refactor_cost = 0;

struct Node {
	string label_type;//a0,b0...
	node_type type;// pi,po,nd
	bool output;
	//Lock_ty& mutex;
	int level;
};

typedef Galois::Graph::FirstGraph<Node,int,true> Graph;

Graph g;

int node_index = 0;
vector<Graph::GraphNode> gnodes;

vector<string> fields;

/*NOT USED: Method to determine nodes with multiple outgoing edges*/
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

/*Class for sorting the nodes at a particular level using priority queue*/
class CompareNodelevel{
	
	public:
		bool operator()(Graph::GraphNode& src1, Graph::GraphNode& src2)
		{
			if(g.getData(src2).level <= g.getData(src1).level)
				return true;
					
			return false;
		}
};

/*Method to determine whether two nodes are equal and hence test for redundancy*/
bool isEqualNodes(Graph::GraphNode gnode1, Graph::GraphNode gnode2){
	int count = 0;
	//cout << "n1: "<<g.getData(gnode1).label_type;
	//cout << " n2: "<<g.getData(gnode2).label_type<<'\n';
	if(g.getData(gnode1).label_type == g.getData(gnode2).label_type){
		//cout<< "same node"<< '\n';
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

/*NOT USED: Method to find the cut from the source node, returns the child nodes in the cut*/
bool find_cut(Graph::GraphNode &top,Graph::GraphNode &child_left,Graph::GraphNode &child_right){
	if(g.getData(top).level != 0 && g.getData(top).level != 1){
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

void parseInput(unordered_map <string, int> &map, unsigned i){
	//Node n;
	for(i;i<fields.size();i++){
		Node n;
		if(fields[i].size()>1){
		  n.type = pi;
		  n.level = 0;
		  size_t semic = fields[i].find_first_of(";");
		  size_t comma = fields[i].find_first_of(",");
		  if (semic != string::npos){
		    n.label_type = fields[i].substr(0,fields[i].length()-1);
			  //remove last char b/c its ',' or ';'
			  map[fields[i].substr(0,fields[i].length()-1)] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));

			  g.addNode(gnodes[gnodes.size()-1]);
		  }
		  else if (comma != string::npos){
			  n.label_type = fields[i].substr(0,fields[i].length()-1);
			  //remove last char b/c its ',' or ';'
			  map[fields[i].substr(0,fields[i].length()-1)] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));

			  g.addNode(gnodes[gnodes.size()-1]);
		  }
		  else{
			  n.label_type = fields[i].substr(0,fields[i].length());
			  //remove last char b/c its ',' or ';'
			  map[fields[i].substr(0,fields[i].length())] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));

			  g.addNode(gnodes[gnodes.size()-1]);		  
		  }
			
			
			//cout << "node index: "<< node_index<<endl;
			node_index++;
		}

	}
}

void parseOutput(unordered_map <string, int> &map, unsigned i){
	//Node n;
	for(i ;i<fields.size();i++){
		Node n;
		if(fields[i].size()>1){
			n.type = pi;
			n.level = 0;
		  size_t semic = fields[i].find_first_of(";");
		  size_t comma = fields[i].find_first_of(",");
		  if (semic != string::npos){
		    n.label_type = fields[i].substr(0,fields[i].length()-1);
			  //remove last char b/c its ',' or ';'
			  map[fields[i].substr(0,fields[i].length()-1)] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));

			  g.addNode(gnodes[gnodes.size()-1]);
		  }

		  else if (comma != string::npos){
		    n.label_type = fields[i].substr(0,fields[i].length()-1);
			  //remove last char b/c its ',' or ';'
			  map[fields[i].substr(0,fields[i].length()-1)] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));

			  g.addNode(gnodes[gnodes.size()-1]);
		  }
		  else{
			  n.label_type = fields[i].substr(0,fields[i].length());
			  //no , or ;
			  map[fields[i].substr(0,fields[i].length())] = node_index;
			  //gnodes[node_index] = g.createNode(nodes[node_index]);
			  gnodes.push_back(g.createNode(n));
			  g.addNode(gnodes[gnodes.size()-1]);		  
		  }
			//cout << "node index: "<< node_index<<endl;
			node_index++;
		}

	}
}

void parseWire(unordered_map <string, int> &map, unsigned i){
	//Node n;
	for(i;i<fields.size();i++){
		Node n;
		if(fields[i].size()>1){
			n.type = nd;
			n.level = -2;
			n.label_type = fields[i].substr(0,fields[i].length()-1);
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

void parseAssign(unordered_map <string, int> &map){
	int level1,level2;
	string f1,f3,f5;

	f1 = fields[1];
	f3 = fields[3];
	//cout << "f1 " <<f1<< " length "<<fields.size()<<'\n';
	if(fields.size()>5){
		f5 = fields[5];
		if(fields[4].compare("|") == 0){

			//Edge weight: 2- Negative 1 - Positive
			//f3 = f3.substr(0,f3.length());
			//remove ';' from end
			size_t tilde = f5.find_first_of(';');
			if(tilde != string::npos){
				f5 = f5.substr(0,f5.length()-1);
			}

			size_t invertf3 = f3.find_first_of('~');
			size_t invertf5 = f5.find_first_of('~');
			//true = has ~
			if(invertf3 != string::npos){
				f3 = f3.substr(1,f3.length());

				g.getEdgeData(g.addEdge(gnodes[map[f3]]//src
								,gnodes[map[f1]])) = 1;//dest
			}
			else
				g.getEdgeData(g.addEdge(gnodes[map[f3]]//src
								,gnodes[map[f1]])) = 2;//dest

			if(invertf5 != string::npos){
				f5 = f5.substr(1,f5.length());
				g.getEdgeData(g.addEdge(gnodes[map[f5]]
								,gnodes[map[f1]])) = 1;
			}
			else
				g.getEdgeData(g.addEdge(gnodes[map[f5]]
								,gnodes[map[f1]])) = 2;

			if(g.getData(gnodes[map[f1]]).type == po)
				g.getData(gnodes[map[f1]]).output = false;

			level1 = g.getData(gnodes[map[f3]]).level;
			level2 = g.getData(gnodes[map[f5]]).level;
			g.getEdgeData(g.addEdge(gnodes[map[f1]]
							,gnodes[map[f3]])) = 3;//back edges to determine fanins -> wt. = 3
			g.getEdgeData(g.addEdge(gnodes[map[f1]]
							,gnodes[map[f5]])) = 3;//back edges to determins fanins -> wt. = 3
		}
		else if(fields[4].compare("&") == 0){
			size_t tilde = f5.find_first_of(';');
			if(tilde != string::npos){
				f5 = f5.substr(0,f5.length()-1);
			}
			size_t invert = f3.find_first_of("~");
			if (invert != string::npos){
				f3 = f3.substr(1,f3.length());
				//addEdge(src,dest)
				//remove the '~' from the start of string
				//map[string] returns node index to find which node
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 2;//
				level1 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]], gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			else{
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 1;
				level1 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]], gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			invert = f5.find_first_of("~");
			if (invert != string::npos){
				f5 = f5.substr(1,f5.length());
				//remove first and last char b/c '~' to start and ';' at th
				//cout <<fields[5] << " substr"<<fields[5].substr(1,fields[5].length()-1)<<" "<<map[fields[5].substr(1,fields[5].length()-1)]<< " "<< nodes[map[fields[5].substr(1,fields[5].length()-1)]].label_type <<endl;
				g.getEdgeData(g.addEdge(gnodes[map[f5]],gnodes[map[f1]])) = 2;
				level2 = g.getData(gnodes[map[f5]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]], gnodes[map[f5]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			else{
				f5 = f5.substr(0,f5.length());
				g.getEdgeData(g.addEdge(gnodes[map[f5]],gnodes[map[f1]])) = 1;
				level2 = g.getData(gnodes[map[f5]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]], gnodes[map[f5]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
		}
		//dealing with space after f1 and no '&' or '|'
		else{
			cout << "do i go here?\n";
			f3 = fields[4];
			size_t invert = f3.find_first_of("~");
			if (invert != string::npos){
				f3 = f3.substr(1,f3.length()-2);
				//remove first and last char b/c '~' to start and ';' at th
				//cout <<fields[5] << " substr"<<fields[5].substr(1,fields[5].length()-1)<<" "<<map[fields[5].substr(1,fields[5].length()-1)]<< " "<< nodes[map[fields[5].substr(1,fields[5].length()-1)]].label_type <<endl;
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 2;
				level2 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
						  gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
			else{
				f3 = f3.substr(0,f3.length()-1);
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 1;
				level2 = g.getData(gnodes[map[f3]]).level;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],
						  gnodes[map[f3]])) = 3;// Back edges to determine fanins -> wt. = 3
			}
		}
	}
	else{
		//need edge between f1 and f3
		//remove ~ if necessary
		//size of 5 means there is ; alone at the end
		//size 4 means abc; connected
		if(fields.size() == 4){
			size_t invert = f3.find_first_of("~");
			if (invert != string::npos){

				f3 = f3.substr(1,f3.length()-2);

				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 2;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],gnodes[map[f3]])) = 3;
				//alex adding levels
				level2 = g.getData(gnodes[map[f3]]).level;
				level1 = g.getData(gnodes[map[f3]]).level;
			}
			else{
				f3 = f3.substr(0,f3.length()-1);
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 1;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],gnodes[map[f3]])) = 3;
				//alex adding levels
				level2 = g.getData(gnodes[map[f3]]).level;
				level1 = g.getData(gnodes[map[f3]]).level;
			}
		}
		else if(fields.size() == 5){
			size_t invert = f3.find_first_of("~");
			if (invert != string::npos){
				f3 = f3.substr(1,f3.length());
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 2;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],gnodes[map[f3]])) = 3;
				//alex adding levels
				level2 = g.getData(gnodes[map[f3]]).level;
				level1 = g.getData(gnodes[map[f3]]).level;
			}
			else{
				//cout << fields[0]<< " f1"<<fields[1]<< " f2"<<fields[2]<< " f3"<<fields[3]<< " f4"<<fields[4]<< "\n";
				//cout << "f3 " << f3<<'\n';
				f3 = f3.substr(0,f3.length());
				//cout << "after f3 " << f3<<'\n';
				g.getEdgeData(g.addEdge(gnodes[map[f3]],gnodes[map[f1]])) = 1;
				g.getEdgeData(g.addEdge(gnodes[map[f1]],gnodes[map[f3]])) = 3;
				//alex adding levels
				level2 = g.getData(gnodes[map[f3]]).level;
				level1 = g.getData(gnodes[map[f3]]).level;
			}
		}
		else{
			cout << "error. hope we dont see\n";
		}
	}


	if(level1>=level2)
		g.getData(gnodes[map[fields[1]]]).level= level1+1;
	else
		g.getData(gnodes[map[fields[1]]]).level= level2+1;

}

void parseFileintoGraph(string inFile, unordered_map <string, int> &map){
	ifstream fin;
	string previous = "";
	//Node output;
	fin.open(inFile); // open a file
	if (!fin.good()){
		cout << "file not found\n";
	}
	else{
		// read each line of the file
		while (!fin.eof()){
			// read an entire line into memory
			string line;
			getline(fin,line);
			trim(line);
			split(fields, line, is_any_of(" "),token_compress_on);
			//Node n;
			if(fields[0].compare("input") == 0){
				parseInput(map, 1);
				previous = "input";
			}
			else if(fields[0].compare("output") == 0){
				parseOutput(map, 1);
				previous = "output";
			}
			else if(fields[0].compare("wire") == 0){
				parseWire(map, 1);
				previous = "wire";
			}
			else if(fields[0].compare("assign") == 0){
				parseAssign(map);
				previous = "assign";
			}
			/*multiple lines gets 2nd line
			 * eg Input a b c d
			 *		e f g h
			 */
			else if(previous.compare("input")==0){
				parseInput(map, 0);
			}
			else if(previous.compare("output")==0){
				parseOutput(map, 0);
			}
			else if(previous.compare("wire")==0){
				parseWire(map, 0);
			}
		}
	}
}


/*Method to fetch the children from the source node*/
bool getChildren(Graph::GraphNode parent, Graph::GraphNode &child1, Graph::GraphNode &child2){
	bool  a = true;
	for (Graph::edge_iterator edge : g.out_edges(parent)){
		if(g.getEdgeData(edge)==3){
			if (a){
				child1 = g.getEdgeDst(edge);
				a = !a;
			}
			else
				child2 = g.getEdgeDst(edge);
		}
	}
	if(child1 == NULL || child2 == NULL)
		return false;
}

bool make_replacement(Graph::GraphNode node, Graph::GraphNode match_node){
	Graph::GraphNode child1,child2,temp;
	if(!getChildren(node,child1,child2))
		return false;

	/*Find the matching node which is redundant and needs to be removed*/
	for(Graph::edge_iterator edge : g.out_edges(match_node)){
		temp = g.getEdgeDst(edge);
		if(!((isEqualNodes(child1,temp))||isEqualNodes(child2,temp))){
			g.getEdgeData(g.addEdge(node,temp))=g.getEdgeData(edge);
			g.getEdgeData(g.addEdge(temp,node))=3;//Assigning back edges from the parent nodes of the removed copy node to the original node
		}
		
	}

	/*Remove the incoming edges as Galois removeNode method deletes only the outgoing edges from the node*/
	g.removeEdge(child1,g.findEdge(child1,match_node));
	g.removeEdge(child2,g.findEdge(child2,match_node));
	g.removeNode(match_node);
	cout<<"Node: label "<<g.getData(node).label_type<<"level: "<<g.getData(node).level<<"Match Node: label "<<g.getData(match_node).label_type<<"level: "<<g.getData(match_node).level<<endl;
	return true;
}

//*Checking whether the cut represents an xnor gate */
bool checkxnor(Graph::GraphNode node){
	Graph::GraphNode inode1=NULL,inode2=NULL;
	Graph::GraphNode input1=NULL,input2=NULL,input3=NULL,input4=NULL;

	if(!getChildren(node,inode1,inode2))
		return false;		

	if(g.getData(inode1).level == 0 || g.getData(inode2).level == 0)
		return false;
	//cout<<g.getData(inode1).label_type<<" "<<g.getData(inode2).label_type<<endl;
	if(!getChildren(inode1, input1, input2))
		return false;

	if(!getChildren(inode2, input3, input4))
		return false;

	/*
	cout << "got children\n";
	cout <<"node "<< g.getData(node).label_type << "\n";
	cout <<"child1 "<< g.getData(inode1).label_type << "\n";
	cout <<"child2 "<< g.getData(inode2).label_type << "\n";
	*/

	if(g.getEdgeData(g.findEdge(inode1,node))==2 && g.getEdgeData(g.findEdge(inode2,node))==2){
		if(isEqualNodes(input1,input3)&&isEqualNodes(input2,input4)){
			if(g.getEdgeData(g.findEdge(input1,inode1))!=g.getEdgeData(g.findEdge(input3,inode2))){
				//cout << "nequal edges\n";
				if(g.getEdgeData(g.findEdge(input2,inode1))!=g.getEdgeData(g.findEdge(input4,inode2))){
					//cout << "true\n";
					return true;
				}
			}
		}
	}
	//cout<<"false\n";
	return false;
}

void convertxor_cost(Graph::GraphNode node){
	Graph::GraphNode inode1=NULL,inode2=NULL;
	Graph::GraphNode input1=NULL,input2=NULL,input3=NULL,input4=NULL;
	string child_label,parent_label;

	/*Find the cut and the child nodes from the source node*/
	if(!getChildren(node,inode1,inode2))
		return;

	if(!getChildren(inode1, input1, input2))
		return;

	if(!getChildren(inode2, input3, input4))
		return;

	/*XNOR cut found replacing it with XOR cut, permuting the inputs and reassigning the edges*/
	//START
	if(g.getEdgeData(g.findEdge(input1,inode1))==2){
		//cout<<"Reassigning edge for "<<g.getData(input1).label_type<<g.getData(inode1).label_type<<endl;
		g.getEdgeData(g.addEdge(input1,inode1))=1;
		//cout<<"Changed edge data "<<g.getEdgeData(g.findEdge(input1,inode1))<<endl;		
	}
	else
		g.getEdgeData(g.addEdge(input2,inode1))=1;
	
	if(g.getEdgeData(g.findEdge(input3,inode2))==1)
		g.getEdgeData(g.addEdge(input3,inode2))=2;
	else
		g.getEdgeData(g.addEdge(input4,inode2))=2;
	//END

	/*Inverting the output edges from the top node in the cut*/
	//START
	for (Graph::edge_iterator edge : g.out_edges(node)){
		//cout<<"Node: "<<g.getData(node).label_type<<" -> "<<g.getData(g.getEdgeDst(edge)).label_type<<"Weight ="<< g.getEdgeData(edge)<<endl;
		if(g.getEdgeData(edge) == 2)
			g.getEdgeData(edge) = 1;
		else if(g.getEdgeData(edge) == 1)
			g.getEdgeData(edge) = 2;
	}

	/*If the top node is primary output, invert the boolean output variable for the node*/ 
	if(g.getData(node).type == po){
		//cout<<"Node: "<<g.getData(node).label_type<<"Output of node:"<<g.getData(node).output<<endl;
		if(g.getData(node).output == true)
			g.getData(node).output = false;
		else
			g.getData(node).output = true;
	}
	//END	
			
	/*Compute cost and removing redundant nodes using make_replacement()*/
	for (Graph::edge_iterator edge : g.out_edges(input1)){
		Graph::GraphNode dst = g.getEdgeDst(edge);
		if((g.getData(dst).label_type != g.getData(inode1).label_type) &&  (g.getData(dst).label_type != g.getData(inode2).label_type)){
			if(isEqualNodes(dst,inode1)){
				//cout<<"Match found: "<< g.getData(dst).label_type <<endl;
				if(make_replacement(inode1,dst))
					refactor_cost++;
			}
			else if(isEqualNodes(dst,inode2)){
				//cout<<"Match found: "<< g.getData(dst).label_type <<endl;
				if(make_replacement(inode2,dst))
					refactor_cost++;
			}
		}
	}
}

#if 0
struct Process { 
	void operator()(Graph::GraphNode src, Galois::UserContext<Graph::GraphNode>& ctx) {  
		//cout<<"Node in process:"<<g.getData(src).label_type<<" "<<"Level:"<<g.getData(src).level<<endl;
		if(checkxnor(src)){
			//cout << "true\n";
			convertxor_cost(src);
		}
	}
};
#endif

struct Process { 
	void operator()(Graph::GraphNode src) { 
		//cout<<"Node in process do_all:"<<g.getData(src).label_type<<" "<<"Level:"<<g.getData(src).level<<endl;
		//mutex.lock();
		if(checkxnor(src)){
			convertxor_cost(src);
		}
		//mutex.unlock();	
	}
};


int main(int argc, char *argv[]) {
	Galois::StatManager statManager;
	unordered_map <string, int> map;
	priority_queue<Graph::GraphNode, vector<Graph::GraphNode>, CompareNodelevel> pq;
	Galois::Timer t;

	if ( argc != 3 ){
    	cout<<"usage: "<< argv[0] <<" <filename> <Num of Threads>\n";
		return 0;
	}
  cout << "Parsing file " << argv[1] << endl;
  	parseFileintoGraph(argv[1],map);
  cout << "Successfully parsed file" << endl;
	unsigned int numThreads = atoi(argv[2]);
	numThreads = Galois::setActiveThreads(numThreads);

	//Print graph first time

	for(Graph::GraphNode src : g){
   		//Graph::GraphNode src = *ii;
		//cout <<"Src: "<< g.getData(src).label_type;
		//cout <<" Level: "<<g.getData(src).level;
		
		if(g.getData(src).level > 1)
			pq.push(src);
  }		
/*
  cout << "P1_do_549"<< g.getData(gnodes[map["P1_do_549"]]).level <<'\n';
  cout << "n282135: "<< g.getData(gnodes[map["n282135"]]).label_type <<'\n';
		for (Graph::edge_iterator edge : g.out_edges(gnodes[map["n282135"]])) {
   			Graph::GraphNode dst = g.getEdgeDst(edge);
	     		cout <<" dest: "<< g.getData(dst).label_type;
     			int edgeData = g.getEdgeData(edge);
     			//Not printing the back edges which is used for accessing children
			if(edgeData != 3){
				cout<<" dest: "<< g.getData(dst).label_type;
				cout<<" edge data: "<< edgeData << '\n';
			}
   		}*/
	   	//cout <<endl;
 	//}


	clock_t cstart = clock();
	t.start();
	clock_t cend = 0;
	int level=2;
	vector<Graph::GraphNode> temp;
	while(!pq.empty()){
		//cout<<"Node inside level"<<level<<":"<<g.getData(pq.top()).label_type<<endl;
		while(g.getData(pq.top()).level==level && !pq.empty()){
			//cout<<"Node inside level while 2 loop "<<level<<":"<<g.getData(pq.top()).label_type<<endl;
			temp.push_back(pq.top());
			pq.pop();
			if(pq.empty()){
				//cout << "empty\n";
				break;
			}
			//cout<<"Next Node inside level "<<level<<" : "<<g.getData(pq.top()).label_type<<" next lvl: "<<g.getData(pq.top()).level<<'\n';
		}
		//cout << "\noutside 2nd while\n";
		//cout<<"Temp size:"<<temp.size()<<endl;
		
			//std::cout <<"Node in temp vector:"<< g.getData(temp[i]).label_type<<" level: "<<level<<endl;
		
		#if 0
        		Galois::for_each(temp.begin(),temp.end(),Process());
		#endif
		Galois::do_all(temp.begin(),temp.end(), Process(),Galois::loopname(""));
		
		temp.erase(temp.begin(),temp.end());
		//cout<<"Before level increment:"<<g.getData(pq.top()).level<<endl;
		level++;
    	}
	cend = clock();
	t.stop();	

	//cout << "Printing reduced graph \n";
	//Print graph after refactoring of nodes
	/*
	for ( Graph::GraphNode src : g){
		//Graph::GraphNode src = *ii;
		cout <<"Src: "<< g.getData(src).label_type;
		cout <<" Level: "<<g.getData(src).level;
		
		if(g.getData(src).type == po)
			cout<<" Primary Output output var: "<<g.getData(src).output;

		for(Graph::edge_iterator edge : g.out_edges(src)){
			Graph::GraphNode dst = g.getEdgeDst(edge);
			//cout<<" dest: "<< g.getData(dst).label_type;
			int edgeData = g.getEdgeData(edge);
			//Not printing the back edges which is used for accessing children
			if(edgeData != 3){
				cout<<" dest: "<< g.getData(dst).label_type;
				cout<<" edge data: "<< edgeData;
			}
		}
		cout <<endl;
 	}
 	*/
	cout << "Number of nodes after: " << g.size() << '\n';
	cout<<"Algorithm execution time: "<<t.get()<<" msec"<<endl;
	cout<<"Number of nodes reduced: "<<refactor_cost<<endl;

	return 0;
}
