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
    Ref<AudioComponent> leftFootSound;
	Ref<AudioComponent> rightFootSound;
    Ref<AudioComponent> musicSound;
    long long startTime;

protected:
    void OnBegin() override;


	void OnEnd() override
	{
		bodyComp = nullptr;
		graphicsComp = nullptr;
        jumpSound = nullptr;
		leftFootSound = nullptr;
		rightFootSound = nullptr;

	}
public:
	bool bFalling = false;
	bool bJumping = false;
	bool bWalking = false;
	bool bEnableInput = true;
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

	inline static glm::vec2 CalculateJumpVelocity(float xDistance, float yDistance) {
		glm::vec2 _vel = { (abs(xDistance) > jumpXDragClamp ? (xDistance >= 0 ? 1.f : -1.f) * jumpXDragClamp : xDistance), (yDistance > jumpYDragClamp ? jumpYDragClamp : yDistance) };
        glm::vec2 mapped_vel = { GEMath::sign(_vel.x) * GEMath::MapRange(abs(_vel.x), 0, jumpXDragClamp, 0, 1), GEMath::MapRange(_vel.y, 0, jumpYDragClamp, 0, 1)};
        glm::vec2 vel = { mapped_vel.x * jumpXMultipler, mapped_vel.y * jumpYMultipler };
        return vel;
	}

    inline static glm::vec2 DragRequiredForVelocity(const glm::vec2& velocity) {
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

	void HandleMobileInput(const std::vector<FTouchInfo>& m, Timestep timestep);

    void OnUpdate(Timestep timestep) override;
    

	void PredictPath(float xDistance, float yDistance);
			


	
	Ref<CharacterGraphics> graphicsComp;
	Ref<QuadColliderComponent> groundComp;
	float startxPos = -1.f;
	float startyPos = -1.f;
	float lastxpos = -1;
	float lastypos = -1;
	uint64_t touchId = 0;
	uint64_t touchTime = 0;


};
