#include "Str.h"
#include <cstring>
#include <iostream>
using namespace std;

// string의 길이를 입력받아 그만한 길이의 공백 문자열을 만듬
Str::Str (int leng) {
	// 예외처리: 길이가 0 미만이면 에러 메시지 출력
	if(leng < 0) {
		cerr << "Invalid string length error\n";
		len = 0;
	}
	str = new char[leng+1];
	for(int i = 0; i <= leng; i++) {
		str[i] = ' ';
	}
	str[leng] = '\0';
	len = leng;
}

// string의 내용을 입력받아 그대로 초기화함
Str::Str (char * neyong) {
	// 예외처리: 입력받은 char *가 NULL을 가리킴
	if(neyong == NULL) {
		cerr << "Invalid char pointer error\n";
		str = new char[1];
		str[0] = '\0';
		len = 0;
	}
	else {
		len = strlen(neyong);
		str = new char[len+1];
		strcpy(str, neyong);
	}
}

// 소멸자
Str::~Str () {
	delete[] str;
}

// string의 길이를 리턴하는 함수
int Str::length (void) {
	return len;
}

// string의 내용을 리턴하는 함수
char * Str::contents (void) {
	return str;
}

// string의 내용을 다른 string(class Str 타입 또는 char * 타입)과 비교하는 함수
int Str::compare (class Str& a) {
	return strcmp(str, a.contents());
}

int Str::compare(char *a) {
	return strcmp(str, a);
}

// 입력받은 a의 내용을 복사해오는 함수
void Str::operator=(char * a) {
	// 예외처리: 입력받은 char *가 NULL을 가리킴
	delete[] str;
	if(a == NULL) {
		len = 0;
		str = new char[0];
		str[0] = '\0';
	}
	else {
		len = strlen(a);
		str = new char[len+1];
		strcpy(str, a);
	}
}

void Str::operator=(class Str& a) {
	delete[] str;
	len = a.length();
	str = new char[len+1];
	strcpy(str, a.contents());
}
