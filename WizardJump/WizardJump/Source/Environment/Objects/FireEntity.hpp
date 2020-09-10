#include "GEngine.h"

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

	long id = 0;

	inline void OnBegin() override
	{
		SetEntityScale({ 1,2,1 });
		SetEntityPosition({ 1.5,1.5,1 });
		spriteComp = CreateGameObject<SpriteComponent>();
		animComp = CreateGameObject<SpriteAnimationComponent>();
		audioComp = CreateGameObject<AudioComponent>("Content/Audio/fireSound.ogg", true, true, false, .2f);
		AddComponent(spriteComp);
		AddComponent(animComp);
		AddComponent(audioComp);


		spriteSheet = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/fireAnim.png"),
			{ 3,0 }, { 64,128 }, { 1,1 });
		id = spriteComp->CreateSubTexturedQuad({ 0,0,10 }, 0.f, { 2,2,1 }, { 1,1,1,1 }, spriteSheet);

		
		animComp->SetFrameAnimation(16, 8, true, [this](int frame) {
			int p1 = (frame-1) % 8;
			//int p2 = 3-floor(frame / 8);
			spriteSheet->SetCoords({ p1, 0 }, { 64,128 });
			spriteComp->SetSubTexture(id, spriteSheet);
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