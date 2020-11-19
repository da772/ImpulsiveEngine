#include "gepch.h"

#include "UI/DialogFrame.hpp"


DialogFrame::DialogFrame(const glm::vec3& position, float textSpeed, const std::string& title,const std::string& icon, const std::string& text) : m_position(position), m_characterPerSecond(textSpeed), m_title(title), m_text(text), m_icon(icon)
{
	bUpdates = true;
	m_font = Font::Create("Content/Fonts/Wizard.ttf", 120.f);

	
}

DialogFrame::~DialogFrame()
{
	m_font = nullptr;
}

void DialogFrame::OnBegin()
{
	m_uiComponent = CreateGameObject<UIComponent>();
	m_spriteAnimComponent = CreateGameObject<SpriteAnimationComponent>();
	m_audioComponent = CreateGameObject<AudioComponent>("Content/Audio/keyPress.ogg", false, false, true, 1.f, 1.f);
	AddComponent(m_spriteAnimComponent);
	AddComponent(m_uiComponent);
	AddComponent(m_audioComponent);


	Ref<Texture2D> dialogTexture = Texture2D::Create("Content/Textures/dialogFrame.png", TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);
	m_dialogId = m_uiComponent->CreateQuad(m_position, 0, { 2.f, 0.f, 1 }, { 1,1,1,1 }, dialogTexture, true);
	dialogTexture = Texture2D::Create(m_icon, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);

	m_iconId = m_uiComponent->CreateQuad({ m_position.x-.745f,m_position.y, m_position.z-1.f }, 0, { .5f, 0.f, 1 }, { 1,1,1,1 }, dialogTexture);

	m_titleId = m_uiComponent->CreateText(m_title, m_font, { m_position.x-.45f, m_position.y + m_uiComponent->GetQuadScale(m_dialogId).y / 5.75f, m_position.z +1 }, { m_titleScale, m_titleScale, 2.f }, { 1,1,1,1 });
	
	m_textId = m_uiComponent->CreateText(m_text, m_font, { m_position.x - .46f, m_position.y, m_position.z + 1 }, { m_textScale, m_textScale, 1.4f }, { 1,1,1,0 });

	m_audioComponent->SetLooping(true);
	m_audioComponent->SetPlaying(true);
	
	m_spriteAnimComponent->SetFrameAnimation(m_characterPerSecond, m_uiComponent->GetTextSize(m_textId)-1, false, [this](int frame) {
	
		
		m_uiComponent->SetTextColor(m_textId, { 1,1,1,1 }, m_textPos, frame - m_textPos);
		m_textPos++;
		
		if ( frame == m_uiComponent->GetTextSize(m_textId) - 1) {
			// play sound
			m_audioComponent->SetLooping(false);
		}	
		
	});
	

}

void DialogFrame::OnEnd()
{
	m_spriteAnimComponent = nullptr;
	m_uiComponent = nullptr;
}

void DialogFrame::OnUpdate(Timestep timestep)
{

}
