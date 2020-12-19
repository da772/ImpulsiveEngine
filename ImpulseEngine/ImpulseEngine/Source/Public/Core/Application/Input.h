#pragma once
#include "Public/Core/Core.h"

namespace GEngine {

	class Collider;
	class Event;

	class GE_API Input 
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float>  GetMousePosition();
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

	extern Input* Create_Input();




}
