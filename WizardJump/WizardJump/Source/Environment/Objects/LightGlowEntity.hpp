#include "GEngine.h"
#include "Lighting/LightComponent.h"

using namespace GEngine;

class LightGlowEntity : public Entity {
public:
	LightGlowEntity(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::vec2& scale = glm::vec2(1, 1), const glm::vec2& lightScale = glm::vec2(1, 1), const glm::vec4& color = glm::vec4 (1,1,1,1 )) : m_position(position), m_scale(scale), m_lightScale(lightScale), m_color(color)  { m_tag = "Light Glow Entity"; };
	~LightGlowEntity() {};

protected:
	Ref<SpriteAnimationComponent> animComp;
	Ref<LightComponent> lightComp;

	ShapeID id = 0;
	ShapeID id2 = 0;

	glm::vec4 m_color;

	glm::vec3 m_position;
	glm::vec2 m_lightScale;
	glm::vec2 m_scale;

	inline void OnBegin() override
	{

		SetEntityScale({ m_scale.x, m_scale.y, 1 });
		SetEntityPosition(m_position);
		animComp = CreateGameObject<SpriteAnimationComponent>();
		lightComp = CreateGameObject<LightComponent>();
		AddComponent(animComp);

		AddComponent(lightComp);


		id2 = lightComp->AddCircleLight({ 0,0 }, 0, { m_lightScale.x,m_lightScale.y }, {m_color});


		animComp->SetFrameAnimation(8, 4, true, [this](int frame) {
			if (frame % 2 == 0) {
				float op = Random::FloatRange(.1f, .3f);
				lightComp->EditCircleColor(id2, { m_color.x, m_color.y, m_color.z, m_color.w + op / 10.f });
				lightComp->EditCircleSize(id2, { m_lightScale.x + op, m_lightScale.y + op });
			}

			});



	}


	inline void OnEnd() override
	{
	
		animComp = nullptr;


		//throw std::logic_error("The method or operation is not implemented.");
	}


	void OnUpdate(Timestep timestep) override
	{
		//throw std::logic_error("The method or operation is not implemented.");
	}

};
