#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

class AdjList {
    public:
	AdjList();
	~AdjList();
	void read(const char * fn);
	void view_data();
	void add_edge(int a, int b);
	void read_line(string line, vector<int> * tmp );
	void collapse_random_edge();
	int run_karger();
    private:
	int edge_count;
	int node_count;
	vector< vector<int> * > edges;
	vector< vector<int> * > nodes;
};

AdjList::AdjList() : edge_count(0), node_count(0) {
};

AdjList::~AdjList() {
//    vector< vector<int> * >::iterator it;
//    for(it = edges.begin(); it != edges.end(); ++it) {
//	delete *it;
//    };
//    for(it = nodes.begin(); it != nodes.end(); ++it) {
//	delete *it;
//    };
};

void AdjList::add_edge( int a, int b ) {
    vector<int> * edge = new vector<int> (2);
    if ( edge == NULL ) { abort(); };
    edge->at(0) = a;
    edge->at(1) = b;
    edges.push_back(edge);
    edge_count++;
};

void AdjList::read_line( string line, vector<int> * tmp ) {
    int n;
    istringstream sstr (line);
    while(sstr >> n) {
	cout << "read " << n << endl;
	tmp->push_back(n);
    };
};

void AdjList::read(const char * fn) {
    ifstream inf (fn);
    string line;
    int n;
    vector<int> tmp;
    while (getline(inf, line)) {
	read_line(line, &tmp);
	if ( tmp.size() ) {
	    n = tmp.at(0);
	    node_count++;
	    if ( n > nodes.size() ) {
		cout << "resize to " << n << endl;
		nodes.resize(n);
	    };
	    cout << "add node at idx " << n-1 << endl;
	    nodes.at(n-1) = new vector<int>;
	    if (nodes.at(n-1) == NULL) { abort(); };
	    for(vector<int>::iterator it = tmp.begin()+1; it != tmp.end(); ++it) {
		cout << "add " << *it << " to node " << n << "of size" << nodes.at(n-1)->size() << endl;
		add_edge(n-1, ((*it)-1) );
		nodes.at(n-1)->push_back((*it)-1);
	    };
	tmp.clear();
	};
    };
};

void AdjList::collapse_random_edge() {
    cout << "in collapse_random_edge" << endl;
    int rand_numb = rand() % (edge_count - 1);
    vector<int> * edge = edges.at(rand_numb);
    vector<int> * node1 = nodes.at(edge->at(0)); 
    vector<int> * node2 = nodes.at(edge->at(1)); 
    vector<int> * new_node = new vector<int>; 
    cout << "in collapse_random_edge" << edge->at(0) << " " << edge->at(1) << endl;
    node2->end();
    if ( node1 != node2 ) {
	for(vector<int>::iterator it = node1->begin(); it != node1->end(); ++it) {
	    if( nodes.at(*it) != node1 && nodes.at(*it) != node2 ) {
		new_node->push_back(*it);
	    };
	};
	for(vector<int>::iterator it = node2->begin(); it != node2->end(); ++it) {
	    if( nodes.at(*it) != node1 && nodes.at(*it) != node2 ) {
		new_node->push_back(*it);
	    };
	};
	//delete node2;
	nodes.at(edge->at(0)) = new_node;
	nodes.at(edge->at(1)) = new_node;
	node_count--;
    };
    swap( edges.at(rand_numb), edges.at( edge_count-1 ) );
    edge_count--;
    view_data();
};

int AdjList::run_karger() {
    while (node_count > 2 ) {
	cout << "node count is " << node_count << endl;
	collapse_random_edge();
    };
    return nodes.at(0)->size();
};

void AdjList::view_data() {
    cout << "data " << endl;
    int j;
    int i;
    for(i = 0; i < nodes.size(); i++ ) {
	cout << "node size is " << nodes[i]->size() << " " << nodes[i] << endl;
	for( j = 0; j < nodes[i]->size(); j++ ) {
	    cout << nodes[i]->at(j) << endl;
	};
    };
    cout << "/data " << endl;
    cout << "edges" << endl;
    for(i = 0; i < edge_count; i++ ) {
	cout << edges[i]->at(0) << " " << edges[i]->at(1) << endl;
    };
    cout << "/edges" << endl;
};

int main( int argc, char ** argv ) {
    AdjList G;
    G.read(argv[1]);
    G.view_data();
    cout << "result " << G.run_karger() << endl;
};

