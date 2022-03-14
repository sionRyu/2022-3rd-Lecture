#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

#define PROBABILITY			0.6
#define PRINT_TO_SCREEN		1
#define PRINT_TO_FILE		1
#define OUTPUT_FILE_NAME	"maze.maz"
ofstream file(OUTPUT_FILE_NAME);

void printLine(int N);
void printRow(bool rightWay[], bool downWay[], int N);
int getIndex(int y, int x, int N);
void constructMaze(int currentRow, int M, int N, int previousDisjoint[]);
int main(void) {
	int M, N;
	cin >> N >> M;
	srand((unsigned)time(NULL));
	constructMaze(0, M, N, NULL);
	printLine(N);
	return 0;
}

/**************************************************************
	function	  :	printLine
	description   :	print a single horizontal line
	input	      :	(int) N, number of columns
	return		  :	(none)
***************************************************************/
void printLine(int N) {
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << "+-";
		if(PRINT_TO_SCREEN)
			cout << "+-";
	}
	if(PRINT_TO_FILE)
		file << '+' << endl;
	if(PRINT_TO_SCREEN)
		cout << '+' << endl;
}

/**************************************************************
	function	  :	printRow
	description   :	print grid information fo a row
	input	      :	(int) rightWay[i]: if i-th grid has a way right
					(int) upWay[i]: if i-th grid has a way up
   					(int) N, number of columns
	return		  :	(none)
***************************************************************/
void printRow(bool rightWay[], bool upWay[], int N) {
	if(PRINT_TO_FILE)
		file << '+';
	if(PRINT_TO_SCREEN)
		cout << '+';
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << (upWay[i]?' ':'-') << '+';
		if(PRINT_TO_SCREEN)
			cout << (upWay[i]?' ':'-') << '+';
	}

	if(PRINT_TO_FILE)
		file << endl << '|';
	if(PRINT_TO_SCREEN)
		cout << endl << '|';
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << ' ' << (rightWay[i]?' ':'|');
		if(PRINT_TO_SCREEN)
			cout << ' ' << (rightWay[i]?' ':'|');
	}
	if(PRINT_TO_FILE)
		file << endl;
	if(PRINT_TO_SCREEN)
		cout << endl;
}

/**************************************************************
	function	  :	getIndex
	description   :	get index of grid [y][x]
	input	      :	(int) y, number of current row
					(int) x, number of current column
   					(int) N, number of columns
	return		  :	(int) index of grid[y][x]
***************************************************************/
int getIndex(int y, int x, int N) {
	return y*N+x;
}

/**************************************************************
	function	  :	constructMaze
	description   :	recursively performs Eller's Algorithm
	input	      :	(int) currentRow, number of current row
					(int) M, number of rows
   					(int) N, number of columns
    				(int) previousDisjoint, disjoint set labels
   								  of the previous row cells
	return		  :	(none)
***************************************************************/
void constructMaze(int currentRow, int M, int N, int previousDisjoint[]) {

	int * disjoint = new int[N];	// disjoint[i]: label of disjoint set of grid[currentRow][i]
	bool * rightWay = new bool[N];	// rightWay[i]: grid[currentRow][i] has a way right
	bool * upWay = new bool[N];		// upWay[i]   : grid[currentRow][i] has a way up
	map<int, int> setSize;			// setSize[i] : number of k's such that previousDisjoint[k] = i
	map<int, bool> setFlag;         // setFlag[i] : true if there exists a k such that upway[k] = true, previousDisjoint[k] = i
	
	// initialize; all cells have walls both right and up
	fill_n(rightWay, N, false);
	fill_n(upWay, N, false);
	
	// boundary condition
	if(currentRow == M) {
		delete[] disjoint;
		delete[] rightWay;
		delete[] upWay;
		return;
	}
	
	// special case: the first row
	if(currentRow == 0) {
		// initialize DISJOINT. randomly make horizontal ways.
		for(int i = 0; i < N; i++) {
			disjoint[i] = getIndex(i, 0, N);
			if(i > 0 && 1.0*rand()/RAND_MAX <= PROBABILITY) {
				disjoint[i] = disjoint[i-1];
				rightWay[i-1] = true;
			}
		}
		// print the first row
		printRow(rightWay, upWay, N);
		// release memory no longer used
		delete[] rightWay;
		delete[] upWay;
		// recursively call function for next line
		constructMaze(currentRow+1, M, N, disjoint);
	}

	// trivial case: the second row to the last row
	else {
		// initialize DISJOINT. initialize SET_SIZE.
		for(int i = 0; i < N; i++) {
			disjoint[i] = getIndex(i, currentRow, N);
			setSize[previousDisjoint[i]]++;
		}

		// randomly make vertical ways.
		for(int i = 0; i < N; i++) {
			setSize[previousDisjoint[i]]--;			
			if(1.0*rand()/RAND_MAX <= PROBABILITY || (!setFlag[previousDisjoint[i]] && !setSize[previousDisjoint[i]])) {
				upWay[i] = true;
				setFlag[previousDisjoint[i]] = true;
				disjoint[i] = previousDisjoint[i];
			}
		}
		// release memory no longer used.
		delete[] previousDisjoint;

		// randomly make horizontal ways.
		for(int i = 1; i < N; i++) {
			// if two neighboring cells are in different disjoint sets,
			if(disjoint[i-1] != disjoint[i]) {
				// make way with probability. if this is the last row, we must make way.
				if(1.0*rand()/RAND_MAX <= PROBABILITY || currentRow == M-1) {
					for(int j = 0; j < N; j++) {
						if(disjoint[j] == disjoint[i] && i != j) {
							disjoint[j] = disjoint[i-1];
						}
					}
					disjoint[i] = disjoint[i-1];
					rightWay[i-1] = true;
				}
			}
		}
		// print this row.
		printRow(rightWay, upWay, N);

		// release memory no longer used.
		delete rightWay;
		delete upWay;

		// recursively call function for next line.
		constructMaze(currentRow+1, M, N, disjoint);
	}
}