#include<iostream>

int main()
{
	int scores[10]{};
	int amount{ 0 };

	std::cout << "각 학생의 성적을 입력하세요." << std::endl;

	for (int i = 0; i < 10; i++)
	{
		std::cout << "학생" << i + 1 << ": ";
		std::cin >> scores[i];
		amount += scores[i];
	}

	float average{};
	average = (float)amount / 10;

	std::cout << "평균 점수 : " << average << " 점";
}