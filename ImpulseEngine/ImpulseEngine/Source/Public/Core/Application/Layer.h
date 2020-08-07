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

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timeStep) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}
		virtual void OnDraw() {};

		inline const std::string GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName = "test";
	};

}