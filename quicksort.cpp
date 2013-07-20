#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

enum PartitionMethod { FIRST, LAST, THREE };

vector<int> * get_numbers( char * fn ) {
    vector<int> * numbers = new vector<int>;
    ifstream infile(fn);
    int a;
    while (infile >> a ) {
	numbers->push_back(a);
    };
    return numbers;
};

int median_of_three(vector<int> * A, int p, int r) {
    int m = (p+r)/2;
    if(A->at(p) < A->at(m))
    {
        if(A->at(p) >= A->at(r))
            return p;
        else if(A->at(m) < A->at(r))
            return m;
    }
    else
    {
        if(A->at(p) < A->at(r))
            return p;
        else if(A->at(m) >= A->at(r))
            return m;
    }
    return r;
};


int partition( vector<int> * A, int p, int r, PartitionMethod m ) {
    int q;
    switch (m) {
	case FIRST:
	    q = p;
	    break;
	case LAST:
	    q = r;
	    break;
	case THREE:
	    q = median_of_three(A, p, r);
	    break;
    };
    swap( A->at(q), A->at(p) ); //move pivot to front
    int i = (p+1);
    int j;
    for (j=i; j <= r; j++) {
	if (A->at(j) < A->at(p)) {
	    swap( A->at(i), A->at(j) );
	    i++;
	};
    };
    swap( A->at(p), A->at(i-1) );
    return (i-1);
};

void quicksort( vector<int> * A, int p, int r, PartitionMethod m, long int * number_cmps ) {
    if( p < r ) {
	int q = partition( A, p, r, m );
	(*number_cmps) += (r-p);
	quicksort( A, p, q-1, m, number_cmps );
	quicksort( A, q+1, r, m, number_cmps );
    };
};

int main( int argc, char ** argv ) {
    PartitionMethod m;
    char * fn;
    int c;
    while ((c = getopt (argc, argv, "123f:")) != -1) {
	switch (c) {
	    case '1':
		m = FIRST;
		break;
	    case '2':
		m = LAST;
		break;
	    case '3':
		m = THREE;
		break;
	    case 'f':
		fn = optarg;
		break;
	    case '?':
		if (optopt == 'f')
		     fprintf (stderr, "Option -%c requires an argument.\n", optopt);
		else if (isprint (optopt))
		     fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		else
		    fprintf (stderr,
			"Unknown option character `\\x%x'.\n",
			optopt);
	        return 1;
	    default:
		abort ();
	};
    };
    vector<int>* numbers = get_numbers( fn );
    long int number_cmps = 0;
    quicksort( numbers, 0, numbers->size() - 1, m, &number_cmps );
    cout << "comparisons: " << number_cmps << "\n";
};

