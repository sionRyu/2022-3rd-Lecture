#include "header.h"

int main(void) {
	int tc;
	tc = scan_integer(stdin);
	while(tc--) {
		int n = scan_integer(stdin);
		int * array = count_digits(n);
		print_integer_array(array, 9, stdout);
		free(array);
	}
	return 0;
}
