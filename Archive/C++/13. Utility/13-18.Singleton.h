#pragma once

#include <iostream>

// 싱글톤 클래스는 그 자체로 역할을 수행할 수 있어야 하므로, 상속받지 못하도록 final을 붙인다.
class Singleton final
{
	// 생성자는 private으로 숨긴다.
private:
	Singleton() {}
	Singleton(const Singleton&) {}
	void operator=(const Singleton&) {}

public:
	~Singleton();
	static Singleton& Instance()
	{
		static Singleton instance;
		return instance;
	}

public:
	void Func1();
	void Func2();
	// 나머지 일반 멤버 변수나 함수는 기존과 그대로 작성한다.
};

