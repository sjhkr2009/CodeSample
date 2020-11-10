﻿#include "Queue.h"
#include <iostream>

void PrintInfo();
void ProcessUserInput(Queue& queue);

int main()
{
	Queue myQueue{};

	PrintInfo();
	ProcessUserInput(myQueue);
}

void PrintInfo()
{
	std::cout << "<QUEUE>" << std::endl;
	std::cout << "[1] Enqueue" << std::endl;
	std::cout << "[2] Dequeue" << std::endl;
	//std::cout << "[3] Sort" << std::endl;
	std::cout << "[9] Exit" << std::endl;
	std::cout << "-----------" << std::endl;
}

void ProcessUserInput(Queue& queue)
{
	int input{ -1 };
	bool isExit{ false };

	while (!isExit)
	{
		queue.Print();

		std::cout << "> ";
		std::cin >> input;

		switch (input)
		{
			case Queue::ENQUEUE:
			{
				int value{};
				std::cout << "\tEnqueue value >> ";
				std::cin >> value;
				queue.Enqueue(value);
				break;
			}

			case Queue::DEQUEUE:
				queue.Dequeue();
				break;

			/*case SORT:
				QSort(queue);
				break;*/

			case Queue::EXIT:
				std::cout << "큐 동작을 종료합니다." << std::endl;
				queue.Print();
				isExit = true;
				break;

			default:
				std::cout << "잘못된 입력입니다." << std::endl;
				break;
		}
	}
}
