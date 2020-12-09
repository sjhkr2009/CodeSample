#pragma once
#include <array>

// X Input ������� Input Ŭ���� ����
// ���콺: Ŀ�� ��ǥ ����
// Ŭ��, Ű���� �Է�: �迭�� ����


// MS Input

// Direct Input (DX8���� ���)
// - (Ư�� ���� ���� ��Ʈ�ѷ�����)�ſ� ��Ȯ�� ������ ����
// - ���� �ΰ� ���
// - Ű ���� (Action Mapping)

// X Input (���� �����)
// - XBox Controller�� 100% ȣȯ
// - ���� ����
// - ����

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
// ����: ȯ�濡 ����(ex: ������ �迭) ������ ������ ���� ������� ������ ������ ���� ����.
