#include "Timer.h"
#include <Windows.h>	// CPU�� ���� ī���͸� ������ �ʿ��� ���

namespace MyUtility
{
	Timer::Timer() :
		mdSecondsPerCount{ 0.0 },
		mdDeltaTime{ -1.0 },
		mfScale{ 1.f },
		mllBaseTime{ 0 },
		mllPausedTime{ 0 },
		mllStopTime{ 0 },
		mllPrevTime{ 0 },
		mllCurrTime{ 0 },
		mbStopped{ false }
	{
		long long countsPerSec;
		// QueryPerformanceFrequency(long long ptr) : ���� �� ������ ���� ���� ��, PC�� CPU ����(=�ʴ� �۾� ���డ��Ƚ��)�� ����ؼ� �����صд�. �� ������ �����´�.
		// LARGE_INTEGER : 64��Ʈ�� long long Ÿ���� ĳ�����Ͽ� ���� �ǰ�, �ƴ� ��� 2���� �ڷ����� �����Ͽ� ����.
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		// �۾� 1ȸ�� �ɸ��� �ð��� �����صд�.
		mdSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}

	void Timer::Start()
	{
		long long currTime;
		// QueryPerformanceCounter(long long ptr) : ���� �� �۾�Ƚ���� ��� ������Ű�� �����ϰ� �ִ� ��. 
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllBaseTime = currTime;
		mllPrevTime = currTime;
		mllStopTime = 0;
		mbStopped = false;
	}

	void Timer::Stop()
	{
		if (mbStopped)
			return;
		
		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllStopTime = currTime;
		mbStopped = true;
	}

	void Timer::Resume()
	{
		if (!mbStopped)
			return;

		long long currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mllPausedTime += (currTime - mllStopTime);

		mllPrevTime = currTime;
		mllStopTime = 0;
		mbStopped = false;
	}

	void Timer::Update()
	{
		if (mbStopped)
		{
			mdDeltaTime = 0.0;
			return;
		}

		__int64 currTime;	// long long ��� __int64�� ����ص� ��. (Microsoft ����)
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mllCurrTime = currTime;

		// ������ �� ����ð�(deltaTime) = (���� ������ �۾�Ƚ�� - ���� ������ �۾�Ƚ��) * (�۾� �� �ɸ��� �ð�)
		mdDeltaTime = (mllCurrTime - mllPrevTime) * mdSecondsPerCount;

		mllPrevTime = mllCurrTime;

		// ��Ÿ�� ������ ��� ���������� ��Ȳ�̹Ƿ�, �ð� ��꿡 ������ ���� �ʵ��� 0���� �����Ѵ�.
		mdDeltaTime = (mdDeltaTime < 0.0) ? 0.0 : mdDeltaTime;
	}

	float Timer::TotalTime() const
	{
		if (mbStopped)
		{
			return static_cast<float>(mllStopTime - mllPausedTime - mllBaseTime) * mdSecondsPerCount;
		}
		else
		{
			return static_cast<float>(mllCurrTime - mllPausedTime - mllBaseTime) * mdSecondsPerCount;
		}
	}

	float Timer::DeltaTime() const
	{
		return static_cast<float>(mdDeltaTime * mfScale);
	}

	void Timer::SetScale(float scale)
	{
		mfScale = scale;
	}
}
