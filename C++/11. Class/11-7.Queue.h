#pragma once

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

class Queue
{
public:
	enum Command
	{
		ENQUEUE = 1,
		DEQUEUE = 2,
		EXIT = 9
	};

	Data* mpFirst;
	Data* mpLast;
	
private:
	int mCount;

public:
	Queue();
	~Queue();
	int GetCount() const { return mCount; }

	void Enqueue(int value);
	bool Dequeue();
	void Print();
};