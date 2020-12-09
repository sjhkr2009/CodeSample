#include "Timer.h"
#include <Windows.h>	// CPU의 성능 카운터를 쓰려면 필요한 헤더

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
		// QueryPerformanceFrequency(long long ptr) : 부팅 후 윈도우 구동 시작 시, PC는 CPU 성능(=초당 작업 수행가능횟수)을 계산해서 저장해둔다. 이 정보를 가져온다.
		// LARGE_INTEGER : 64비트면 long long 타입을 캐스팅하여 쓰면 되고, 아닐 경우 2개의 자료형을 조합하여 쓴다.
		QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

		// 작업 1회당 걸리는 시간을 저장해둔다.
		mdSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}

	void Timer::Start()
	{
		long long currTime;
		// QueryPerformanceCounter(long long ptr) : 부팅 후 작업횟수를 계속 증가시키며 저장하고 있는 값. 
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

		__int64 currTime;	// long long 대신 __int64를 사용해도 됨. (Microsoft 전용)
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mllCurrTime = currTime;

		// 프레임 간 경과시간(deltaTime) = (현재 프레임 작업횟수 - 이전 프레임 작업횟수) * (작업 당 걸리는 시간)
		mdDeltaTime = (mllCurrTime - mllPrevTime) * mdSecondsPerCount;

		mllPrevTime = mllCurrTime;

		// 델타가 음수일 경우 비정상적인 상황이므로, 시간 계산에 영향을 주지 않도록 0으로 세팅한다.
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
