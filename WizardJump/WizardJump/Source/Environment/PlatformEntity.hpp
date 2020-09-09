#pragma once
#pragma once
#include "GEngine.h"

using namespace GEngine;


enum class EPlatformType {

	MIDDLE = 0,
	MIDDLE_LEFT,
	MIDDLE_RIGHT,
	TOP,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM,
	BOTTOM_LEFT,
	BOTTOM_RIGHT

};



class PlatformEntity : public Entity {
public:

	inline PlatformEntity(const glm::vec2 pos, const glm::vec2 scale = glm::vec2(1,1), const float rot = 0) : pos(pos), scale(scale), rot(rot) {};
	~PlatformEntity() {};

	bool debug = false;

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	const EPlatformType GetPlatformType(int x, int y);


	glm::vec2 scale;
	glm::vec2 pos;
	float rot;
	int rows = 0;
	int columns = 0;
	float rowSize = .5f, columnSize = .5f;

	Ref<SpriteComponent> m_debugSprite;
	Ref<SpriteComponent> m_sprite;
	Ref<QuadColliderComponent> m_colliderWall;
	Ref<QuadColliderComponent> m_colliderFloor;

};
