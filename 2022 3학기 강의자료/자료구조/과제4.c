#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/* �Ʒ� ����ü �����ϰ� �������� ��� ����
�������� ��� �� 0�� ó�� */
typedef struct threadedTree* threadedPointer;
typedef struct threadedTree {
	char data;
	short int leftThread, rightThread;
	threadedPointer leftChild, rightChild;
} threadedTree;

/* ������ �Լ���
�Լ��� ��ȯ���̳� parameter�� ���� ����������, �Լ����� ���� �Ұ�
�Լ��� ���� �� �ش� �Լ� �̱������� �����ϰ� ���� �̺ο� */
threadedPointer initTree(FILE* fp); // ���� ����
threadedPointer insucc(threadedPointer tree); // ���� Ȯ��
threadedPointer inpredec(threadedPointer tree); // ���� ����
void insertRight(threadedPointer parent, threadedPointer child, char data); // ���� Ȯ��
void insertLeft(threadedPointer parent, threadedPointer child, char data); // ���� ����
threadedPointer tinorderSearch(threadedPointer tree, char data); // ���� ����
void tinorder(threadedPointer tree); // ���� Ȯ��
void tpreorder(threadedPointer tree); // ���� ����

void main() {
	/* main �Լ��� ������ �� ����
	main �Լ� ���� �� 0�� ó�� */
	FILE* fp = fopen("input.txt", "r");
	threadedPointer my_tree = initTree(fp);

	tinorder(my_tree);
	printf("\n");
	tpreorder(my_tree->leftChild);
	printf("\n");

	fclose(fp);
	return;
}