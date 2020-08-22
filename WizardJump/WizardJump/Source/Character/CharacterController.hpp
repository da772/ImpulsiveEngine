#pragma once

#include "GEngine.h"
#include "Character/CharacterBody.hpp"
#include "Character/CharacterGraphics.hpp"

using namespace GEngine;

class CharacterController : public Component {
public:
	CharacterController() { 

		bUpdates = true;
	
	};
	~CharacterController() {};

	Ref<CharacterBody> bodyComp;
	

protected:
	void OnBegin() override
	{
		bodyComp = CreateGameObject<CharacterBody>();
		GetEntity()->AddComponent(bodyComp);
		graphicsComp = static_pointer_cast<CharacterEntity>(GetEntity())->m_spriteComponent;
		
	}


	void OnEnd() override
	{
		bodyComp = nullptr;
		graphicsComp = nullptr;

	}

	bool bFalling = false;
	bool bJumping = false;
	float walkAnimThreshold = .05f;
    uint64_t jumpTime = 0;

	void OnUpdate(Timestep timestep) override
	{
		const glm::vec2& vel = bodyComp->GetVelocity();
		const bool ground = bodyComp->isGrounded();

		if (!ground) {
			bFalling = true;
			graphicsComp->Falling();
        }

		if (bodyComp->isGrounded() && !Input::IsKeyPressed(GE_KEY_SPACE) && !bFalling && !bJumping) {
			if (Input::IsKeyPressed(GE_KEY_LEFT)) {
				bodyComp->AddVelocity({ -5 * timestep, 0 });
			} else if (Input::IsKeyPressed(GE_KEY_RIGHT)) {
				bodyComp->AddVelocity({ 5 * timestep, 0 });				
			}
		} 
		if (bodyComp->isGrounded() && Input::IsKeyPressed(GE_KEY_SPACE) && !bFalling && !bJumping) {
			bJumping = true;
			graphicsComp->JumpCrouch([this, timestep]() {
				graphicsComp->JumpStart([this, timestep]() {
					bJumping = false;
					bodyComp->AddVelocity({ 0, 5.f }); });
				});
		} 

		/*
			The beginning of a touch the state is    0
			If the touch moves the state is		     1
			When the touch ends the state is         2

			State:
				0 - the touch could have just started or it hasn't moved
				  - if the touch is 0 after the jump threshold there's no way we jumped because it has never been set to the moved state
				  - if we are 0 after the threshold then we must always walk
				1 - the touch has just moved
				  - if the touch has moved then we need to see if we moved in the correct direction
				  - if the player did move in the correct direction we need to then wait for the touch to be released
				  - once the touch is released we will release the player from the charging state and move them to the jumping state
				  - the end of the jumping state should signify whether we are allowed to jump again
				  - if we fail to meet the jump requirements we should then begin to walk until the touch has been released
				2 - the touch is over
				  - if the touch is over and we never triggered the jumping state then we need to reset our touch variables
		*/

		if (Mobile_Input::GetTouchCount() == 1) {
			for (std::pair<uint64_t, FTouchInfo> key : Mobile_Input::GetTouches()) {
				/*
					State 0:
							- Check if we have already been processed
							- if we haven't been processed then process
				*/
                if ( key.second.state == 0 && ground) {
					if (touchId == 0) {
						startxPos = key.second.x;
						startyPos = key.second.y;
						bJumping = false;
						touchId = key.second.id;
						GE_CORE_ASSERT(touchId, "invalid touch id");
						touchTime = key.second.time / 1e6;
						GE_CORE_ASSERT(touchTime != 0, "TIME IS ZERO");
					}
                } else if (key.second.state == 1 && touchId == key.second.id && ground) {
					lastxpos = key.second.x;
					lastypos = key.second.y;
                } else if ( (key.second.state == 2 || key.second.state == 3) && touchId == key.second.id && ground) {
                    if (bJumping) {
						touchId = 0;
						float yDistance = (key.second.y - startyPos+  Application::GetHeight()*.01f)/Application::GetHeight();
                        float xDistance = -(lastxpos - startxPos+  Application::GetWidth()*.01f)/Application::GetWidth();
						if (yDistance < .001f) {
							bJumping = false;
							graphicsComp->Idle();
						} else {
							graphicsComp->JumpStart([this, timestep, xDistance, yDistance]() {
								bJumping = false;
								
								bodyComp->AddVelocity({ 10 * (xDistance > .2f ? .2f : xDistance), 40.f * (yDistance > .2f ? .2f : yDistance) });
							});
						}
					}
					else {
						touchId = 0;
					}
                }

				if (touchId != 0 && Time::GetEpochTimeMS() - (key.second.GetTime() / 1e6) > 150) {
					lastxpos = key.second.x;
					lastypos = key.second.y;
					if (!bJumping && !bFalling) {
						float xPos = key.second.x;
						float width = (float)Application::GetWidth();
						//GE_CORE_DEBUG("TOUCH: {0}, {1}", key.second.x, key.second.y);
						if (xPos >= width / 2.f) {
							bodyComp->AddVelocity({ 5 * timestep, 0 });
						}
						else {
							bodyComp->AddVelocity({ -5 * timestep, 0 });
						}
					}
					else {
						if (bJumping) {
							float xDistance = -(lastxpos - startxPos + Application::GetWidth() * .01f) / Application::GetWidth();
							int nDir = xDistance > 0 ? 1 : -1;
							if (nDir != graphicsComp->dir && !graphicsComp->bAnimating) {
								graphicsComp->dir = nDir;
								graphicsComp->animState = MovementAnim::None;
								graphicsComp->Crouch();
							}
						}
					}
				}
				else  if (touchId != 0 && key.second.y > startyPos + Application::GetHeight() * .01f) {
						if (!bFalling && !bJumping && ground) {
							bJumping = true;
							lastxpos = key.second.x;
							lastypos = key.second.y;
							jumpTime = touchTime;
							graphicsComp->JumpCrouch();
						}
				}
			}

		}

		if (!bJumping) {
			if (vel.x > walkAnimThreshold || vel.x < -walkAnimThreshold) {
				graphicsComp->dir = vel.x > .01f ? 1 : -1;
				if (ground) {
					if (bFalling) {
                        //GE_LOG_DEBUG("LAND WALK");
						graphicsComp->LandIdle([this]() { bFalling = false;  graphicsComp->Idle(); });
					}
					else {
						if (graphicsComp->animState != MovementAnim::Walk) {
							graphicsComp->Walk();
						}
					}
				}
			}
			else if (vel.x <= walkAnimThreshold && vel.x >= -walkAnimThreshold) {
				if (ground) {
					if (bFalling) {
                    //GE_LOG_DEBUG("LAND IDLE");
						graphicsComp->LandIdle([this]() { bFalling = false;  graphicsComp->Idle(); });
					}
					else {
						graphicsComp->Idle();
					}
				}
			}
		}


	}

	
	Ref<CharacterGraphics> graphicsComp;
	Ref<QuadColliderComponent> groundComp;
	float startxPos = -1.f;
	float startyPos = -1.f;
	float lastxpos = -1;
	float lastypos = -1;
	uint64_t touchId = 0;
	uint64_t touchTime = 0;


};
