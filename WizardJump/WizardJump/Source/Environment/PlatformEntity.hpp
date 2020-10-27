#pragma once
#pragma once
#include "GEngine.h"

using namespace GEngine;




class PlatformEntity : public Entity {
public:

	PlatformEntity(const glm::vec2 pos, const glm::vec2 scale = glm::vec2(1, 1), const float zOrder = 5, const std::string& entityTag = "Platform Entity", const float rot = 0);
	~PlatformEntity() {};

	

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;



	glm::vec2 scale;
	glm::vec2 pos;
	float rot;
	float zOrder = 1;

	Ref<SpriteComponent> m_debugSprite;
	Ref<SpriteComponent> m_sprite;

};
