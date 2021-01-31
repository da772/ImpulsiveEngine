#pragma once

#include "GEngine.h"

using namespace GEngine;

struct FParalaxBackground {

	ShapeID id;
	Weak<Texture2D> texture;
	float speed;
	Vector2f scale;
	float zOrder;
	Vector2f offset = Vector2f(0, 0);
	Vector2f pos = Vector2f(0, 0);
	bool bInit = false;
};


class BackgroundEntity : public Entity {

public:
	BackgroundEntity() { m_tag = "BackgroundEntity"; bUpdates = true; };
	~BackgroundEntity() {};

	void AddParalaxBackground(const std::string& name, Ref<Texture2D> texture, const Vector2f& scale, float speed, float zOrder, const Vector2f& offset = Vector2f(0,0));
	void RemoveParalaxBackground(const std::string& name);

	const std::unordered_map < std::string, FParalaxBackground>& GetBackgrounds() const { return m_backgrounds; }


protected:

	std::unordered_map < std::string, FParalaxBackground> m_backgrounds;
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_backgroundSprite;
	Camera* m_camera;

};