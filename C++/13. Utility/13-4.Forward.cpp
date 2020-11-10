#include <iostream>

// R-value Reference : 참조형 R-value. 값을 의미 그대로 ‘이동’시키기 위해 도입. type&& 형태를 가진다.

// 팩토리 패턴을 통해 객체를 생성할 때 매개변수를 변수가 아닌 R-value로 넘길 때가 있다. 이 때 매개변수를 온전하게 넘겨주기 위해 사용된다.
//		ㄴ 이 개념을 perfect forwarding이라 하며, std::forward<T>(a)와 같이 사용한다.

class Factory
{
public:
	template<typename T>
	static void Create(T&& x)
	{
		std::cout << std::forward<T>(x) << " Created." << std::endl;
	}
};

int main()
{
	Factory::Create(2);
	Factory::Create("ㅎㅎ");
	Factory::Create(5.f);
	Factory::Create('a');
}