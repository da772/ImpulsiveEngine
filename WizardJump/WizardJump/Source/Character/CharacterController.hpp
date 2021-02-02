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
	bool bEnableJump = true;
	bool bEnableWalk = true;
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

	inline void SetInputFilterFunction(std::function<bool(const FTouchInfo& touch)> f) {
		m_inputFilterFunc = f;
	}

	inline bool GetIsJumping() const {
		return bJumping; 
	};

	void ResetInput();

	Vector2f GetPredictedJumpVel(float newY);

	std::unordered_map<uint64_t, FTouchInfo> m;

	/* TRAJECTORTY TEST */
	std::vector<float> trajectory_pos;
	Vector2f _realVel;

	inline static Vector2f CalculateJumpVelocity(float xDistance, float yDistance) {
		Vector2f _vel = { (abs(xDistance) > jumpXDragClamp ? (xDistance >= 0 ? 1.f : -1.f) * jumpXDragClamp : xDistance), (yDistance > jumpYDragClamp ? jumpYDragClamp : yDistance) };
        Vector2f mapped_vel = { GEMath::sign(_vel.x) * GEMath::MapRange(abs(_vel.x), 0, jumpXDragClamp, 0, 1), GEMath::MapRange(_vel.y, 0, jumpYDragClamp, 0, 1)};
        Vector2f vel = { mapped_vel.x * jumpXMultipler, mapped_vel.y * jumpYMultipler };
        return vel;
	}

    inline static Vector2f DragRequiredForVelocity(const Vector2f& velocity) {
        return { GEMath::sign(velocity.x) * GEMath::MapRange(abs(velocity.x) / jumpXMultipler, 0, 1, 0, jumpXDragClamp), GEMath::MapRange(velocity.y / jumpYMultipler, 0,1, 0, jumpYDragClamp) };
	}


protected:

    /*
        PC Touch Emulation
    */
    uint64_t pcTouchTime = 0;
    int pcX = 0, pcY = 0, pcState = -1;

	std::function<bool(const FTouchInfo& touch)> m_inputFilterFunc = nullptr;


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
