#include "gepch.h"

#include "UI/DialogFrame.hpp"


DialogFrame::DialogFrame(const Vector3f& position, float textSpeed, const std::string& title,const std::string& icon, const Vector4f& color, const std::string& text, bool animateOpen, bool animateClose) : m_position(position), m_characterPerSecond(textSpeed), m_title(title), m_text(text), m_icon(icon), m_color(color)
{
	bUpdates = true;
	m_font = Font::Create("Content/Fonts/Wizard.ttf", 120.f);
	m_font->LoadCharactersEN();
	this->animateOpen = animateOpen;
	this->animateClose = animateClose;
}

DialogFrame::~DialogFrame()
{
	m_font = nullptr;
}

void DialogFrame::SetOnDialogComplete(const std::function<void()>& f)
{
	m_onComplete = f;
}

void DialogFrame::OnBegin()
{

	m_uiComponent = CreateGameObject<UIComponent>();
	m_spriteAnimComponent = CreateGameObject<SpriteAnimationComponent>();
	m_audioComponent = CreateGameObject<AudioComponent>("Content/Audio/keyPress.ogg", false, false, true, 1.f, 1.f);
	
	AddComponent(m_spriteAnimComponent);
	AddComponent(m_uiComponent);
	AddComponent(m_audioComponent);
	m_audioComponent->SetPitch(2.f);


	Ref<Texture2D> dialogTexture = Texture2D::Create("Content/Textures/dialogFrame.png", TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);
	m_dialogId = m_uiComponent->CreateQuad(m_position, 0, { 2.f, 0.f, 1 }, { 1,1,1,1 }, dialogTexture, true);
	dialogTexture = Texture2D::Create(m_icon, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);
	dialogScale = m_uiComponent->GetQuadScale(m_dialogId);
	
	
	m_iconId = m_uiComponent->CreateQuad({ m_position.x-.745f,m_position.y, m_position.z-1.f }, 0, { .5f, 0.f, 1 }, { 1,1,1,1 }, dialogTexture);
	m_backgroundId = m_uiComponent->CreateQuad({ m_position.x - .745f,m_position.y, m_position.z - 2.f }, 0, { .5f, 0.f, 1 }, m_color, nullptr);
	iconScale = m_uiComponent->GetQuadScale(m_iconId);
	

	if (animateOpen) {
		isAnimating = true;
		m_uiComponent->SetColor (m_dialogId, { 1,1,1, 0 });
		m_uiComponent->SetColor(m_iconId, { 1,1,1, 0 });
		m_uiComponent->SetColor(m_backgroundId, { m_color.x,m_color.y,m_color.z, 0 });

		m_spriteAnimComponent->SetFrameAnimation(30, 15, false, [this](int frame) {
			m_uiComponent->SetColor(m_dialogId, { 1,1,1, frame/15.f });
			m_uiComponent->SetColor(m_iconId, { 1,1,1, frame/15.f });
			m_uiComponent->SetColor(m_backgroundId, { m_color.x,m_color.y,m_color.z, m_color.w*(frame / 15.f)});
			//m_uiComponent->SetScale(m_iconId, { (float)frame * iconScale.x / 15.f,(float)frame * iconScale.y / 15.f,1 });
			//m_uiComponent->SetScale(m_dialogId, { (float)frame * dialogScale.x / 15.f,(float)frame * dialogScale.y / 15.f,1 });

			if (frame >= 15) {
				
				m_titleId = m_uiComponent->CreateText(m_title, m_font, { m_position.x - .45f, m_position.y + m_uiComponent->GetQuadScale(m_dialogId).y / 5.75f, m_position.z + 1 }, { m_titleScale, m_titleScale, 2.f }, { 1,1,1,1 });

				m_textId = m_uiComponent->CreateText(m_text, m_font, { m_position.x - .46f, m_position.y, m_position.z + 1 }, { m_textScale, m_textScale, 1.4f }, { 1,1,1,0 });

				m_audioComponent->SetLooping(true);
				m_audioComponent->SetPlaying(true);
				ThreadPool::AddMainThreadFunction([this]() {
					m_isTyping = true;
					isAnimating = false;
					m_spriteAnimComponent->SetFrameAnimation(m_characterPerSecond, m_uiComponent->GetTextSize(m_textId), false, [this](int frame) {
						m_uiComponent->SetTextColor(m_textId, { 1,1,1,1 }, m_textPos, frame - m_textPos);
						m_textPos++;

						if (frame == m_uiComponent->GetTextSize(m_textId)) {
							m_audioComponent->SetLooping(false);
						}

						});
					});
			}
			
			});
	} else {
		m_isTyping = true;
		m_titleId = m_uiComponent->CreateText(m_title, m_font, { m_position.x - .45f, m_position.y + m_uiComponent->GetQuadScale(m_dialogId).y / 5.75f, m_position.z + 1 }, { m_titleScale, m_titleScale, 2.f }, { 1,1,1,1 });

		m_textId = m_uiComponent->CreateText(m_text, m_font, { m_position.x - .46f, m_position.y, m_position.z + 1 }, { m_textScale, m_textScale, 1.4f }, { 1,1,1,0 });

		m_audioComponent->SetLooping(true);
		m_audioComponent->SetPlaying(true);
		m_spriteAnimComponent->SetFrameAnimation(m_characterPerSecond, m_uiComponent->GetTextSize(m_textId), false, [this](int frame) {
			m_uiComponent->SetTextColor(m_textId, { 1,1,1,1 }, m_textPos, frame - m_textPos);
			m_textPos++;
			if (frame == m_uiComponent->GetTextSize(m_textId)) {
				m_audioComponent->SetLooping(false);
			}
			});
	}

	m_buttonComponent = CreateGameObject<ButtonComponent>(m_position, 0.f, Vector3f( dialogScale.x,  dialogScale.y, 1.f ), Vector4f(1, 0, 0, 0.f), Vector2f(0, 0));
	AddComponent(m_buttonComponent);
	m_buttonComponent->SetOnEvent([this](const Event& event) {

		//GE_LOG_DEBUG("Button Press: {0}, Animating: {1}", event.GetName(), isAnimating);
		if (isAnimating) return;
		if (event.GetEventType() == EventType::MouseButtonReleased || event.GetEventType() == EventType::TouchReleased) {
			if (m_textPos == m_uiComponent->GetTextSize(m_textId)) {
				
				if (!m_sticky) {
					if (animateClose) {
						m_uiComponent->RemoveText(m_titleId);
						m_uiComponent->RemoveText(m_textId);
						isAnimating = true;
						m_spriteAnimComponent->SetFrameAnimation(30, 15, false, [this](int frame) {
							m_uiComponent->SetColor(m_dialogId, { 1,1,1, 1.f-(frame / 15.f) });
							m_uiComponent->SetColor(m_iconId, { 1,1,1, 1.f-(frame / 15.f) });
							m_uiComponent->SetColor(m_backgroundId, { m_color.x,m_color.y,m_color.z, m_color.w * (1.f - (frame / 15.f)) });
							//m_uiComponent->SetScale(m_iconId, { iconScale.x - ((float)frame * iconScale.x / 15.f),iconScale.y - ((float)frame * iconScale.y / 15.f),1 });
							//m_uiComponent->SetScale(m_dialogId, { dialogScale.x - ((float)frame * dialogScale.x / 15.f), dialogScale.y - ((float)frame * dialogScale.y / 15.f),1 });

							if (frame >= 15) {
								if (m_onComplete) {
									m_onComplete();
								}
								ThreadPool::AddMainThreadFunction([this]() {
									isAnimating = false;
									Destroy();
								});
							}

						});
					}
					else {
						if (m_onComplete) {
							m_onComplete();
						}
						Destroy();
					}
				}
			}
			else {
				m_spriteAnimComponent->Stop();
				m_audioComponent->SetLooping(false);
				m_uiComponent->SetTextColor(m_textId, { 1,1,1,1 }, m_textPos, m_uiComponent->GetTextSize(m_textId)-m_textPos);
				m_textPos = m_uiComponent->GetTextSize(m_textId) ;
			}
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
