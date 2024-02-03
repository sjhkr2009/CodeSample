#include <iostream>

struct Stat 
{
	int hp;
	int mp;
};

int main()
{
	// 동적 메모리 할당 (Dynamic Memory Allocation)

	int array[5]{};
	// 지금까지는 배열 개수를 고정적으로 지정했다.
	// 배열에 크기를 동적으로 할당하는 방식으로 변경해보자.

	// 동적 배열을 가리킬 포인터
	int* pArray{};

	
	// 1. 옛날 스타일(1) - C Allocation
	
	// 배열의 원소 개수, 원소 하나의 크기(byte)를 입력한다. sizeof로 크기(byte)를 구할 수 있다.
	// 배열을 만든 후 첫 번째 원소의 포인터를 void*로 반환한다. 원하는 타입의 포인터로 캐스팅하여 가져온다.
	pArray = static_cast<int*>(calloc(5, sizeof(int)));

	// calloc을 통해 만든 배열은 자동으로 값이 초기화된다. 즉 int의 기본값인 0 이 들어가 있다.
	std::cout << "calloc : " << pArray[0] << std::endl;

	// 메모리 해제는 free를 사용한다. 수동으로 해제해주지 않으면 프로그램을 종료할 때까지 메모리 공간을 계속 사용하게 된다.
	// calloc을 사용할 때는 free를 잊어버리지 않도록 주의한다.
	free(pArray);



	// 2. 옛날 스타일(2) - Memory Allocation

	// 메모리에 할당할 공간의 크기를 바이트 단위로 직접 지정한다.
	// 위와 달리 할당할 크기만 입력하므로, 원소 하나의 크기에 원소 개수를 곱한 값을 입력해야 한다.
	pArray = static_cast<int*>(malloc(5 * sizeof(int)));

	// calloc과 달리 값을 초기화하지 않는다.
	// 따라서 속도가 더 빠르기 때문에, 게임 프로그래머는 calloc보단 이쪽을 사용한다.
	std::cout << "malloc : " << pArray[0] << std::endl;

	// malloc 역시 메모리 해제를 잊지 않도록 유의한다.
	free(pArray);



	// 3. 모던 C++ - new / delete

	// 옛날 스타일은 선언하는 과정이 너무 길다는 단점이 있다. (바이트 크기와 캐스팅 등을 모두 적어야 함)
	// 모던 C++에서는 new type[] 으로 선언하는 방식을 사용한다.
	pArray = new int[5];

	// free 대신 delete로 메모리를 해제한다. new로 배열을 할당했으므로 delete에 배열임을 명시하여 delete[]로 쓰는 게 좋다.
	delete[] pArray;



	// 4. 응용
	
	// 변수 하나인 경우
	int* pInt{};
	pInt = new int;
	delete pInt;


	// 구조체인 경우
	Stat* pStat{};
	pStat = new Stat;
	
	pStat->hp = 100; // 구조체 포인터로 값 할당하기. (*Stat).hp 와 동일하다.
	pStat->mp = 10;

	delete pStat;


	// 주의사항 (1)
	// 메모리 공간이 부족하면 에러는 뜨지 않으나, new가 실행되지 않아서 포인터가 nullptr이 된다.
	// 안전하게 쓰려면 if(포인터 != nullptr) 를 체크해주는게 좋다.
	// 안 쓰는 프로그래머도 많고 어차피 프로그램이 종료되므로 체크해야 하는 것은 아니지만, 유저 입장에서 게임이 그냥 꺼지는 것보다 메모리 부족을 알려주고 꺼지는 게 나으니 체크해주는게 좋다.


	// 주의사항 (2)
	// 이미 사용이 끝나 메모리 해제된 포인터는, 누군가 다시 사용하지 않도록 빈 포인터로 바꿔준다.
	pInt = nullptr;
	pStat = nullptr;

	// ★ 정리 ★
	// 1) new 이후에 delete 꼭 사용하기
	// 2) new가 실행되었는지 예외처리 해주기
	// 3) delete 후에는 포인터 초기화하기
}