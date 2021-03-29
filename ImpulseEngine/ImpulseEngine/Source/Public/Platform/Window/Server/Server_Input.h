#pragma once
#include "Public/Core/Application/Input.h"

namespace GEngine {

	class GE_API Server_Input : public Input {

	protected:
		inline virtual bool IsKeyPressedImpl(int keycode) override { return false; };
		inline virtual bool IsMouseButtonPressedImpl(uint64_t button) override { return false; };
		inline virtual float GetMouseXImpl(uint64_t id) override { return -1.f; };
		inline virtual float GetMouseYImpl(uint64_t id) override { return -1.f; };
		inline virtual std::pair<float, float> GetMousePositionImpl(uint64_t id) override {return {-1.f, -1.f};};
		inline virtual void ProcessEvent(const Event& e) override { return;};
	};

};


