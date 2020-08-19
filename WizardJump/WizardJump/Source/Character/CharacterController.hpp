#pragma once

#include "GEngine.h"
#include "Character/CharacterBody.hpp"

using namespace GEngine;

class CharacterController : public Component {
public:
	CharacterController() { 

		bUpdates = true;
	
	};
	~CharacterController() {};

protected:
	void OnBegin() override
	{
		bodyComp = CreateGameObject<CharacterBody>();
		GetEntity()->AddComponent(bodyComp);
	}


	void OnEnd() override
	{
		
	}


	void OnUpdate(Timestep timestep) override
	{
		if (Input::IsKeyPressed(GE_KEY_LEFT)) {
			bodyComp->AddVelocity({ -5 * timestep, 0 });
		}
		if (Input::IsKeyPressed(GE_KEY_RIGHT)) {
			bodyComp->AddVelocity({ 5 * timestep, 0 });
		}

		if (Mobile_Input::GetTouchCount() == 1) {
			for (std::pair<uint64_t, FTouchInfo> key : Mobile_Input::GetTouches()) {
				if (Time::GetEpochTimeMS() - key.second.GetTime() > 300) {

				}
				else {
					float xPos = key.second.x / (float)Application::GetWidth();
					GE_CORE_DEBUG("TOUCH: {0}, {1}", key.second.x, key.second.y);
					if (xPos >= 0) {
						bodyComp->AddVelocity({ 5 * timestep, 0 });
					}
					else {
						bodyComp->AddVelocity({ -5 * timestep, 0 });
					}
				}
			}
		}

	}

	Ref<CharacterBody> bodyComp;

};