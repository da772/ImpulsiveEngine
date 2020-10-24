#pragma once

#include "GEngine.h"

using namespace GEngine;

struct FParalaxBackground {

	ShapeID id;
	Weak<Texture2D> texture;
	float speed;
	glm::vec2 scale;
	float zOrder;
	glm::vec2 offset = glm::vec2(0, 0);
	glm::vec2 pos = glm::vec2(0, 0);
	bool bInit = false;
};


class BackgroundEntity : public Entity {

public:
	BackgroundEntity() { bUpdates = true; };
	~BackgroundEntity() {};

	void AddParalaxBackground(const std::string& name, Ref<Texture2D> texture, const glm::vec2& scale, float speed, float zOrder, const glm::vec2& offset = glm::vec2(0,0));
	void RemoveParalaxBackground(const std::string& name);

protected:

	std::unordered_map < std::string, FParalaxBackground> m_backgrounds;
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_backgroundSprite;
	Camera* m_camera;

};