#include <iostream>
#include <time.h>
#include <random>
#include <vector>

void Title(std::string text) { std::cout << text << " : "; }
void Endl() { std::cout << std::endl; }

int main()
{
	// ������ ����� ���� ���� ����
	std::srand(static_cast<unsigned int>(time(nullptr)));
	Title("srand�� ���� ���� ���");
	for (int i = 0; i < 10; i++)
	{
		std::cout << rand() << ' ';
	}
	Endl();

	// �ٸ� �� ������ �ð��� �õ�� �ִ´ٴ� ����� ������ �־�����, ������ �м��Ͽ� ��ŷ���� �� �ִ�.
	// ���� ���ӿ����� �޸��� Ʈ������ �˰����� ���� ����Ѵ�.
	std::random_device rd;	// ��ǻ�� ��ǰ �� ���Ƿ� �ϳ��� ����̽� ��ȣ�� ��ȯ�Ѵ�.
	std::mt19937 rnd( rd() );	// �޸��� Ʈ�����ʹ� std::mt19937 �� ����Ѵ�. #include <random> �ʿ�

	Title("�޸��� Ʈ������");
	for (int i = 0; i < 20; i++)
	{
		// random_device�� mt19937 ��ü�� ��� () �����ڷ� ���� ����� �� �ִ�.
		std::cout << rnd() << ' ';
	}
	Endl();



	// �޸��� Ʈ������ Ȱ��

	// a���� b������ �� �߿� ������ ���ڸ� �̴� ����� ������ ����.
	// a + (rand() % (b - a + 1)) 

	int startPoint{ 0 };
	int endPoint{ 10 };
	int count{ endPoint - startPoint + 1 };

	std::random_device rd;
	std::mt19937 gen(rd());
	Title("�޸��� Ʈ������ - 0���� 10����");
	for (int i = 0; i < 20; i++)
	{
		std::cout << startPoint + (gen() % count) << ' ';
	}
	Endl();


	// �޸��� Ʈ�����Ϳ����� Ȯ���� ������ ������ �� �ִ�.
	// �����ڰ� Ȯ���� ������ �� ���ȴ�.

	// �յ� ���� :	��� ���ڰ� �յ��� ������ �����Ѵ�.
	//				�ѵ� ���� ���ڰ� �����ϰ� ���� �����ϴ� ������ ������ �� �ִ�.

	std::uniform_int_distribution<> dist(startPoint, endPoint);
	Title("�޸��� Ʈ������ - 0���� 10���� (�յ� ����)");
	for (int i = 0; i < 20; i++)
	{
		std::cout << dist(gen) << ' ';
	}
	Endl();

	// �� �����ϱ�
	std::vector<int> deck{ 1,2,3,4,5,6,7,8,9,10 };
	std::uniform_int_distribution<> dist2(0, deck.size() - 1);

	int index1, index2;
	for (int i = 0; i < deck.size(); i++)
	{
		index1 = i;
		index2 = dist(gen);
		std::swap(deck[index1], deck[index2]);
	}
	Title("�� �����ϱ� - �޸��� Ʈ������");
	for (auto e : deck)
	{
		std::cout << e << ' ';
	}
	Endl();

	// ����� �⺻ �����Ǵ� ���� ����� ������ ����. (������, ������, ���� ������� �޸��� Ʈ������ Ŭ������ �Ѱ��ش�)
	std::shuffle(deck.begin(), deck.end(), gen);
	Title("�� �����ϱ� - STL ����");
	for (auto e : deck)
	{
		std::cout << e << ' ';
	}
	Endl();
}