#include <iostream>

enum Type
{
	INT, FLOAT, DOUBLE
};

void Add(void* a, void* b, void* result, Type type)
{
	// 1. 연산하려는 a,b를 타입에 맞는 포인터로 변환하고, 해당 포인터에 *를 붙여 값을 가져와 더한다.
	// 2. 결과값을 저장해 둔다. (case문 내에서 변수를 선언하려면 블록을 사용해야 함)
	// 3. 결과를 저장할 매개변수도 타입에 맞는 포인터로 변환한 후, 포인터의 값에 결과값을 대입한다. (case문 내부 변수는 블록이 끝나면 초기화되니 주의)

	// 결과 저장할 곳을 매개변수로 받는 대신 반환형으로 쓰고 싶으면, 반환형을 auto로 하고 -> (a) 와 같이 타입을 지정하면 된다.
	// 이 방식에 관해서는 13-8 (Decltype) 코드 참조.

	switch (type)
	{
		case INT:
		{
			int intValue{ *static_cast<int*>(a) + *static_cast<int*>(b) };
			*static_cast<int*>(result) = intValue;
			return;
		}
		case FLOAT:
		{
			float floatValue{ *static_cast<float*>(a) + *static_cast<float*>(b) };
			*static_cast<float*>(result) = floatValue;
			return;
		}
		case DOUBLE:
		{
			double doubleValue{ *static_cast<double*>(a) + *static_cast<double*>(b) };
			*static_cast<double*>(result) = doubleValue;
			return;
		}
		default:
			return;
	}
}


int main()
{
	int a{ 3 };
	int b{ 4 };
	int ab{};

	float c{ 2.7f };
	float d{ 3.1f };
	float cd{};

	Add(&a, &b, &ab, INT);
	Add(&c, &d, &cd, FLOAT);

	std::cout << ab << '\t' << cd << std::endl;
}