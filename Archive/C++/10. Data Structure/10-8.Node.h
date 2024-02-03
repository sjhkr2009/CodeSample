#pragma once

#include <iostream>

struct ListNode
{
	int data{ 0 };

	ListNode* pPrev{ nullptr };
	ListNode* pNext{ nullptr };
};