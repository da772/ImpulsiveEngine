#pragma once

namespace GEngine {

	class Renderable {

	class RenderPipeline;

	public:
		Renderable() {};
		virtual ~Renderable() {};

		virtual void Render() = 0;
		
		inline int GetPriority() { return m_Priority; }
		inline void SetPipeline(Ref<RenderPipeline> p) { pipeline = p; }
		inline void SetTime(long long time) { m_Time = time; }
		inline void SetPriority(int priority) { m_Priority = priority; }
		inline long long GetTime() { return m_Time; }

	protected:
		int m_Priority = 0;
		Ref<RenderPipeline> pipeline;
		long long m_Time;


	};
}