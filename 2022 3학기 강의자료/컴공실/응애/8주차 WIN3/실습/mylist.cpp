#include "stdafx.h"
#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>

mylist::mylist(){
	num = 0;
	head = prev_node = curr_node = NULL;
}

mylist::~mylist(){
	move_first();
	while(isEmpty()==false) del();
}

bool mylist::isEmpty(void){
	if(num==0) return true;
	return false;
}

void mylist::add(Point* tpoint){
	node *new_node, *temp;
	Point *temp_point;
	/** ���ο� ��带 ����� tpoint�� ���� �ְ�,
	    ���� �����ǰ� �ִ� linkedlist�� �� �տ� �ִ´�.
	    ���� �� list�� ������ �ϳ� ������Ű���� �Ѵ�.
    **/

	/// �޸� �Ҵ�
	new_node = new node;
	temp_point = new Point;
	if(new_node == NULL || temp_point == NULL)
		exit(-1);

	/// temp_point ä���
	temp_point->x = tpoint->x;
	temp_point->y = tpoint->y;

	/// new_node->point ä���
	new_node->point = temp_point;

	/// linked list�� �� �տ� new_node �߰�
	temp = head;
	head = new_node;
	head->next = temp;

	num++;
}

node* mylist::del(void){
	node* temp;
	if(curr_node==NULL)
		return NULL;

	temp = curr_node;
	prev_node->next = curr_node->next;
	curr_node = curr_node->next;
	free(temp->point);
	free(temp);
	num--;
	return curr_node;
}

node* mylist::move_first(void){
	prev_node = head;
	curr_node = head;
	return curr_node;
}

node* mylist::move_next(void){
	if(curr_node == NULL)
		return NULL;
	prev_node = curr_node;
	curr_node = curr_node->next;
	return curr_node;
}
