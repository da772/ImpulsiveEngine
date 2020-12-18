#pragma once
#pragma once
#include "GEngine.h"

using namespace GEngine;




class PlatformEntity : public Entity {
public:

	PlatformEntity(const Vector2f pos, const Vector2f scale = Vector2f(1, 1), const float zOrder = 5, const std::string& entityTag = "Platform Entity", const float rot = 0);
	~PlatformEntity() {};

	

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;



	Vector2f scale;
	Vector2f pos;
	float rot;
	float zOrder = 1;

	Ref<SpriteComponent> m_debugSprite;
	Ref<SpriteComponent> m_sprite;

};
