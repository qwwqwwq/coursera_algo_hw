#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

class AdjList {
    public:
	AdjList();
	void read(const char * fn);
	void load_nodes();
	void add_edge(int a, int b);
	void read_line(string line, vector<int> * tmp );
	void collapse_random_edge();
	int run_karger();
    private:
	int edge_count;
	int node_count;
	vector< vector<int> * > edges;
	vector< vector<int> * > nodes;
	vector< vector<int> * > node_storage;
};

AdjList::AdjList() : edge_count(0), node_count(0) {
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
		nodes.resize(n);
		node_storage.resize(n);
	    };
	    nodes.at(n-1) = new vector<int>;
	    node_storage.at(n-1) = new vector<int>;
	    if (nodes.at(n-1) == NULL) { abort(); };
	    if (node_storage.at(n-1) == NULL) { abort(); };
	    for(vector<int>::iterator it = tmp.begin()+1; it != tmp.end(); ++it) {
		add_edge(n-1, ((*it)-1) );
		nodes.at(n-1)->push_back((*it)-1);
		node_storage.at(n-1)->push_back((*it)-1);
	    };
	tmp.clear();
	};
    };
};

void AdjList::collapse_random_edge() {
    int rand_numb = rand() % (edge_count-1);
    vector<int> * edge = edges.at(rand_numb);
    vector<int> * node1 = nodes.at(edge->at(0)); 
    vector<int> * node2 = nodes.at(edge->at(1)); 
    vector<int> * new_node = new vector<int>; 
    vector<int>::iterator it;
    if ( node1 != node2 ) {
	for( it = node1->begin(); it != node1->end(); ++it) {
	    if( nodes.at(*it) != node1 && nodes.at(*it) != node2 ) {
		new_node->push_back(*it);
	    };
	};
	for( it = node2->begin(); it != node2->end(); ++it) {
	    if( nodes.at(*it) != node1 && nodes.at(*it) != node2 ) {
		new_node->push_back(*it);
	    };
	};
	vector<int>::iterator it = new_node->begin();
	nodes.at(edge->at(0))->assign(it, new_node->end() );

	vector< vector<int> * >::iterator it2;
	for(it2 = nodes.begin(); it2 != nodes.end(); ++it2) {
	    if( *it2 == node2 ) {
		*it2 = node1;
	    };
	};
	node_count--;
    };
    swap( edges.at(rand_numb), edges.at( edge_count-1 ) );
    edge_count--;
};

int AdjList::run_karger() {
    while (node_count > 2 ) {
	collapse_random_edge();
    };
    int output = nodes.at(0)->size();
    load_nodes();
    node_count = nodes.size();
    edge_count = edges.size();
    return output;
};

void AdjList::load_nodes() {
    int i = 0;
    vector< vector<int> * >::iterator it2;
    nodes.clear();
    nodes.resize(node_storage.size());
    vector<int>::iterator it;
    for(it2 = node_storage.begin(); it2 != node_storage.end(); ++it2) {
	nodes.at(i) = new vector<int>;
	if( nodes.at(i) == NULL ) { abort(); };
	nodes.at(i)->assign( it = (*it2)->begin(), (*it2)->end());
	i++;
    };
};

int main( int argc, char ** argv ) {
    AdjList G;
    G.read(argv[1]);
    int lowest = 10000;
    int tmp = 10000;
    for(int i = 0; i < 10000; i++ ) {
	tmp = G.run_karger();
	if( tmp < lowest ) { lowest = tmp; };
    };
    cout << "result " << lowest << endl;
};

