#include <iostream>
#include <numeric>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>


typedef struct HeapCmp {
    bool operator ()(const int first, const int second) const {
	if ( max ) {
	    return (first < second);
	} else {
	    return (first > second);
	};
    }; 
    bool max;
} HeapCmp;

class Medians {
    public:
	int read(const char * fn);
	int read_line( std::string line );
    private:
	std::vector<int> min_heap;
	std::vector<int> max_heap;
	std::vector<int> medians;
};

int Medians::read_line( std::string line ) {
    int n;
    std::istringstream sstr (line);
    sstr >> n;
    return n;
};

int Medians::read(const char * fn) {
    std::ifstream inf (fn);
    std::string line;
    std::vector<int> *heap, *other_heap;
    HeapCmp *cmp, *other_cmp;
    HeapCmp max_heap_cmp;
    max_heap_cmp.max = false;
    HeapCmp min_heap_cmp;
    min_heap_cmp.max = true;
    int n;
    int count = 0;
    while (std::getline(inf, line)) {
	n = read_line(line);
	if( !count ) {
	    max_heap.push_back(n);
	    medians.push_back(n);
	    count++;
	    continue;
	};
	if ( n < max_heap.at(0) ) {
	    cmp = &min_heap_cmp;
	    other_cmp = &max_heap_cmp;
	    heap = &min_heap;
	    other_heap = &max_heap;
	} else {
	    cmp = &max_heap_cmp;
	    other_cmp = &min_heap_cmp;
	    heap = &max_heap;
	    other_heap = &min_heap;
	};
	heap->push_back(n); 
	std::push_heap (heap->begin(),heap->end(), (*cmp));
	switch (heap->size() - other_heap->size()) {
	    case 2:
		std::pop_heap (heap->begin(),heap->end(),(*cmp)); 
		other_heap->push_back( heap->back() );
		heap->pop_back();
		std::push_heap (other_heap->begin(),other_heap->end(), (*other_cmp));
		medians.push_back( min_heap.at(0) );
		break;
	    case 1:
		medians.push_back( heap->at(0) );
		break;
	    case 0:
		medians.push_back( min_heap.at(0) );
		break;
	};
    };
    long long int sum = std::accumulate(medians.begin(),medians.end(),0);
    return (int)(sum % 10000);
};


int main( int argc, char ** argv ) {
    Medians m;
    std::cout << m.read(argv[1]) << std::endl;
};
