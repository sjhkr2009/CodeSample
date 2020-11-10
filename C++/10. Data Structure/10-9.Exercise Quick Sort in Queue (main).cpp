#include "Queue.h"
#include <iostream>

void PrintInfo();
void ProcessUserInput(Queue& queue);

int main()
{
	Queue myQueue;

	Initialize(myQueue);

	PrintInfo();
	ProcessUserInput(myQueue);

	ReleaseAll(myQueue);
}

void PrintInfo()
{
	std::cout << "<QUEUE>" << std::endl;
	std::cout << "[1] Enqueue" << std::endl;
	std::cout << "[2] Dequeue" << std::endl;
	std::cout << "[3] Sort" << std::endl;
	std::cout << "[9] Exit" << std::endl;
	std::cout << "-----------" << std::endl;
}

void ProcessUserInput(Queue& queue)
{
	int input{ -1 };
	bool isExit{ false };

	while (!isExit)
	{
		Print(queue);

		std::cout << "> ";
		std::cin >> input;

		switch (input)
		{
			case ENQUEUE:
			{
				int value{};
				std::cout << "\tEnqueue value >> ";
				std::cin >> value;
				Enqueue(queue, value);
				break;
			}

			case DEQUEUE:
				Dequeue(queue);
				break;

			case SORT:
				QSort(queue);
				break;

			case EXIT:
				std::cout << "큐 동작을 종료합니다." << std::endl;
				Print(queue);
				isExit = true;
				break;

			default:
				std::cout << "잘못된 입력입니다." << std::endl;
				break;
		}
	}
}
