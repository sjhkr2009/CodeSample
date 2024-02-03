#include "Stack.h"
#include <iostream>

int main()
{
	Stack myStack;

	myStack.Push(3);
	myStack.Push(4);
	myStack.Push(10);

	myStack.Print();
	std::cout << "--------------------" << std::endl;

	myStack.Pop();
	myStack.Print();
}