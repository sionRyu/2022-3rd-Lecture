#ifndef __STACK_H__
#define __STACK_H__

#include "LinkedList.h"

template <typename T>
class Stack:public LinkedList<T> {
public:
    bool Delete(T &element); // 맨 뒤의 원소를 삭제
};

template <typename T>
bool Stack<T>::Delete(T &element) {
    if(this->first == NULL) return false;
    element = this->first->data;
    Node<T> *t = this->first;
    this->first = this->first->link;
    delete t;
    this->current_size--;
    return true;
}

#endif // __STACK_H__
