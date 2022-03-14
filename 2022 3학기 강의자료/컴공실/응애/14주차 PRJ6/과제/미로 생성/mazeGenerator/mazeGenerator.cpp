#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>

struct cell {
	bool right; // TRUE if cell has way on right
	bool up;	// TRUE if cell has way above
};
cell ** maze;

#define PROBABILITY			0.5
#define PRINT_TO_SCREEN		1
#define PRINT_TO_FILE		1
#define OUTPUT_FILE_NAME	"maze.maz"
std::ofstream file(OUTPUT_FILE_NAME);
int M, N;

void printLine();
void printRow(cell line[]);
int getIndex(int y, int x);
void constructMaze(int currentRow, int previousDisjoint[]);
void deleteWalls(int numberToDelete);
int main(void) {
	std::cin >> N >> M;

	// initialize maze data
	srand((unsigned)time(NULL));
	maze = new cell *[M];
	for (int i = 0; i < N; i++) {
		maze[i] = new cell[N];
		for (int j = 0; j < M; j++) {
			maze[i][j].right = false;
			maze[i][j].up = false;
		}
	}

	// construct perfect maze
	constructMaze(0, NULL);

	// delete walls to make imperfect maze
	deleteWalls(std::min(M, N) / 2);

	// print maze
	for (int i = 0; i < M; i++) {
		printRow(maze[i]);
	}
	printLine();

	// deallocate memory
    for(int i = 0; i < N; i++) {
        delete[] maze[i];
    }
    delete maze;
	return 0;
}

/**************************************************************
	function	  :	printLine
	description   :	print a single horizontal line
	input	      :	(none)
	return		  :	(none)
***************************************************************/
void printLine() {
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << "+-";
		if(PRINT_TO_SCREEN)
			std::cout << "+-";
	}
	if(PRINT_TO_FILE)
		file << "+\n";
	if(PRINT_TO_SCREEN)
		std::cout << "+\n";
}

/**************************************************************
	function	  :	printRow
	description   :	print grid information fo a row
	input	      :	(cell []) line[i]: wall info of cell i
	return		  :	(none)
***************************************************************/
void printRow(cell line[]) {
	if(PRINT_TO_FILE)
		file << '+';
	if(PRINT_TO_SCREEN)
		std::cout << '+';
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << (line[i].up?' ':'-') << '+';
		if(PRINT_TO_SCREEN)
			std::cout << (line[i].up?' ':'-') << '+';
	}

	if(PRINT_TO_FILE)
		file << "\n|";
	if(PRINT_TO_SCREEN)
		std::cout << "\n|";
	for(int i = 0; i < N; i++) {
		if(PRINT_TO_FILE)
			file << ' ' << (line[i].right?' ':'|');
		if(PRINT_TO_SCREEN)
			std::cout << ' ' << (line[i].right?' ':'|');
	}
	if(PRINT_TO_FILE)
		file << std::endl;
	if(PRINT_TO_SCREEN)
		std::cout << std::endl;
}

/**************************************************************
	function	  :	getIndex
	description   :	get index of grid [y][x]
	input	      :	(int) y, number of current row
					(int) x, number of current column
	return		  :	(int) index of grid[y][x]
***************************************************************/
int getIndex(int y, int x) {
	return x*N+y;
}

/**************************************************************
	function	  :	constructMaze
	description   :	recursively performs Eller's Algorithm
	input	      :	(int) currentRow, number of current row
    				(int) previousDisjoint, disjoint set labels
   								  of the previous row cells
	return		  :	(none)
***************************************************************/
void constructMaze(int currentRow, int previousDisjoint[]) {

    // disjoint[i]: label of disjoint set of maze[currentRow][i]
    int * disjoint = new int[N];

    // setSize[i] : number of k's such that previousDisjoint[k] = i
    // setFlag[i] : TRUE if there exists a k such that maze[currentRow][k].up = TRUE and previousDisjoint[k] = i
	std::map<int, int> setSize;
	std::map<int, bool> setFlag;

	// boundary condition
	if(currentRow == M) {
		delete[] disjoint;
		return;
	}

	// special case: the first row
	if(currentRow == 0) {
		// initialize DISJOINT. randomly make horizontal ways.
		for(int i = 0; i < N; i++) {
			disjoint[i] = getIndex(i, 0);
			if(i > 0 && 1.0*rand()/RAND_MAX <= PROBABILITY) {
				for (int j = 0; j < N; j++) {
					if (disjoint[j] == disjoint[i] && i != j) {
						disjoint[j] = disjoint[i - 1];
					}
				}
				disjoint[i] = disjoint[i - 1];
				maze[currentRow][i-1].right = true;
			}
		}
		// recursively call function for next line
		constructMaze(currentRow+1, disjoint);
	}

	// trivial case: the second row to the last row
	else {
		// initialize DISJOINT. initialize SET_SIZE.
		for(int i = 0; i < N; i++) {
			disjoint[i] = getIndex(i, currentRow);
			setSize[previousDisjoint[i]]++;
		}

		// randomly make vertical ways.
		for(int i = 0; i < N; i++) {
			setSize[previousDisjoint[i]]--;
			if(1.0*rand()/RAND_MAX <= PROBABILITY || (!setFlag[previousDisjoint[i]] && !setSize[previousDisjoint[i]])) {
				maze[currentRow][i].up = true;
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
					maze[currentRow][i-1].right = true;
				}
			}
		}

		// recursively call function for next line.
		constructMaze(currentRow+1, disjoint);
	}
}

/**************************************************************
.function	  :	deleteWalls
description   :	delete some walls from a maze
input	      :	(int) numberToDelete, number of walls to delete
return		  :	(none)
***************************************************************/
void deleteWalls(int numberToDelete) {
	int numberOfWalls;
	int wallToDelete;
	int t, i, j;

	// count number of walls in maze
	numberOfWalls = 0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (i != 0)
				numberOfWalls += !maze[i][j].up;
			if (j != N - 1)
				numberOfWalls += !maze[i][j].right;
		}
	}

	for (int k = 0; k < numberToDelete; k++) {
		// randomly pick a wall
		wallToDelete = rand() % numberOfWalls + 1;

		// find and delete the wall
		t = 0;
		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				if (i != 0 && !maze[i][j].up) {
					t++;
					if (t == wallToDelete) {
						maze[i][j].up = true;
						break;
					}
				}
				if (j != N-1 && !maze[i][j].right) {
					t++;
					if (t == wallToDelete) {
						maze[i][j].right = true;
						break;
					}
				}
			}
			if (j < N) {
				break;
			}
		}

		// reduce total number of walls
		numberOfWalls--;
	}
}
