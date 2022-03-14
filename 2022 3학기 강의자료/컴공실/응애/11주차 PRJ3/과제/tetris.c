#include "tetris.h"

/* tiny helper functions */
int max(int a, int b) {
    return (a>b)?a:b;
}

int min(int a, int b) {
    return (a<b)?a:b;
}

void error(const char * msg) {
	printw(msg);
	getch();
	endwin();
	system("clear");
	exit(-1);
}

int recommend_limit;
// change this function pointer to switch between "recommend" or "modified_recommend" functions
int (*recommendFunc)(recptr) = modified_recommend;
static struct sigaction act, oact;
int recommendFlag;
int main() {
	int exit=0, i;
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

	while(!exit) {
		clear();
		switch(menu()) {
			case MENU_PLAY: play();break;
			case MENU_RANK: rank();break;
			case MENU_RECO: recommendedPlay(); break;
			case MENU_EXIT: exit=1;break;
			default: break;
		}
	}

	writeRankFile();
	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	int i,j;

	memset(field, 0, sizeof(field));

	for (i = 0; i < BLOCK_NUM; i++) {
		nextBlock[i] = rand() % 7;
	}

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;
	gameOver=0;
	timed_out=0;

	recommend_limit = min(BLOCK_NUM, RECOMMEND_LIMIT);

	DrawOutline();
	DrawField();
	GetRecommend();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i,j;
	/* 블럭이 떨어지는 공간의 테두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 테두리를 그린다.*/
	move(2,WIDTH+10);
	if(VISIBLE_BLOCKS > 1)
		printw("NEXT BLOCK");
	for(i = 0; i < VISIBLE_BLOCKS-1; i++)
		DrawBox(3+i*6, WIDTH+10, 4, 8);

	/* hold block을 보여주는 공간의 테두리를 그린다. */
	move(3+(VISIBLE_BLOCKS-1)*6,WIDTH+10);
	printw("HOLD");
	DrawBox(4+(VISIBLE_BLOCKS-1)*6, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 테두리를 그린다.*/
	move(10+(VISIBLE_BLOCKS-1)*6,WIDTH+10);
	printw("SCORE");
	DrawBox(11+(VISIBLE_BLOCKS-1)*6,WIDTH+10,1,8);
}

int GetCommand() {
	int command;

	command = wgetch(stdscr);
	switch(command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case FALL:
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	case 'h':
	case 'H':
		command = HOLD;
		break;
	default:
		command = NOTHING;
		break;
	}
	if(command != QUIT && recommendFlag == 1) {
		return KEY_DOWN;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret=1;
	int drawFlag=0;
	switch(command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = RotatePosition(field,nextBlock[0],(blockRotate+1)%4,&blockY,&blockX)))
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
		blockY = DropPosition(field, blockY, blockX, nextBlock[0], blockRotate);
		drawFlag = 1;
		break;
	case HOLD:
		Hold();
		break;
	default:
		drawFlag = 1;
		break;
	}
	if(drawFlag) {
		DrawChange(field, nextBlock[0], blockRotate, blockY, blockX);
	}
	return ret;
}

void DrawField() {
	int i,j;
	for(j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for(i = 0; i < WIDTH; i++) {
			if(field[j][i]) {
				attron(A_REVERSE|COLOR_PAIR(field[j][i]));
				printw(" ");
				attroff(A_REVERSE|COLOR_PAIR(field[j][i]));
			}
			else printw(".");
		}
	}
}

