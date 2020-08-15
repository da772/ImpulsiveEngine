#pragma once
#include "Public/Core/Core.h"

namespace GEngine {

	class Collider;
	class Event;

	class GE_API Input 
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode);  };
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); };
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); };
		inline static std::pair<float, float>  GetMousePosition() { return s_Instance->GetMousePositionImpl(); };
		static void ProcessEvents(const Event& e);
		static void Create();

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(uint64_t button) = 0;
		virtual float GetMouseXImpl(uint64_t id = 0) = 0;
		virtual float GetMouseYImpl(uint64_t id = 0) = 0;
		virtual std::pair<float,float> GetMousePositionImpl(uint64_t id = 0) = 0;
		virtual void ProcessEvent(const Event& e) = 0;
		Weak<Collider> m_lastCollider;
	private:
		static Input* s_Instance;

	};




}
