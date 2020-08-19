#pragma once

#include "GEngine.h"


using namespace GEngine;

class CharacterGraphics : public SpriteComponent {

public:
	CharacterGraphics() {};
	~CharacterGraphics() {};

protected:
	void OnBegin() override
	{
		SpriteComponent::OnBegin();
		m_characterSpriteSheet = SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/wizard.png"),
			{ 0,1 }, { 74,74 }, { 1,1 });
		CreateSubTexturedQuad({ 0,0,0 }, 0.f, { 2,2,1 }, { 1,1,1,1 }, m_characterSpriteSheet, 1.f);
		
	}


	void OnEnd() override
	{
		SpriteComponent::OnEnd();
		m_characterSpriteSheet = nullptr;
		m_animationComp = nullptr;
	}


	Ref<SpriteAnimationComponent> m_animationComp;
	Ref<SubTexture2D> m_characterSpriteSheet;
};
