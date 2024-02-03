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
	// Ư�� �ε����� ���� �������� (��ȯ�� �Ͼ�� ������ const)
	char operator [] (int index) const;
	// Ư�� �ε����� ���� �����ϱ� (�ٲ� ���� �����ؾ� �ϹǷ� �������� char& ���� ��ȯ)
	char& operator [] (int index);

	// ���ڿ� ��ġ��
	String operator + (const String& string);
	// ���ڿ� ����
	String operator = (const String& string);
	// ���ڿ� ��ģ �� ���� (+= �����ڴ� �����ؼ� �� �� ������, ����� ������ ������ �ϹǷ� void ��ȯ���� ���´�)
	void operator += (const String& string);

	// ���ڿ� ����ϱ�
	friend std::ostream& operator << (std::ostream& os, const String string);
};

