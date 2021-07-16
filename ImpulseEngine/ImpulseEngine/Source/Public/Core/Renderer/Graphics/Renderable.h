#pragma once

namespace GEngine {

	class Camera;
	class RenderPipeline;

	class Renderable {

	

	public:
		Renderable() {};
		virtual ~Renderable() {};

		virtual void Render(Camera* cam) = 0;
		
		inline int GetPriority() { return m_Priority; }
		inline void SetPipeline(RenderPipeline* p) { pipeline = p; }
		inline void SetTime(long long time) { m_Time = time; }
		inline void SetPriority(int priority) { m_Priority = priority; }
		inline long long GetTime() { return m_Time; }

	protected:
		int m_Priority = 0;
		RenderPipeline* pipeline = nullptr;
		long long m_Time;


	};
}