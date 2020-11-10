#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList() :
	mCount(0),
	mpFirst(nullptr),
	mpLast(nullptr)
{
}

LinkedList::~LinkedList()
{
	RemoveAll();
	
	mCount = 0;
	mpFirst = nullptr;
	mpLast = nullptr;
}

void LinkedList::AddLast(int value)
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

bool LinkedList::RemoveFirst()
{
	if (mCount == 0)
	{
		std::cout << Log_Empty() << std::endl;
		return false;
	}

	Data* pFirst{ mpFirst };
	mCount--;
	std::cout << pFirst->mData << Log_RemoveFirst() << std::endl;

	if (mCount == 0)
	{
		mpFirst = nullptr;
		mpLast = nullptr;
	}
	else
	{
		mpFirst = pFirst->mpNext;
		mpFirst->mpPrev = nullptr;
	}

	delete pFirst;
	return true;
}

bool LinkedList::RemoveLast()
{
	if (mCount == 0)
	{
		std::cout << Log_Empty() << std::endl;
		return false;
	}

	Data* pLast{ mpFirst };
	mCount--;
	std::cout << pLast->mData << Log_RemoveLast() << std::endl;

	if (mCount == 0)
	{
		mpFirst = nullptr;
		mpLast = nullptr;
	}
	else
	{
		mpLast = pLast->mpPrev;
		mpLast->mpNext = nullptr;
	}

	delete pLast;
	return true;
}

void LinkedList::RemoveAll()
{
	Data* pCurrent{ mpFirst };
	Data* pNext{ nullptr };

	while (pCurrent != nullptr)
	{
		pNext = pCurrent->mpNext;
		delete pCurrent;
		pCurrent = pNext;
	}

	mCount = 0;
}

void LinkedList::Print()
{
	if (mCount == 0)
	{
		std::cout << Log_Empty() << std::endl;
		return;
	}
	
	Data* pCurrent{ mpFirst };

	while (pCurrent != nullptr)
	{
		std::cout << pCurrent->mData << ' ';
		pCurrent = pCurrent->mpNext;
	}
	std::cout << std::endl;
}

void LinkedList::Print_r()
{
	if (mCount == 0)
	{
		std::cout << Log_Empty() << std::endl;
		return;
	}

	Data* pCurrent{ mpLast };

	while (pCurrent != nullptr)
	{
		std::cout << pCurrent->mData << ' ';
		pCurrent = pCurrent->mpPrev;
	}
	std::cout << std::endl;
}

std::string Queue::Log_Empty() const
{
	return "Queue is Empty!";
}

std::string Queue::Log_RemoveFirst() const
{
	return " is Dequeued.";
}

std::string Stack::Log_Empty() const
{
	return "Stack is Empty!";
}

std::string Stack::Log_RemoveLast() const
{
	return " is Poped.";
}
