#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* 아래 구조체 제외하고 전역변수 사용 금지
전역변수 사용 시 0점 처리 */
typedef struct threadedTree* threadedPointer;
typedef struct threadedTree {
	char data;
	short int leftThread, rightThread;
	threadedPointer leftChild, rightChild;
} threadedTree;

/* 구현할 함수들
함수의 반환형이나 parameter는 수정 가능하지만, 함수명은 변경 불가
함수명 변경 시 해당 함수 미구현으로 간주하고 점수 미부여 */
threadedPointer initTree(FILE* fp); // 직접 구현
threadedPointer insucc(threadedPointer tree); // 교재 확인
threadedPointer inpredec(threadedPointer tree); // 직접 구현
void insertRight(threadedPointer parent, threadedPointer child, char data); // 교재 확인
void insertLeft(threadedPointer parent, threadedPointer child, char data); // 직접 구현
threadedPointer tinorderSearch(threadedPointer tree, char data); // 직접 구현
void tinorder(threadedPointer tree); // 교재 확인
void tpreorder(threadedPointer tree); // 직접 구현

void main() {
	/* main 함수는 수정할 수 없음
	main 함수 수정 시 0점 처리 */
	FILE* fp = fopen("input.txt", "r");
	threadedPointer my_tree = initTree(fp);

	tinorder(my_tree);
	printf("\n");
	tpreorder(my_tree->leftChild);
	printf("\n");

	fclose(fp);
	return;
}