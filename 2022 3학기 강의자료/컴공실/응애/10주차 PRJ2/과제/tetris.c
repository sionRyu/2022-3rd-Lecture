#include "tetris.h"

static struct sigaction act, oact;

static void error(const char *);

int main(){
	int exit=0;
	int i, j;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	// initialize to use color
	start_color();
	for(i = 0; i < 7; i++) {
		init_pair(blockColor[i], blockColor[i], COLOR_BLACK);
	}

	createRankList();

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	writeRankFile();
	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=NO_BLOCK;

	for(i = 0; i < BLOCK_NUM; i++)
		nextBlock[i] = rand()%7;

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 테두리를 그린다.*/
	move(2,WIDTH+10);
	if(BLOCK_NUM > 1)
		printw("NEXT BLOCK");
	for(i = 0; i < BLOCK_NUM-1; i++)
		DrawBox(3+i*6, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 테두리를 그린다.*/
	move(3+(BLOCK_NUM-1)*6,WIDTH+10);
	printw("SCORE");
	DrawBox(4+(BLOCK_NUM-1)*6,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		command = FALL;
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	case FALL:
		blockY = DropPosition(blockY, blockX, nextBlock[0], blockRotate);
		drawFlag = 1;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]!=NO_BLOCK){
				attron(A_REVERSE|COLOR_PAIR(field[j][i]));
				printw(" ");
				attroff(A_REVERSE|COLOR_PAIR(field[j][i]));
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(5+(BLOCK_NUM-1)*6,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j, k;
	for( k = 1; k < BLOCK_NUM; k++) {
		for( i = 0; i < 4; i++ ){
			move(4+i+(k-1)*6,WIDTH+13);
			for( j = 0; j < 4; j++ ){
				if( block[nextBlock[k]][0][i][j] == 1 ){
					attron(A_REVERSE|COLOR_PAIR(blockColor[nextBlock[k]]));
					printw(" ");
					attroff(A_REVERSE|COLOR_PAIR(blockColor[nextBlock[k]]));
				}
				else printw(" ");
			}
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE|COLOR_PAIR(blockColor[blockID]));
				printw("%c",tile);
				attroff(A_REVERSE|COLOR_PAIR(blockColor[blockID]));
			}
		}

	move(HEIGHT,WIDTH+10);
}

void EraseBlock(int y, int x, int blockID, int blockRotate) {
	int i,j;
	char tile = '.';
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				printw("%c",tile);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawShadow(int y, int x, int blockID,int blockRotate){
	char tile = '/';
	int dropY;
	dropY = DropPosition(y, x, blockID, blockRotate);
	DrawBlock(dropY, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID,int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch('o');
	for(i=0;i<width;i++)
		addch('-');
	addch('o');
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch('|');
		move(y+j+1,x+width+1);
		addch('|');
	}
	move(y+j+1,x);
	addch('o');
	for(i=0;i<width;i++)
		addch('-');
	addch('o');
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = ProcessCommand(GetCommand());
		if(command==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
			newRank(score);
			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

int CheckToMove(short f[HEIGHT][WIDTH],int currentBlock,
                int blockRotate, int blockY, int blockX){
	/* checks if the block can be placed on
	   a 4*4 grid in the field where the left-upper cell is (blockX, blockY)
	*/
	int i, j;
	int x, y;
	for(i = 0; i < 4; i++) {		// i = dy
		for(j = 0; j < 4; j++) {	// j = dx
			if(block[currentBlock][blockRotate][i][j] == 1) {
				y = blockY+i;
				x = blockX+j;
				// if the block is to be placed out of grid
				if(!(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT)) {
					return 0;
				}
				// if the grid is already occupied
				if(f[y][x] != NO_BLOCK) {
					return 0;
				}
			}
		}
	}

	return 1;
}

void DrawChange(short f[HEIGHT][WIDTH],int command,int currentBlock,
                int blockRotate, int blockY, int blockX){
	/* erases the previous form and draws the new image */

	int dropY;
	// erases the previous form according to COMMAND
	switch(command) {
		case KEY_UP:
			dropY = DropPosition(blockY, blockX, currentBlock, (blockRotate+3)%4);
			EraseBlock(dropY, blockX, currentBlock, (blockRotate+3)%4);
			EraseBlock(blockY, blockX, currentBlock, (blockRotate+3)%4);
			break;
			case KEY_DOWN:
			EraseBlock(blockY-1, blockX, currentBlock, blockRotate);
			break;
		case KEY_LEFT:
			dropY = DropPosition(blockY, blockX+1, currentBlock, blockRotate);
			EraseBlock(dropY, blockX+1, currentBlock, blockRotate);
			EraseBlock(blockY, blockX+1, currentBlock, blockRotate);
			break;
		case KEY_RIGHT:
			dropY = DropPosition(blockY, blockX-1, currentBlock, blockRotate);
			EraseBlock(dropY, blockX-1, currentBlock, blockRotate);
			EraseBlock(blockY, blockX-1, currentBlock, blockRotate);
	}
	// draw the new image
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig){
	/* check if a down-move is possible.
	   if so, make a down-move.
	   otherwise, check for the game-over condition
	   if game is not over, add the block to field. */

	int i;

	// if a down-move is possible
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)) {
		// make a down-move
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		// add block to the field
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		// if the block stacks to the top of field
		if(blockY == -1) {
			// set the game-over flag
			gameOver = 1;
		}
		else {
			// delete full lines and apply to score
			score += DeleteLine(field);
			// generate the next block
			for(i = 0; i < BLOCK_NUM-1; i++) {
				nextBlock[i] = nextBlock[i+1];
			}
			nextBlock[BLOCK_NUM-1] = rand()%7;
			DrawNextBlock(nextBlock);
			// move position to top
			blockY = -1;
			blockX = WIDTH/2-2;
			blockRotate = 0;
			PrintScore(score);
		}
		DrawField();
	}
	// reset timer flag
	timed_out = 0;
}

int AddBlockToField(short f[HEIGHT][WIDTH],
                    int currentBlock, int blockRotate, int blockY, int blockX){
	/* places the block on the field at the given position */
	int i, j;
	int count;
	count = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(block[currentBlock][blockRotate][i][j] == 1) {
				if(0 <= blockY+i && blockY+i < HEIGHT && 0 <= blockX+j && blockX+j < WIDTH) {
					f[blockY+i][blockX+j] = blockColor[currentBlock];
					if(blockY+i == HEIGHT-1)
						count++;
					else if(f[blockY+i+1][blockX+j] != NO_BLOCK)
						count++;
				}
			}
		}
	}
	return count * 10;
}

