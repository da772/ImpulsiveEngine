#pragma once

#include "GEngine.h"

using namespace GEngine;

class CharacterBody : public Component {
public:
	CharacterBody() {

	};
	~CharacterBody() {};


	inline void AddVelocity(Vector2f v) {
		m_quadCollider->IncreaseLinearVelocity(v.x, v.y);
	}

	inline void SetVelocityX(float x) {
		m_quadCollider->SetVelocityX(x);
	}
	
	inline void SetVelocityY(float y) {
		m_quadCollider->SetVelocityY(y);
	}

	inline void SetVelocity(Vector2f v) {
		m_quadCollider->SetVelocity(v.x, v.y);
	}

	inline Vector2f GetVelocity() {
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
		Vector2f pos = Vector2f(0.f, .075f);
		Vector2f scale = Vector2f(.4f, 1.3f);

		Vector2f groundPos = Vector2f(0.f, -.85f);
		Vector2f groundScale = Vector2f(.4f, .1f);
		m_quadCollider = CreateGameObject<QuadColliderComponent>(true, true, Vector2f(0.f,0.f));
		m_groundCollider = CreateGameObject<QuadColliderComponent>(true, false, Vector2f(0.f,0.f));

		
		GetEntity()->AddComponent(m_quadCollider);
		GetEntity()->AddComponent(m_groundCollider);
		
		ColliderID id = m_quadCollider->CreateQuad(pos, scale, 1, 0, "characterBody");
		ColliderID id2 = m_quadCollider->CreateCircle({ 0.f,-.6f }, { .2f, .2f }, 1, 0, "characterBody");
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
			m_debugSprite->CreateQuad({ pos.x,pos.y,11.f }, 0, { scale.x,scale.y,1 }, { 1,0,0,.25f });
			m_debugSprite->CreateQuad({ groundPos.x,groundPos.y,11}, 0, { groundScale.x,groundScale.y,1 }, { 0,1,0,.25f });
		}

		m_quadCollider->WakeBody();
		m_groundCollider->WakeBody();
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
