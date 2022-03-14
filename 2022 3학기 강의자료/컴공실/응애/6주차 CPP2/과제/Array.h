#ifndef __ARRAY__
#define __ARRAY__
#include <iostream>
#include <cstdlib>
using namespace std;

template <typename T>
class Array{
protected:
    T *data;
    int len;
public:
    Array() {};
    Array(int size);
    ~Array();

    int length() const;

    virtual T& operator[](int i);
    virtual T operator[](int i) const;

    void print();
};

// ������
template <typename T>
Array<T>::Array (int size) {
	// size�� 0 ������ ���̸� �����޽����� ����ϰ� ����
	if(size <= 0) {
		cerr << "Negative size error!\n";
		exit(-1);
	}

	// size�� ����̸� new�� ����Ͽ� �迭 data�� �Ҵ�, len �� �ʱ�ȭ
	data = new T[size];
	if(data == NULL) {
		cerr << "Memory allocation error!\n";
		exit(-1);
	}
	len = size;
}

// �Ҹ���; �Ҵ�� �޸� ����
template <typename T>
Array<T>::~Array (void) {
	delete[] data;
}

// �迭�� ũ�� ����
template <typename T>
int Array<T>::length (void) const {
	return len;
}

// �迭�� ���Ҹ� �����ϰų� ���� ��ȯ�ϴ� �κ����� []�������� ���������̴�.
template <typename T>
T & Array<T>::operator[] (int i) {	// left value : �迭�� ���Ҹ� ����
	static T tmp;
	// �迭�� �ε����� ���� ���� ������ �ش� ���Ҹ� ����
	if(0 <= i && i < len) {
		return data[i];
	}
	// �׷��� ������ ���� �޽��� ����ϰ� tmp�� ����
	else {
		cerr << "Array bound error!\n";
		return tmp;
	}
}

template <typename T>
T Array<T>::operator[] (int i) const {	// right value : �迭�� ������ ���� ��ȯ
	// �迭�� �ε����� ���� ���� ������ ���� ����
	if(0 <= i && i < len) {
		return data[i];
	}
	// �׷��� ������ ���� �޽����� ����ϰ� 0�� ����
	else {
		cerr << "invalid size parameter (0 <= i < len)\n";
		return 0;
	}
}

// �迭�� ��� ������ ������ִ� �Լ�
template <typename T>
void Array<T>::print (void) {
	cout << '[';
	for(int i = 0; i < len; i++) {
		cout << ' ' << data[i];
	}
	cout << "]\n";
}

#endif	// __ARRAY__
