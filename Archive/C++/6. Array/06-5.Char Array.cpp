#include<iostream>

int main()
{
	float value{ 10.396738f };
	char output[128]{};

	//원래 sprintf를 사용했으나, 문자열을 담을 배열 공간이 부족하면 buffer overrun이 발생하므로 안전하지 않다. sprintf_s는 safe의 약자 s를 붙여서, 버퍼의 크기를 지정한다.
	sprintf_s(output, 128, "%.1f", value); //%는 형식을 지정한다는 의미, f는 float로 지정한다는 의미.

	std::cout << output << std::endl;

	for (int i = 0; i < 20; i++)
	{
		sprintf_s(output, 128, "%2d", i);
		std::cout << output << std::endl;
		
		sprintf_s(output, 128, "%02d", i);
		std::cout << output << std::endl;

		sprintf_s(output, 128, "%5d", i);
		std::cout << output << std::endl;

		sprintf_s(output, 128, "%d번 학생의 점수는 %02.2f입니다.", i, value);
		std::cout << output << std::endl;
	}
}