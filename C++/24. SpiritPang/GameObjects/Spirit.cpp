#include "PCH.h"
#include "Spirit.h"
#include <sstream>
#include <iomanip>

using namespace DirectX;
using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)

Spirit::Spirit(Type type) :
	AnimatedActor(L"Assets/Spirits.png", L"Assets/Spirits.json", L"Assets/AnimSpirit.json"),
	m_type{ type },
	m_state{},
	m_destination{},
	m_pangParticle{}
{
	SetSpiritImage();
}

// 이 오브젝트의 타입에 따른 이미지를 세팅합니다.
void Spirit::SetSpiritImage()
{
	std::wstringstream name;
	name.str(L"");
	name << L"spirit" << static_cast<int>(m_type);

	AnimatedActor::SetAnimation(name.str().c_str());
}

void Spirit::SetType(Type type, bool changeSprite)
{
	m_type = type;

	if (changeSprite)
	{
		SetSpiritImage();
		SetPivot(0.f, 0.f);
	}
}

bool Spirit::Contain(float x, float y)
{
	if (x < GetPosition().x || x >(GetPosition().x + GetCurrentSize().x) ||
		y < GetPosition().y || y >(GetPosition().y + GetCurrentSize().y))
	{
		return false;
	}
	
	return true;
}

void Spirit::SetDestination(float x, float y)
{
	m_destination.x = x, m_destination.y = y;
	m_prevPos = GetPosition();

	if ((GetPosition().x != x) || (GetPosition().y != y))
	{
		m_state |= State::Move;
	}

	Utility::DebugString(L"[Spirit::SetDestination] Set Spirit [%d]'s Destnation (%0.1f, %0.1f) to (%0.1f, %0.1f).",
		m_index, GetPosition().x, GetPosition().y, m_destination.x, m_destination.y);
}

void Spirit::Update(double delta)
{
	Actor::Update(delta);
	
	if (m_state & State::Move)
	{
		if (MoveLerp() < 0.066f)
		{
			SetPosition(m_destination.x, m_destination.y);
			m_state &= ~(State::Move);

			if (m_state & State::ReturnOnMoveEnd)
			{
				SetDestination(m_prevPos.x, m_prevPos.y);
				m_state &= ~(State::ReturnOnMoveEnd);
			}
		}
	}
}

float Spirit::MoveLerp()
{
	XMFLOAT2 beforePos{ GetPosition().x, GetPosition().y };
	
	float destX = (beforePos.x * (1.f - moveSpeed)) + (m_destination.x * moveSpeed);
	float destY = (beforePos.y * (1.f - moveSpeed)) + (m_destination.y * moveSpeed);
	
	SetPosition(destX, destY);

	float moveDist = std::max(std::abs(destX - beforePos.x), std::abs(destY - beforePos.y));
	//Utility::DebugString(L"[Spirit::MoveLerp] Spirit [%d] is moved (%0.1f, %0.1f) to (%0.1f, %0.1f). (distance: %0.1f)", m_index, beforePos.x, beforePos.y, GetPosition().x, GetPosition().y, moveDist);
	return moveDist;
}

// temp: 이 개체의 디버그 정보를 출력합니다.
void Spirit::Debug()
{
	Utility::DebugString(L"[Spirit::Debug] Index: %d (%d, %d), Pos: (%0.1f, %0.1f) = (%0.1f, %0.1f), Type: %d, Active: %d",
		m_index, (m_index % 7), (m_index / 7), GetPosition().x, GetPosition().y,
		(GetPosition().x - 100.f) / 80.f, (GetPosition().y - 100.f) / 80.f, 
		static_cast<int>(m_type), IsActive());
}

// 블럭을 비활성화 상태로 변경하고, 파괴 이펙트를 출력합니다.
bool Spirit::Destroy()
{
	if (!IsActive())
	{
		return false;
	}
	SetActive(false);

	m_pangParticle->SetPosition(
		GetPosition().x + (GetCurrentSize().x / 2.f), 
		GetPosition().y + (GetCurrentSize().y / 2.f)
	);
	m_pangParticle->Play();
	
	return true;
}

#pragma warning(pop)
