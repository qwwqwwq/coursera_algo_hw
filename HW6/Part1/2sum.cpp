#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <boost/unordered_map.hpp>

typedef struct hasher {
    size_t operator()(long int v) const {
	return v;
    };
} hasher;

class TwoSum {
    public:
	void read(const char * fn);
	long int read_line( std::string line );
	bool find_sum(long int target);
	int find_range(int lower, int upper);
    private:
	boost::unordered_map<long int,bool,hasher> numbers;
	boost::unordered_map<long int,bool,hasher> multi_numbers;
	std::list<int> numbers_to_check;
};


long int TwoSum::read_line( std::string line ) {
    long int n;
    std::istringstream sstr (line);
    sstr >> n;
    return n;
};

void TwoSum::read(const char * fn) {
    std::ifstream inf (fn);
    std::string line;
    long int n;
    int count = 0;
    while (std::getline(inf, line)) {
	count++;
    };
    numbers.reserve(count);
    count = 0;
    inf.clear();
    inf.seekg(0);
    while (std::getline(inf, line)) {
	n = read_line(line);
	if( numbers.find(n) == numbers.end() ) {
	    numbers[n] = n;
	} else {
	    multi_numbers[n] = true; 
	};
	count++;
    };
};

bool TwoSum::find_sum(long int target) {
    long int bucket;
    for( auto it = numbers_to_check.begin(); it != numbers_to_check.end(); it++ ) {
	if( numbers.find((*it) - target) == numbers.end() ) { continue; };
	bucket = numbers.bucket( (*it) - target );
	for(auto it_local = numbers.begin(bucket); it_local != numbers.end(bucket); ++it_local) {
	    if ( ( it_local->first + target ) != (*it) ) { continue; }; 
	    if ( it_local->first != target || (it_local->first == target && multi_numbers.find(target) != multi_numbers.end()) ) {
		std::cout << target << " + " << it_local->first << " = " << (*it) << " ( " << target + it_local->first << std::endl;
		numbers_to_check.erase(it);
		return true; 
	    };
	};
    };
    return false;
};

int TwoSum::find_range(int lower, int upper) {
    int count = 0;
    numbers_to_check.resize(upper-lower);
    for (int i=lower; i<= upper; i++ ) {
	numbers_to_check.push_back(i);
    };
    for( auto it = numbers.begin(); it != numbers.end(); ++it ) {
	if( find_sum(it->first) ) { 
	    ++count; 
	};
    };	
    return count;
};


int main( int argc, char ** argv ) {
    TwoSum ts;
    ts.read(argv[1]);
    std::cout << ts.find_range( atoi(argv[2]), atoi(argv[3]) ) << std::endl;
};
