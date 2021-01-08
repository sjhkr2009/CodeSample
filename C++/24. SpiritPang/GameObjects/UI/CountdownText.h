#pragma once
#include "AnimatedActor.h"

namespace SpiritPang
{
    class CountdownText :
        public DX::AnimatedActor
    {
    public:
        CountdownText(std::function<void()> evt);
        ~CountdownText() {};

        virtual void Update(double deltaTime) override;
        virtual void Play() override { SetPivot(0.5f, 0.5f), SetScale(1.0f); AnimatedActor::Play(); };

    protected:
        virtual void KeyFrameAnimation(int nowIndex) override;
        virtual void OnAnimationEnd() override;

    private:
        std::function<void()> m_onDone;
    };
}