void PrintScore(int score) {
	move(12+(VISIBLE_BLOCKS-1)*6, WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j, k;
	for(k = 1; k < VISIBLE_BLOCKS; k++) {
		for(i = 0; i < 4; i++){
			move(4+i+(k-1)*6, WIDTH+13);
			for(j = 0; j < 4; j++ ) {
				if(block[nextBlock[k]][0][i][j] == 1) {
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
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1, j+x+1);
				attron(A_REVERSE|COLOR_PAIR(blockColor[blockID]));
				printw("%c",tile);
				attroff(A_REVERSE|COLOR_PAIR(blockColor[blockID]));
			}
		}
	}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch('o');
	for(i = 0; i < width; i++) {
		addch('-');
	}
	addch('o');
	for(j = 0; j < height; j++){
		move(y+j+1,x);
		addch('|');
		move(y+j+1,x+width+1);
		addch('|');
	}
	move(y+j+1,x);
	addch('o');
	for(i = 0; i < width; i++) {
		addch('-');
	}
	addch('o');
}

void DrawBlockWithFeatures(int blockY, int blockX, int currentBlock, int blockRotate) {
	DrawRecommend(recommendY, recommendX, currentBlock, recommendR);
	if(recommendFlag == 0) {
		DrawShadow(blockY, blockX, currentBlock, blockRotate);
	}
	DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
}

void play() {
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
			if(recommendFlag == 0) {
				newRank(score);
			}
			return;
		}
	} while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	if(recommendFlag == 0) {
		newRank(score);
	}
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,
                int blockRotate, int blockY, int blockX) {
	/* checks if the block can be placed on
	   a 4*4 grid in the field where the left-upper cell is (blockX, blockY)
	*/
	int i, j;
	int x, y;

	for(i = 0; i < 4; i++) {		// i = dy
		for(j = 0; j < 4; j++) {	// j = dx
			if(block[currentBlock][blockRotate][i][j]) {
				y = blockY+i;
				x = blockX+j;
				// if the block is to be placed out of grid
				if(!(0 <= x && x < WIDTH && 0 <= y && y < HEIGHT)) {
					return 0;
				}
				// if the grid is already occupied
				if(f[y][x] != 0) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int RotatePosition(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int* blockY, int* blockX) {
	int i, j;
	int ymin, xmin;
	int ymax, xmax;

	ymin = xmin = 50;
	xmax = ymax = -1;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(block[currentBlock][blockRotate][i][j]) {
				if (f[i+*blockY][j+*blockX]) {
                    return 0;
				}
				ymin = min(ymin, i);
				xmin = min(xmin, j);
				ymax = max(ymax, i);
				xmax = max(xmax, j);
			}
		}
	}

	// make sure the block is in the field
	while(ymin + *blockY < 0) {
        (*blockY)++;
	}
	while(xmin + *blockX < 0) {
        (*blockX)++;
	}
	while(ymax + *blockY >= HEIGHT) {
        (*blockY)--;
	}
	while(xmax + *blockX >= WIDTH) {
        (*blockX)--;
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int currentBlock,int blockRotate, int blockY, int blockX) {
	DrawField();
	DrawHoldBlock();
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
}

void BlockDown(int sig) {
	int i, j;
	int deletedLines;

	if(recommendFlag == 1) {
		blockY = recommendY;
		blockX = recommendX;
		blockRotate = recommendR;
	}


	// if a down-move is possible, move down.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) {
		blockY++;
		DrawChange(field, nextBlock[0], blockRotate, blockY, blockX);
	}
	// else, place block on field
	else {

		// game-over situations
		if(!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) {
			gameOver = 1;
			timed_out = 0;
			return;
		}
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++)
                if (block[nextBlock[0]][blockRotate][i][j] && blockY + i < 0) {
                    gameOver = 1;
					timed_out = 0;
					return;
				}
		}


		// accumulate score
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX, 1);
		deletedLines = DeleteLine(field);
		score += deletedLines * deletedLines * 100;

		// refresh block list & position
		for (i = 0; i < BLOCK_NUM - 1; i++) {
			nextBlock[i] = nextBlock[i+1];
		}
		nextBlock[BLOCK_NUM-1] = rand()%7;
		DrawNextBlock(nextBlock);
		blockX = WIDTH / 2 - 2;
		blockY = -1;
		blockRotate = 0;

		// refresh recommendation
		GetRecommend();

		// refresh screen
		PrintScore(score);
		DrawField();
	}
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX, int playMode) {
	int result;
	int dx[] = {-1, 0, 1};
	int dy[] = {0, 1, 0};
	int i, j, k;

	result = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++)
			if(block[currentBlock][blockRotate][i][j]) {
				f[blockY+i][blockX+j] = blockColor[currentBlock];
				// play mode
				// calculate score according to floor touch and block touch at the bottom
				if(playMode) {
					result += ((blockY+i+1 >= HEIGHT || f[blockY+i+1][blockX+j]))*10;
				}
				// evaluation mode
				// calculate value according to floor touch, wall touch, and block touch
				else {
					// check three directions, left, right, and down, for floor/wall/block.
					for(k = 0; k < 3; k++) {
						// case: touched the floor
						if(blockY+i+dy[k] >= HEIGHT) {
							result += FLOOR_TOUCH_ADVANTAGE;
						}
						// case: touched the wall
						else if(blockX+j+dx[k] < 0 ||blockX + j + dx[k] >= WIDTH) {
							result += WALL_TOUCH_ADVANTAGE;
						}
						// case: touched a block
						else if(blockY+i+dy[k] >= 0 && field[blockY+i+dy[k]][blockX+j+dx[k]]) {
							result += BLOCK_TOUCH_ADVANTAGE;
						}
					}
				}
			}
	}

	return result;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	int deletedLines;
	int flag;
	int x, y, y1;

	deletedLines = 0;
	for(y = HEIGHT-1; y >= 0; y--) {
		for(x = 0; x < WIDTH; x++) {
			if(f[y][x] == 0) {
				break;
			}
		}

		// if all tiles of this row is a block
		// delete this row
		if(x == WIDTH) {
			flag = 1;
			deletedLines++;
			for(y1 = y; y1 > 0; y1--) {
				for(x = 0; x < WIDTH; x++) {
					f[y1][x] = f[y1-1][x];
				}
			}
		}
	}
	if(flag)
		deletedLines += DeleteLine(f);
	return deletedLines;
}

