#pragma once

#include <cmath>
#include <cstdint>

namespace DX
{
	using ull = unsigned long long;
	using uint = unsigned int;

	class StepTimer
	{
	public:
		StepTimer() :
			mQpcMaxDelta{ 0 },
			mElapsedTicks{ 0 },
			mTotalTicks{ 0 },
			mFrameCount{ 0 },
			mFramePerSecond{ 0 },
			mFrameThisSecond{ 0 },
			mQpcSecondCounter{ 0 },
			mIsFixedTimeStep{ false },
			mLeftOverTicks{ 0 },
			mTargetElapsedTicks{ TicksPerSecond / DisplayFrequency }
		{
			bool result = QueryPerformanceFrequency(&mQpcFrequncy);
			if (!result)
			{
				throw std::exception("QueryPerformanceFrequency Failed!");
			}

			result = QueryPerformanceCounter(&mQpcLastTick);
			if (!result)
			{
				throw std::exception("QueryPerformanceCounter Failed!");
			}

			mQpcMaxDelta = static_cast<ull>(mQpcFrequncy.QuadPart / MinUpdateCountPerSeconds);
		}

	public:
		unsigned long long GetElapsedTicks() const noexcept { return mElapsedTicks; }
		double GetElapsedSeconds() const noexcept { return TicksToSeconds(mElapsedTicks); }

		unsigned long long GetTotalTicks() const noexcept { return mTotalTicks; }
		double GetTotalSeconds() const noexcept { return TicksToSeconds(mTotalTicks); }

		unsigned int GetTotalFrameCount() const noexcept { return mFrameCount; }

		unsigned int GetFramePerSeconds() const noexcept { return mFramePerSecond; }

		void SetFixesTimeStep(bool isFixed) { mIsFixedTimeStep = isFixed; }
		void SetTargetDeltaTime(unsigned long long targetDeltaTicks) { mTargetElapsedTicks = targetDeltaTicks; }
		void SetTargetDeltaTime(double targetDeltaTime) { mTargetElapsedTicks = SecondsToTicks(targetDeltaTime); }

		void ResetElapsedTime()
		{
			bool result = QueryPerformanceCounter(&mQpcLastTick);
			if (!result)
			{
				throw std::exception("QueryPerformanceCounter Failed!");
			}

			mLeftOverTicks = 0;
			mFramePerSecond = 0;
			mFrameThisSecond = 0;
			mQpcSecondCounter = 0;
		}

		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// 1. 델타틱 측정, FPS 및 Update 여부 기록을 위한 변수 생성
			LARGE_INTEGER currentTime;
			if (!QueryPerformanceCounter(&currentTime))
			{
				throw std::exception("QueryPerformanceCounter Failed!");
			}
			ull deltaTick = static_cast<ull>(currentTime.QuadPart - mQpcLastTick.QuadPart);
			mQpcLastTick = currentTime;

			mQpcSecondCounter += deltaTick;
			bool isUpdated = false;

			// 2. 델타틱 최대치 연산 및 표준 진동수(Canonical Tick Format)에 맞게 변환
			deltaTick = std::min(deltaTick, mQpcMaxDelta);
			deltaTick *= (TicksPerSecond / static_cast<ull>(mQpcFrequncy.QuadPart));


			// 3. 고정 프레임에서의 Update문 처리
			if (mIsFixedTimeStep)
			{
				if (std::abs(static_cast<int64_t>(mTargetElapsedTicks - deltaTick)) < TicksPerSecond / static_cast<ull>(FixedDeltaTickClampSensitivity))
				{
					deltaTick = mTargetElapsedTicks;
				}

				mLeftOverTicks += deltaTick;
				if (mLeftOverTicks >= mTargetElapsedTicks)
				{
					mElapsedTicks = mTargetElapsedTicks;
					mTotalTicks += mTargetElapsedTicks;
					mLeftOverTicks -= mTargetElapsedTicks;

					update();
					isUpdated = true;
				}
			}
			// 4. 고정 프레임이 아닐 경우의 Update문 처리
			else
			{
				mElapsedTicks = deltaTick;
				mTotalTicks += deltaTick;

				update();
				isUpdated = true;
			}

			// 5. 프레임 계산 (업데이트 호출 시)
			if (isUpdated)
			{
				mFrameCount++;
				mFrameThisSecond++;
			}

			// 6. 초당 프레임 계산 (1초 경과 시)
			if (mQpcSecondCounter >= static_cast<ull>(mQpcFrequncy.QuadPart))
			{
				mFramePerSecond = mFrameThisSecond;
				mFrameThisSecond = 0;
				mQpcSecondCounter %= static_cast<ull>(mQpcFrequncy.QuadPart);
			}
		}

	public:
		// 초당 Update 최소 호출 수.
		// 초당 이 횟수 이하로 호출되면 게임의 비정상적 동작을 방지하기 위해 델타타임이 지정된 수치(CPU 진동수 / 최소 실행횟수)로 조정됩니다.
		static const uint MinUpdateCountPerSeconds = 10;
		
		// * Fixed Frame Rate
		//------------------------------------------------
		static const ull TicksPerSecond = 10000000;					// 초당 틱 지정
		// 클램핑 민감도. 프레임 드랍 방지를 위해 Tick의 호출 주기가 목표 주사율과 근접할 때는 Update문을 이번 틱에 바로 호출합니다.
		// 델타틱과 Update 호출 주기의 차이가 '1 / 클램핑 민감도'(초) 이내일 경우 Update문이 호출됩니다.
		static const uint FixedDeltaTickClampSensitivity = 4000;
		static const uint DisplayFrequency = 60;					// 모니터 주사율 (hz)
		static constexpr double TicksToSeconds(ull ticks) noexcept
		{
			return static_cast<double>(ticks) / TicksPerSecond;
		}
		static constexpr ull SecondsToTicks(double seconds) noexcept
		{
			return static_cast<ull>(seconds * TicksPerSecond);
		}
		//------------------------------------------------

	private:
		LARGE_INTEGER mQpcFrequncy;	// CPU의 초당 연산 횟수 (Hz)
		LARGE_INTEGER mQpcLastTick;	// (매 프레임 재설정) 최근 연산한 시간

		ull mQpcMaxDelta;		// 델타틱의 최대치

		ull mElapsedTicks;		// 이전 Update 이후의 경과 틱
		ull mTotalTicks;		// 게임 시작 후 지금까지의 누적 틱

		uint mFrameCount;		// 게임 시작 후 지금까지의 누적 프레임
		uint mFramePerSecond;	// (매 초 재설정) 초당 프레임
		uint mFrameThisSecond;	// (매 초 초기화) 이번 초에 누적된 프레임
		ull mQpcSecondCounter;	// (매 초 재설정) 시간 측정용. CPU의 초당 진동수보다 크면 진동수만큼 값을 빼면서 1초가 경과한 것으로 간주한다.

		// * Fixed Frame Rate
		//------------------------------------------------
		bool mIsFixedTimeStep;	// 고정 프레임 레이트 활성화 여수
		ull mLeftOverTicks;		// (매 프레임 재설정) 최근 Update문 호출로부터 경과한 틱. 목표 델타틱보다 크면 델타틱만큼 값을 빼면서 Update문을 호출한다.
		ull mTargetElapsedTicks;// 지정된 Update문 호출 주기
		//------------------------------------------------
	};
}
