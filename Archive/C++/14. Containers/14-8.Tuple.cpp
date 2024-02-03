#include <iostream>
#include <tuple>

int main()
{
	// 튜플
	// 여러 타입의 값 묶음을 저장할 수 있는 컨테이너.

	std::tuple<int, float, int, std::string> t;

	// 값을 가져올 때는 std::get을 사용해야 한다.
	std::get<0>(t);
	


	// 예시
	// 트랙 번호, 가수명, 곡 제목을 가진 노래 리스트를 가정하자.

	std::tuple<int, std::string, std::string> song{};

	// make_tuple()로 요소를 추가할 수 있다.
	song = std::make_tuple(1, "Seven Oops", "Orange");

	std::cout << std::get<0>(song) << " : " <<
		std::get<1>(song) << " - " <<
		std::get<2>(song);

}