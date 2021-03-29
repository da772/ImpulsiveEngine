#pragma once

#include "Public/Core/Core.h"
#include "Public/Core/Events/Event.h"

#include "Public/Core/Util/Timestep.h"


namespace GEngine {


	class GE_API Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		inline virtual void Begin() {};
		inline virtual void OnAttach() {}
		inline virtual void OnDetach() {}
		inline virtual void OnUpdate(Timestep timeStep) {}
		inline virtual void OnEvent(Event& event) {}
		inline virtual void OnImGuiRender() {}
		inline virtual void OnDraw() {};		
		inline virtual void End() {};

		inline const std::string GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName = "test";
	};

}