#pragma once

#include "Public/Core/Application/Input.h"

namespace GEngine {

	class GE_API GLFW_Input : public Input {

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(uint64_t button) override;
		virtual float GetMouseXImpl(uint64_t id = 0) override;
		virtual float GetMouseYImpl(uint64_t id = 0) override;
		virtual std::pair<float, float> GetMousePositionImpl(uint64_t id = 0) override;

	};



}
