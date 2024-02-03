#pragma once

#include <iostream>

class String
{
private:
	char*	mString;
	int		mLength;

public:
	String();
	String(const char* content);
	String(int length);
	String(const String& string);

	~String();

public:
	int GetLength() const { return mLength; }
	char* GetContent() const { return mString; }

public:
	// 특정 인덱스의 글자 가져오기 (변환은 일어나지 않으니 const)
	char operator [] (int index) const;
	// 특정 인덱스에 글자 대입하기 (바뀐 값을 대입해야 하므로 참조형인 char& 으로 반환)
	char& operator [] (int index);

	// 문자열 합치기
	String operator + (const String& string);
	// 문자열 대입
	String operator = (const String& string);
	// 문자열 합친 후 대입 (+= 연산자는 연속해서 쓸 수 없으며, 사용후 문장이 끝나야 하므로 void 반환형을 갖는다)
	void operator += (const String& string);

	// 문자열 출력하기
	friend std::ostream& operator << (std::ostream& os, const String string);
};

