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
			// 1. ��Ÿƽ ����, FPS �� Update ���� ����� ���� ���� ����
			LARGE_INTEGER currentTime;
			if (!QueryPerformanceCounter(&currentTime))
			{
				throw std::exception("QueryPerformanceCounter Failed!");
			}
			ull deltaTick = static_cast<ull>(currentTime.QuadPart - mQpcLastTick.QuadPart);
			mQpcLastTick = currentTime;

			mQpcSecondCounter += deltaTick;
			bool isUpdated = false;

			// 2. ��Ÿƽ �ִ�ġ ���� �� ǥ�� ������(Canonical Tick Format)�� �°� ��ȯ
			deltaTick = std::min(deltaTick, mQpcMaxDelta);
			deltaTick *= (TicksPerSecond / static_cast<ull>(mQpcFrequncy.QuadPart));


			// 3. ���� �����ӿ����� Update�� ó��
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
			// 4. ���� �������� �ƴ� ����� Update�� ó��
			else
			{
				mElapsedTicks = deltaTick;
				mTotalTicks += deltaTick;

				update();
				isUpdated = true;
			}

			// 5. ������ ��� (������Ʈ ȣ�� ��)
			if (isUpdated)
			{
				mFrameCount++;
				mFrameThisSecond++;
			}

			// 6. �ʴ� ������ ��� (1�� ��� ��)
			if (mQpcSecondCounter >= static_cast<ull>(mQpcFrequncy.QuadPart))
			{
				mFramePerSecond = mFrameThisSecond;
				mFrameThisSecond = 0;
				mQpcSecondCounter %= static_cast<ull>(mQpcFrequncy.QuadPart);
			}
		}

	public:
		// �ʴ� Update �ּ� ȣ�� ��.
		// �ʴ� �� Ƚ�� ���Ϸ� ȣ��Ǹ� ������ �������� ������ �����ϱ� ���� ��ŸŸ���� ������ ��ġ(CPU ������ / �ּ� ����Ƚ��)�� �����˴ϴ�.
		static const uint MinUpdateCountPerSeconds = 10;
		
		// * Fixed Frame Rate
		//------------------------------------------------
		static const ull TicksPerSecond = 10000000;					// �ʴ� ƽ ����
		// Ŭ���� �ΰ���. ������ ��� ������ ���� Tick�� ȣ�� �ֱⰡ ��ǥ �ֻ����� ������ ���� Update���� �̹� ƽ�� �ٷ� ȣ���մϴ�.
		// ��Ÿƽ�� Update ȣ�� �ֱ��� ���̰� '1 / Ŭ���� �ΰ���'(��) �̳��� ��� Update���� ȣ��˴ϴ�.
		static const uint FixedDeltaTickClampSensitivity = 4000;
		static const uint DisplayFrequency = 60;					// ����� �ֻ��� (hz)
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
		LARGE_INTEGER mQpcFrequncy;	// CPU�� �ʴ� ���� Ƚ�� (Hz)
		LARGE_INTEGER mQpcLastTick;	// (�� ������ �缳��) �ֱ� ������ �ð�

		ull mQpcMaxDelta;		// ��Ÿƽ�� �ִ�ġ

		ull mElapsedTicks;		// ���� Update ������ ��� ƽ
		ull mTotalTicks;		// ���� ���� �� ���ݱ����� ���� ƽ

		uint mFrameCount;		// ���� ���� �� ���ݱ����� ���� ������
		uint mFramePerSecond;	// (�� �� �缳��) �ʴ� ������
		uint mFrameThisSecond;	// (�� �� �ʱ�ȭ) �̹� �ʿ� ������ ������
		ull mQpcSecondCounter;	// (�� �� �缳��) �ð� ������. CPU�� �ʴ� ���������� ũ�� ��������ŭ ���� ���鼭 1�ʰ� ����� ������ �����Ѵ�.

		// * Fixed Frame Rate
		//------------------------------------------------
		bool mIsFixedTimeStep;	// ���� ������ ����Ʈ Ȱ��ȭ ����
		ull mLeftOverTicks;		// (�� ������ �缳��) �ֱ� Update�� ȣ��κ��� ����� ƽ. ��ǥ ��Ÿƽ���� ũ�� ��Ÿƽ��ŭ ���� ���鼭 Update���� ȣ���Ѵ�.
		ull mTargetElapsedTicks;// ������ Update�� ȣ�� �ֱ�
		//------------------------------------------------
	};
}
