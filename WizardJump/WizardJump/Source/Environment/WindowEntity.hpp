#pragma once

#include <GEngine.h>



using namespace GEngine;


class LightComponent;

class WindowEntity : public Entity {
public:
	WindowEntity(const Vector3f position, const Vector2f& scale, const Vector4f& color);



private:
	Vector3f m_position;
	Vector2f m_scale;
	Ref<SpriteComponent> m_spriteComponent;
	Ref<LightComponent> m_lightComponent;
	Vector4f m_lightColor;
	long m_lightId = -1;

protected:
	void OnBegin() override;
	void OnEnd() override;
	void OnUpdate(Timestep timestep) override;

};