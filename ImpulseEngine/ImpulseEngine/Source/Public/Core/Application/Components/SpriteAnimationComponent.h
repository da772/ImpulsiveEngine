#pragma once


#include "Public/Core/Application/Component.h"

namespace GEngine {

	class ScriptObject;

	class SpriteAnimationComponent : public Component {
	public:
		inline SpriteAnimationComponent() { bUpdates = true; };
		virtual ~SpriteAnimationComponent();
		void SetFrameAnimation(uint8_t fps, uint8_t maxFrames, bool loop = true, std::function<void(int)> animateFrameFunction = nullptr);
		void SetFrameAnimation_Script(uint8_t fps, uint8_t maxFrames, bool loop = true, Ref<ScriptObject> scriptFunction = nullptr);
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


	protected:
		virtual void OnBegin() override;


		virtual void OnEnd() override;


		virtual void OnUpdate(Timestep timestep) override;

	};







}

