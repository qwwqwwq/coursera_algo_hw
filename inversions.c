#include <stdio.h>
#include <stdlib.h>

int merge( int * a, int p, int q, int r ) {
    int A_len = (q-p+1);
    int B_len = (r-q);
    int * A = malloc(A_len * sizeof(int) );
    int * B = malloc(B_len * sizeof(int) );
    if( A == NULL || B == NULL ) {
	printf("Failed to allocate sub arrays \n" );
	exit(1);
    };
    int i,j,k;
    i = 0; j = 0;
    for( k = p; k <= r; k++ ) {
	if( k <= q ) {
	    A[i] = a[k];
	    i++;
	} else {
	    B[j] = a[k];
	    j++;
	};
    };

    int inv = 0;
    i = 0; j = 0;
    for( k = p; k <= r; k++ ) {
	if ( ( A[i] <= B[j] && i < A_len ) || j >= B_len ) {
	    a[k] = A[i];
	    i++;
	} else {
	    a[k] = B[j];
	    if( ( i < A_len ) ) {
		inv+=(A_len-i);
	    };
	    j++;
	};
    };
    free(A);
    free(B);
    return inv;
};

void inversions( int * numbers, int p, int r, long int * inv ) {
    if ( p < r && p >= 0 && r > 0) {
	int q = (p+r)/2;
	inversions( numbers, p, q, inv );
	inversions( numbers, q+1, r, inv );
	(*inv)+=merge( numbers, p, q, r );
    };
};

int main( int argc, char ** argv) {
    if ( !(argc-1) ) { return 0; };
    FILE * input;
    input = fopen( argv[1], "r" );
    int number;
    int buf_size = (sizeof(int) * 10000);
    int * numbers = malloc(buf_size);
    if( numbers == NULL ) { 
	printf("Failed to allocate main array." );
	exit(0); 
    };
    int i = 0;
    while (1) {
	fscanf(input, "%d", &number );
	if( feof(input) ) { 
	    break; 
	};
	numbers[i] = number;
	i++;
	if( (sizeof(int)*i) >= buf_size ) {
	    numbers = realloc( numbers, buf_size + ( sizeof(int) * 10000) );
	    if( numbers == NULL ) { 
		printf("Failed to allocate main array.\n" );
		exit(0); 
	    };
	    buf_size += (sizeof(int) * 10000);
	};
    };
    long int inv = 0;
    printf( "data is %d long \n", i );
    inversions( numbers, 0, (i-1), &inv );
    printf("%ld inversions.\n", inv );
    free(numbers);
    return 0;
};
