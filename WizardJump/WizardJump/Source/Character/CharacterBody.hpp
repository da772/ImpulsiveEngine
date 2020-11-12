#pragma once

#include "GEngine.h"

using namespace GEngine;

class CharacterBody : public Component {
public:
	CharacterBody() {

	};
	~CharacterBody() {};

	inline void AddVelocity(glm::vec2 v) {
		m_quadCollider->IncreaseLinearVelocity(v.x, v.y);
	}

	inline void SetVelocityX(float x) {
		m_quadCollider->SetVelocityX(x);
	}
	
	inline void SetVelocityY(float y) {
		m_quadCollider->SetVelocityY(y);
	}

	inline void SetVelocity(glm::vec2 v) {
		m_quadCollider->SetVelocity(v.x, v.y);
	}

	inline glm::vec2 GetVelocity() {
		return m_quadCollider->GetLinearVelocity();
	}

	bool debug = false;
	int groundedCount = 0;

	bool isGrounded() { return groundedCount > 0; };

	Ref<QuadColliderComponent> m_quadCollider;
	Ref<QuadColliderComponent> m_groundCollider;
protected:
	inline void OnBegin() override
	{
		glm::vec2 pos = glm::vec2(0, .075f);
		glm::vec2 scale = glm::vec2(.4f, 1.3f);

		glm::vec2 groundPos = glm::vec2(0, -.85f);
		glm::vec2 groundScale = glm::vec2(.4f, .1f);
		m_quadCollider = CreateGameObject<QuadColliderComponent>(true, true, glm::vec2(0,0));
		m_groundCollider = CreateGameObject<QuadColliderComponent>(true, false, glm::vec2(0,0));

		
		GetEntity()->AddComponent(m_quadCollider);
		GetEntity()->AddComponent(m_groundCollider);
		
		ColliderID id = m_quadCollider->CreateQuad(pos, scale, 1, 0, "characterBody");
		ColliderID id2 = m_quadCollider->CreateCircle({ 0,-.6f }, { .2f, .2f }, 1, 0, "characterBody");
		ColliderID id1 = m_groundCollider->CreateQuad(groundPos, groundScale, 1, 0, "characterBodyGround");

		m_groundCollider->SetOnCollideFunction(id1, [this](Ref<PhysicsCollision> other) {
			GE_CORE_DEBUG("COLLISION: {0}", other->tag);
			if ( other->tag == "ground") {
				groundedCount++;;
			}
			});

		m_groundCollider->SetEndCollideFunction(id1, [this](Ref<PhysicsCollision> other) {
			GE_CORE_DEBUG("COLLISION End: {0}", other->tag);
			if (other->tag == "ground") {
				groundedCount--;;
			}
			});

		if (debug) {
			m_debugSprite = CreateGameObject<SpriteComponent>();
			GetEntity()->AddComponent(m_debugSprite);
			m_debugSprite->CreateQuad({ pos.x,pos.y,11 }, 0, { scale.x,scale.y,1 }, { 1,0,0,.25f });
			m_debugSprite->CreateQuad({ groundPos.x,groundPos.y,11}, 0, { groundScale.x,groundScale.y,1 }, { 0,1,0,.25f });
		}

		m_quadCollider->WakeBody();
	}


	inline void OnEnd() override
	{
		m_quadCollider = nullptr;
		m_debugSprite = nullptr;
	}


	void OnUpdate(Timestep timestep) override
	{
	
	}

	

	
	Ref<SpriteComponent> m_debugSprite;

};
