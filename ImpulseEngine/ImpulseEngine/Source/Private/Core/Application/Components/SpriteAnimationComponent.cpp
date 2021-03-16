#include "gepch.h"
#include "Public/Core/Application/Components/SpriteAnimationComponent.h"
#include "Public/Core/Util/Time.h"
#include "Public/Core/FileSystem/FileSystem.h"

namespace GEngine {

	SpriteAnimationComponent::~SpriteAnimationComponent()
	{
		m_animateFrameFunction = nullptr;
	}

	void SpriteAnimationComponent::SetFrameAnimation(const uint8_t fps, const uint8_t maxFrames, bool loop, const std::function<void(int)> animateFrameFunction)
	{
		m_MaxFrames = maxFrames;
		m_FPS = fps;
		m_framePerMs = 1000.0f / fps;
		b_animateFrame = true;
		b_loop = loop;
		m_startTime = -1;
		m_currentFrame = 0;
		if (animateFrameFunction != nullptr) {
			m_animateFrameFunction = animateFrameFunction;
		}
	}

	void SpriteAnimationComponent::SetFrameAnimationFPS(uint8_t fps)
	{
		m_FPS = fps;
		m_framePerMs = 1000.0f / fps;
	}

	void SpriteAnimationComponent::RemoveFrameAnimation()
	{
		b_animateFrame = false;
		m_animateFrameFunction = nullptr;
	}

	void SpriteAnimationComponent::Stop()
	{
		b_animateFrame = false;
	}

	void SpriteAnimationComponent::Start()
	{
		b_animateFrame = true;
	}

	void SpriteAnimationComponent::AnimateFrame(long long time)
	{
		long timePassed = time - m_startTime;

		if (m_startTime == -1) {
			m_startTime = time;
			int frameInc = 1;
			if (frameInc >= m_MaxFrames) {
				m_currentFrame = m_MaxFrames;
				m_animateFrameFunction(m_MaxFrames);
				m_startTime = time;
				if (!b_loop)
					b_animateFrame = false;
				return;
			}
			m_currentFrame = frameInc;
			m_animateFrameFunction(frameInc);
			return;
		}

		if (timePassed > m_framePerMs) {
			int frameInc = floor(timePassed / m_framePerMs)+1;
			frameInc = m_MaxFrames > frameInc ? frameInc : m_MaxFrames;
			if (frameInc >= m_MaxFrames) {
				m_animateFrameFunction(m_MaxFrames);
				m_startTime = time;
				if (!b_loop)
					b_animateFrame = false;
				return;
			}
			if (m_currentFrame == frameInc)
				return;
			m_currentFrame = frameInc;
			m_animateFrameFunction(frameInc);
			return;
		}

		
	}

	void SpriteAnimationComponent::OnBegin()
	{

	}

	void SpriteAnimationComponent::OnEnd()
	{

	}

	void SpriteAnimationComponent::OnUpdate(Timestep timestep)
	{
		uint64_t time = Time::GetEpochTimeMS();

		//if (m_startTime == -1)
		//	m_startTime = time;

		if (b_animateFrame)
			AnimateFrame(time);

		


	}


}
