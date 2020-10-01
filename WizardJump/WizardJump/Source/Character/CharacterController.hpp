#pragma once

#include "GEngine.h"
#include "Character/CharacterBody.hpp"
#include "Character/CharacterGraphics.hpp"

using namespace GEngine;

class CharacterController : public Component {
public:
	CharacterController() : Component() { 

		bUpdates = true;
	
	};
	~CharacterController() {};

	Ref<CharacterBody> bodyComp;
    Ref<AudioComponent> jumpSound;
    Ref<AudioComponent> landSound;
    Ref<AudioComponent> musicSound;
    long long startTime;

protected:
    void OnBegin() override;


	void OnEnd() override
	{
		bodyComp = nullptr;
		graphicsComp = nullptr;
        jumpSound = nullptr;
        landSound = nullptr;

	}
public:
	bool bFalling = false;
	bool bJumping = false;
	bool bWalking = false;
	const float walkAnimThreshold = .03f;
	const float maxWalkSpeed = 2.5f;
    const int walkDelay = 125;
	const float walkAcceleration = 5.f;
    const float jumpThreshold = .005f;
	const static float jumpYMultipler;
	const static float jumpXMultipler;
    const static float jumpXDragClamp;
    const static float jumpYDragClamp;
    static const float maxJumpXVel;
    static const float maxJumpYVel;



	std::unordered_map<uint64_t, FTouchInfo> m;

	/* TRAJECTORTY TEST */
	std::vector<float> trajectory_pos;
	glm::vec2 _realVel;

	static glm::vec2 CalculateJumpVelocity(float xDistance, float yDistance) {
		glm::vec2 _vel = { (abs(xDistance) > jumpXDragClamp ? (xDistance >= 0 ? 1.f : -1.f) * jumpXDragClamp : xDistance), (yDistance > jumpYDragClamp ? jumpYDragClamp : yDistance) };
        glm::vec2 mapped_vel = { GEMath::sign(_vel.x) * GEMath::MapRange(abs(_vel.x), 0, jumpXDragClamp, 0, 1), GEMath::MapRange(_vel.y, 0, jumpYDragClamp, 0, 1)};
        glm::vec2 vel = { mapped_vel.x * jumpXMultipler, mapped_vel.y * jumpYMultipler };
        return vel;
	}

    static glm::vec2 DragRequiredForVelocity(const glm::vec2& velocity) {
        return { GEMath::sign(velocity.x) * GEMath::MapRange(abs(velocity.x) / jumpXMultipler, 0, 1, 0, jumpXDragClamp), GEMath::MapRange(velocity.y / jumpYMultipler, 0,1, 0, jumpYDragClamp) };
	}


protected:

    /*
        PC Touch Emulation
    */
    uint64_t pcTouchTime = 0;
    int pcX = 0, pcY = 0, pcState = -1;


    void SetJumping(bool bJump) {
        bJumping = bJump;
        graphicsComp->ShowDirectionIndicator(bJumping);
        graphicsComp->ShowPowerBar(bJumping);
    }

