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
    Ref<AudioComponent> jumpSound;
    Ref<AudioComponent> landSound;
    Ref<AudioComponent> musicSound;

protected:
	void OnBegin() override
	{
		bodyComp = CreateGameObject<CharacterBody>();
		GetEntity()->AddComponent(bodyComp);
        jumpSound = CreateGameObject<AudioComponent>("Content/Audio/testJump01.ogg", false, false, true);
        landSound = CreateGameObject<AudioComponent>("Content/Audio/dink.ogg", false, false, true);
        musicSound = CreateGameObject<AudioComponent>("Content/Audio/test.ogg", true, true, true, .25f);

        GetEntity()->AddComponent(landSound);
        GetEntity()->AddComponent(jumpSound);
        GetEntity()->AddComponent(musicSound);
		graphicsComp = static_pointer_cast<CharacterEntity>(GetEntity())->m_spriteComponent;
		
	}


	void OnEnd() override
	{
		bodyComp = nullptr;
		graphicsComp = nullptr;

	}

	bool bFalling = false;
	bool bJumping = false;
	bool bWalking = false;
	const float walkAnimThreshold = .03f;
	const float maxWalkSpeed = 2.5f;
    const int walkDelay = 100;
	const float walkAcceleration = 5.f;
    const float jumpThreshold = .005f;

    /*
        PC Touch Emulation
    */
    uint64_t pcTouchTime = 0;
    int pcX = 0, pcY = 0, pcState = -1;


	void OnUpdate(Timestep timestep) override
	{
		const glm::vec2& vel = bodyComp->GetVelocity();
		const bool ground = bodyComp->isGrounded();
        std::unordered_map<uint64_t, FTouchInfo> m = Mobile_Input::GetTouches();
#ifdef GE_CONSOLE_APP
		/*
			PC Controls
		*/
        int lstX = Input::GetMouseX();
        int lstY = Input::GetMouseY();
        bool addTouch = false;
        ImGuiIO& io = ImGui::GetIO();
        
#if !defined(GE_DIST)
        if (io.WantCaptureKeyboard || io.WantCaptureMouse) goto consoleEnd;
#endif
        if (Input::IsMouseButtonPressed(0)) {
            if (pcState == -1 || pcState == 2) {
                pcTouchTime = Time::GetEpochTimeNS();
                pcX = lstX;
                pcY = lstY;
                pcState = 0;
                addTouch = true;
            }
            else if (pcState == 0 || pcState == 1) {
                if (lstY != pcY || lstX != pcX) {
                    pcState = 1;
                    pcX = lstX;
                    pcY = lstY;
                    addTouch = true;
                }
            }
        }
        else {
            if (pcState != -1) {
                if (pcState == 2) {
                    pcState = -1;
                    return;
                }
				pcState = 2;
				pcX = lstX;
				pcY = lstY;
                addTouch = true;
            }
        }

        if (pcState != -1) {
            m[pcTouchTime] = FTouchInfo(pcTouchTime, pcState, pcX, pcY, .5f, pcTouchTime);
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
#if !defined(GE_DIST)
        consoleEnd:
#endif
#endif
		/*
			Mobile Controls

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
        
		if (Mobile_Input::GetTouchCount() <= 2 && Mobile_Input::GetTouchCount() > 0 || m.size() > 0) {
            std::vector<FTouchInfo> touches;
            if (m.size() == 2) {
                bool hasEnd = false;
                for (std::pair<uint64_t, FTouchInfo> key : m) {
                    const FTouchInfo& touch = key.second;
                    touches.push_back(touch);
                    if (!CollisionDetection::CheckPointUI(touch.x, touch.y)) {
                        if (touch.state == 2 ) {
                            if (hasEnd) {
                                hasEnd = false;
                                break;
                            }
                            hasEnd = true;
                            if (bWalking && ground) {
                                   bodyComp->SetVelocityX(0.f);
                                   bWalking = false;
                               }
                        }
                    } else {
                        hasEnd = false;
                    }
                }
                if (hasEnd) {
                    std::sort(touches.begin(), touches.end(), [] (const FTouchInfo& obj1, const FTouchInfo& obj2 ) {
                        return obj1.state > obj2.state;
                    });
                    HandleMobileInput(touches, timestep);
                } else {
                    bJumping = false;
                    if (bWalking && ground) {
                        bodyComp->SetVelocityX(0.f);
                    }
                    bWalking = false;
                    touchId = 0;
                }
            } else {
                HandleMobileInput({m.begin()->second}, timestep);
            }
		

        }

		if (!ground) {
			if (bFalling) {
				float nDir = vel.x > .01f ? 1 : vel.x < -.01f ?  -1 : graphicsComp->dir;
				if (graphicsComp->dir != nDir) {
					graphicsComp->dir = nDir;
					graphicsComp->animState = MovementAnim::None;
					graphicsComp->Falling();
				}
			}
			bFalling = true;
			graphicsComp->Falling();
		}

		if (!bJumping) {
			if (vel.x > walkAnimThreshold || vel.x < -walkAnimThreshold) {
                graphicsComp->dir = vel.x > .01f ? 1 : vel.x < -.01f ?  -1 : graphicsComp->dir;
				if (ground) {
					if (bFalling) {
						bodyComp->SetVelocityX(0);
                        landSound->SetPlaying(true);
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
				//graphicsComp->dir = vel.x > 0.f ? 1 : -1;
				if (ground) {
					if (bFalling) {
                        landSound->SetPlaying(true);
						graphicsComp->LandIdle([this]() { bFalling = false;  graphicsComp->Idle(); });
					}
					else {
						graphicsComp->Idle();
					}
				}
			}
		}
	}
    
    
    void HandleMobileInput(const std::vector<FTouchInfo>& m, Timestep timestep) {
        const glm::vec2& vel = bodyComp->GetVelocity();
        const bool ground = bodyComp->isGrounded();
        for ( const FTouchInfo& touch : m) {
                    /*
                        State 0:
                                - Check if we have already been processed
                                - if we haven't been processed then process
                    */

                    if (!ground) {
                        touchId = 0;
                        bJumping = false;
                    }

                    if ( touch.state == 0 && ground) {
                        if (touchId == 0) {
                            startxPos = touch.x;
                            startyPos = touch.y;
                            bJumping = false;
                            touchId = touch.id;
                            GE_CORE_ASSERT(touchId, "invalid touch id");
                            touchTime = touch.time / 1e6;
                            GE_CORE_ASSERT(touchTime != 0, "TIME IS ZERO");
                        }
                    } else if (touch.state == 1 && touchId == touch.id && ground) {
                        lastxpos = touch.x;
                        lastypos = touch.y;
                    } else if ( (touch.state == 2 || touch.state == 3) && ground) {
                        if (bJumping && touchId == touch.id ) {
                            touchId = 0;
                            float yDistance = (touch.y - startyPos+  Application::GetHeight()*jumpThreshold)/Application::GetHeight();
                            
                            float xDistance = 2.f*-(lastxpos-(float)Application::GetWidth()/2.f)/(float)Application::GetWidth();
                            GE_CORE_DEBUG("Last Pos: {0}, Current Pos: {1}, Width: {2}, xDistance: {3}", lastxpos, touch.x, Application::GetWidth(), xDistance);
                            if (yDistance < .001f) {
                                bJumping = false;
                                graphicsComp->Idle();
                            } else {
                                graphicsComp->JumpStart([this, xDistance, yDistance]() {
                                    bJumping = false;
                                    jumpSound->SetPlaying(true);
                                    bodyComp->AddVelocity({ 10 * (abs(xDistance) > .2f ? (xDistance >= 0 ? 1.f : -1.f) *.2f : xDistance), 40.f * (yDistance > .2f ? .2f : yDistance) });
                                });
                            }
                        }
                        if (bWalking) {
                            bWalking = false;
                            bodyComp->SetVelocityX(0);
                        }
                        touchId = 0;
                    }

				
            if (touch.state < 2 && Time::GetEpochTimeMS() - (touch.time / 1e6) > walkDelay) {
                        lastxpos = touch.x;
                        lastypos = touch.y;
                        if (!bJumping && !bFalling && ground) {
                            float xPos = touch.x;
                            float width = (float)Application::GetWidth();
                            
                            float xVel = vel.x;
                            float nXVel = xVel + walkAcceleration * timestep;
                            if ((touchId == 0 || !bJumping) && xPos >= width / 2.f) {
                                if (bWalking && GEMath::sign(nXVel) != GEMath::sign(vel.x)) {
                                    bodyComp->SetVelocityX(0);
                                }
                                bWalking = true;
                                if (nXVel <= maxWalkSpeed) {
                                    bodyComp->AddVelocity({ walkAcceleration * timestep, 0 });
                                } else if ( (nXVel - maxWalkSpeed) >= 0.01f) {
                                    bodyComp->SetVelocityX(maxWalkSpeed);
                                }
                            }
                            else if (touchId != 0 || !bJumping) {
                                float nXVel = xVel - walkAcceleration * timestep;
                                if (bWalking && GEMath::sign(nXVel) != GEMath::sign(vel.x)) {
                                    bodyComp->SetVelocityX(0);
                                }
                                bWalking = true;
                                
                                if (nXVel >= -maxWalkSpeed) {
                                    bodyComp->AddVelocity({ -walkAcceleration * timestep, 0 });
                                }
                                else if ( (nXVel + maxWalkSpeed <= -0.01f) ) {
                                    bodyComp->SetVelocityX(-maxWalkSpeed);
                                }
                                
                            }
                        
                        }
                        else {
                            if (!ground) {
                                bWalking = false;
                                bJumping = false;
                            }
                            else if (bJumping) {
                                int nDir = (touch.x >= (float)Application::GetWidth() / 2.f) ? -1 : 1;
                                if (nDir != graphicsComp->dir && !graphicsComp->bAnimating) {
                                    graphicsComp->dir = nDir;
                                    graphicsComp->animState = MovementAnim::None;
                                    graphicsComp->Crouch();
                                }
                            }
                        }
                    }
                    else if (touchId != 0 && touch.y > startyPos + Application::GetHeight() * jumpThreshold) {
                            if (!bFalling && !bJumping && ground) {
                                bJumping = true;
                                lastxpos = touch.x;
                                lastypos = touch.y;
                                graphicsComp->JumpCrouch();
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
