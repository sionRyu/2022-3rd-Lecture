#include <stdlib.h>
#include "..\ExternDoc.h"
#include "..\UI\Zoom.h"
#include "..\UI\MsgView.h"
#include "..\Graphics\DrawFunc.h"
#include "..\Example\Example.h"

#define ERROR_NUMBER -1

//function prototype
static void drawDirect(CDC *pDC);
static void drawBuffered();

//Start of user code
#include <float.h>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>

/* Thickness of lines to draw the maze and path. */
#define CELL_SIZE 5.0
#define PATH_SIZE 4.0

/* set colors for maze elements */
#define mazeColor		RGB(0x00,0x00,0xff)
#define pathColor		RGB(0xff,0x00,0x00)
#define traceColor		RGB(0x94,0x94,0x94)
#define startPointColor	RGB(0x00,0xff,0x00)
#define endPointColor	RGB(0x00,0xff,0xff)

/* Data structure to represent a maze and the trace.
 * maze[i][j] =  1) wall		(If maze[i][j] is a wall.)
 *				 2) unvisited	(If maze[i][j] is a room and not reached.)
 *				 3) k (0~3)		(If maze[i][j] is a room reached from maze[i-di[k]][j-dj[k]].)
 *				 4) wall+k+1	(Temporary mark left during DFS or BFS. Must be cleared after search.)
 */
#define unvisited	5
#define wall		6
std::vector<std::vector<char> > maze;
int width, height;

/* traceMarked: true if the trace is marked on the maze.
 * mazeSolved: true if the ending position is reached. */
bool traceMarked = false;
bool mazeSolved = false;

/* (di, dj) represent upper, lower, left, right move. */
int di[] = {-1, 1, 0, 0},
	dj[] = {0, 0, -1, 1};
/* (xBase, yBase): Screen coordinate of left-upper corner of maze */
#define xBase	2.0
#define yBase	2.0
/* (iStart, jStart): Starting position to find path.
 * The default starting position (1,1) is the left-upper corner. */
#define iStart	1
#define jStart	1
/* (iEnd, jEnd): Ending position to find path
 * The default ending position (height-2, width-2) is the right-lower corner. */
#define iEnd	(height-2)
#define jEnd	(width-2)

/* Custom helper functions */
static void drawLineSegment(int, int, int, int, const COLORREF);
static void eraseTrace();

/******************************************************************
* function	: void freeMemory()
*
* remark	: Free dynamically allocated memory
*******************************************************************/
void freeMemory(){
	//start of the user code
	/* No dynamic allocation used in this source code.
	 * Here we initialize WIDTH, HEIGHT, MAZE, and unmark flags.
	 */
	width = 0;
	height = 0;
	std::vector<std::vector<char> >().swap(maze);
	traceMarked = false;
	mazeSolved = false;
	//end of usercode
}

/*****************************************************************
* function	: bool readFile(const char* filename)
* argument	: cons char* filename - filename to be opened
* return	: true if success, otherwise false
* remark	: After read data file, physical view must be set;
*			  otherwise drawing will not be executed correctly.
*			  The window will be invalidated after readFile()
*			  returns true.
******************************************************************/
bool readFile(const char* filename){
	//start of the user code
	/* Essentially, this function reads the file FILENAME and
	 * fill the MAZE vector just like the file.
	 * Walls ('+' and '-' in the file) are marked WALL in MAZE,
	 * and rooms (' ' in the file) are marked unvisited in MAZE.
	 */
	char cBuff;
	FILE * fp;

	fp = fopen(filename, "rt");
	if(fp == NULL) {
		goto errorReadFile;
	}

	/* Read first line to determine WIDTH.
	 * Note that WIDTH = ((number of '+' before first new line character) * 2 - 1)
	 */
	while((cBuff = fgetc(fp)) != '\n') {
		/* A maze file should not consist of a single line, so it must have at least one '\n'. */
		if(cBuff == EOF) {
			goto errorReadFile;
		}
		/* Count the number of '+' to determine WIDTH. */
		if(cBuff == '+') {
			width++;
		}
	}
	width = width * 2 - 1;
	fclose(fp);

	/* Read the whole file and fill the MAZE vector
	 * Note that HEIGHT = (number of valid lines)
	 */
	fp = fopen(filename, "rt");
	if(fp == NULL) {
		goto errorReadFile;
	}
	while(1) {
		std::vector<char> vectorBuff(width);

		/* Read a single line and fill buffer */
		for(int i = 0; i < width; i++) {
			cBuff = fgetc(fp);

			/* This means we encountered End Of File, or a line shorter than WIDTH. */
			if(cBuff == EOF || cBuff == '\n') {
				break;
			}

			/* Fill buffer according to input character */
			switch(cBuff) {
			case '+':
			case '|':
			case '-':
				vectorBuff[i] = wall;
				break;
			case ' ':
				vectorBuff[i] = unvisited;
				break;
			default: // No other character than '+', '|', '-', ' ' shall appear.
				goto errorReadFile;
			}
		}
		/* If we encountered End Of File, or a line shorter than WIDTH,
		 * we assume that we reached the end of the maze, so stop reading.
		 */
		if(cBuff == EOF || cBuff == '\n') {
			break;
		}

		/* Now we successfully copied a line to VECTOR_BUFF.
		 * Add it to MAZE and increment HEIGHT. */
		maze.push_back(vectorBuff);
		height++;

		/* Move file pointer to the next line. */
		while(1) {
			cBuff = fgetc(fp);
			if(cBuff == EOF || cBuff == '\n') {
				break;
			}
		}
		/* Stop reading if we meet End Of File. */
		if(cBuff == EOF) {
			break;
		}
	}
	fclose(fp);

	/* Set window screen according to the WIDTH and HEIGHT of MAZE. */
	setWindow(0, 0, CELL_SIZE*(width+4), CELL_SIZE*(height+4), 1);
	return true;

	/* Error situation: undo every change done in this function. */
errorReadFile:
	if(fp) {
		fclose(fp);
	}
	freeMemory();
	return false;
	//end of usercode
}

