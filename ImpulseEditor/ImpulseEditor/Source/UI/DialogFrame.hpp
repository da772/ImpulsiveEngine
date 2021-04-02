#pragma once

#include <GEngine.h>


using namespace GEngine;


class DialogFrame : public Entity {
public:
	DialogFrame(const uint32_t& id, const Vector3f& position, float textSpeed, const std::string& title, const std::string& icon, const Vector4f& color, const std::string& text, bool animateOpen = true, bool animateClose = true);
	~DialogFrame();

	void SetOnDialogComplete(const std::function<void()>& f );
	inline void SetStickyFrame(bool b) { m_sticky = b; }

protected:
	Vector3f m_position;
	std::string m_text;
	std::string m_title;
	std::string m_icon;
	float m_textScale = .35f;
	float m_titleScale = .45f;
	bool animateOpen = true;
	bool animateClose = true;
	Vector4f m_color;
	UIComponent* m_uiComponent;
	AudioComponent* m_audioComponent;
	SpriteAnimationComponent* m_spriteAnimComponent;
	ButtonComponent* m_buttonComponent;
	float m_characterPerSecond = 20.f;
	bool m_sticky = false;
	bool isAnimating = false;
	Ref<Font> m_font;
	int m_textPos = 0;
	int maxChars = 180;

	Vector2f dialogScale;
	Vector2f iconScale;

	std::function<void()> m_onComplete = nullptr;

	bool m_isTyping = false;

	ShapeID m_dialogId, m_iconId, m_backgroundId;


	

	std::string m_titleId, m_textId = "";

public:
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnUpdate(Timestep timestep) override;

};