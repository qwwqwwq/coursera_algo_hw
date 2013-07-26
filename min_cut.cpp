#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class AdjList {
    public:
	AdjList();
	~AdjList();
	void read(const char * fn);
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
    vector< vector<int> * >::iterator it;
    for(it = edges.begin(); it != edges.end(); ++it) {
	delete *it;
    };
    for(it = nodes.begin(); it != nodes.end(); ++it) {
	delete *it;
    };
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
	    nodes.at(n-1) = new vector<int>;
	    if (nodes.at(n-1) == NULL) { abort(); };
	    for(vector<int>::iterator it = tmp.begin()+1; it != tmp.end(); ++it) {
		cout << "add " << *it << " to node " << n << "of size" << nodes.at(n-1)->size() << endl;
		add_edge(n-1, ((*it)-1) );
		nodes.at(n-1)->push_back(*it);
	    };
	tmp.clear();
	};
    };
};

void AdjList::collapse_random_edge() {
    cout << "in collapse_random_edge" << endl;
    int rand_numb = edge_count - 1;
    vector<int> * edge = edges.at(rand_numb);
    cout << "in collapse_random_edge edge is " << rand_numb << " " << edge->at(0) << " " << edge->at(1) << " " << nodes.size() << endl;
    cout << "in collapse_random_edge nodes are" << nodes.at(edge->at(0)) << " " << nodes.at(edge->at(1)) << endl;
    if ( nodes.at(edge->at(0)) != nodes.at(edge->at(1)) ) {
	for(vector<int>::iterator it = nodes.at(edge->at(1))->begin(); it != nodes.at(edge->at(1))->end(); ++it) {
	    if( (*it)-1 != edge->at(0) && (*it)-1 != edge->at(1) ) {
		nodes.at(edge->at(0))->push_back((*it)-1);
	    };
	};
	delete nodes.at(edge->at(1));
	nodes.at(edge->at(1)) = nodes.at(edge->at(0));
	node_count--;
    } else {
	cout << "SAME" << endl;
    };
    swap( edges.at(rand_numb), edges.at( edge_count-1 ) );
    edge_count--;
};

int AdjList::run_karger() {
    while (node_count > 2 ) {
	cout << "node count is " << node_count << endl;
	collapse_random_edge();
    };
    return nodes.at(0)->size();
};

int main( int argc, char ** argv ) {
    AdjList G;
    G.read(argv[1]);
    cout << G.run_karger() << endl;
};