/**************************************************************
* function	: bool writeFile(const char* filename)
*
* argument	: const char* filename - filename to be written
* return	: true if success, otherwise false
* remark	: Save user data to a file
****************************************************************/
bool writeFile(const char* filename){
	//start of the user code
	bool flag;
	flag = 0;

	return flag;
	//end of usercode
}

/******************************************************************
* function	: void DFS()
*
* remark	: Find a path with DFS
*******************************************************************/
void DFS() {
	//begin of user code
	std::stack<std::pair<int, int> > s;

	/* Erase the existing trace on the maze. */
	eraseTrace();
	traceMarked = true;

	/* Push the starting position to stack. */
	s.push(std::make_pair(iStart, jStart));
	/* Start DFS. */
	while (!s.empty()) {
		/* Pop from stack. */
		int i = s.top().first;
		int j = s.top().second;
		s.pop();
		/* Mark trace on maze. */
		if(maze[i][j] > wall)
			maze[i][j] -= wall+1;
		/* Check if the ending position is reached. */
		if(i == iEnd && j == jEnd) {
			mazeSolved = true;
			break;
		}
		/* Examine four directions to move. */
		for(int k = 0; k < 4; k++) {
			int iNext = i+di[k];
			int jNext = j+dj[k];
			/* If (iNext, jNext) is a valid position */
			if(0 <= iNext && iNext < height
				&& 0 <= jNext && jNext < width) {
					/* If maze[iNext][jNext] is an unvisited room */
					if(maze[iNext][jNext] == unvisited) {
						/* Push the position to stack and temporarily mark it. */
						s.push(std::make_pair(iNext, jNext));
						maze[iNext][jNext] = k+wall+1;
					}
			}
		}
	}
	
	/* Empty the stack and undo temporary marks. */
	while(!s.empty()) {
		/* Pop a position. */
		int i = s.top().first;
		int j = s.top().second;
		s.pop();
		/* If the position is temporarily marked, unmark it. */
		if(maze[i][j] > wall)
			maze[i][j] = unvisited;
	}
	//end of user code
}


/******************************************************************
* function	: void BFS()
*
* remark	: Find a path with BFS
*******************************************************************/
void BFS() {
	//begin of user code
	//end of user code
}

/************************************************************************
* fucntion	: void drawMain(CDC* pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Main drawing function. Called by CMFC_MainView::OnDraw()
*************************************************************************/
void drawMain(CDC *pDC){
	//if direct drawing is defined
#if defined(GRAPHICS_DIRECT)
	drawDirect(pDC);
	//if buffered drawing is defined
#elif defined(GRAPHICS_BUFFERED)
	drawBuffered();
#endif
}

/************************************************************************
* function	: static void drawDirect(CDC *pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Direct drawing routines here.
*************************************************************************/
static void drawDirect(CDC *pDC){
	//begin of user code
	//Nothing to write currently.
	//end of user code
}