	void OnUpdate(Timestep timestep) override
	{

        //if (trajectory_pos.size() > 0)
          //  Renderer::DrawDebugLines(trajectory_pos, glm::vec4(1, 0, 0, 1.f));
		const glm::vec2& vel = bodyComp->GetVelocity();
		const bool ground = bodyComp->isGrounded();
        m = Mobile_Input::GetTouches();
#ifdef GE_CONSOLE_APP
		/*
			PC Controls
		*/
        int lstX = Input::GetMouseX();
        int lstY = Input::GetMouseY();
        bool addTouch = false;

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
                    SetJumping(false);
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
                        graphicsComp->LandIdle([this]() { if (!graphicsComp) return; bFalling = false;  graphicsComp->Idle(); });
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


                    if (CollisionDetection::CheckPointUI(touch.x, touch.y) || startTime > touch.time) {
                        touchId = 0;
                        SetJumping(false);
                        bWalking = false;
                        if (ground) {
                            bodyComp->SetVelocityX(0);
                        }
                        continue;
                    }

                    if (!ground) {
                        touchId = 0;
                        SetJumping(false);
                    }

                    if ( touch.state == 0 && ground) {
                        if (touchId == 0) {
                            startxPos = touch.x;
                            startyPos = touch.y;
                            SetJumping(false);
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
                                SetJumping(false);
                                graphicsComp->Idle();
                            } else {
                                graphicsComp->ShowDirectionIndicator(false);
                                graphicsComp->JumpStart([this, xDistance, yDistance]() {
                                    SetJumping(false);
                                    jumpSound->SetPlaying(true);
                                    bodyComp->SetVelocity(CalculateJumpVelocity(xDistance, yDistance));
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
                                SetJumping(false);
                            }
                            else if (bJumping) {

                                int nDir = (touch.x >= (float)Application::GetWidth() / 2.f) ? -1 : 1;
								float yDistance = (touch.y - startyPos + Application::GetHeight() * jumpThreshold) / Application::GetHeight();
								float xDistance = 2.f * -(lastxpos - (float)Application::GetWidth() / 2.f) / (float)Application::GetWidth();

                                /* DEBUG */
                                if (touch.state == 1)
                                    PredictPath(xDistance, yDistance);

                                glm::vec2 _vel = CalculateJumpVelocity(xDistance, yDistance);
                                float _deg = glm::degrees(atan2(-_vel.x, _vel.y));
                                GE_LOG_DEBUG("{0},{1} : {2}", _vel.x, _vel.y, _deg);
                                graphicsComp->SetDirectionIndicator(_deg);
                                graphicsComp->SetPowerBar(GEMath::MapRange(yDistance, 0, jumpYDragClamp, 0, 1.f));
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
                                SetJumping(true);
                                lastxpos = touch.x;
                                lastypos = touch.y;
                                graphicsComp->JumpCrouch();
                            }
                    }
                }
    }


  

    void PredictPath(float xDistance, float yDistance) {
            _realVel = CalculateJumpVelocity(xDistance, yDistance);
           // GE_LOG_WARN("PREDICTED VEL: {0},{1}", _realVel.x, _realVel.y);
            trajectory_pos.clear();
            std::vector<Weak<PhysicsBody>> ignoreBodies = { bodyComp->m_groundCollider->GetPhysicsBody(), bodyComp->m_quadCollider->GetPhysicsBody() };
            glm::vec2 _startPos = { GetEntityPosition().x,  bodyComp->m_quadCollider->GetPosition().y - bodyComp->m_quadCollider->GetScale().y/2.f};
			for (int i = 0; i < 180; i++) {
				glm::vec2 newPos = Physics::GetTrajectoryPoint2D(_startPos ,
					_realVel, i);
                glm::vec2 newPos2 = Physics::GetTrajectoryPoint2D({ _startPos.x, _startPos.y + bodyComp->m_quadCollider->GetScale().y },
					_realVel, i);
                
				if (trajectory_pos.size() >= 3) {
                    Ref<RayCastInfo> info = nullptr;
                    Ref<RayCastInfo> info2 = nullptr;
                    glm::vec2 rayPos = { trajectory_pos[trajectory_pos.size() - 3],trajectory_pos[trajectory_pos.size() - 2] };
                    if (rayPos != newPos) {
                        info = Physics::RayCast2D(rayPos, { newPos.x,newPos.y }, ignoreBodies);
                        info2 = Physics::RayCast2D(rayPos, { newPos2.x,newPos2.y }, ignoreBodies);
                    }
					if (info && info->physicsBody.lock() != nullptr) {
						//if (info->physicsBody.lock())
						//	GE_CORE_INFO("HIT OBJECT, {0} Bounce: {1}", info->physicsBody.lock()->GetComponent()->GetTag(), info->physicsBody.lock()->GetBounce());
                        trajectory_pos.push_back(info->hitPoint.x);
                        trajectory_pos.push_back(info->hitPoint.y);
                        trajectory_pos.push_back(1.f);
                        /*
						if (info->physicsBody.lock() && info->physicsBody.lock()->GetBounce() != 0.f) {
							//GE_LOG_INFO("Normal: {0}, {1}", info->hitNormal.x, info->hitNormal.y);
                            //_realVel = GEMath::reflect(_realVel * info->physicsBody.lock()->GetBounce(), info->hitNormal);
							//GE_CORE_DEBUG("{0}, {1}", _realVel.x, _realVel.y);
							//i = 0.f;
							//_startPos = info->hitPoint;
							continue;
						}
                        */

						break;
					}

                    if (info2 && info2->physicsBody.lock() != nullptr) {
                        //if (info->physicsBody.lock())
                        //	GE_CORE_INFO("HIT OBJECT, {0} Bounce: {1}", info->physicsBody.lock()->GetComponent()->GetTag(), info->physicsBody.lock()->GetBounce());
                        trajectory_pos.push_back(info2->hitPoint.x);
                        trajectory_pos.push_back(info2->hitPoint.y);
                        trajectory_pos.push_back(1.f);
                        /*                            if (info2->physicsBody.lock() && info2->physicsBody.lock()->GetBounce() != 0.f) {
                            //GE_LOG_INFO("Normal: {0}, {1}", info->hitNormal.x, info->hitNormal.y);
                            _realVel = GEMath::reflect(_realVel * info2->physicsBody.lock()->GetBounce(), info2->hitNormal);
                            //GE_CORE_DEBUG("{0}, {1}", _realVel.x, _realVel.y);
                            i = 0.f;
                            _startPos = info2->hitPoint;
                            continue;
                        }
						*/


                        break;
                    }
                     
					
                }
                trajectory_pos.push_back(newPos.x);
                trajectory_pos.push_back(newPos.y);
                trajectory_pos.push_back(1.f);
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
