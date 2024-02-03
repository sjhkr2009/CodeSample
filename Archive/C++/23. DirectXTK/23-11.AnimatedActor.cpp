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

// 이 오브젝트가 가진 애니메이션 정보를 담은 json 파일을 로딩합니다.
void AnimatedActor::LoadAnimation(const LPCWSTR jsonFile)
{
	m_animations = JsonManager::Instance().CreateDocument(jsonFile);
}

/// <summary>
/// json파일 내부의 애니메이션 이름을 통해 출력할 애니메이션 정보를 불러옵니다. LoadAnimation으로 json파일이 먼저 로딩되어야 합니다.
/// (json파일은 "animations" 배열 형태이며, 배열의 각 애니메이션 정보는 "name", "loop", "frames" [ {"name", "duration"} ] 형식으로 구성됩니다.)
/// </summary>
/// <param name="animName">: "animations" 배열의 오브젝트 내에서, 출력할 애니메이션의 "name" 정보와 매개변수가 일치해야 합니다.</param>
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

// 현재 애니메이션에서 (index) 번째에 담긴 프레임 정보에 따라 출력할 이미지와 지속시간을 설정합니다.
void AnimatedActor::SetFrameIndex(int index)
{
	if (m_animations == nullptr)
		return;

	const auto& frame = (*m_currentAnim)[L"frames"].GetArray()[index];
	SetFrame(frame[L"name"].GetString());
	m_timePerFrame = frame[L"duration"].GetFloat();
}

/// <summary>
/// 매 프레임 현재 이미지가 출력된 시간을 갱신하고, 지속시간이 끝난 경우 다음 프레임을 세팅합니다.
/// 일시정지 상태 또는 지속시간이 0인 경우는 아무 동작도 하지 않습니다.
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
