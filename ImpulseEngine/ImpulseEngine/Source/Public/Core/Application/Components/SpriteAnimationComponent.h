#pragma once


#include "Public/Core/Application/Components/Scripts/NativeScript.h"

namespace GEngine {


	class GE_API SpriteAnimationComponent : public NativeScript {
	public:
		inline SpriteAnimationComponent(Entity* e) : NativeScript(e) { go_tag = "Sprite Animation Component"; bUpdates = true; };
		virtual ~SpriteAnimationComponent();
		void SetFrameAnimation(const uint8_t fps, const uint8_t maxFrames, bool loop = true, const std::function<void(int)> animateFrameFunction = nullptr);
		void SetFrameAnimationFPS(uint8_t fps);
		void RemoveFrameAnimation();
		inline int GetFrameAnimation() { return m_currentFrame; }
		void Stop();
		void Start();

	private:
		int m_MaxFrames = -1;
		int m_FPS = -1;
		float m_framePerMs = 0;
		int m_currentFrame = 0;
		long long m_timePassedFrame = 0;
		bool b_animateFrame = false;
		bool b_loop = true;

		std::function<void(int)> m_animateFrameFunction = nullptr;

		void AnimateFrame(long long time);
		long long m_startTime = -1;


	public:
		virtual void OnBegin() override;
		virtual void OnEnd() override;
		virtual void OnUpdate(Timestep timestep) override;

	};







}

