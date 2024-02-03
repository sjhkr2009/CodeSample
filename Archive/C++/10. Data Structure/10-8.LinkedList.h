#pragma once

#include "Node.h"

struct IntList
{
	ListNode* pHead{ nullptr };
	ListNode* pTail{ nullptr };
	int count{ 0 };

	ListNode* AddFirst(int value);
	ListNode* AddLast(int value);
	void PrintList();
	int RemoveFirst();
	int RemoveLast();
	bool IsEmpty();
	void Clear();
};