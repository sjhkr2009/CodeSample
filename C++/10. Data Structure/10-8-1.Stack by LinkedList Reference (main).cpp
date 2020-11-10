#include "Stack.h"
#include <iostream>

void PrintInfo();
void ProcessUserInput(Stack& stack);

int main()
{
	Stack myStack;

	Initialize(myStack);

	PrintInfo();
	ProcessUserInput(myStack);

	ReleaseAll(myStack);
}

void PrintInfo()
{
	std::cout << "<STACK>" << std::endl;
	std::cout << "[1] push" << std::endl;
	std::cout << "[2] pop" << std::endl;
	std::cout << "[3] exit" << std::endl;
	std::cout << "-----------" << std::endl;
}

void ProcessUserInput(Stack& stack)
{
	int cmd{ -1 };
	bool isExit{ false };

	while (!isExit)
	{
		Print(stack);

		std::cout << "> ";
		std::cin >> cmd;

		switch (cmd)
		{
			case PUSH:
			{
				int value{};
				std::cout << "\tpush value >> ";
				std::cin >> value;
				Push(stack, value);
				break;
			}

			case POP:
				Pop(stack);
				break;
			
			case EXIT:
				isExit = true;
				break;
			
			default:
				std::cout << "Invalid Command!" << std::endl;
				break;
		}
	}
}
