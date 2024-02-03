#include <iostream>
#include <vector>

// Vector : 컨테이너에서 제공하는 동적 배열의 하나. (C#의 List와 유사함)

class Monster
{
public:
	int mHp;
	int mAttack;

	void Print()
	{
		std::cout << mHp << ", " << mAttack << std::endl;
	}
};

// (편의상 만들어둠)
void PrintTitle(std::string text = "")
{
	std::cout << text << std::endl;
}

int main()
{
	PrintTitle("기존 클래스 배열");
	// 기존의 클래스 배열 선언
	Monster monsters[]{
		{10, 15},
		{20, 25},
		{30, 40}
	};
	for (int i = 0; i < 3; i++)
	{
		monsters[i].Print();
	}

	PrintTitle("벡터를 통해 선언한 클래스 배열");
	// vector를 통한 선언. 초기화식은 똑같이 동작한다.
	std::vector<Monster> mVector{
		{10, 12},
		{15, 20},
		{26, 33}
	};
	// 인덱서도 사용 가능하지만, 배열과 달리 크기가 고정되지 않는다. size()로 원소의 개수를 가져올 수 있다. (C#의 List.Count)
	for (int i = 0; i < mVector.size(); i++)
	{
		mVector[i].Print();
	}

	PrintTitle("벡터: 원소 추가 및 이터레이터 테스트");
	// push_back : 원소 추가 (C#의 List.Add와 유사함)
	Monster newOne;
	newOne.mHp = 50;
	newOne.mAttack = 15;
	mVector.push_back(newOne);

	mVector.push_back(Monster{ 30, 17 });

	// iterator: vector 내의 특정 원소를 가리키는 포인터. ++ 연산자로 다음 원소로 이동할 수 있다.
	// begin()과 end()로 벡터의 첫 원소와 마지막 원소에 접근할 수 있다.
	std::vector<Monster>::iterator itr;

	for (itr = mVector.begin(); itr != mVector.end(); itr++)
	{
		itr->Print();
	}

	PrintTitle("auto 키워드로 출력");
	// auto 키워드로 컴파일러가 타입을 알아서 판단하게 할 수 있다. (C#의 var)
	// 표기가 긴 iterator와 같은 타입의 선언을 이렇게 줄일 수 있다.
	for (auto itr2 = mVector.begin(); itr2 != mVector.end(); itr2++)
	{
		itr2->Print();
	}

	PrintTitle("범위 기반 for문");
	// for(타입 원소 : 컨테이너) 형태를 통해, 범위 기반(Range Based)의 반복문을 만들 수 있다. (C#의 foreach)
	for (auto& element : mVector) // 각 원소의 복사가 일어나지 않도록 auto에 &를 붙여 참조형으로 만들어준다.
	{
		element.Print();
	}

}
