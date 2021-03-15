#include "Character/CharacterController.hpp"

#include "Character/CharacterEntity.h"

const float CharacterController::maxJumpXVel = 5.f;
const float CharacterController::maxJumpYVel = 5.f;
const float CharacterController::jumpXDragClamp = .35f;
const float CharacterController::jumpYDragClamp = .3f;

const float CharacterController::jumpYMultipler = 10.f;
const float CharacterController::jumpXMultipler = 4.f;

static void DrawTrajectoryLines(const std::vector<float>& traj, Ref<SpriteComponent> spriteComp, float scale = .1f, const Vector4f& color = Vector4f(0.f), const Vector3f& offset = Vector3f(0.f), Ref<Texture2D> texture = nullptr);

void CharacterController::OnBegin()
{
	startTime = Time::GetEpochTimeNS();
	bodyComp = CreateGameObject<CharacterBody>();
    spriteComp = CreateGameObject<SpriteComponent>();
	jumpSound = CreateGameObject<AudioComponent>("Content/Audio/jumpRustling.ogg", false, false, true, .75f);
    leftFootSound = CreateGameObject<AudioComponent>("Content/Audio/sneakerConcrete.ogg", false, false, true, .75f);
    rightFootSound = CreateGameObject<AudioComponent>("Content/Audio/sneakerConcrete.ogg", false, false, true, .75f);
	//musicSound = CreateGameObject<AudioComponent>("Content/Audio/test.ogg", true, true, true, .25f);

	GetEntity()->AddComponent(spriteComp);
	GetEntity()->AddComponent(bodyComp);
	GetEntity()->AddComponent(leftFootSound);
	GetEntity()->AddComponent(rightFootSound);
	GetEntity()->AddComponent(jumpSound);
	//GetEntity()->AddComponent(musicSound);
	graphicsComp = static_pointer_cast<CharacterEntity>(GetEntity())->m_spriteComponent;

    graphicsComp->footDownCallback = [this](uint8_t foot) {
        switch (foot) {
        default:
        case 0:
        {
            if (!leftFootSound->IsPlaying()) {
                leftFootSound->SetPitch(1.f + Random::FloatRange(0.f, 0.15f));
                leftFootSound->SetPlaying(true);
            }
            break;
        }
        case 1: 
        {
            if (!rightFootSound->IsPlaying()) {
                rightFootSound->SetPitch(1.f + Random::FloatRange(0.f, 0.15f));
                rightFootSound->SetPlaying(true);
            }
            break;
        }
        case 2:
        {
            if (!rightFootSound->IsPlaying() && !leftFootSound->IsPlaying()) {
                leftFootSound->SetPitch(1.f + Random::FloatRange(0.f, .15f));
                rightFootSound->SetPitch(1.f + Random::FloatRange(0.f, .15f));
                rightFootSound->SetPlaying(true);
                leftFootSound->SetPlaying(true);
            }
            break;
        }
        }
    };
	
}

void CharacterController::ResetInput()
{
	startxPos = -1.f;
	startyPos = -1.f;
	lastxpos = -1;
	lastypos = -1;
	touchId = 0;
	touchTime = 0;
	bFalling = false;
	bWalking = false;
    SetJumping(false);
}

GEngine::Vector2f CharacterController::GetPredictedJumpVel(float newY)
{
	float yDistance = (newY - startyPos + Application::GetHeight() * jumpThreshold) / Application::GetHeight();

	float xDistance = 2.f * -(lastxpos - (float)Application::GetWidth() / 2.f) / (float)Application::GetWidth();
	return { xDistance, yDistance };
}

