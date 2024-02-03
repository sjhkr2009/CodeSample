#include "GameLogic.h"
#include <string>
#include <random>
#include <vector>

namespace Solitaire
{
	void GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;

		// ��� ���
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		// ī�� ����
		CreateCards(hwnd);
	}

	void GameLogic::Release()
	{
		// �޸� ����ȭ(memory fragmentation) ������ ����, ������ �������� �����ϴ� ������ ������ �� ����.
		// ���� ���⸸ �ϸ� ����Ʈ ���� -> �Ҹ��� ȣ�� -> �޸� ���� ������ ����ȴ�.
		mDeck.clear();
		mBackground.reset();
	}

	void GameLogic::CreateCards(HWND hwnd)
	{
		// ����) �ּ��� 'TODO'�� ������ ��� '�۾� ���'�� �߰��ȴ�. [����] - [�۾� ���] (Ctrl + \, T) ���� Ȯ�� ����.
		// TODO : 40�� ���� ����, ¦���� �����

		std::vector<Type> types;

		while (static_cast<int>(types.size()) < (BOARD_COLUMN * BOARD_ROW))
		{
			// ����, ��, �� ī�带 ������ 2�徿 ������ش�
			int modulator{ (types.size() % 6) };
			switch (modulator)
			{
				case 0:
					types.push_back(Type::Wolf);
					types.push_back(Type::Wolf);
					break;
				case 2:
					types.push_back(Type::Dragon);
					types.push_back(Type::Dragon);
					break;
				case 4:
					types.push_back(Type::Bear);
					types.push_back(Type::Bear);
					break;
				default:
					break;
			}
		}

		std::random_device rd;
		std::mt19937 gen(rd());

		std::shuffle(types.begin(), types.end(), gen);

		int posX{ 15 }, posY{ 10 };
		// ���� ��ġ(offset):	(15, 10)
		// ũ��:				(100, 140)
		// ����(padding):		(10, 10)
		int cardIndex{ 0 };

		for (int y = 0; y < BOARD_ROW; y++)
		{
			posX = 15;
			for (int x = 0; x < BOARD_COLUMN; x++)
			{
				mDeck.push_back(
					Card(mHwnd, cardIndex, types[cardIndex++], posX, posY)
				);

				posX += 100 + 10; // ī�� ũ�� + �е�
			}
			posY += 140 + 10;
		}
	}

	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		// ��� �׸���
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
		
		// ī�� �׸���
		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		// Ŭ�� Ƚ�� ��� (�귯�� �� ��Ʈ, ��ġ �Է��Ͽ� ����)
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		// FontFamily�� ���� �������� �ʰ� ���� �Է��Ͽ� Font�� ������ �� �ִ�.
		Gdiplus::Font font(L"���� ���", 20);
		Gdiplus::PointF pos(895.f, 20.f);
		graphics.DrawString(L"Ŭ����", -1, &font, pos, &brush);
		// Rect�� ����Ͽ� DrawString�� ����Ϸ��� ���ڿ� ������ �ʿ��ϴ�.
		// ���ڸ� wstring���� �ٲٷ��� std::to_wstring�� ����Ѵ�. #include <string> �ʿ�
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);		// ��� ���� (���� ����)
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);	// ��� ���� (���� ����)
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, COUNT_RECT, &format, &brush);
	}

	void GameLogic::OnClick(int x, int y)
	{
		Card* pCurrentCard{ nullptr };

		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				// TODO : Ŭ���� ī�忡 ���� ó��
				pCurrentCard = &card;
				break;
			}
		}

		if (pCurrentCard)	// pCard�� nullptr�� �ƴ� ���
		{
			// Ŭ���� �κ��� ��� �ݿ��ǵ��� �ٽ� �׸��� �Ѵ�.
			RECT rct{
				static_cast<LONG>(COUNT_RECT.GetLeft()),
				static_cast<LONG>(COUNT_RECT.GetTop()),
				static_cast<LONG>(COUNT_RECT.GetRight()),
				static_cast<LONG>(COUNT_RECT.GetBottom())
			};
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)
			{
				mpSelectedCard = pCurrentCard;
			}
			else
			{
				mFlipCount++;

				if (mpSelectedCard->GetType() == pCurrentCard->GetType())
				{
					if (mpSelectedCard->GetIndex() == pCurrentCard->GetIndex())
					{
						// ���� ī�带 �� �� Ŭ���� ����, �׳� �ٽ� �����´�.
						mpSelectedCard->Flip(false);
					}
					else
					{
						// �ո����� �� ī�带 �����ֵ��� �ش� ������ ��ȿȭ�ϰ�
						mpSelectedCard->Invalidate();
						pCurrentCard->Invalidate();

						// ������ ���� �� 0.5�ʰ� ����ߴٰ�
						UpdateWindow(mHwnd);
						Sleep(500);

						// �ٽ� ��ȿȭ�ؼ� (���� ������ ���� ��) ���ŵ� ī���� ����� �����ְ� �Ѵ�.
						mpSelectedCard->Invalidate();
						pCurrentCard->Invalidate();

						mDeck.remove_if(
							[&](Card& card)
							{
								return mpSelectedCard->GetIndex() == card.GetIndex();
							}
						);
						mDeck.remove_if(
							[&](Card& card)
							{
								return pCurrentCard->GetIndex() == card.GetIndex();
							}
						);
					}
					// TODO : ���� ����� ��� Game Win ó��
				}
				else
				{
					// UpdateWindow: â�� ������ �����Ѵ�. ó������ ������ Sleep(500)�� ���� �� �����츦 �ٽ� �׸� ���̴�.
					UpdateWindow(mHwnd);
					Sleep(500);

					mpSelectedCard->Flip(false);
					pCurrentCard->Flip(false);
					// TODO : �ι�° ī�� �׸��� �������� �ʰ� �ٷ� ������ �� ���� �ʿ�
				}
				mpSelectedCard = nullptr;
			}
		}
	}
}