#pragma once

#include "GEngine.h"

using namespace GEngine;

class CharacterBody : public Component {
public:
	CharacterBody() {

	};
	~CharacterBody() {};

	void AddVelocity(glm::vec2 v) {
		m_quadCollider->IncreaseLinearVelocity(v.x, v.y);
	}

	glm::vec2 GetVelocity() {
		return m_quadCollider->GetLinearVelocity();
	}

	bool debug = true;
	int groundedCount = 0;

	bool isGrounded() { return groundedCount > 0; };


protected:
	void OnBegin() override
	{
		glm::vec2 pos = glm::vec2(0, -.05);
		glm::vec2 scale = glm::vec2(.4f, 1.65f);

		glm::vec2 groundPos = glm::vec2(0, -1.f);
		glm::vec2 groundScale = glm::vec2(.4f, .3f);
		m_quadCollider = CreateGameObject<QuadColliderComponent>(true, true, pos, scale, 0.f, 1.f);
		m_groundCollider = CreateGameObject<QuadColliderComponent>(true, false, groundPos, groundScale, 0.f, 1.f);

		m_groundCollider->SetOnCollideFunction([this](Ref<QuadColliderComponent> other) {
			GE_CORE_DEBUG("COLLISION: {0}", other->GetTag());
			if (other->GetTag() == "ground") {
				groundedCount++;;
			}
			});

		m_groundCollider->SetEndCollideFunction([this](Ref<QuadColliderComponent> other) {
			GE_CORE_DEBUG("COLLISION End: {0}", other->GetTag());
			if (other->GetTag() == "ground") {
				groundedCount--;;
			}
			});
		
		GetEntity()->AddComponent(m_quadCollider);
		GetEntity()->AddComponent(m_groundCollider);



		if (debug) {
			m_debugSprite = CreateGameObject<SpriteComponent>();
			GetEntity()->AddComponent(m_debugSprite);
			m_debugSprite->CreateQuad({ pos.x,pos.y,1 }, 0, { scale.x,scale.y,1 }, { 1,0,0,.25f });
			m_debugSprite->CreateQuad({ groundPos.x,groundPos.y,1 }, 0, { groundScale.x,groundScale.y,1 }, { 0,1,0,.25f });
		}

	}


	void OnEnd() override
	{
		m_quadCollider = nullptr;
		m_debugSprite = nullptr;
	}


	void OnUpdate(Timestep timestep) override
	{
	
	}

	

	Ref<QuadColliderComponent> m_quadCollider;
	Ref<QuadColliderComponent> m_groundCollider;
	Ref<SpriteComponent> m_debugSprite;

};
