#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <boost/unordered_map.hpp>

class Node {
    public:
	void add_edge( int E );
	void add_reverse_edge( int E );
	std::vector<int> edges;
	std::vector<int> redges;
};

void Node::add_edge(int E) {
    edges.push_back(E);
};

void Node::add_reverse_edge(int E) {
    redges.push_back(E);
};

class Graph {
    public:
	void read(const char * fn);
	void read_line( std::string line, std::vector<int> * tmp );
	void DFS();
	void DFS2();
	void DFS_visit(int i);
	void sort_nodes();
	bool reverse;
    private:
	std::vector<Node> nodes;
	std::vector<bool> visited;
	std::vector<int> times;
	boost::unordered_map<int,int> node_order;
	boost::unordered_map<int,int> times_to_node;
	int t;
	int node_number;
	int current_scc_size;
};

void Graph::read_line( std::string line, std::vector<int> * tmp ) {
    int n;
    std::istringstream sstr (line);
    while(sstr >> n) {
	tmp->push_back(n);
    };
};

void Graph::read(const char * fn) {
    std::ifstream inf (fn);
    std::string line;
    int n;
    node_number = 0;
    std::vector<int> tmp;
    while (std::getline(inf, line)) {
	read_line(line, &tmp);
	if ( tmp.size() == 2 ) {
	    if( node_order.find(tmp.at(0)) == node_order.end() ) {
		node_order[tmp.at(0)] = node_number;
		node_number++;
	    };
	    if( node_order.find(tmp.at(1)) == node_order.end() ) {
		node_order[tmp.at(1)] = node_number;
		node_number++;
	    };
	};
	tmp.clear();
    };
    nodes.resize(node_number);
    times.resize(node_number);
    visited.insert(visited.begin(), node_number, false);
    inf.clear();
    inf.seekg(0);
    while (std::getline(inf, line)) {
	read_line(line, &tmp);
	if ( tmp.size() == 2 ) {
	    nodes.at(node_order[tmp.at(1)]).add_reverse_edge(node_order[tmp.at(0)]);
	    nodes.at(node_order[tmp.at(0)]).add_edge(node_order[tmp.at(1)]);
	    };
	tmp.clear();
    };
    node_order.clear();
};

void Graph::DFS() {
    reverse = true;
    t = 0;
    for(int i = 0; i < nodes.size(); i++) {
	if( !visited.at(i) ) {
	    DFS_visit(i);
	};
    };
    visited.assign(node_number, false);
};

void Graph::DFS2() {
    reverse = false;
    int node_idx;
    int SCC_number = 0;
    for(int i = (node_number-1); i >= 0; i--) {
	node_idx = times_to_node[i];
	if( !visited.at(node_idx) ) {
	    current_scc_size = 0;
	    DFS_visit(node_idx);
	    std::cout << "size of SCC " << SCC_number << ": " << current_scc_size << std::endl;
	    SCC_number++;
	};
    };
};

void Graph::DFS_visit(int i) {
    if ( !reverse ) { current_scc_size++; };
    visited.at(i) = true;
    std::vector<int>::iterator it,itstart,itend;
    if ( reverse ) {
	itstart = nodes.at(i).redges.begin();
	itend = nodes.at(i).redges.end();
    } else {
	itstart = nodes.at(i).edges.begin();
	itend = nodes.at(i).edges.end();
    };
    for( it = itstart; it != itend; ++it ) { 
	if( !visited.at(*it) ) {
	    DFS_visit(*it);
	};
    };
    if ( reverse ) {
	times.at(i) = t;
	times_to_node[t] = i;
	t++;
    };
};

int main( int argc, char ** argv ) {
    Graph G;
    G.read(argv[1]);
    G.DFS();
    G.DFS2();
};

