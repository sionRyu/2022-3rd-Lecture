#include "RangeArray.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// 생성자
RangeArray::RangeArray (int low, int high) : Array (high-low+1) {
	this->low = low;
	this->high = high;
}

// 소멸자
RangeArray::~RangeArray () {
}

// index의 최소값을 리턴
int RangeArray::baseValue () {
	return low;
}

// index의 최대값을 리턴
int RangeArray::endValue () {
	return high;
}

// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 다중정의이다.
int & RangeArray::operator[](int i) {	// left value : 배열에 원소를 삽입
	static int tmp;
	// 배열의 인덱스가 범위 내에 있으면 해당 원소를 리턴
	if (low <= i && i <= high) {
		return data[i-low];
	}
	// 그렇지 않으면 에러메시지를 출력하고 tmp를 리턴
	else {
		cerr << "Array bound error!\n";
		return tmp;
	}
}

int RangeArray::operator[] (int i) const {	// right value : 배열의 원소의 값을 반환
	// 배열의 인덱스가 범위 내에 있으면 값을 리턴
	if (low <= i && i <= high) {
		return data[i-low];
	}
	// 그렇지 않으면 에러메시지를 출력하고 0을 리턴
	else {
		cerr << "Array bound error!\n";
		return 0;
	}
}
