#include<iostream>

int main()
{
	// BMI = (몸무게) / (키 * 키)

	float weight{};
	float height{};
	
	std::cout << "몸무게와 키를 입력하세요." << std::endl;

	std::cout << "몸무게(kg) : ";
	std::cin >> weight;
	std::cout << "키(m) : ";
	std::cin >> height;

	float bmi{};
	bmi = weight / (height * height);

	std::string result{};
	if (bmi <= 18.5f)
	{
		result = "저체중";
	}
	else if (bmi < 25)
	{
		result = "정상";
	}
	else if (bmi < 30)
	{
		result = "과체중";
	}
	else
	{
		result = "비만";
	}

	std::cout << std::endl << "당신은 " << result << " 입니다." 
		<< " (BMI : " << bmi << ")" <<  std::endl;
}