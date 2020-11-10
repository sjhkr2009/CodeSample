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
	// private ������ ������ ��� �Ѵٸ�, public �Լ��� ���� ������ �� �ִ�. �Լ��� const�� ���̸� ��� ������ ������ �� ����.
	// �� �Լ����� ���� ó���� ������ �߰����� ������ ������ �� �ִ�. �̷� �Լ��� ���� Getter / Setter ��� �Ѵ�. (C#�� ������Ƽ�� ���)
	int GetCount() const { return mCount; }
	void SetCount(int value) { mCount = (value >= 0) ? value : 0; }

	Stack();
	~Stack();
	void Push(int value);
	bool Pop();
	void Print();
};