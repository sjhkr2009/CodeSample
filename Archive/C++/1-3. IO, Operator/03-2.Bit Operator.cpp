#include<iostream>

int main()
{
	unsigned int color{};

	int alpha{ 255 };
	int red{ 255 };
	int green{ 0 };
	int blue{ 0 };

	color = (alpha << 24) | (red << 16) | (green << 8) | (blue);

	std::cout << color
		<< " (" << std::hex << color << std::dec << ") "
		<< std::endl;

	int a{};
	int r{};
	int g{};
	int b{};

	a = color >> 24;
	r = (color << 8) >> 24;
	g = (color << 16) >> 24;
	b = (color << 24) >> 24;
	std::cout << "Alpha : " << a << std::endl
		<< "Red : " << r << std::endl
		<< "Green : " << g << std::endl
		<< "Blue : " << b << std::endl;

	a = (color >> 24) & 0xFF;
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = (color >> 0) & 0xFF; // n을 매개변수로 (color >> n) & 0xFF를 반환하는 형태의 함수로 만들 수 있다.
	std::cout << "Alpha : " << a << std::endl
		<< "Red : " << r << std::endl
		<< "Green : " << g << std::endl
		<< "Blue : " << b << std::endl;
}