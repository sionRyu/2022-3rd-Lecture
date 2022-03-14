#ifndef __GROWABLE_ARRAY_H__
#define __GROWABLE_ARRAY_H__
#include "Array.h"
using namespace std;

template <typename T>
class GrowableArray:public Array<T> {
public:
	GrowableArray(int size);
    virtual T& operator[](int i);
};

// ������
template <typename T>
GrowableArray<T>::GrowableArray (int size) {
	// size�� 0 ������ ���̸� �����޽����� ����ϰ� ����
	if(size <= 0) {
		cerr << "Negative size error!\n";
		exit(-1);
	}

	// size�� ����̸� new�� ����Ͽ� �迭 data�� �Ҵ�, len �� �ʱ�ȭ
	this->data = new T[size];
	if(this->data == NULL) {
		cerr << "Memory allocation error!\n";
		exit(-1);
	}
	this->len = size;
}


// �迭�� ���Ҹ� �����ϰų� ���� ��ȯ�ϴ� �κ����� []�������� ���������̴�.
template <typename T>
T & GrowableArray<T>::operator[] (int i) const {	// left value : �迭�� ���Ҹ� ����
	// �迭�� �ε����� ���� ���� ������ �ش� ���Ҹ� ����
	if(0 <= i && i < this->len) {
		return this->data[i];
	}
	// �׷��� ������ ����� ������ ������ ũ�⸦ 2�� �÷��� �߰��� ������ 0���� �ʱ�ȭ
	else {
        // �ʿ��� ��ŭ ũ�⸦ Ȯ��
        int new_len = this->len;
		while(new_len <= i)
            new_len *= 2;
        // ���ο� �迭�� �޸𸮸� �Ҵ�
        T *new_data = new T[new_len];
        if(new_data == NULL) {
            cerr << "Memory allocation error!\n";
            exit(-1);
        }
        // ���ο� �迭�� ���� �迭�� ������ ����
        for(int i = 0; i < this->len; i++)
            new_data[i] = this->data[i];
        for(int i = this->len; i < new_len; i++)
            new_data[i] = 0;
        // ���� �迭�� �޸𸮸� ����
        T *t = this->data;
        this->data = new_data;
        this->len = new_len;
        delete[] t;
        // 0�� ��ȯ
		return this->data[i];
	}
}

#endif // __GROWABLE_ARRAY_H__
