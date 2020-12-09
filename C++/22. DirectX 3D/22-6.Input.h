#pragma once
#include <array>

// X Input 기반으로 Input 클래스 구현
// 마우스: 커서 좌표 저장
// 클릭, 키보드 입력: 배열로 관리


// MS Input

// Direct Input (DX8에서 폐기)
// - (특히 게임 전용 컨트롤러에서)매우 정확한 데이터 수집
// - 진동 부가 기능
// - 키 매핑 (Action Mapping)

// X Input (현재 사용중)
// - XBox Controller에 100% 호환
// - 헤드셋 지원
// - 쉬움

namespace MyUtility
{
	class Input
	{
	private:
		std::array<bool, 256>	mKeys;
		int						mCursorX, mCursorY;

	public:
		void Initialize();
		void SetKeyDown(unsigned int key);
		void SetKeyUp(unsigned int key);
		void SetCursor(int x, int y);

		void GetCursorPosition(int& x, int& y);
		bool IsKeyDown(unsigned int key);
	};
}
// 참고: 환경에 따라(ex: 리눅스 계열) 파일의 마지막 줄이 비어있지 않으면 에러가 나니 주의.
