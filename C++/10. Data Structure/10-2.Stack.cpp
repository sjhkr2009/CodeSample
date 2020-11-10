#include <iostream>

const int STACK_SIZE{ 10 };

enum Command
{
	PUSH = 1,
	POP = 2
};

struct Stack
{
	int container[STACK_SIZE]{};
	int topIndex{ -1 };
};

// 단순히 커맨드 정보를 출력하기 위한 함수
void PrintInfo();

// 사용자에게 스택에 대한 입력 받기
void ProcessUserInput(Stack& stack);

// 스택의 내용 출력
void PrintStack(Stack& stack);

// 스택에 내용 삽입/추출
void Push(Stack& stack, int value);
void Pop(Stack& stack);


int main()
{
	Stack myStack;

	PrintInfo();
	ProcessUserInput(myStack);
}

void PrintInfo()
{
	std::cout << "<STACK>" << std::endl <<
		"[1] PUSH, [2] POP" << std::endl <<
		"-------------------------" << std::endl;
}

void ProcessUserInput(Stack& stack)
{
	int command{}, value{};

	while (true)
	{
		std::cout << " > ";
		std::cin >> command;

		switch (command)
		{
			case PUSH:
				std::cout << "\tpush value >> ";
				std::cin >> value;
				Push(stack, value);
				break;

			case POP:
				Pop(stack);
				break;

			default:
				std::cout << "잘못된 명령입니다." << std::endl;
				break;
		}

		PrintStack(stack);
	}
}

void PrintStack(Stack& stack)
{
	std::cout << "----- stack -----" << std::endl;

	if (stack.topIndex < 0)
	{
		std::cout << "EMPTY" << std::endl;
		return;
	}

	for (int i = stack.topIndex; i >= 0; --i)
	{
		std::cout << stack.container[i] << std::endl;
	}

	std::cout << "------------------" << std::endl;
}

void Push(Stack& stack, int value)
{
	if (stack.topIndex >= STACK_SIZE - 1)
	{
		std::cout << " Stack is FULL!" << std::endl;
		return;
	}
	stack.container[++stack.topIndex] = value;
}

void Pop(Stack& stack)
{
	if (stack.topIndex < 0)
	{
		std::cout << " Stack is EMPTY!" << std::endl;
		return;
	}
	std::cout << stack.container[stack.topIndex--] << " is pop." << std::endl;
}
