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

// 생성자
template <typename T>
Array<T>::Array (int size) {
	// size가 0 이하의 값이면 에러메시지를 출력하고 종료
	if(size <= 0) {
		cerr << "Negative size error!\n";
		exit(-1);
	}

	// size가 양수이면 new를 사용하여 배열 data를 할당, len 값 초기화
	data = new T[size];
	if(data == NULL) {
		cerr << "Memory allocation error!\n";
		exit(-1);
	}
	len = size;
}

// 소멸자; 할당된 메모리 해제
template <typename T>
Array<T>::~Array (void) {
	delete[] data;
}

// 배열의 크기 리턴
template <typename T>
int Array<T>::length (void) const {
	return len;
}

// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 다중정의이다.
template <typename T>
T & Array<T>::operator[] (int i) {	// left value : 배열에 원소를 삽입
	static T tmp;
	// 배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴
	if(0 <= i && i < len) {
		return data[i];
	}
	// 그렇지 않으면 에러 메시지 출력하고 tmp를 리턴
	else {
		cerr << "Array bound error!\n";
		return tmp;
	}
}

template <typename T>
T Array<T>::operator[] (int i) const {	// right value : 배열의 원소의 값을 반환
	// 배열의 인덱스가 범위 내에 있으면 값을 리턴
	if(0 <= i && i < len) {
		return data[i];
	}
	// 그렇지 않으면 에러 메시지를 출력하고 0을 리턴
	else {
		cerr << "invalid size parameter (0 <= i < len)\n";
		return 0;
	}
}

// 배열의 모든 내용을 출력해주는 함수
template <typename T>
void Array<T>::print (void) {
	cout << '[';
	for(int i = 0; i < len; i++) {
		cout << ' ' << data[i];
	}
	cout << "]\n";
}

#endif	// __ARRAY__
