#include "gepch.h"
#include "Public/Core/Application/Components/UI/UIComponent.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Renderer/Graphics/Font.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"

#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Util/Utility.h"

#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	Ref<BatchRenderer> UIComponent::s_ShapeFactory = nullptr;


	UIComponent::UIComponent(Entity* e, Ref<Shader> shader) : Component(e)
	{
		go_tag = "UI Component";
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("EngineContent/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = shader != nullptr ? shader : Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::UI, Ref<Quad>(new Quad()), 5000, m_Shader));
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
		m_entity->GetTransform()->AddTransformCallback(GetHash(), [this](Transform* transform, TransformData transData) {
			
			for (ShapeID id : m_ids) {
				Vector3f pos = s_ShapeFactory->GetShapePosition(id);
				Vector3f nPos = pos - transData.GetWorldPosition() + transform->GetWorldPosition();
				if (pos != nPos)
					s_ShapeFactory->SetPosition(id, nPos);
				float rot = s_ShapeFactory->GetShapeRotation(id);
				float nRot = rot - transData.GetWorldRotation().z + transform->GetWorldRotation().z;
				if (rot != nRot)
					s_ShapeFactory->SetRotation(id, nRot);
				Vector2f _scale = s_ShapeFactory->GetShapeScale(id);
				Vector3f scale(_scale.x, _scale.y, 1.f);
				Vector3f nScale = scale - transData.GetWorldScale().z + transform->GetWorldScale().z;
				if (scale != nScale)
					s_ShapeFactory->SetScale(id, { nScale.x, nScale.y });
			}
			
			});
	}



	UIComponent::UIComponent(Entity* e) : Component(e)
	{
		go_tag = "UI Component";
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("EngineContent/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::UI, Ref<Quad>(new Quad()), 5000, m_Shader));
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
		m_entity->GetTransform()->AddTransformCallback(GetHash(), [this](Transform* transform, TransformData transData) {
			
			//	GE_CORE_DEBUG("{0}, {1}, {2}", transData.position.x, transData.position.y, transData.position.z);
			for (ShapeID id : m_ids) {
				Vector3f pos = s_ShapeFactory->GetShapePosition(id);
				Vector3f nPos = pos - transData.GetWorldPosition() + transform->GetWorldPosition();
				if (pos != nPos)
					s_ShapeFactory->SetPosition(id, nPos);
				float rot = s_ShapeFactory->GetShapeRotation(id);
				float nRot = rot - transData.GetWorldRotation().z + transform->GetWorldRotation().z;
				if (rot != nRot)
					s_ShapeFactory->SetRotation(id, nRot);
				Vector2f _scale = s_ShapeFactory->GetShapeScale(id);
				Vector3f scale(_scale.x, _scale.y, 1.f);
				Vector3f nScale = scale - transData.GetWorldScale().z + transform->GetWorldScale().z;
				if (scale != nScale)
					s_ShapeFactory->SetScale(id, { nScale.x, nScale.y });
			}
			});

	}

	UIComponent::~UIComponent()
	{
		m_entity->GetTransform()->RemoveTransformCallback(GetHash());
		ClearQuads();

	}


	static Vector3f GetTextureAspectScale(const Vector3f& scale, const float& textureWidth, const float& textureHeight, const bool aspectRatio = true) {

		Vector3f _scale = scale;
		const Vector3f __scale = scale;
		float tWidth = textureWidth;
		float tHeight = textureHeight;

		float twMax = tWidth > tHeight ? tWidth : tHeight;
		float wMax = Application::GetWidth() > Application::GetHeight() ? Application::GetWidth() : Application::GetHeight();
		float buttonX = ((float)tWidth / twMax) / (wMax != (float)Application::GetWidth() ? (Application::GetWidth() / wMax) : 1.f);
		float buttonY = ((float)tHeight / twMax) / (wMax != (float)Application::GetHeight() ? (Application::GetHeight() / wMax) : 1.f);


		float _s = __scale.x > __scale.y ? __scale.x : __scale.y;
		float _b = __scale.x > __scale.y ? buttonX : buttonY;
		if (aspectRatio) {
			_scale.x = buttonX * (_s / _b);
			_scale.y = buttonY * (_s / _b);
		}

		return _scale;
	}


	static Vector3f GetAspectScale(const Vector3f& aspect, const Vector3f& scale) 
	{

		Vector3f _scale;

		float tWidth = aspect.x;
		float tHeight = aspect.y;

		float twMax = tWidth > tHeight ? tWidth : tHeight;
		float wMax = Application::GetWidth() > Application::GetHeight() ? Application::GetWidth() : Application::GetHeight();
		float buttonX = ((float)tWidth) / (wMax != (float)Application::GetWidth() ? (Application::GetWidth() / wMax) : 1.f);
		float buttonY = ((float)tHeight) / (wMax != (float)Application::GetHeight() ? (Application::GetHeight() / wMax) : 1.f);

		float _s = scale.x > scale.y ? scale.x : scale.y;
		float _b = scale.x > scale.y ? buttonX : buttonY;
		_scale.x = buttonX * (_s / _b);
		_scale.y = buttonY * (_s / _b);
		_scale.z = 1;

		return _scale;
	}

	const ShapeID UIComponent::CreateQuad(const Vector3f& _pos, const float rot /*= 0*/, const Vector3f& scale /*= { 1,1,1 }*/, const Vector4f& _color /*= { 1,1,1,1.f }*/, Ref<Texture2D> texture /*= nullptr*/, bool aspectRatio, const Vector2f& textureScale /*= 1*/, const float alphaChannel)
	{

		Vector3f _scale = scale * m_entity->GetTransform()->GetWorldScale();

		float tWidth = texture ? texture->GetWidth() : 1.f;
		float tHeight = texture ? texture->GetHeight() : 1.f;
		if (aspectRatio)
			_scale = GetTextureAspectScale(_scale, tWidth, tHeight, aspectRatio);
		else {
			_scale = scale * m_entity->GetTransform()->GetWorldScale();
		}

		_scale = GetTextureAspectScale(_scale, tWidth, tHeight, aspectRatio);

		const ShapeID id = s_ShapeFactory->AddShape(_pos+m_entity->GetTransform()->GetWorldPosition(), rot, _scale.xy(), _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}


	const ShapeID UIComponent::CreateSubTexturedQuad(const Vector3f& _pos, const float rot, const Vector3f& scale, const Vector4f& _color, Ref<SubTexture2D> texture, const Vector2f& textureScale /*= 1.f*/, const float alphaChannel)
	{
		const ShapeID id = s_ShapeFactory->AddShape(_pos+m_entity->GetTransform()->GetWorldPosition(), rot, scale.xy(), _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}

	const std::string UIComponent::CreateText(const std::string& string, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color)
	{

		/* Should use Framebuffer size of pipeline */
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();

		char hash[16];
		Utility::GenerateHash(hash, 16);
		while (m_text.find(hash) != m_text.end()) {
			Utility::GenerateHash(hash, 16);
		}

		Ref<StringInfo> data = font->DrawString(string, scale.z / scale.x, width, height);
		std::vector<ShapeID> ids;

		for (CharacterData& d : data->charData) {
			ShapeID id = CreateSubTexturedQuad(m_entity->GetTransform()->GetWorldPosition() + Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, d.texture, { 1,1 }, 1);
			ids.push_back(id);
		}
		m_text[hash] = { data,std::move(ids), font };
		return hash;
	}

	const std::string UIComponent::CreateText_u32(uint32_t* string, int len, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color)
	{
		/* Should use Framebuffer size of pipeline */
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();

		char hash[16];
		Utility::GenerateHash(hash, 16);
		while (m_text.find(hash) != m_text.end()) {
			Utility::GenerateHash(hash, 16);
		}

		Ref<StringInfo> data = font->DrawString_u32(string, len, scale.z / scale.x, width, height);
		std::vector<ShapeID> ids;

		for (CharacterData& d : data->charData) {
			ShapeID id = CreateSubTexturedQuad(m_entity->GetTransform()->GetWorldPosition() + Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, d.texture, { 1,1 }, 1);
			ids.push_back(id);
		}
		m_text[hash] = { data,std::move(ids), font };
		return hash;
	}

	const void UIComponent::AddText(const std::string& id, const std::string& text, const Vector3f& pos, const Vector3f& scale, const Vector4f& color)
	{
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();
		UI_TextInfo& ids = m_text[id];
		int lastSize = ids.info->charData.size();
		Ref<StringInfo> data = ids.font->AppendString(ids.info, text, scale.z / scale.x, width, height);
		ids.info = data;
		int newSize = ids.info->charData.size();
		for (int i = lastSize; i < newSize; i++) {
			const CharacterData& d = ids.info->charData[i];
			ShapeID id = CreateSubTexturedQuad(m_entity->GetTransform()->GetWorldPosition() + Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, d.texture, { 1,1 }, 1);
			ids.shapes.push_back(id);
		}

	}

	void UIComponent::RemoveText(const std::string& id)
	{
		UI_TextInfo& ids = m_text[id];
		for (ShapeID l : ids.shapes) {
			Remove(l);
		}

		ids.shapes.clear();
		
		m_text.erase(id);
	}

	void UIComponent::SetTextColor(const std::string& id, const Vector4f& color, int index, int count /*= 1*/)
	{
		UI_TextInfo& info = m_text[id];
		for (int i = index; i < index + count; i++) {
			SetColor(info.shapes[i], color);
		}
		
	}

	uint32_t UIComponent::GetTextSize(const std::string& id)
	{
		return m_text[id].shapes.size();
	}

	void UIComponent::UnloadGraphics() {
		if (s_ShapeFactory) {
			s_ShapeFactory->UnloadGraphics();
		}
	}
	void UIComponent::ReloadGraphics() {
		if (s_ShapeFactory) {
			s_ShapeFactory->ReloadGraphics();
		}
	}

	void UIComponent::SetPosition(const ShapeID id, const Vector2f& position)
	{
		if (id >= 0) {
			s_ShapeFactory->SetPosition(id, position);
		}
		else {
			GE_CORE_ASSERT(false, "SETUP MOVE TEXT");
		}
	}

    Vector2f UIComponent::GetQuadScale(const ShapeID& id) {
        return s_ShapeFactory->GetShapeScale(id);
    }

	void UIComponent::SetZOrder(ShapeID id, float zOrder)
	{
		if (id > 0) {
			s_ShapeFactory->SetZOrder(id, zOrder + m_entity->GetTransform()->GetWorldPosition().z);
		}
	}

	void UIComponent::SetScale(const ShapeID id, const Vector3f& scale)
	{
		if (id > 0) {
			s_ShapeFactory->SetScale(id, scale);
		}
	}

	void UIComponent::SetColor(const ShapeID id, const Vector4f& color)
	{
		s_ShapeFactory->SetColor(id, color);
	}

	GEngine::Ref<GEngine::Texture2D> UIComponent::GetTexture(const ShapeID id)
	{
		return s_ShapeFactory->GetShapeTexture(id);
	}

	void UIComponent::Remove(ShapeID id)
	{
		if (id > 0) {
			s_ShapeFactory->RemoveShape(id);
			m_ids.erase(std::find(m_ids.begin(), m_ids.end(), id));
		}
	}

	void UIComponent::Remove(const std::string& hash)
	{
		RemoveText(hash);
	}

	void UIComponent::ClearQuads()
	{
		for (ShapeID id : m_ids) {
			s_ShapeFactory->RemoveShape(id);
		}
		
		m_text.clear();
		m_ids.clear();
	
	}

	void UIComponent::OnUpdate(Timestep ts)
	{
		GE_CORE_ERROR("UPDATING?");
	}

	void UIComponent::OnBegin()
	{
		
	}

	void UIComponent::OnEnd()
	{
		
	}

}
