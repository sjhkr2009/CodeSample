#pragma once

// ���⼭�� DXUT ��Ÿ���� �����Ͽ� �ð� ���� ����� ���� �����غ���� �Ѵ�.


// * ����: Microsoft Utility

// DXUT(DirectX Utility) (DX9)
// �ε� ���� ������ ���� �ϱ� ���� ���������� DX10 ���ʹ� �������.

// XNA (DX10~)
// DirectX Utility�� ��ü�ϱ� ���� ������ C# ��� �����ӿ�ũ. 2014�� ���� �ߴ�.

// XTK (DX11~)
// DXUT�� ��ü�ϴ� C++ ��� �����ӿ�ũ. ������� �����.

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

