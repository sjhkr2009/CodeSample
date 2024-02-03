#include "PCH.h"

using namespace DirectX;
using namespace DX;

AnimatedActor::AnimatedActor() : Actor(),
m_animations{},
m_currentAnim{},
m_paused{},
m_isLoop{},
m_totalElapsed{},
m_timePerFrame{},
m_frameIndex{},
m_totalFrameCount{}
{
}

AnimatedActor::~AnimatedActor()
{
}

// �� ������Ʈ�� ���� �ִϸ��̼� ������ ���� json ������ �ε��մϴ�.
void AnimatedActor::LoadAnimation(const LPCWSTR jsonFile)
{
	m_animations = JsonManager::Instance().CreateDocument(jsonFile);
}

/// <summary>
/// json���� ������ �ִϸ��̼� �̸��� ���� ����� �ִϸ��̼� ������ �ҷ��ɴϴ�. LoadAnimation���� json������ ���� �ε��Ǿ�� �մϴ�.
/// (json������ "animations" �迭 �����̸�, �迭�� �� �ִϸ��̼� ������ "name", "loop", "frames" [ {"name", "duration"} ] �������� �����˴ϴ�.)
/// </summary>
/// <param name="animName">: "animations" �迭�� ������Ʈ ������, ����� �ִϸ��̼��� "name" ������ �Ű������� ��ġ�ؾ� �մϴ�.</param>
void AnimatedActor::SetAnimation(const LPCWSTR animName)
{
	if (m_animations == nullptr)
		return;

	const auto& anims = (*m_animations)[L"animations"].GetArray();
	for (auto& e : anims)
	{
		if (std::wcscmp(e[L"name"].GetString(), animName) == 0)
		{
			m_currentAnim = &e;
			m_isLoop = e[L"loop"].GetBool();
			m_totalFrameCount = e[L"frames"].GetArray().Size();

			Reset();
			SetFrameIndex(0);
			return;
		}
	}

	Utility::DebugString(L"[AnimatedActor::SetAnimation] Can't find animation &s", animName);
}

// ���� �ִϸ��̼ǿ��� (index) ��°�� ��� ������ ������ ���� ����� �̹����� ���ӽð��� �����մϴ�.
void AnimatedActor::SetFrameIndex(int index)
{
	if (m_animations == nullptr)
		return;

	const auto& frame = (*m_currentAnim)[L"frames"].GetArray()[index];
	SetFrame(frame[L"name"].GetString());
	m_timePerFrame = frame[L"duration"].GetFloat();
}

/// <summary>
/// �� ������ ���� �̹����� ��µ� �ð��� �����ϰ�, ���ӽð��� ���� ��� ���� �������� �����մϴ�.
/// �Ͻ����� ���� �Ǵ� ���ӽð��� 0�� ���� �ƹ� ���۵� ���� �ʽ��ϴ�.
/// </summary>
void AnimatedActor::Update(double deltaTime)
{
	if (m_paused || !m_timePerFrame)
		return;

	int prevFrameIndex = m_frameIndex;

	m_totalElapsed += static_cast<float>(deltaTime);
	if (m_totalElapsed > m_timePerFrame)
	{
		m_frameIndex = m_isLoop ?
			(m_frameIndex + 1) % m_totalFrameCount : std::min(m_frameIndex + 1, m_totalFrameCount - 1);
	}

	if (prevFrameIndex != m_frameIndex)
	{
		m_totalElapsed -= m_timePerFrame;
		SetFrameIndex(m_frameIndex);
	}
}
