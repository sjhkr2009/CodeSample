#include<iostream>

int main()
{
	int triangles[]
	{
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1
	};

	int row{};
	int column{};

	std::cout << "행과 열을 입력하세요." << std::endl;
	std::cout << "행(1~3) : ";
	std::cin >> row;
	std::cout << "열(1~5) : ";
	std::cin >> column;

	int targetIndex{};
	targetIndex = ((row - 1) * 5) + (column - 1);

	std::cout << row << "행 " << column << "열의 값은 " << triangles[targetIndex] << " 입니다.";
}