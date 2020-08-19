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

	bool debug = true;


protected:
	void OnBegin() override
	{
		glm::vec2 pos = glm::vec2(0, -.05);
		glm::vec2 scale = glm::vec2(.4f, 1.65f);
		m_quadCollider = CreateGameObject<QuadColliderComponent>(true, true, pos, scale, 0.f, 1.f);
		GetEntity()->AddComponent(m_quadCollider);
		if (debug) {
			m_debugSprite = CreateGameObject<SpriteComponent>();
			GetEntity()->AddComponent(m_debugSprite);
			m_debugSprite->CreateQuad({ pos.x,pos.y,1 }, 0, { scale.x,scale.y,1 }, { 1,0,0,.25f });
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
	Ref<SpriteComponent> m_debugSprite;

};