int DropPosition(char field[HEIGHT][WIDTH], int y, int x, int blockID, int blockRotate) {
	int i, j;

	for(; y < HEIGHT; y++) {
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				if(y + i >= 0 && block[blockID][blockRotate][i][j] && field[y+i][x+j]) {
					break;
				}
				if(block[blockID][blockRotate][i][j] && y+i >= HEIGHT) {
					break;
				}
			}
			if(j < 4) {
				break;
			}
		}
		if(i < 4) {
			break;
		}
	}
	return y-1;
}

void DrawShadow(int y, int x, int blockID,int blockRotate) {
	y = DropPosition(field, y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList() {
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

void rank() {
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

void writeRankFile() {
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

void newRank(int score) {
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

	// case: user did not enter name
	if(name[0] == '\0') {
		return;
	}

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

void DrawRecommend(int y, int x, int blockID,int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(const recptr root) {
	recnode node;
	recptr childNode = &node;
	int x, y, rotate;
	int currentScore;
	int FullLine;
	int result;

	result = -1;

	// try all four rotational positions
	for(rotate = 0; rotate < 4; rotate++) {
		// try all possible horizontal options
		for(x = -2; x < WIDTH + 3; x++) {
			// skip impossible tries
			if(!CheckToMove(root->field, nextBlock[root->lv], rotate, 0, x)) {
				continue;
			}

			// initialize child node
			memcpy(childNode->field, root->field, sizeof(field));
			childNode->lv = root->lv + 1;

			// place block at this position and get score
			y = 0;
			y = DropPosition(childNode->field, y, x, nextBlock[root->lv], rotate);
			currentScore = AddBlockToField(childNode->field, nextBlock[root->lv], rotate, y, x, 1);
			FullLine = DeleteLine(childNode->field);
			currentScore += (FullLine*FullLine)*100;

			// recursively try for next blocks
			if(childNode->lv < recommend_limit) {
				currentScore += recommend(childNode);
			}

			// determine best child node
			if(result <= currentScore) {
				if(root->lv == 0 && (result < currentScore || y > recommendY)) {
					recommendY = y;
					recommendX = x;
					recommendR = rotate;
				}
				result = currentScore;
			}

		}
	}

	return result;
}

int modified_recommend(const recptr root) {
	recnode node;
	recptr childNode = &node;
	int x, y, rotate;
	int value;
	int FullLine;
	int result;

	result = -1;

	// try only meaningful rotational positions
	for(rotate = 0; rotate < possibleRotate[nextBlock[root->lv]]; rotate++) {
		// try all horizontal positions
		for(x = -2; x < WIDTH + 3; x++) {
			// skip impossibel tries
			if(!CheckToMove(root->field, nextBlock[root->lv], rotate, 0, x)) {
				continue;
			}

			// initialize child node
			memcpy(childNode->field, root->field, sizeof(field));
			childNode->lv = root->lv + 1;

			// add block to field and get value of the field with non-play mode
			y = 0;
			y = DropPosition(childNode->field, y, x, nextBlock[root->lv], rotate);

			value = AddBlockToField(childNode->field, nextBlock[root->lv], rotate, y, x, 0);

			// add values according to deleted lines
			FullLine = DeleteLine(childNode->field);
			value += (FullLine*FullLine) * LINE_CLEAR_ADVANTAGE;

			// recursively try for all next blocks
			if (childNode->lv < recommend_limit) {
				value += modified_recommend(childNode);
			}
			// evaluate this field
			else {
				value += Evaluate(childNode->field);
			}

			// determine best child
			if (result <= value) {
				if (root->lv == 0 && (result < value || y > recommendY)) {
					recommendY = y;
					recommendX = x;
					recommendR = rotate;
				}
				result = value;
			}

		}
	}

	return result;
}

void recommendedPlay() {
    recommendFlag = 1;
    play();
	recommendFlag = 0;
}

void Hold(void) {
	int tmp, i;
	// if this is the first hold
	// hold the block and push next block
	if(holdBlock < 0) {
		holdBlock = nextBlock[0];
		for (i = 0; i < BLOCK_NUM - 1; i++) {
			nextBlock[i] = nextBlock[i + 1];
		}
		nextBlock[BLOCK_NUM - 1] = rand() % 7;
	}
	// else, swap hold block and current block
	else {
		tmp = holdBlock;
		holdBlock = nextBlock[0];
		nextBlock[0] = tmp;
	}

	DrawHoldBlock();
	DrawNextBlock(nextBlock);
	blockY = -1;
	blockX = WIDTH/2 - 2;
	blockRotate = 0;
	GetRecommend();
	DrawField();
}

void DrawHoldBlock() {
	int i, j;
	if(holdBlock < 0) {
		return;
	}
	for(i = 0; i < 4; i++) {
		move(5+i+(VISIBLE_BLOCKS-1)*6, WIDTH+13);
		for(j = 0; j < 4; j++) {
			if(block[holdBlock][0][i][j]) {
				attron(A_REVERSE|COLOR_PAIR(blockColor[holdBlock]));
				printw(" ");
				attroff(A_REVERSE|COLOR_PAIR(blockColor[holdBlock]));
			}
			else {
				printw(" ");
			}
		}
	}
}

int GetRecommend() {
	recnode node;
	recptr root = &node;
	memcpy(root->field, field, sizeof(field));
	root->lv = 0;
	recommendX = recommendY = recommendR = 0;
	return recommendFunc(root);
}

/* value a field according to holes and height */
int Evaluate(char field[HEIGHT][WIDTH]) {
	int value;
	int y, x;
	int height, maxheight;
	int hole, flag;

	value = 0;
	maxheight = 0;

	// count holes and give penalty
	// also, find the highest column and give penalty
	for (x = 0; x < WIDTH; x++) {
		hole = 0;
		flag = 0;
		height = 0;
		for (y = 0; y < HEIGHT; y++) {
			if (field[y][x]) {
				height = y;
				flag = 1;
				value += hole*HOLE_PENALTY;
				hole = 0;
			}
			else {
				hole += flag;
			}
		}
		maxheight = max(maxheight, height);
	}
	value += maxheight * HEIGHT_PENALTY;

	return value;
}
