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

#define CELL_SIZE 5
const COLORREF mazeColor = RGB(0,0,0xff);
// data structure to represent a maze
/* MAZE is represented as a two-dimensional array (implemented with a vector of vectors)
 * Walls ('+' and '-' in the input file) are marked TRUE in MAZE,
 * and ways (' ' in the input file) are marked FALSE in MAZE.
 */
std::vector<std::vector<bool> > maze;
int width, height;

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
	 * Walls ('+' and '-' in the file) are marked TRUE in MAZE,
	 * and ways (' ' in the file) are marked FALSE in MAZE.
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
		std::vector<bool> vectorBuff(width);

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
				vectorBuff[i] = true;
				break;
			case ' ':
				vectorBuff[i] = false;
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
	return true; //edit after finish this function

	/* Error situation: undo every change done in this function. */
errorReadFile:
	if(fp) {
		fclose(fp);
	}
	width = 0;
	height = 0;
	std::vector<std::vector<bool> >().swap(maze);
	return false;
	//end of usercode
}

/******************************************************************
* function	: void freeMemory()
*
* remark	: Free dynamically allocated memory
*******************************************************************/
void freeMemory(){
	//start of the user code
	/* No dynamic allocation used in this source code.
	 * Here we initialize the WIDTH and HEIGHT, and the MAZE.
	 */
	width = 0;
	height = 0;
	std::vector<std::vector<bool> >().swap(maze);
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
	//Nothing to write currently.
	//end of user code
}


/******************************************************************
* function	: void BFS()
*
* remark	: Find a path with BFS
*******************************************************************/
void BFS() {
	//begin of user code
	//Nothing to write currently.
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
* function	: static void drawDirect(CDC *pDC
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

	/* x0, y0: Coordinate of left-upper corner of maze */
	double x0 = 2.0;
	double y0 = 2.0;

	// Draw a maze on screen that resembles the MAZE vector
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if(maze[i][j]) {
				DrawSolidBox_I(x0 + CELL_SIZE * j, y0 + CELL_SIZE * i,
                   x0 + CELL_SIZE * (j + 1), y0 + CELL_SIZE * (i + 1),
                   0, mazeColor, mazeColor);
			}
		}
	}
	//end of the user code
}
