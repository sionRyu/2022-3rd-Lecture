#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Unit
{
	int number;
}Unit;

char field[1000][1000];

int main(){
	srand(time(NULL)); // 난수 초기화하기
	int WIDTH, HEIGHT;

	printf("Input WIDTH :");
	scanf_s("%d", &WIDTH);
	printf("Input HEIGHT :");
	scanf_s("%d", &HEIGHT);

	int random = rand() % WIDTH;

	Unit** unit = (Unit**)malloc(sizeof(Unit*)*HEIGHT);
	for (int v = 0; v < HEIGHT; v++)
	{
		unit[v] = (Unit*)malloc(sizeof(Unit)*WIDTH);
	}
	
	

	

	for (int i = 0; i < 2*HEIGHT+1; i++)
	{
		for (int j = 0; j < 2*WIDTH+1; j++)
		{
			if (i%2 == 0 && j%2 == 0)
			{
				field[i][j] = '+';
			}
			if (i%2 == 0 && j%2 == 1)
			{
				field[i][j] = '-';
			}
			if (i % 2 == 1 && j % 2 == 0)
			{
				field[i][j] = '|';
			}
			if (i % 2 == 1 && j % 2 == 1)
			{
				field[i][j] = ' ';
			}
		}
	}

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			unit[x][y].number = WIDTH*x + y;
		}
	}
	
	for (int m = 0; m < HEIGHT; m++)
	{
		random = rand() % WIDTH; // 난수 생성
		for (int y = 0; y < WIDTH; y++) // 벽없애기, 집합 변경
		{
			random = rand() % 2;
			if (random == 0 &&m!=0&& y != 0 && unit[m][y].number != unit[m][y - 1].number)
			{
				field[2*m][2 * y-1] = ' ';
				unit[m][y].number = unit[m][y - 1].number;
			}
		}

		int drawcount = 0;
		for (int z = 0; z < WIDTH; z++)
		{
			random = rand() % 2;
			if (random == 0 && z != 0 && m!=0)
			{
				field[2 * m ][2 * z - 1] = ' ';
				drawcount++;
			}
			if (z > 0 && unit[m][z - 1].number != unit[m][z].number && drawcount == 0 && m!=0)
			{
				field[2 * m][2 * z - 1] =' ';
			}
			if (unit[m][z - 1].number != unit[m][z].number)
			{
				drawcount = 0;
			}
		}
	}

	for (int y = 0; y < WIDTH; y++) // 벽없애기, 집합 변경
	{
		random = rand() % 2;
		if (random == 0 && y != 0 && unit[HEIGHT-1][y].number != unit[HEIGHT - 1][y - 1].number)
		{
			field[2*HEIGHT-1][2 * y] = ' ';
			unit[HEIGHT - 1][y].number = unit[HEIGHT - 1][y - 1].number;
		}
	}

	FILE *fp = fopen("maze.maz", "wt");
	for (int k = 0; k <2*HEIGHT+1; k++)
	{
		for (int x = 0; x < 2*WIDTH+1; x++)
		{
			fprintf(fp,field[k][x]);
		}
		fprintf(fp,"\n");
	}
	
	fclose(fp);
}