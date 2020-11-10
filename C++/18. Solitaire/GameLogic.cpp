#include "GameLogic.h"
#include <string>
#include <random>
#include <vector>

namespace Solitaire
{
	void GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;

		// 배경 깔기
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		// 카드 생성
		CreateCards(hwnd);
	}

	void GameLogic::Release()
	{
		// 메모리 단편화(memory fragmentation) 방지를 위해, 생성한 역순으로 해제하는 습관을 가지는 게 좋다.
		// 덱을 비우기만 하면 리스트 비우기 -> 소멸자 호출 -> 메모리 해제 순으로 진행된다.
		mDeck.clear();
		mBackground.reset();
	}

	void GameLogic::CreateCards(HWND hwnd)
	{
		// 참고) 주석이 'TODO'로 시작할 경우 '작업 목록'에 추가된다. [보기] - [작업 목록] (Ctrl + \, T) 에서 확인 가능.
		// TODO : 40장 임의 생성, 짝수로 만들기

		std::vector<Type> types;

		while (static_cast<int>(types.size()) < (BOARD_COLUMN * BOARD_ROW))
		{
			// 늑대, 용, 곰 카드를 번갈아 2장씩 만들어준다
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
		// 시작 위치(offset):	(15, 10)
		// 크기:				(100, 140)
		// 간격(padding):		(10, 10)
		int cardIndex{ 0 };

		for (int y = 0; y < BOARD_ROW; y++)
		{
			posX = 15;
			for (int x = 0; x < BOARD_COLUMN; x++)
			{
				mDeck.push_back(
					Card(mHwnd, cardIndex, types[cardIndex++], posX, posY)
				);

				posX += 100 + 10; // 카드 크기 + 패딩
			}
			posY += 140 + 10;
		}
	}

	void GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		// 배경 그리기
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
		
		// 카드 그리기
		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		// 클릭 횟수 출력 (브러쉬 및 폰트, 위치 입력하여 생성)
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64));
		// FontFamily를 따로 선언하지 않고 직접 입력하여 Font를 생성할 수 있다.
		Gdiplus::Font font(L"맑은 고딕", 20);
		Gdiplus::PointF pos(895.f, 20.f);
		graphics.DrawString(L"클릭수", -1, &font, pos, &brush);
		// Rect를 사용하여 DrawString을 사용하려면 문자열 포맷이 필요하다.
		// 숫자를 wstring으로 바꾸려면 std::to_wstring을 사용한다. #include <string> 필요
		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);		// 가운데 정렬 (가로 기준)
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);	// 가운데 정렬 (세로 기준)
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1, &font, COUNT_RECT, &format, &brush);
	}

	void GameLogic::OnClick(int x, int y)
	{
		Card* pCurrentCard{ nullptr };

		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				// TODO : 클릭한 카드에 대한 처리
				pCurrentCard = &card;
				break;
			}
		}

		if (pCurrentCard)	// pCard가 nullptr이 아닐 경우
		{
			// 클릭수 부분이 즉시 반영되도록 다시 그리게 한다.
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
						// 같은 카드를 두 번 클릭한 경우는, 그냥 다시 뒤집는다.
						mpSelectedCard->Flip(false);
					}
					else
					{
						// 앞면으로 된 카드를 보여주도록 해당 영역을 무효화하고
						mpSelectedCard->Invalidate();
						pCurrentCard->Invalidate();

						// 윈도우 갱신 후 0.5초간 대기했다가
						UpdateWindow(mHwnd);
						Sleep(500);

						// 다시 무효화해서 (제거 로직이 끝난 후) 제거된 카드의 모습을 보여주게 한다.
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
					// TODO : 덱이 비었을 경우 Game Win 처리
				}
				else
				{
					// UpdateWindow: 창을 강제로 갱신한다. 처리하지 않으면 Sleep(500)이 끝난 후 윈도우를 다시 그릴 것이다.
					UpdateWindow(mHwnd);
					Sleep(500);

					mpSelectedCard->Flip(false);
					pCurrentCard->Flip(false);
					// TODO : 두번째 카드 그림을 보여주지 않고 바로 뒤집는 점 개선 필요
				}
				mpSelectedCard = nullptr;
			}
		}
	}
}