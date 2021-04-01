#pragma once

#include "GEngine.h"

using namespace GEngine;

class CharacterBody : public Component {
public:
	CharacterBody(Entity* e) : Component(e) {
		go_tag = "Character Body";
		bUpdates = true;
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

	QuadColliderComponent* m_quadCollider;
	QuadColliderComponent* m_groundCollider;
	ColliderID circleColliderID = 0;
	ColliderID quadColliderID = 0;
protected:
	inline void OnBegin() override
	{
		Vector2f pos = Vector2f(0.f, .075f);
		Vector2f scale = Vector2f(.4f, 1.3f);

		Vector2f groundPos = Vector2f(0.f, -.85f);
		Vector2f groundScale = Vector2f(.4f, .1f);

		
		m_quadCollider = m_entity->AddComponent<QuadColliderComponent>(true, true, Vector2f(0.f, 0.f));
		m_quadCollider->b_UpdatePos = false;
		m_groundCollider = m_entity->AddComponent<QuadColliderComponent>(true, false, Vector2f(0.f, 0.f));
		
		quadColliderID = m_quadCollider->CreateQuad(pos, scale, 1, 0, "characterBody");
		circleColliderID = m_quadCollider->CreateCircle({ 0.f,-.65f }, { .2f, .2f }, 1, 0, "characterBody");
		ColliderID id1 = m_groundCollider->CreateQuad(groundPos, groundScale, 1, 0, "characterBodyGround");

		m_groundCollider->SetOnCollideFunction(id1, [this](Ref<PhysicsCollision> other) {
			//GE_CORE_DEBUG("COLLISION: {0}", other->tag);
			if ( other->tag == "ground") {
				groundedCount++;;
			}
			});

		m_groundCollider->SetEndCollideFunction(id1, [this](Ref<PhysicsCollision> other) {
			//GE_CORE_DEBUG("COLLISION End: {0}", other->tag);
			if (other->tag == "ground") {
				groundedCount--;;
			}
			});

		if (debug) {
			m_debugSprite = m_entity->AddComponent<SpriteComponent>();
			m_debugSprite->CreateQuad({ pos.x,pos.y,11.f }, 0, { scale.x,scale.y,1 }, { 1,0,0,.25f });
			m_debugSprite->CreateQuad({ 0, -.65f, 12.f }, 0, { .4f,.4f,1.f }, { 0,0,1,.5f });
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
		if (m_quadCollider) {
			if (m_quadCollider->GetPhysicsBody()) {
				m_entity->SetPosition({ m_quadCollider->GetPhysicsBody()->GetPosition(), m_entity->GetPosition().z });
				m_entity->SetPosition({ 0,0,m_quadCollider->GetPhysicsBody()->GetRotation() });
			}
		}
	}

	

	
	SpriteComponent* m_debugSprite;

};