/***********************************************************************
* function	: static void drawBuffered()
*
* remark	: Buffered drawing routines here.
************************************************************************/
static void drawBuffered(){
	//start of the user code
	/* Iterate through MAZE vector. */
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			/* If there is a wall at this position. */
			if(maze[i][j] == wall) {
				/* (x0, y0): Screen coordinate of left-upper point of wall to be drawn
				 * (x1, y1): Screen coordinate of right-lower point of wall to be drawn */
				double x0 = xBase + CELL_SIZE * j,
					   y0 = yBase + CELL_SIZE * i;
				double x1 = xBase + CELL_SIZE * (j + 1),
					   y1 = yBase + CELL_SIZE * (i + 1);
				/* Draw the wall to screen. */
				DrawSolidBox_I(x0, y0, x1, y1, 0, mazeColor, mazeColor);
			}
			/* If there is a trace at this position, draw it to screen. */
			if(0 <= maze[i][j] && maze[i][j] < 4) {
				int k = (int)maze[i][j];
				drawLineSegment(i, j, i-di[k], j-dj[k], traceColor);
			}
		}
	}

	/* If the maze is solved, draw the path. */
	if(mazeSolved) {
		int i = iEnd;
		int j = jEnd;
		/* Follow the trace from (iEnd, jEnd) to (iStart, jStart) and draw it to screen. */
		while(!(i == iStart && j == jStart)) {
			int k = (int)maze[i][j];
			drawLineSegment(i, j, i-di[k], j-dj[k], pathColor);
			i = i-di[k];
			j = j-dj[k];
		}
	}
	
	/* Draw the starting position and ending position to screen. */
	drawLineSegment(iStart, jStart, iStart, jStart, startPointColor);
	drawLineSegment(iEnd, jEnd, iEnd, jEnd, endPointColor);
	//end of the user code
}

/* The following are custom helper functions. */
/************************************************************************
* function	: static void drawLineSegment(int i0, int j0,
*							int i1, int j1, COLORREF color)
*
* argument	: (i0, j0), (i1, j1) - rooms to draw a path between.
*			: color - color of the line
* remark	: (i0, j0) and (i1, j1) must be same or adjacent cells.
*************************************************************************/
static void drawLineSegment(int i0, int j0, int i1, int j1, const COLORREF color) {
	/* This function is used only to connect two adjacent rooms.
	 * Check conditions for this restriction. */
	ASSERT(0 <= i0 && i0 < height);
	ASSERT(0 <= j0 && j0 < width);
	ASSERT(0 <= i1 && i1 < height);
	ASSERT(0 <= j1 && j1 < width);
	ASSERT(std::abs(i0-i1) + std::abs(j0-j1) <= 1); // (i0, j0) and (i1, j1) are same or adjacent cells.
	ASSERT(maze[i0][j0] != wall);
	ASSERT(maze[i1][j1] != wall);
	ASSERT(CELL_SIZE>=PATH_SIZE);

	// Make sure that (i0, j0) is the left/upper cell of the two.
	if(i0 > i1) {
		std::swap(i0, i1);
	}
	if(j0 > j1) {
		std::swap(j0, j1);
	}

	/* (x0, y0): Screen coordinate of left-upper point of path to be drawn
	 * (x1, y1): Screen coordinate of right-lower point of path to be drawn */
	double x0 = xBase + CELL_SIZE * j0 + (CELL_SIZE-PATH_SIZE),
		   y0 = yBase + CELL_SIZE * i0 + (CELL_SIZE-PATH_SIZE);
	double x1 = xBase + CELL_SIZE * (j1 + 1) - (CELL_SIZE-PATH_SIZE),
		   y1 = yBase + CELL_SIZE * (i1 + 1) - (CELL_SIZE-PATH_SIZE);

	/* Draw the path on screen. */
	DrawSolidBox_I(x0, y0, x1, y1, 0, color, color);
}

/************************************************************************
* function	: static void eraseTrace()
*
* remark	: Erase the trace from MAZE and leave only wall information.
*************************************************************************/
static void eraseTrace() {
	/* Erase the trace from MAZE.
	 * maze[i][j] = unvisited: the cell (i, j) is a room.
	 * maze[i][j] = wall: the cell (i, j) is a wall. */
	if(traceMarked) {
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				/* Everything other than a wall is a room. */
				if(maze[i][j] != wall) {
					maze[i][j] = unvisited;
				}
			}
		}
	}
}