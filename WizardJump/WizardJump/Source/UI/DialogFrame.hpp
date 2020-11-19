#pragma once

#include <GEngine.h>


using namespace GEngine;


class DialogFrame : public Entity {
public:
	DialogFrame(const glm::vec3& position, float textSpeed, const std::string& title, const std::string& icon, const std::string& text);
	~DialogFrame();

protected:
	glm::vec3 m_position;
	std::string m_text;
	std::string m_title;
	std::string m_icon;
	float m_textScale = .35f;
	float m_titleScale = .45f;
	Ref<UIComponent> m_uiComponent;
	Ref<AudioComponent> m_audioComponent;
	Ref<SpriteAnimationComponent> m_spriteAnimComponent;
	float m_characterPerSecond = 20.f;
	Ref<Font> m_font;
	int m_textPos = 0;

	ShapeID m_dialogId, m_iconId;

	

	std::string m_titleId, m_textId = "";


	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnUpdate(Timestep timestep) override;

};
