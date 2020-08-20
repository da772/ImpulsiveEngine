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
		graphicsComp = dynamic_pointer_cast<CharacterEntity>(GetEntity())->m_spriteComponent;
		
	}


	void OnEnd() override
	{
		bodyComp = nullptr;
		graphicsComp = nullptr;

	}

	bool bFalling = false;
	bool bJumping = false;
	float walkAnimThreshold = .05f;


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

		

		if (Mobile_Input::GetTouchCount() == 1) {
			for (std::pair<uint64_t, FTouchInfo> key : Mobile_Input::GetTouches()) {
				//GE_LOG_DEBUG("STATE: {0} {1}", key.second.GetID(), key.second.GetState());
				if (Time::GetEpochTimeMS() - key.second.GetTime() > 250) {
					lastxpos = key.second.x;
					lastypos = key.second.y;
					if (!bJumping && !bFalling) {
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
                        if (bJumping) {
                            float xDistance = -(lastxpos - startxPos +  Application::GetWidth()*.01f)/Application::GetWidth();
                            int nDir = xDistance > 0 ? 1 : -1;
                            if (nDir != graphicsComp->dir && !graphicsComp->bAnimating) {
                                graphicsComp->dir = nDir;
                                graphicsComp->animState = MovementAnim::None;
                                graphicsComp->Crouch();
                            }
                            
                            
                        }
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
                        if (!bFalling && !bJumping) {
                            bJumping = true;
                            graphicsComp->JumpCrouch();
                        }
                        
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

				graphicsComp->JumpStart([this, timestep, xDistance, yDistance]() {
					bJumping = false;
					bodyComp->AddVelocity({ 10 * (xDistance > .2f ? .2f : xDistance), 40.f * (yDistance > .2f ? .2f : yDistance) });
					});
			}
			else {
				bJumping = false;
				graphicsComp->Idle();
			}
			touchId = 0;
			touchTime = 0;
			lastxpos = -1;
			lastypos = -1;
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
