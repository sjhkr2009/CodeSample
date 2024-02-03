#include<iostream>

int main()
{
	// while - 1부터 10까지 더하기

	int sum1{ 0 };
	int sum2{ 0 };
	int count{ 1 };

	do 
	{
		sum1 += count;
		++count;
	} while (count <= 10);

	count = 1;

	while (count <= 10)
	{
		sum2 += count;
		++count;
	}
	std::cout << sum1 << " " << sum2;
}