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

// 생성자
template <typename T>
GrowableArray<T>::GrowableArray (int size) {
	// size가 0 이하의 값이면 에러메시지를 출력하고 종료
	if(size <= 0) {
		cerr << "Negative size error!\n";
		exit(-1);
	}

	// size가 양수이면 new를 사용하여 배열 data를 할당, len 값 초기화
	this->data = new T[size];
	if(this->data == NULL) {
		cerr << "Memory allocation error!\n";
		exit(-1);
	}
	this->len = size;
}


// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 다중정의이다.
template <typename T>
T & GrowableArray<T>::operator[] (int i) const {	// left value : 배열에 원소를 삽입
	// 배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴
	if(0 <= i && i < this->len) {
		return this->data[i];
	}
	// 그렇지 않으면 사이즈가 가능할 때까지 크기를 2배 늘려서 추가된 공간은 0으로 초기화
	else {
        // 필요한 만큼 크기를 확보
        int new_len = this->len;
		while(new_len <= i)
            new_len *= 2;
        // 새로운 배열에 메모리를 할당
        T *new_data = new T[new_len];
        if(new_data == NULL) {
            cerr << "Memory allocation error!\n";
            exit(-1);
        }
        // 새로운 배열에 기존 배열의 내용을 복사
        for(int i = 0; i < this->len; i++)
            new_data[i] = this->data[i];
        for(int i = this->len; i < new_len; i++)
            new_data[i] = 0;
        // 기존 배열의 메모리를 해제
        T *t = this->data;
        this->data = new_data;
        this->len = new_len;
        delete[] t;
        // 0을 반환
		return this->data[i];
	}
}

#endif // __GROWABLE_ARRAY_H__
