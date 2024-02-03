#pragma once

#include <iostream>

class Node
{
public:
	Node(int value)
	{
		mData = value;
		mpNext = nullptr;
	}
	~Node()
	{
		mData = 0;
		mpNext = nullptr;
	}

public:
	int mData;
	Node* mpNext;
};

class Stack
{
public:
private:
	int mCount;
	Node* mpTop;

public:
	// private 변수를 가져다 써야 한다면, public 함수를 통해 제어할 수 있다. 함수에 const를 붙이면 멤버 변수를 변경할 수 없다.
	// 이 함수에서 예외 처리를 포함한 추가적인 동작을 실행할 수 있다. 이런 함수를 각각 Getter / Setter 라고 한다. (C#의 프로퍼티와 비슷)
	int GetCount() const { return mCount; }
	void SetCount(int value) { mCount = (value >= 0) ? value : 0; }

	Stack();
	~Stack();
	void Push(int value);
	bool Pop();
	void Print();
};