void CharacterController::OnUpdate(Timestep timestep) {

    trajDrawTimer += timestep.GetMilliseconds();
    if (bdrawTrajectory && trajectory_pos.size() > 0 && trajDrawTimer > 16 && bRedrawTrajectory) {
        bRedrawTrajectory = false;
        DrawTrajectoryLines(trajectory_pos, spriteComp, trajectorySize,  trajectoryColor, {GetEntityPosition().xy(), -trajectoryZorder }, trajectoryTexture);
        trajDrawTimer = 0;
    }
    //Renderer::DrawDebugLines(trajectory_pos, Vector4f(1, 0, 0, 1.f));
    const Vector2f& vel = bodyComp->GetVelocity();
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
                    if (touch.state == 2) {
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
                }
                else {
                    hasEnd = false;
                }
            }
            if (hasEnd) {
                std::sort(touches.begin(), touches.end(), [](const FTouchInfo& obj1, const FTouchInfo& obj2) {
                    return obj1.state > obj2.state;
                    });
                HandleMobileInput(touches, timestep);
            }
            else {
                SetJumping(false);
                if (bWalking && ground) {
                    bodyComp->SetVelocityX(0.f);
                }
                bWalking = false;
                touchId = 0;
            }
        }
        else {
            HandleMobileInput({ m.begin()->second }, timestep);
        }


    }



    if (!ground) {
        if (bFalling) {
            float nDir = vel.x > .01f ? 1 : vel.x < -.01f ? -1 : graphicsComp->dir;
            if (graphicsComp->dir != nDir) {
                graphicsComp->dir = nDir;
                graphicsComp->animState = MovementAnim::None;
                if (graphicsComp->animState != MovementAnim::Jump)
                    graphicsComp->Falling();
            }
        }
        if (graphicsComp->animState != MovementAnim::Jump) {
            bFalling = true;
            graphicsComp->Falling();
        }
    }


    if (!bJumping) {
        if (vel.x > walkAnimThreshold || vel.x < -walkAnimThreshold) {
            graphicsComp->dir = vel.x > .01f ? 1 : vel.x < -.01f ? -1 : graphicsComp->dir;
            if (ground) {
                if (bFalling) {
                    if (abs(vel.y) <= 0.001f) {
                        bodyComp->SetVelocityX(0);
                        graphicsComp->LandIdle([this]() { bFalling = false;  graphicsComp->Idle(); });
                    }
                }
                else {
                    if (graphicsComp->animState != MovementAnim::Walk && graphicsComp->animState != MovementAnim::Falling && graphicsComp->animState != MovementAnim::Jump) {
                        graphicsComp->Walk();
                    }
                }
            }
        }
        else if (vel.x <= walkAnimThreshold && vel.x >= -walkAnimThreshold) {
            if (ground) {
                if (bFalling) {
                    if (abs(vel.y) <= 0.001f) {
                        graphicsComp->LandIdle([this]() { if (!graphicsComp) return; bFalling = false;  graphicsComp->Idle(); });
                    }
                }
                else {
                    if (graphicsComp->animState != MovementAnim::Jump)
                        graphicsComp->Idle();
                }
            }
        }
    }
}

