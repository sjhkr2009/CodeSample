﻿#include <iostream>
#include <memory>

// 가변 템플릿 (Variadic template)

class Factory
{
	
	/*
	아래와 같이 플레이어마다 다른 형태나 개수의 매개변수를 받아야 한다고 가정하자.
	Warrior* Make();
	Archer* Make(int arrow);
	Wizard* Make(Element element, int level);
	*/

	// 기존 템플릿의 타입 뒤에 ... 를 붙여서, 여러 개의 인자를 가변적으로 받을 수 있다.
	
	template<class T, class... Types>
	T* Make(Types&& ... args) // 가변 타입을 매개변수로 받을 때도 ... 를 사용하고 변수명을 적는다. 생성자에 변수를 값으로 전달해야 하므로 &&로 우측값을 넘긴다.
	{
		return new T( std::forward<Types>(args) ... ); // std::forward로 가변 인자를 생성자 T의 매개변수로 넣어준다.
		// forward에 관해서는 13-4 참고
	}

	template<class T, class... Types>
	T* Make_Unique(Types&& ... args)
	{
		auto ptr = unique_ptr<t>(new T(std::forward<Types>(args)...));
		return (T*)(ptr.get());
	}
};

int main()
{
	
}