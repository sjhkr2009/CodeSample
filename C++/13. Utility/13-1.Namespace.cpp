#include <iostream>

// 이름 공간(namespace)

namespace Game
{
	class Player
	{

	};
}

namespace Sound
{
	class Player // namespace가 다를 경우, 동일한 이름의 타입이나 변수, 함수를 만들 수 있다.
	{

	};
}

int main()
{
	Game::Player warrior;
	Sound::Player bgmPlayer;

	using namespace Game;

	// namespace를 사용한 후에는, 해당 namespace 안의 요소에 접근할 때 범위 지정 연산자(::)를 생략할 수 있다.
	Player mage;

	using namespace Sound;

	// 2개 이상의 namespace에서 같은 이름이 충돌할 경우(모호한 경우), 어느 공간 소속인지 표시해야 한다.
	//Player fxPlayer; // -> Error!
	Sound::Player fxPlayer;

	// 따라서 namespace은 여러 코드를 include할 때 중복되지 않도록, 헤더에는 사용하지 말아야 한다.
	// (협업 시 본인이 작성하는 cpp파일을 제외하면 남용하지 말 것)
}
