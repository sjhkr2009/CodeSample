#pragma once

// 여기서는 DXUT 스타일을 참고하여 시간 관리 기능을 직접 구현해보기로 한다.


// * 참고: Microsoft Utility

// DXUT(DirectX Utility) (DX9)
// 인디 게임 개발을 쉽게 하기 위해 등장했지만 DX10 부터는 사라졌다.

// XNA (DX10~)
// DirectX Utility를 대체하기 위해 등장한 C# 기반 프레임워크. 2014년 지원 중단.

// XTK (DX11~)
// DXUT를 대체하는 C++ 기반 프레임워크. 현재까지 사용중.

namespace MyUtility
{
	class Timer
	{
	public:
		Timer();

	public:
		void	Start();
		void	Stop();
		void	Resume();

		void	Update();

		float	TotalTime() const;
		float	DeltaTime() const;
		void	SetScale(float scale);

	private:
		double		mdSecondsPerCount;
		double		mdDeltaTime;
		float		mfScale;

		long long	mllBaseTime;
		long long	mllPausedTime;
		long long	mllStopTime;
		long long	mllPrevTime;
		long long	mllCurrTime;

		bool		mbStopped;
	};
}

