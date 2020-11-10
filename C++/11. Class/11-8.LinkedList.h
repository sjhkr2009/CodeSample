#pragma once

#include <iostream>

class Data
{
public:
	int mData;
	Data* mpNext;
	Data* mpPrev;

public:
	Data(int value) :
		mData{ value },
		mpNext{ nullptr },
		mpPrev{ nullptr }
	{}
	~Data()
	{
		mpNext = nullptr;
		mpPrev = nullptr;
	}
};

class LinkedList
{
public:
	Data* mpFirst;
	Data* mpLast;

private:
	int mCount;

public:
	LinkedList();
	~LinkedList();
	int GetCount() const { return mCount; }

	void AddLast(int value);
	bool RemoveFirst();
	bool RemoveLast();
	void RemoveAll();
	void Print();
	void Print_r();

	virtual std::string Log_Empty() const { return "Linked List is Empty!"; }
	virtual std::string Log_RemoveFirst() const { return ""; }
	virtual std::string Log_RemoveLast() const { return ""; }
};

class Queue : LinkedList
{
public:
	enum Command
	{
		ENQUEUE = 1,
		DEQUEUE = 2,
		PRINT = 3,
		CLEAR = 8,
		EXIT = 9
	};

public:
	void Enqueue(int value) { AddLast(value); }
	bool Dequeue() { return RemoveFirst(); }
	void Print() 
	{ 
		LinkedList::Print();
		std::cout << "Queue Count : " << GetCount() << std::endl;
	}
	void Clear() { RemoveAll(); }

	virtual std::string Log_Empty() const;
	virtual std::string Log_RemoveFirst() const;
};

class Stack : LinkedList
{
public:
	enum Command
	{
		PUSH = 1,
		POP = 2,
		PRINT = 3,
		CLEAR = 8,
		EXIT = 9
	};

public:
	void Push(int value) { AddLast(value); }
	bool Pop() { return RemoveLast(); }
	void Print() 
	{
		LinkedList::Print_r(); 
		std::cout << "Stack Count : " << GetCount() << std::endl;
	}
	void Clear() { RemoveAll(); }

	virtual std::string Log_Empty() const;
	virtual std::string Log_RemoveLast() const;
};
