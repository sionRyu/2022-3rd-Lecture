#include "header.h"

/* count the number of each digit from 1 to N and return array */
int * count_digits(const int n) {
	int *array = (int *)calloc(n, sizeof(9));

	// check for memory allocation
	if(array == NULL) {
		fprintf(stderr, "Memory allocation error\n");
		exit(-1);
	}

	for(int i = 1; i < n; i++) {
		int t = i;
		// check each digit of I and count it on ARRAY
		while(t) {
			array[t%10]++;
		t /= 10;
		}
	}

	return array;
}
