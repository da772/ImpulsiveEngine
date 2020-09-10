#pragma once
#pragma once
#include "GEngine.h"

using namespace GEngine;





class BackgroundTiledEntity : public Entity {
public:

	inline BackgroundTiledEntity(const glm::vec2 pos, const glm::vec2 scale = glm::vec2(1, 1), const float rot = 0) : pos(pos), scale(scale), rot(rot) {};
	~BackgroundTiledEntity() {};

	bool debug = false;

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	const uint8_t GetPlatformType(int x, int y);


	glm::vec2 scale;
	glm::vec2 pos;
	float rot;
	int rows = 0;
	int columns = 0;
	float rowSize = .5f, columnSize = .5f;

	Ref<SpriteComponent> m_debugSprite;
	Ref<SpriteComponent> m_sprite;
};
