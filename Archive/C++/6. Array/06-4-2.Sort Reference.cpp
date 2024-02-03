#include<iostream>

int main()
{
	int scores[10]{};

	for (int i = 0; i < 10; i++)
	{
		std::cin >> scores[i];
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = i + 1; j < 10; j++)
		{
			if (scores[i] < scores[j])
			{
				int temp{ scores[i] };
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		std::cout << scores[i] << ", ";
	}
}