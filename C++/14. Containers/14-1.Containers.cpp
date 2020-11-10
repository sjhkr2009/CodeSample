#include <iostream>
#include <forward_list>
#include <list>
#include <deque>
#include <set>

// C++ Standard Library : ISO 표준 기구에서 정하는 추가/확장 기능. 컨테이너, 반복자, 알고리즘 등이 포함된다.

// 컨테이너(Container) : 자료구조를 위한 템플릿 클래스

int main()
{
	// 단일 연결 리스트

	std::forward_list<int> list1{ 1, 4, 7, 3, 0, 2 };
	
	// 리스트 종류는 내부적으로 sort 함수를 가지고 있다. vector처럼 복잡하게 쓸 필요 없음.
	list1.sort();

	list1.sort(std::less<int>()); //내림차순
	list1.sort(std::greater<int>()); //오름차순


	// 양방향 연결 리스트

	std::list<int> list2{ 1, 4, 7, 3, 0, 2 };


	// 데크 (Double-Ended Queue)
	// 큐와 비슷하지만 양방향이다. 앞에서도, 뒤에서도 원소를 넣거나 뺄 수 있다.

	std::deque<int> deq{ 1, 4, 7, 3, 0, 2 };

	deq.push_back(5);
	deq.pop_front();
	deq.pop_back();


	// set : 단일 키를 가진 이진 탐색 트리
	// 부모가 자식의 key가 되는 형태의 트리 구조.

	std::set<int> set{};
}
