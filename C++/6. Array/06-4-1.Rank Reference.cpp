#include<iostream>

int main()
{
	int scores[10]{};

	for (int i = 0; i < 10; i++)
	{
		std::cin >> scores[i];
	}

	int rank[10]{};

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (scores[i] < scores[j])
			{
				rank[i]++;
			}
		}
		rank[i]++;
	}

	for (int i = 0; i < 10; i++)
	{
		std::cout << rank[i] << ", ";
	}
}