#ifndef __STR_H__
#define __STR_H__

class Str {
private:
	char *str; 	// string의 내용
	int len; 	// stirng의 길이

public:
	Str(int); // string의 길이를 입력 받는 생성자
	Str(char *); // string의 내용을 입력 받는 생성자
	~Str(); // 소멸자
	int length(); // string의 길이를 리턴하는 함수
	char *contents(); // string의 내용을 리턴하는 함수
	int compare(class Str&); // 입력받은 Str의 내용과 strcmp
	int compare(char *); // 입력받은 string의 내용과 strcmp
	void operator= (char *); // string의 값을 대입
	void operator= (class Str&); // Str의 내용을 대입
};

#endif // __STR_H__
