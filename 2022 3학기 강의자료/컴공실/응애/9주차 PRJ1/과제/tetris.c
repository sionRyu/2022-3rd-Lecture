#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

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
//		blockY = DropPosition(blockY, blockX, nextBlock[0], blockRotate);
//		drawFlag = 1;
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
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
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
					attron(A_REVERSE);
					printw(" ");
					attroff(A_REVERSE);
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
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
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

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

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

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,
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
				if(f[y][x] == 1) {
					return 0;
				}
			}
		}
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,
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

int AddBlockToField(char f[HEIGHT][WIDTH],
                    int currentBlock, int blockRotate, int blockY, int blockX){
	/* places the block on the field at the given position */
	int i, j;
	int count;
	count = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(block[currentBlock][blockRotate][i][j] == 1) {
				if(0 <= blockY+i && blockY+i < HEIGHT && 0 <= blockX+j && blockX+j < WIDTH) {
					f[blockY+i][blockX+j] = 1;
					if(blockY+i == HEIGHT-1)
						count++;
				}
			}
		}
	}
	return count * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	/* deletes all full lines and returns score
	   according to number of deleted lines */
	int count;		// counts number of deleted lines
	int full;		// flag variable to signal if a line is full
	int i, j, k;

	count = 0;
	for(i = 0; i < HEIGHT; i++) { // y-coordinate
		full = 1;				  // flag that line i is full
		for(j = 0; j < WIDTH; j++) {
			if(f[i][j] == 0) {
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
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
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
