#include<iostream>

int main()
{
	int scores[10]{};
	int sortedScores[10]{};
	int rankboard[10]{};

	int minScore{ static_cast<int>(1 << 31) - 1 };
	int maxScore{ static_cast<int>(1 << 31) };
	
	std::cout << "학생들의 점수를 입력하세요." << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "학생 " << i + 1 << ": ";
		std::cin >> scores[i];
		sortedScores[i] = scores[i];

		minScore = (scores[i] < minScore) ? scores[i] : minScore;
		maxScore = (scores[i] > maxScore) ? scores[i] : maxScore;

		for (int j = i; j > 0; j--)
		{
			if (sortedScores[j] > sortedScores[j - 1])
			{
				int temp{ sortedScores[j] };
				sortedScores[j] = sortedScores[j - 1];
				sortedScores[j - 1] = temp;
			}
			else
			{
				break;
			}
		}
	}

	// 1. 최고점/최저점 출력
	std::cout << "최고점 : " << maxScore << std::endl;
	std::cout << "최저점 : " << minScore << std::endl;

	// 2. 높은 점수 순서대로 출력
	int rank{ 1 };
	for (int i = 0; i < 10; i++)
	{
		if (i != 0 && (sortedScores[i] < sortedScores[i - 1]))
		{
			rank = i + 1;
		}
		std::cout << rank << "등 : " << sortedScores[i] << " 점" << std::endl;

		for (int j = 0; j < 10; j++)
		{
			if (scores[j] == sortedScores[i])
			{
				rankboard[j] = rank;
			}
		}
	}

	// 3. 각 학생의 등수 출력
	for (int i = 0; i < 10; i++)
	{
		std::cout << i + 1 << "번째 학생: " << scores[i] << "점, " << rankboard[i] << "등" << std::endl;
	}

}