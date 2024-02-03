#include "Queue.h"
#include <iostream>

Queue::Queue() :
	mpFirst{ nullptr },
	mpLast{ nullptr },
	mCount{ 0 }
{
}

Queue::~Queue()
{
	mpFirst = nullptr;
	mpLast = nullptr;
}

void Queue::Enqueue(int value)
{
	Data* pNew = new Data(value);

	if (mCount == 0)
	{
		mpFirst = pNew;
		mpLast = pNew;
		mCount++;
		return;
	}

	mpLast->mpNext = pNew;
	pNew->mpPrev = mpLast;

	mpLast = pNew;
	mCount++;
}

bool Queue::Dequeue()
{
	if (mCount == 0)
	{
		std::cout << "QUEUE IS EMPTY!" << std::endl;
		return false;
	}

	Data* pDequeued{ mpFirst };
	mCount--;
	std::cout << pDequeued->mData << " is Dequeued." << std::endl;

	if (mCount == 0)
	{
		mpFirst = nullptr;
		mpLast = nullptr;
	}
	else
	{
		mpFirst = pDequeued->mpNext;
		mpFirst->mpPrev = nullptr;
	}

	delete pDequeued;
	return true;
}

void Queue::Print()
{
	Data* pCurrent{ mpFirst };

	while (pCurrent != nullptr)
	{
		std::cout << pCurrent->mData << ' ';
		pCurrent = pCurrent->mpNext;
	}

	std::cout << std::endl << 
		"Queue Count : " << mCount << std::endl;
}