void CharacterController::HandleMobileInput(const std::vector<FTouchInfo>& m, Timestep timestep) {
    if (!bEnableInput)
        return;
   
    const Vector2f& vel = bodyComp->GetVelocity();
    const bool ground = bodyComp->isGrounded();
    for (const FTouchInfo& touch : m) {
		if (m_inputFilterFunc) {
			if (!m_inputFilterFunc(touch)) continue;
		}
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

        if (touch.state == 0 && ground) {
            if (touchId == 0) {
                startxPos = touch.x;
                startyPos = touch.y;
                SetJumping(false);
                touchId = touch.id;
                GE_CORE_ASSERT(touchId, "invalid touch id");
                touchTime = touch.time / 1e6;
                GE_CORE_ASSERT(touchTime != 0, "TIME IS ZERO");
            }
        }
        else if (touch.state == 1 && touchId == touch.id && ground) {
            lastxpos = touch.x;
            lastypos = touch.y;
        } else if ((touch.state == 2 || touch.state == 3) && ground) {
            if (bJumping && touchId == touch.id) {
                touchId = 0;
                float yDistance = (touch.y - startyPos + Application::GetHeight() * jumpThreshold) / Application::GetHeight();

                float xDistance = 2.f * -(lastxpos - (float)Application::GetWidth() / 2.f) / (float)Application::GetWidth();
                //GE_CORE_DEBUG("Last Pos: {0}, Current Pos: {1}, Width: {2}, xDistance: {3}, yDistance: {4}", lastxpos, touch.x, Application::GetWidth(), xDistance, yDistance);
                if (yDistance < .08f) {
                    SetJumping(false);
                    graphicsComp->Idle();
                }
                else {
					graphicsComp->JumpStart([this, xDistance, yDistance]() {
						graphicsComp->animState = MovementAnim::None;
                        bodyComp->SetVelocity(CalculateJumpVelocity(xDistance, yDistance));
						jumpSound->SetPlaying(true);
						graphicsComp->ShowDirectionIndicator(false);
						SetJumping(false);
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
            if (!bJumping && !bFalling && ground && bEnableWalk) {
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
                        bodyComp->AddVelocity({ (float)(walkAcceleration * timestep), 0.0 });
                    }
                    else if ((nXVel - maxWalkSpeed) >= 0.01f) {
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
                        bodyComp->AddVelocity({ (float)(-walkAcceleration * timestep), 0.0f });
                    }
                    else if ((nXVel + maxWalkSpeed <= -0.01f)) {
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
                    if (touch.state == 1 && bdrawTrajectory)
                        PredictPath(xDistance, yDistance);

                    Vector2f _vel = CalculateJumpVelocity(xDistance, yDistance);
                    float _deg = glm::degrees(atan2(-_vel.x, _vel.y));
                    //GE_LOG_DEBUG("{0},{1} : {2}", _vel.x, _vel.y, _deg);
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
        else if (bEnableJump && touchId != 0 && touch.y > startyPos + Application::GetHeight() * jumpThreshold) {
            if (!bFalling && !bJumping && ground) {
                SetJumping(true);
                lastxpos = touch.x;
                lastypos = touch.y;
                graphicsComp->JumpCrouch();
            }
        }
    }
}

void CharacterController::PredictPath(float xDistance, float yDistance) {
    bRedrawTrajectory = true;
    _realVel = CalculateJumpVelocity(xDistance, yDistance);
    // GE_LOG_WARN("PREDICTED VEL: {0},{1}", _realVel.x, _realVel.y);
    trajectory_pos.clear();
    std::vector<Weak<PhysicsBody>> ignoreBodies = { bodyComp->m_groundCollider->GetPhysicsBody(), bodyComp->m_quadCollider->GetPhysicsBody() };
    FColliderQuad quad = bodyComp->m_quadCollider->GetQuadCollider(bodyComp->quadColliderID);
    Vector2f _startPos = { GetEntityPosition().x, GetEntityPosition().y + quad.position.y - (quad.scale.y/2.f) - trajectoryOffset };
    for (int i = 0; i < 180; i++) {
        Vector2f newPos = Physics::GetTrajectoryPoint2D(_startPos,
            _realVel, i);
        Vector2f newPos2 = Physics::GetTrajectoryPoint2D({ _startPos.x, _startPos.y + bodyComp->m_quadCollider->GetScale().y},
            _realVel, i);

        if (trajectory_pos.size() >= 3) {
            Ref<RayCastInfo> info = nullptr;
            Ref<RayCastInfo> info2 = nullptr;
            Vector2f rayPos = { trajectory_pos[trajectory_pos.size() - 3],trajectory_pos[trajectory_pos.size() - 2] };
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

void DrawTrajectoryLines(const std::vector<float>& traj, Ref<SpriteComponent> spriteComp, float _scale, const Vector4f& col, const Vector3f& offset, Ref<Texture2D> texture)
{
    std::vector<ShapeID> quads = spriteComp->GetQuads();
    int quadCounter = 0;
    for (int i = 0; i < traj.size(); i += 3) {
        Vector3f pos = &traj[i];
        float rot = 0;
        Vector3f scale(.1f, _scale, 1.f);
        if (i < traj.size() - 3) {
            Vector3f pos2 = &traj[i+3];
            rot = GEMath::RadToDeg(atan2(pos.y - pos2.y, pos.x - pos2.x));
            scale.x += abs(pos2.y - pos.y)/2.f;
        }
        else {
			Vector3f pos2 = &traj[i - 3];
			rot = GEMath::RadToDeg(atan2(pos2.y - pos.y, pos2.x - pos.x));
			scale.x += abs(pos2.y - pos.y)/2.f;
        }
        if (quadCounter < quads.size()) {
            spriteComp->SetSafeParams(quads[quadCounter], pos - offset, rot, scale, col);
        } else {
            spriteComp->CreateQuad(pos - offset, rot, scale, col, texture);
        }
        quadCounter++;
    }

    if (quadCounter < quads.size()) {
        int rmAmt = quads.size() - quadCounter;
        std::vector<ShapeID> __quads = spriteComp->GetQuads();
        int sz = __quads.size();
        while (rmAmt > 0) {
            spriteComp->RemoveQuad(__quads[sz - rmAmt--]);
        }
    }
}




