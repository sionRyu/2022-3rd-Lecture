#include "header.h"

/* prints the first SIZE elements of an integer array ARRAY to FP */
void print_integer_array(const int *array, const size_t size, FILE* fp) {

	//  if FP does not represent a file, return
	if(fp == NULL) {
		return;
	}

	// print the first SIZE elements of ARRAY
	for(int i = 0; i < size; i++) {
		fprintf(fp, "%d ", array[i]);
	}
	putc('\n', fp);
}

/* reads an integer from FP */
int scan_integer(FILE *fp) {

	// if FP does not represent a file, return
	if(fp == NULL) {
		return 0;
	}

	// read an integer from FP
	int t;
	fscanf(fp, "%d", &t);
	return t;
}