int DeleteLine(short f[HEIGHT][WIDTH]){
	/* deletes all full lines and returns score
	   according to number of deleted lines */
	int count;		// counts number of deleted lines
	int full;		// flag variable to signal if a line is full
	int i, j, k;

	count = 0;
	for(i = 0; i < HEIGHT; i++) { // y-coordinate
		full = 1;				  // flag that line i is full
		for(j = 0; j < WIDTH; j++) {
			if(f[i][j] == NO_BLOCK) {
				full = 0;
				break;
			}
		}
		// if line i is full
		if(full) {
			count++;
			// pull all lines above line i one line each
			for(k = i-1; k >= 0; k--) {
				for(j = WIDTH; j > 0; j--) {
					f[k+1][j] = f[k][j];
				}
			}
		}
	}

	return count*count*100;
}

void createRankList(){
	FILE* fp;
	int number_of_rank;
	int score;
	char name[NAMELEN];
	
	int buffer_int;
	int i, temp;
	
	// open "rank.txt"
	fp = fopen(RANK_FILE_NAME, "rt");

	// if file does not exist, create and initialize
	if(fp == NULL) {
		fp = fopen(RANK_FILE_NAME, "wt");
		if(fp == NULL) {
			error("error: cannot create ranking\n");
		}
		fprintf(fp, "0\n");
		// create an empty red-black tree
		tree = rbNewTree();
	}

	// read ranking from file "rank.txt"
	else {
		tree = rbNewTree();
		fscanf(fp, "%d", &number_of_rank);
		
		for(i = 0; i < number_of_rank; i++) {
			fscanf(fp, "%s%d", name, &score);
			rbInsert(tree, rbNewNode(score, name));
		}
	}

	// close "rank.txt"
	fclose(fp);

	rank_change_flag = 0;
}

