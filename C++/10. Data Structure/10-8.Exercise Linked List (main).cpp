#include "LinkedList.h"

// 링크드 리스트를 통해 스택과 큐 동작 구현하기

void ProcessStack(IntList& stack);
void PrintStackManual();
void Push(IntList& stack);
int Pop(IntList& stack);
enum Stack
{
	PUSH = 1,
	POP = 2,
	END_STACK = 3,
	PRINT_STACK = 9
};

void ProcessQueue(IntList& queue);
void PrintQueueManual();
void Enqueue(IntList& queue);
int Dequeue(IntList& queue);
enum Queue
{
	ENQUEUE = 1,
	DEQUEUE = 4,
	END_QUEUE = 5,
	PRINT_QUEUE = 9
};

int main()
{
	IntList list{};

	while (true)
	{
		int input{};
		std::cout << "자료형을 선택하세요 (1: 스택, 2: 큐, 3: 종료) : ";
		std::cin >> input;

		if (input == 1)
		{
			ProcessStack(list);
		}
		else if (input == 2)
		{
			ProcessQueue(list);
		}
		else if (input == 3)
		{
			std::cout << "프로그램을 종료합니다." << std::endl;
			break;
		}
		else
		{
			std::cout << "잘못된 입력입니다." << std::endl;
		}
	}
}

void ProcessStack(IntList& stack)
{
	int input{};
	std::cout << "------ STACK ------" << std::endl;

	while (true)
	{
		PrintStackManual();
		std::cin >> input;

		switch (input)
		{
			case Stack::PUSH:
				Push(stack);
				break;
			case Stack::POP:
				Pop(stack);
				break;
			case Stack::END_STACK:
				std::cout << "스택을 종료합니다." << std::endl;
				stack.PrintList();
				stack.Clear();
				return;
			case Stack::PRINT_STACK:
				if (stack.count == 0)
				{
					std::cout << "Stack is Empty!" << std::endl;
				}
				else
				{
					stack.PrintList();
				}
				break;
			default:
				std::cout << "올바른 입력이 아닙니다." << std::endl;
				break;
		}

		input = 0;
	}
}

void PrintStackManual()
{
	std::cout << "수행할 동작을 입력하세요 (" <<
		static_cast<int>(Stack::PUSH) << ": PUSH, " <<
		static_cast<int>(Stack::POP) << ": POP, " <<
		static_cast<int>(Stack::END_STACK) << ": END, " <<
		static_cast<int>(Stack::PRINT_STACK) << ": PRINT STACK)" << std::endl <<
		">> ";
}

void Push(IntList& stack)
{
	int value{};

	std::cout << "\t값 입력 >> ";
	std::cin >> value;

	std::cout << std::endl;

	stack.AddLast(value);
}

int Pop(IntList& stack)
{
	if (stack.count == 0)
	{
		std::cout << "Stack is Empty!" << std::endl;
		return 0;
	}
	
	int value{ stack.RemoveLast() };
	std::cout << value << " is poped!" << std::endl;
	return value;
}

void ProcessQueue(IntList& queue)
{
	int input{};
	std::cout << "------ QUEUE ------" << std::endl;

	while (true)
	{
		PrintQueueManual();
		std::cin >> input;

		switch (input)
		{
			case Queue::ENQUEUE:
				Enqueue(queue);
				break;
			case Queue::DEQUEUE:
				Dequeue(queue);
				break;
			case Queue::END_QUEUE:
				std::cout << "스택을 종료합니다." << std::endl;
				queue.PrintList();
				queue.Clear();
				return;
			case Queue::PRINT_QUEUE:
				if (queue.count == 0)
				{
					std::cout << "Queue is Empty!" << std::endl;
				}
				else
				{
					queue.PrintList();
				}
				break;
			default:
				std::cout << "올바른 입력이 아닙니다." << std::endl;
				break;
		}

		input = 0;
	}
}

void PrintQueueManual()
{
	std::cout << "수행할 동작을 입력하세요 (" <<
		static_cast<int>(Queue::ENQUEUE) << ": ENQUEUE, " <<
		static_cast<int>(Queue::DEQUEUE) << ": DEQUEUE, " <<
		static_cast<int>(Queue::END_QUEUE) << ": END, " <<
		static_cast<int>(Queue::PRINT_QUEUE) << ": PRINT QUEUE)" << std::endl <<
		">> ";
}

void Enqueue(IntList& queue)
{
	int value{};

	std::cout << "\t값 입력 >> ";
	std::cin >> value;
	
	std::cout << std::endl;

	queue.AddLast(value);
}

int Dequeue(IntList& queue)
{
	if (queue.count == 0)
	{
		std::cout << "Queue is Empty!" << std::endl;
		return 0;
	}
	
	int value{ queue.RemoveFirst() };
	std::cout << value << " is dequeued!" << std::endl;
	return value;
}
