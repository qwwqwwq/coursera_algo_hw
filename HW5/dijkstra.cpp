#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <boost/unordered_map.hpp>
#include <limits>
#include <algorithm>

class Edge {
    public:
	Edge( int, int );
	int destination;
	int dist;
};

Edge::Edge( int to, int d) {
    destination = to;
    dist = d;
};

class Node {
    public:
	Node(std::string);
	void add_edge( int to, int d );
	std::vector<Edge> edges;
	int dist;
	bool popped;
	std::string name;
};

class node_cmp {
    public:
	bool operator() (const Node * n1, const Node * n2) const {
	    return (n1->dist > n2->dist);
	};
};


Node::Node( std::string n ) : dist(std::numeric_limits<int>::max()) {
    name = n;
};

void Node::add_edge(int to, int d) {
    edges.push_back(Edge(to, d));
};

class Graph {
    public:
	void read(const char * fn);
	void read_line( std::string line, std::vector<std::string> * tmp );
	void djikstra();
    private:
	std::vector<Node *> nodes;
	std::vector<Node *> Q;
	std::vector<Node *> previous;
	boost::unordered_map<std::string,int> node_order;
	int t;
	int node_number;
};

void Graph::read_line( std::string line, std::vector<std::string> * tmp ) {
    std::string n;
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
    std::string child;
    int dist;
    std::vector<std::string> tmp;
    std::vector<std::string>::iterator it;
    while (std::getline(inf, line)) {
	read_line(line, &tmp);
	if( node_order.find(tmp.at(0)) == node_order.end() ) {
	    node_order[tmp.at(0)] = node_number;
	    node_number++;
	};
	for( it = tmp.begin()+1; it != tmp.end(); it++ ) {
	    child = (*it).substr(0, (*it).find(","));
	    if( node_order.find(child) == node_order.end() ) {
		node_order[child] = node_number;
		node_number++;
	    };
	};
	tmp.clear();
    };
    nodes.insert(nodes.begin(), node_number, NULL);
    Q.insert(Q.begin(), node_number, NULL);
    previous.insert(previous.begin(), node_number, NULL);
    inf.clear();
    inf.seekg(0);
    while (std::getline(inf, line)) {
	read_line(line, &tmp);
	if( nodes.at(node_order[tmp.at(0)]) == NULL ) {
	    nodes.at(node_order[tmp.at(0)]) = new Node( tmp.at(0) );
	    if ( nodes.at(node_order[tmp.at(0)]) == NULL ) { abort(); };
	    Q.at(node_order[tmp.at(0)]) = nodes.at(node_order[tmp.at(0)]);
	};
	for( it = tmp.begin()+1; it != tmp.end(); it++ ) {
	    child = (*it).substr(0, (*it).find(","));
	    (*it).erase(0, (*it).find(",")+1 );
	    dist = atoi((*it).c_str());
	    if( nodes.at(node_order[child]) == NULL ) {
		nodes.at(node_order[child]) = new Node( child );
		if ( nodes.at(node_order[child]) == NULL ) { abort(); };
		Q.at(node_order[child]) = nodes.at(node_order[child]);
	    };
	    nodes.at(node_order[tmp.at(0)])->add_edge(node_order[child], dist);
	};
	tmp.clear();
    };
    node_order.clear();
};

void Graph::djikstra () {
    Node * v;
    nodes.at(0)->dist = 0;
    std::cout << "Source node is: " << nodes.at(0)->name << std::endl;
    std::make_heap( Q.begin(), Q.end(), node_cmp() );
    int alt;
    while( !Q.empty() ) {
	std::pop_heap (Q.begin(),Q.end());
	v = Q.back();
	v->popped = true;
	Q.pop_back();
	if( v->dist == std::numeric_limits<int>::max() ) { break; };
	std::vector<Edge>::iterator it;
	for(it = v->edges.begin(); it != v->edges.end(); it++ ) {
	    alt = v->dist + (*it).dist;
	    if ( alt < nodes.at((*it).destination)->dist ) {
		nodes.at((*it).destination)->dist = alt;
		previous.at((*it).destination) = v;
		std::make_heap( Q.begin(), Q.end(), node_cmp() );
	    };
	};
    };
    for( std::vector<Node *>::iterator it2 = nodes.begin(); it2 != nodes.end(); it2++ ) {
	std::cout << "Node: " << (*it2)->name << "\tDist: " << (*it2)->dist << std::endl;
    };
};

int main( int argc, char ** argv ) {
    Graph G;
    G.read(argv[1]);
    G.djikstra();
};