void rank(){
	int command;
	int x, y;
	int i;
	char name[NAMELEN];
	rbnode ** nodes = NULL;

	do {
		clear();
		printw("1. list ranks from X to Y\n");
		printw("2. list ranks by a specific name\n");
		printw("3. delete a specific rank\n");
		command = wgetch(stdscr);
	} while(!('1' <= command && command <= '3'));
	echo();
	switch(command) {
		case RANK_BY_RANGE: // list ranks from X to Y
			printw("X: ");
			scanw("%d", &x);
			printw("Y: ");
			scanw("%d", &y);
			
			// check for parameter sanity
			if(x < 1 || x > tree->size) {
				x = 1;
			}
			if(y < 1 || y > tree->size) {
				y = tree->size;
			}

			printw(" rank |       name       |   score   \n");
			printw("-------------------------------------\n");
			// case: search fail
			if(x > y) {
				printw("\nsearch failure: no rank in the list\n");
			}
			// case: query and print
			else {
				nodes = rbSelectByRange(tree, x, y);
				for(i = 0; i < y-x+1; i++) {
					printw(" %4d | %-17s| %-10d\n", i+x, nodes[i]->name, nodes[i]->score);
				}
				if(nodes) {
					free(nodes);
				}
			}
			break;
		case RANK_BY_NAME: // search rank by name
			printw("input the name: ");
			scanw("%s", name);
			
			// get list of all ranks and search for matching name
			printw(" rank |       name       |   score   \n");
			printw("-------------------------------------\n");
			x = 0;
			nodes = rbSelectByRange(tree, 1, tree->size);
			for(i = 0; i < tree->size; i++) {
				if(strcmp(name, nodes[i]->name) == 0) {
					printw(" %4d | %-17s| %-10d\n", i+1, nodes[i]->name, nodes[i]->score);
					x = 1;
				}
			}
			// case: search fail
			if(!x) {
				printw("\nsearch failure: no name in the list\n");
			}
			if(nodes) {
				free(nodes);
			}
			break;
		case RANK_DELETE_BY_NO: // delete data of given rank number
			printw("input the rank: ");
			scanw("%d", &x);

			if(rbDelete(tree, x) == 0) {
				printw("result: the rank deleted\n");
				rank_change_flag = 1;
			}
			else {
				printw("search failure: the rank is not in the list\n");
			}
			break;
	}
	noecho();
	getch();
}

void writeRankFile(){
	FILE *fp;
	rbnode ** nodes;
	int i;

	if(!rank_change_flag) {
		return;
	}

	fp = fopen(RANK_FILE_NAME, "wt");
	if(fp == NULL) {
		error("error: cannot open rank file");
	}

	// get list of all ranks
	nodes = rbSelectByRange(tree, 1, tree->size);

	// write it to file
	fprintf(fp, "%d\n", tree->size);
	for(i = 0; i < tree->size; i++) {
		fprintf(fp, "%s %d\n", nodes[i]->name, nodes[i]->score);
	}

	// free memory and close file
	if(nodes) {
		free(nodes);
	}
	rbDestroyTree(tree);
	fclose(fp);
}

void newRank(int score){
	char name[NAMELEN];
	int y;
	int i;
	int rank;
	rbnode * new_node;
	rbnode ** nodes;
	
	// input a user name
	clear(); echo();
	printw("your name: ");
	scanw("%s", name);
	noecho();
	
	// create a new node and insert it to the tree
	new_node = rbNewNode(score, name);
	rbInsert(tree, new_node);
	rank = rbRankByNode(tree, new_node);
	rank_change_flag = 1;


	// print rank of the new score
	if(tree->size < 5) {
		y = tree->size;
	}
	else {
		y = 5;
	}
	printw(" rank |       name       |   score   \n");
	printw("-------------------------------------\n");
	nodes = rbSelectByRange(tree, 1, y);
	for(i = 0; i < y; i++) {
		if(i == rank-1) {
			attron(A_REVERSE);
		}
			printw(" %4d | %-17s| %-10d\n", i+1, nodes[i]->name, nodes[i]->score);
		if(i == rank-1) {
			attroff(A_REVERSE);
		}
	}

	if(rank > y) {
		attron(A_REVERSE);
		printw(" %4d | %-17s| %-10d\n", rank, name, score);
		attroff(A_REVERSE);
	}

	if(nodes) {
		free(nodes);
	}
	getch();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}

int DropPosition(int y, int x, int blockID, int blockRotate){
	int dropY = y;
	while(CheckToMove(field, nextBlock[0], blockRotate, dropY+1, x))
		dropY++;
	return dropY;
}

static void error(const char * error_message) {
	printw(error_message);
	getch();
	exit(-1);
}
