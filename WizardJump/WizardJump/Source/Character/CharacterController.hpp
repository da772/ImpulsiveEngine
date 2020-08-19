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
				//GE_LOG_DEBUG("STATE: {0} {1}", key.second.GetID(), key.second.GetState());
				if (Time::GetEpochTimeMS() - key.second.GetTime() > 250) {
					lastxpos = key.second.x;
					lastypos = key.second.y;
					if (!bJumping) {
						float xPos = key.second.x;
						float width = (float)Application::GetWidth();
						//GE_CORE_DEBUG("TOUCH: {0}, {1}", key.second.x, key.second.y);
						if (xPos >= width/2.f) {
							bodyComp->AddVelocity({ 5 * timestep, 0 });
						}
						else {
							bodyComp->AddVelocity({ -5 * timestep, 0 });
						}
					} else {
						lastxpos = key.second.x;
						lastypos = key.second.y;
					}
				}
				else {
					if (touchId != key.second.id && touchTime != key.second.time) {
						touchId = 0;
						touchTime = 0;
						lastxpos = -1;
						lastypos = -1;
						bJumping = false;


					} else if (key.second.y > startyPos +  Application::GetHeight()*.01f) {
                        GE_LOG_DEBUG("WE JUMP");
                        bJumping = true;
                    }

					if (key.second.GetState() == 0 && touchId == 0 && touchTime == 0) {
						startxPos = key.second.x;
						startyPos = key.second.y;
						touchId = key.second.id;
						touchTime = key.second.time;
						continue;
					}
					
					
				}
			}
		} else if (Mobile_Input::GetTouchCount() == 0 && touchId != 0) {
					
			if (bJumping && lastypos != -1) 
			{
				float yDistance = (lastypos - startyPos +  Application::GetHeight()*.01f)/Application::GetHeight();
				float xDistance = -(lastxpos - startxPos +  Application::GetWidth()*.01f)/Application::GetWidth();
				GE_LOG_INFO("DISTANCE: {0},{1}", xDistance, yDistance);

				bodyComp->AddVelocity( {10*(xDistance > .2f ? .2f : xDistance), 40.f* (yDistance > .2f ? .2f : yDistance) });

				bJumping = false;
			}
			touchId = 0;
			touchTime = 0;
			lastxpos = -1;
			lastypos = -1;
		}

	}

	Ref<CharacterBody> bodyComp;
	float startxPos = -1.f;
	float startyPos = -1.f;
	float lastxpos = -1;
	float lastypos = -1;
	uint64_t touchId = 0;
	uint64_t touchTime = 0;
	bool bJumping = false;

};
