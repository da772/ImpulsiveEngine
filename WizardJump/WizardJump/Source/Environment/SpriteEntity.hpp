#pragma once

#include <GEngine.h>


using namespace GEngine;

class SpriteEntity : public Entity {

public:
	SpriteEntity(const glm::vec3& position, const glm::vec2& scale, const float rotation, const std::string& tag = "SpriteEntity") : m_position(position), m_scale(scale), m_rot(rotation) {
		
		//bUpdates = false;
		m_tag = tag;
	
	}
	~SpriteEntity() {};

	inline Ref<SpriteComponent> GetSpriteComponent() const { return m_spriteComponent; };
	

protected:
	Ref<SpriteComponent> m_spriteComponent;
	glm::vec3 m_position;
	glm::vec2 m_scale;
	float m_rot;

	inline void OnBegin() override
	{
		SetEntityPosition({ 0,0,0 });
		SetEntityScale({ m_scale.x, m_scale.y, 1 });
		m_spriteComponent = CreateGameObject<SpriteComponent>();
		AddComponent(m_spriteComponent);
	}


	inline void OnEnd() override
	{
		m_spriteComponent = nullptr;
	}


	void OnUpdate(Timestep timestep) override
	{
		
	}

};
