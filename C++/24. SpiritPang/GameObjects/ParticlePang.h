#pragma once
#include "PCH.h"

namespace SpiritPang
{
    class ParticlePang final :
        public DX::AnimatedActor
    {
    public:
        enum ParticleType
        {
            None,
            Disappear,
            BombAttack,
            BombSplash
        };

    private:
        float m_originSize{};
        ParticleType m_type;

    public:
        ParticlePang();
        ~ParticlePang() {};

        void SetSize(float size) { SetScale(size / GetOriginSize().x); }
        void SetOriginBlockSize(float size) { m_originSize = size; }

#pragma warning(push)
#pragma warning(disable:26812)
        void SetAnimation(ParticleType type);
#pragma warning(pop)
    };
}
