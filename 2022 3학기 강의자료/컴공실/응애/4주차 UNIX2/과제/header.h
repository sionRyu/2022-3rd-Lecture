#include <stdio.h>
#include <stdlib.h>

/* input and output functions: io.c */
/* reads a single integer from FP and returns it */
int scan_integer(FILE* fp);

/* prints the first SIZE elements of an integer array ARRAY to FP  */
void print_integer_array(const int *array, const size_t size, FILE * fp);


/* other application functions: apps.c */
/* count the number of each  digits from 1 to N and returns the array */
int* count_digits(const int n);
