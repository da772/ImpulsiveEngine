#include "GEngine.h"
#include "Lighting/LightComponent.h"

using namespace GEngine;

class FireEntity : public Entity {
public:
	FireEntity() {};
	~FireEntity() {};

protected:

	Ref<SpriteComponent> spriteComp;
	Ref<SubTexture2D> spriteSheet;
	Ref<AudioComponent> audioComp;
	Ref<SpriteAnimationComponent> animComp;
	Ref<LightComponent> lightComp;

	long id = 0;
	long id2 = 0;

	std::string m_tag = "Fire Entity";

	inline void OnBegin() override
	{
		SetEntityScale({ 1,1,1 });
		SetEntityPosition({ 7.f, 8.4f, 1 });
		spriteComp = CreateGameObject<SpriteComponent>();
		animComp = CreateGameObject<SpriteAnimationComponent>();
		audioComp = CreateGameObject<AudioComponent>("Content/Audio/fireSound.ogg", true, true, false, .2f);
		lightComp = CreateGameObject<LightComponent>();
		AddComponent(spriteComp);
		AddComponent(animComp);
		AddComponent(audioComp);
		AddComponent(lightComp);


		spriteSheet = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/fireAnim.png"),
			{ 3,0 }, { 64,128 }, { 1,1 });
		id = spriteComp->CreateSubTexturedQuad({ 0,0,10 }, 0.f, { 2,4,1 }, { 1,1,1,1 }, spriteSheet);
		id2 = lightComp->AddCircleLight({0,0}, 0, { 4,4 }, { 1.f,.5f, 0.f,.5f });

		
		animComp->SetFrameAnimation(16, 8, true, [this](int frame) {
			int p1 = (frame-1) % 8;
			//int p2 = 3-floor(frame / 8);
			spriteSheet->SetCoords({ p1, 0 }, { 64,128 });
			spriteComp->SetSubTexture(id, spriteSheet);

			if (frame % 2 == 0) {
				float op = Random::IntRange(1, 5);
				GE_CORE_DEBUG("RANDOM N: {0}", (op-.7f)*10.f);
				lightComp->EditCircleColor(id2, { 1.f,.5f, 0.f, op });
				lightComp->EditCircleSize(id2, { 4 + 10.f* (op - .7f), 4.f + 10.f* (op - .7f )});
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
