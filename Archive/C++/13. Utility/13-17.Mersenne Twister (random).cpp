#include <iostream>
#include <time.h>
#include <random>
#include <vector>

void Title(std::string text) { std::cout << text << " : "; }
void Endl() { std::cout << std::endl; }

int main()
{
	// 이전에 사용한 난수 생성 로직
	std::srand(static_cast<unsigned int>(time(nullptr)));
	Title("srand를 통한 랜덤 출력");
	for (int i = 0; i < 10; i++)
	{
		std::cout << rand() << ' ';
	}
	Endl();

	// 다만 위 로직도 시간을 시드로 넣는다는 명백한 정보가 주어지며, 패턴을 분석하여 해킹당할 수 있다.
	// 실제 게임에서는 메르센 트위스터 알고리즘을 많이 사용한다.
	std::random_device rd;	// 컴퓨터 부품 중 임의로 하나의 디바이스 번호를 반환한다.
	std::mt19937 rnd( rd() );	// 메르센 트위스터는 std::mt19937 로 사용한다. #include <random> 필요

	Title("메르센 트위스터");
	for (int i = 0; i < 20; i++)
	{
		// random_device와 mt19937 개체는 모두 () 연산자로 값을 사용할 수 있다.
		std::cout << rnd() << ' ';
	}
	Endl();



	// 메르센 트위스터 활용

	// a부터 b까지의 수 중에 랜덤한 숫자를 뽑는 방법은 다음과 같다.
	// a + (rand() % (b - a + 1)) 

	int startPoint{ 0 };
	int endPoint{ 10 };
	int count{ endPoint - startPoint + 1 };

	std::random_device rd;
	std::mt19937 gen(rd());
	Title("메르센 트위스터 - 0부터 10까지");
	for (int i = 0; i < 20; i++)
	{
		std::cout << startPoint + (gen() % count) << ' ';
	}
	Endl();


	// 메르센 트위스터에서는 확률의 분포를 제어할 수 있다.
	// 개발자가 확률을 제어할 때 사용된다.

	// 균등 분포 :	모든 숫자가 균등한 비율로 등장한다.
	//				한두 개의 숫자가 과도하게 많이 등장하는 현상을 방지할 수 있다.

	std::uniform_int_distribution<> dist(startPoint, endPoint);
	Title("메르센 트위스터 - 0부터 10까지 (균등 분포)");
	for (int i = 0; i < 20; i++)
	{
		std::cout << dist(gen) << ' ';
	}
	Endl();

	// 덱 셔플하기
	std::vector<int> deck{ 1,2,3,4,5,6,7,8,9,10 };
	std::uniform_int_distribution<> dist2(0, deck.size() - 1);

	int index1, index2;
	for (int i = 0; i < deck.size(); i++)
	{
		index1 = i;
		index2 = dist(gen);
		std::swap(deck[index1], deck[index2]);
	}
	Title("덱 셔플하기 - 메르센 트위스터");
	for (auto e : deck)
	{
		std::cout << e << ' ';
	}
	Endl();

	// 참고로 기본 제공되는 셔플 기능은 다음과 같다. (시작점, 종료점, 셔플 방식으로 메르센 트위스터 클래스를 넘겨준다)
	std::shuffle(deck.begin(), deck.end(), gen);
	Title("덱 셔플하기 - STL 제공");
	for (auto e : deck)
	{
		std::cout << e << ' ';
	}
	Endl();
}