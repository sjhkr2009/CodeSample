#include <iostream>

const int QUEUE_SIZE{ 10 };

enum Command
{
	ENQUEUE = 1,
	DEQUEUE = 2
};

struct Queue
{
	int container[QUEUE_SIZE]{};
	int head{ 0 };
	int tail{ 0 };
	int count{ 0 };
};

// 단순히 커맨드 정보를 출력하기 위한 함수
void PrintInfo();

// 사용자에게 큐에 대한 입력 받기
void ProcessUserInput(Queue& queue);

// 큐의 내용 출력
void PrintQueue(Queue& queue);

// 큐에 내용 삽입/추출
void Enqueue(Queue& queue, int value);
void Dequeue(Queue& queue);


int main()
{
	Queue myQueue;

	PrintInfo();
	ProcessUserInput(myQueue);
}

void PrintInfo()
{
	std::cout << "<QUEUE>" << std::endl <<
		"[1] ENQUEUE, [2] DEQUEUE" << std::endl <<
		"-------------------------" << std::endl;
}

void ProcessUserInput(Queue& queue)
{
	int command{}, value{};

	while (true)
	{
		std::cout << " > ";
		std::cin >> command;

		switch (command)
		{
			case ENQUEUE:
				std::cout << "\tEnqueue value >> ";
				std::cin >> value;
				Enqueue(queue, value);
				break;

			case DEQUEUE:
				Dequeue(queue);
				break;

			default:
				std::cout << "잘못된 명령입니다." << std::endl;
				break;
		}

		PrintQueue(queue);
	}
}

void PrintQueue(Queue& queue)
{
	std::cout << "----- queue -----" << std::endl;

	if (queue.count <= 0)
	{
		std::cout << "EMPTY" << std::endl;
		return;
	}
	
	for (int i = 0; i < queue.count; i++)
	{
		int index{ (queue.head + i) % QUEUE_SIZE };
		std::cout << queue.container[index] << ' ';
	}
	std::cout << std::endl;

	std::cout << "------------------" << std::endl;
}

void Enqueue(Queue& queue, int value)
{
	// count를 세지 않을 경우, head == ((tail + 1) % QUEUE_SIZE) 일 경우 가득 찬 것으로 판단한다.
	if (queue.count >= QUEUE_SIZE)
	{
		std::cout << " Queue is FULL!" << std::endl;
		return;
	}

	queue.container[queue.tail] = value;
	queue.tail = (queue.tail + 1) % QUEUE_SIZE;
	queue.count++;
}

void Dequeue(Queue& queue)
{
	// count를 세지 않을 경우, head == tail 일 경우 가득 찬 것으로 판단한다.
	if (queue.count <= 0)
	{
		std::cout << " Queue is EMPTY!" << std::endl;
		return;
	}
	std::cout << queue.container[queue.head] << " is Dequeued." << std::endl;
	queue.head = (queue.head + 1) % QUEUE_SIZE;
	queue.count--;
}