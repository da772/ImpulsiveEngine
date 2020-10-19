#include "GEngine.h"
#include "Lighting/LightComponent.h"

using namespace GEngine;

class FireEntity : public Entity {
public:
	FireEntity(const glm::vec3 position, const glm::vec2& scale, const glm::vec2& lightScale = glm::vec2(1,1), bool sound = false) : m_position(position), m_scale(scale), m_lightScale(lightScale), m_bsound(sound) {m_tag = "Fire Entity"; };
	~FireEntity() {};

protected:

	Ref<SpriteComponent> spriteComp;
	Ref<SubTexture2D> spriteSheet;
	Ref<AudioComponent> audioComp;
	Ref<SpriteAnimationComponent> animComp;
	Ref<LightComponent> lightComp;

	long id = 0;
	long id2 = 0;

	bool m_bsound = false;

	glm::vec3 m_position;
	glm::vec2 m_lightScale;
	glm::vec2 m_scale;

	inline void OnBegin() override
	{
		
		SetEntityScale({ m_scale.x, m_scale.y, 1 });
		SetEntityPosition(m_position);
		spriteComp = CreateGameObject<SpriteComponent>();
		animComp = CreateGameObject<SpriteAnimationComponent>();
		if (m_bsound) {
			audioComp = CreateGameObject<AudioComponent>("Content/Audio/fireSound.ogg", true, true, false, .2f);
			AddComponent(audioComp);
		}
		lightComp = CreateGameObject<LightComponent>();
		AddComponent(spriteComp);
		AddComponent(animComp);
		
		AddComponent(lightComp);


		spriteSheet = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/torch-01.png"),
			{ 0,0 }, { 32,32 }, { 1,1 });
		id = spriteComp->CreateSubTexturedQuad({ 0,0,0 }, 0.f, { 1,1,1 }, { 1,1,1,1 }, spriteSheet);
		id2 = lightComp->AddCircleLight({0,0}, 0, { m_lightScale.x,m_lightScale.y }, { 1.f,.5f, 0.f,.5f });

		
		animComp->SetFrameAnimation(8, 4, true, [this](int frame) {
			
			spriteSheet->SetCoords({ frame-1, 0 }, { 32,32});
			spriteComp->SetSubTexture(id, spriteSheet);

			if (frame % 2 == 0) {
				float op = Random::FloatRange(.1f, .3f);
				lightComp->EditCircleColor(id2, { 1.f,.5f, 0.f, .5f+op/10.f });
				lightComp->EditCircleSize(id2, { m_lightScale.x +op, m_lightScale.y +op});
			}

		});
		

		
	}


	inline void OnEnd() override
	{
		spriteSheet = nullptr;
		animComp = nullptr;
		audioComp = nullptr;
		spriteComp = nullptr;
		//throw std::logic_error("The method or operation is not implemented.");
	}


	void OnUpdate(Timestep timestep) override
	{
		//throw std::logic_error("The method or operation is not implemented.");
	}

};
