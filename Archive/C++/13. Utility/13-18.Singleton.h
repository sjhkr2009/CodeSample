#pragma once

#include <iostream>

// �̱��� Ŭ������ �� ��ü�� ������ ������ �� �־�� �ϹǷ�, ��ӹ��� ���ϵ��� final�� ���δ�.
class Singleton final
{
	// �����ڴ� private���� �����.
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
	// ������ �Ϲ� ��� ������ �Լ��� ������ �״�� �ۼ��Ѵ�.
};

