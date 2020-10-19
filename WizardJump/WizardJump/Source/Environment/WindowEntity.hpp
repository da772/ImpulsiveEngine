#pragma once

#include <GEngine.h>



using namespace GEngine;


class LightComponent;

class WindowEntity : public Entity {
public:
	WindowEntity(const glm::vec3 position, const glm::vec2& scale, const glm::vec4& color);



private:
	glm::vec3 m_position;
	glm::vec2 m_scale;
	Ref<SpriteComponent> m_spriteComponent;
	Ref<LightComponent> m_lightComponent;
	glm::vec4 m_lightColor;
	long m_lightId = -1;

protected:
	void OnBegin() override;
	void OnEnd() override;
	void OnUpdate(Timestep timestep) override;

};