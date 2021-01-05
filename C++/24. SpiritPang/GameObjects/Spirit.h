#pragma once
#include "ParticlePang.h"

namespace SpiritPang
{
	/// <summary>
	/// 3-Match류 게임의 블럭 역할을 하는 클래스
	/// </summary>
	class Spirit : public DX::AnimatedActor
	{
	public:
		enum Type
		{
			Rock = 0,
			Wind = 1,
			Mini = 2,
			Harmony = 3,
			Poison = 4,
			Tree = 5,
			Light = 6,
			Count,			// Count of Normal Block Types
			Special = 99,	// End of Normal Block
			//------------------------------------
			Special_Bomb = 100
		};

		enum State
		{
			Idle = 0,
			Move = 1,
			ReturnOnMoveEnd = 1 << 1,
			DestroyOnMoveEnd = 1 << 2
		};

	private:
		const float moveSpeed{ 0.2f };

	public:
		Spirit() = delete;
		Spirit(Type type);

		void SetSpiritImage();

		void SetType(Type type, bool changeSprite = true);
		void SetParticle(ParticlePang* destroy) { m_pangParticle = destroy; }
		ParticlePang* GetParticleOnDestroy() const { return m_pangParticle; }

		void SetSize(float size) { SetScale(size / GetOriginSize().x); }
#pragma warning(push)
#pragma warning(disable:26812)
		Type GetType() const { return m_type; }
		bool IsNormalType() const { return (m_type < Type::Special); }

		bool Contain(float x, float y);
		void SetDestination(float x, float y);
		void SetState(State state) { m_state |= static_cast<int>(state); }

		void SetParticleType(ParticlePang::ParticleType type) { m_pangParticle->SetAnimation(type); }
#pragma warning(pop)

		bool IsMoving() const { return (m_state & State::Move); }

		bool Destroy();
// temp
		void Debug();

	private:
		void Update(double delta) override;
		float MoveLerp();

	public:
		int m_index;

	private:
		Type				m_type;
		int					m_state;
		DirectX::XMFLOAT2	m_destination;
		DirectX::XMFLOAT2	m_prevPos;

		ParticlePang*		m_pangParticle;
	};
}

