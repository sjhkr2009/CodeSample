#pragma once

enum Command
{
	PUSH = 1,
	POP = 2,
	EXIT = 3
};

struct Element
{
	int value{};
	Element* pNext{ nullptr };
};

struct Stack
{
	Element* pTop{};
	int count;
};

void Initialize(Stack& stack);
void ReleaseAll(Stack& stack);
void Push(Stack& stack, int value);
bool Pop(Stack& stack);
void Print(Stack& stack);