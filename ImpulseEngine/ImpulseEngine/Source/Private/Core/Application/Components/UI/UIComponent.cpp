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
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"

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
			
			for (const auto& p : m_objects) {
				ShapeID id = p.first;
				if (transform->GetWorldPosition() != transData.GetWorldPosition() && !p.second.text)
					SetPosition(id, m_objects[id].pos.xy());
				if (transform->GetWorldPosition().z != transData.GetWorldPosition().z)
					SetZOrder(id, m_objects[id].pos.z);
				if (transform->GetWorldRotation() != transData.GetWorldRotation() && !p.second.text)
					SetRotation(id, m_objects[id].rot);
				if (transform->GetWorldScale() != transData.GetWorldScale() && !p.second.text)
					SetScale(id, m_objects[id].scale);
			}

			for (const auto& p : m_text) {
				const std::string& id = p.first;
				if (transform->GetWorldPosition() != transData.GetWorldPosition())
					SetPosition(id, m_text[id].pos.xy());
				if (transform->GetWorldPosition().z != transData.GetWorldPosition().z)
					SetZOrder(id, m_text[id].pos.z);
				if (transform->GetWorldScale() != transData.GetWorldScale())
					SetScale(id, m_text[id].scale);
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
			for (const auto& p : m_objects) {
				ShapeID id = p.first;
				if (transform->GetWorldPosition() != transData.GetWorldPosition() && !p.second.text)
					SetPosition(id, m_objects[id].pos.xy());
				if (transform->GetWorldPosition().z != transData.GetWorldPosition().z)
					SetZOrder(id, m_objects[id].pos.z);
				if (transform->GetWorldRotation() != transData.GetWorldRotation() && !p.second.text)
					SetRotation(id, m_objects[id].rot);
				if (transform->GetWorldScale() != transData.GetWorldScale() && !p.second.text)
					SetScale(id, m_objects[id].scale);

			}

			for (const auto& p : m_text) {
				const std::string& id = p.first;
				if (transform->GetWorldPosition() != transData.GetWorldPosition())
					SetPosition(id, m_text[id].pos.xy());
				if (transform->GetWorldPosition().z != transData.GetWorldPosition().z)
					SetZOrder(id, m_text[id].pos.z);
				if (transform->GetWorldScale() != transData.GetWorldScale())
					SetScale(id, m_text[id].scale);
			}
			});

	}

	UIComponent::~UIComponent()
	{
		m_entity->GetTransform()->RemoveTransformCallback(GetHash());
		Clear();
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
		_scale = GetTextureAspectScale(_scale, tWidth, tHeight, aspectRatio);

		const ShapeID id = s_ShapeFactory->AddShape(_pos+m_entity->GetTransform()->GetWorldPosition(), rot+ m_entity->GetTransform()->GetWorldRotation().z, _scale.xy()*m_entity->GetTransform()->GetWorldScale().xy(), _color, texture, textureScale, alphaChannel);
		m_objects[id] = { _pos, rot, scale, _color, texture, nullptr, textureScale, alphaChannel, aspectRatio};
		return id;
	}


	const ShapeID UIComponent::CreateSubTexturedQuad(const Vector3f& _pos, const float rot, const Vector3f& scale, const Vector4f& _color, Ref<SubTexture2D> texture, const Vector2f& textureScale /*= 1.f*/, const float alphaChannel, bool text)
	{
		const ShapeID id = s_ShapeFactory->AddShape(_pos + m_entity->GetTransform()->GetWorldPosition(), rot + m_entity->GetTransform()->GetWorldRotation().z, scale.xy() * m_entity->GetTransform()->GetWorldScale().xy(), _color, texture, textureScale, alphaChannel);
		m_objects[id] = { _pos, rot, scale, _color, nullptr, texture, textureScale, alphaChannel, text};
		return id;
	}

	const std::string UIComponent::CreateText(const std::string& string, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color)
	{

		/* Should use Framebuffer size of pipeline */
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();

		char hash[9];
		hash[8] = '\0';
		Utility::GenerateHash(hash, 8);
		while (m_text.find(hash) != m_text.end()) {
			Utility::GenerateHash(hash, 8);
		}

		Ref<StringInfo> data = font->DrawString(string, scale.z, width, height);
		std::vector<ShapeID> ids;

		Vector3f _scale = scale;
		float tWidth = scale.x;
		float tHeight = scale.y;
		_scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);
		//_scale *= scale.xy();
		for (CharacterData& d : data->charData) {
			ShapeID id = s_ShapeFactory->AddShape(Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z), 0, { d.scale.x * _scale.x , d.scale.y * _scale.y}, color, d.texture, { 1,1 }, 1);
			m_objects[id] = { Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, nullptr, d.texture, { 1,1 }, 1, false, true };
			ids.push_back(id);
		}
		m_text[hash] = { data,std::move(ids), font, pos, scale, std::wstring(string.begin(), string.end()) };
		return hash;
	}

	const std::string UIComponent::CreateText_u32(uint32_t* string, int len, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color)
	{
		/* Should use Framebuffer size of pipeline */
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();

		char hash[9];
		memset(hash, 0, sizeof(char) * 9);
		Utility::GenerateHash(hash, 8);
		while (m_text.find(hash) != m_text.end()) {
			Utility::GenerateHash(hash, 8);
		}

		Ref<StringInfo> data = font->DrawString_u32(string, len, scale.z, width, height);
		std::vector<ShapeID> ids;

		Vector3f _scale = scale;
		float tWidth = scale.x;
		float tHeight = scale.y;
		_scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);

		for (CharacterData& d : data->charData) {
			ShapeID id = s_ShapeFactory->AddShape(Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z), 0, { d.scale.x * _scale.x , d.scale.y * _scale.y }, color, d.texture, { 1,1 }, 1);
			m_objects[id] = { Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, nullptr, d.texture, { 1,1 }, 1, false, true };
			ids.push_back(id);
		}
		m_text[hash] = { data,std::move(ids), font, pos, scale, std::wstring(string, string+len) };
		return hash;
	}

	const void UIComponent::AddText(const std::string& id, const std::string& text, const Vector4f& color)
	{
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();
		UI_TextInfo& ids = m_text[id];
		Vector3f scale = ids.scale;
		Vector3f pos = ids.pos;
		int lastSize = ids.info->charData.size();
		Ref<StringInfo> data = ids.font->AppendString(ids.info, text, scale.z, width, height);
		ids.info = data;
		int newSize = ids.info->charData.size();
		ids.str += std::wstring(text.begin(), text.end());

		Vector3f _scale = scale;
		float tWidth = scale.x;
		float tHeight = scale.y;
		_scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);

		for (int i = lastSize; i < newSize; i++) {
			const CharacterData& d = ids.info->charData[i];
			ShapeID id = s_ShapeFactory->AddShape(Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z), 0, { d.scale.x * _scale.x , d.scale.y * _scale.y }, color, d.texture, { 1,1 }, 1);
			m_objects[id] = { Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, nullptr, d.texture, { 1,1 }, 1, false, true };
			ids.shapes.push_back(id);
		}

	}

	void UIComponent::SetText(const std::string& s, const std::string& text, const Vector4f& color)
	{
		UI_TextInfo& ids = m_text[s];
		for (ShapeID l : ids.shapes) {
			Remove(l);
		}
		ids.shapes.clear();
		int width, height;
		width = GEngine::Application::GetApp()->GetUIResolutionWidth();
		height = GEngine::Application::GetApp()->GetUIResolutionHeight();

		ids.str = std::wstring(text.begin(), text.end());
		
		Ref<StringInfo> data = ids.font->DrawString(text, ids.scale.z, width, height);
		Vector3f pos = ids.pos;
		Vector3f scale = ids.scale;
		Vector3f _scale = scale;
		float tWidth = scale.x;
		float tHeight = scale.y;
		_scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);
		//_scale *= scale.xy();
		for (CharacterData& d : data->charData) {
			ShapeID id = s_ShapeFactory->AddShape(Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z), 0, { d.scale.x * _scale.x , d.scale.y * _scale.y }, color, d.texture, { 1,1 }, 1);
			m_objects[id] = { Vector3f(d.position.x * scale.x + pos.x, d.position.y * scale.y + pos.y, pos.z), 0, { d.scale.x * scale.x , d.scale.y * scale.y , 1 }, color, nullptr, d.texture, { 1,1 }, 1, false, true };
			ids.shapes.push_back(id);
		}
		ids.info = data;

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

	std::string UIComponent::Serialize(int indent /* = 0 */)
	{
		std::string res = "";
		for (const std::pair<std::string, UI_TextInfo>& p : m_text) {
			res += SerializeIndent(indent) + "<Text id=\"" + p.first + "\">\n";

			res += SerializeIndent(indent) + "<String>" + p.second.font->GetTexture()->GetName() + "</String>\n";
			res += SerializeIndent(indent) + "<String>string" + "</String>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.pos.x)+ "<float>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.pos.y)+ "<float>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.pos.z)+ "<float>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.scale.x)+ "<float>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.scale.y)+ "<float>\n";
			res += SerializeIndent(indent) + "<float>" + std::to_string(p.second.scale.z)+ "<float>\n";

			for (const ShapeID& id : p.second.shapes) {
				//m_objects[id]
			}


			res += SerializeIndent(indent) + "</Text>";
		}
		
		return res;

	}

	void UIComponent::SetPosition(const ShapeID id, const Vector2f& position)
	{
		m_objects[id].pos = { position, m_objects[id].pos.z};
		s_ShapeFactory->SetPosition(id, position + m_entity->GetTransform()->GetWorldPosition().xy());
	}

	void UIComponent::SetPosition(const std::string& id, const Vector2f position)
	{
		UI_TextInfo& ids = m_text[id];

		int newSize = ids.info->charData.size();
		Vector3f scale = ids.scale;
		Vector3f pos = { position.x, position.y, ids.pos.z };
		ids.pos = pos;
		pos += m_entity->GetTransform()->GetWorldPosition().xy();
		float tWidth = scale.x;
		float tHeight = scale.y;
		Vector3f _scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);

		for (int i = 0; i < ids.shapes.size(); i++) {
			const CharacterData& d = ids.info->charData[i];
			Vector3f newPos = Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z);
			SetPosition(ids.shapes[i], newPos);
		}
	}

	void UIComponent::SetScale(const std::string& id, const Vector2f scale)
	{
		UI_TextInfo& ids = m_text[id];

		int newSize = ids.info->charData.size();
		Vector3f pos = ids.pos;
		Vector3f _scale = { scale.x, scale.y, ids.scale.z };
		ids.scale = _scale;
		float tWidth = scale.x;
		float tHeight = scale.y;
		_scale = GetTextureAspectScale({ scale.x, scale.y, 1.f }, tWidth, tHeight, 1);
		for (int i = 0; i < ids.shapes.size(); i++) {
			const CharacterData& d = ids.info->charData[i];
			Vector3f newPos = Vector3f(d.position.x * _scale.x + pos.x, d.position.y * _scale.y + pos.y, pos.z);
			Vector3f newScale = { d.scale.x * _scale.x , d.scale.y * _scale.y, _scale.z };
			SetPosition(ids.shapes[i], newPos);
			SetScale(ids.shapes[i], newScale);
		}
	}
	
	void UIComponent::SetZOrder(const std::string& id, float order)
	{
		UI_TextInfo& ids = m_text[id];
		ids.pos.z = order;
		float o = order + m_entity->GetTransform()->GetWorldPosition().z;
		for (int i = 0; i < ids.shapes.size(); i++) {
			const CharacterData& d = ids.info->charData[i];
			SetZOrder(ids.shapes[i], o);
		}
	}

	void UIComponent::SetZOrder(ShapeID id, float zOrder)
	{
		m_objects[id].pos.z = zOrder;
		s_ShapeFactory->SetZOrder(id, zOrder + m_entity->GetTransform()->GetWorldPosition().z);
	}



	void UIComponent::SetRotation(const ShapeID id, const float rot)
	{
		m_objects[id].rot = rot;
		s_ShapeFactory->SetRotation(id, rot + m_entity->GetTransform()->GetWorldRotation().z);
	}

	void UIComponent::SetScale(const ShapeID id, const Vector3f& scale)
	{
		m_objects[id].scale = scale;
		Vector3f _scale = scale * m_entity->GetTransform()->GetWorldScale();
		if (m_objects[id].aspectRatio) {
			Ref<Texture2D> texture = m_objects[id].texture;
			float tWidth = texture ? texture->GetWidth() : 1.f;
			float tHeight = texture ? texture->GetHeight() : 1.f;
			_scale = GetTextureAspectScale(_scale, tWidth, tHeight, m_objects[id].aspectRatio);
		}
		s_ShapeFactory->SetScale(id, _scale);
	}

	void UIComponent::SetColor(const ShapeID id, const Vector4f& color)
	{
		m_objects[id].color = color;
		s_ShapeFactory->SetColor(id, color);
	}

	void UIComponent::SetTexture(const ShapeID id, const Ref<Texture2D> texture)
	{
		m_objects[id].texture = texture;
		m_objects[id].subtexture = nullptr;
		s_ShapeFactory->SetTexture(id, texture);
	}

	GEngine::Vector2f UIComponent::GetLocalScale(const ShapeID& id)
	{
		return m_objects[id].scale;
	}

	GEngine::Vector3f UIComponent::GetLocalPosition(const ShapeID& id)
	{
		return m_objects[id].pos;
	}

	float UIComponent::GetLocalRotation(const ShapeID& id)
	{
		return m_objects[id].rot;
	}

	GEngine::Vector2f UIComponent::GetWorldScale(const ShapeID& id)
	{
		return m_objects[id].scale * m_entity->GetTransform()->GetWorldScale();
	}

	GEngine::Vector3f UIComponent::GetWorldPosition(const ShapeID& id)
	{
		return m_objects[id].pos + m_entity->GetTransform()->GetWorldPosition();
	}

	float UIComponent::GetWorldRotation(const ShapeID& id)
	{
		return m_objects[id].rot + m_entity->GetTransform()->GetWorldRotation().z;
	}

	GEngine::Vector4f UIComponent::GetColor(const ShapeID& id)
	{
		return m_objects[id].color;
	}

	GEngine::Ref<GEngine::Texture2D> UIComponent::GetTexture(const ShapeID& id)
	{
		return m_objects[id].texture;
	}

	GEngine::Ref<GEngine::SubTexture2D> UIComponent::GetSubTexture(const ShapeID& id)
	{
		return m_objects[id].subtexture;
	}

	GEngine::Vector2f UIComponent::GetTextureScale(const ShapeID& id)
	{
		return m_objects[id].textureScale;
	}

	const GEngine::UIObject& UIComponent::GetObject(const ShapeID& id)
	{
		return m_objects[id];
	}

	const GEngine::UI_TextInfo& UIComponent::GetText(const std::string& id)
	{
		return m_text[id];
	}

	const std::unordered_map<GEngine::ShapeID, GEngine::UIObject>& UIComponent::GetObjects() const
	{
		return m_objects;
	}

	const std::unordered_map<std::string, GEngine::UI_TextInfo>& UIComponent::GetTexts() const
	{
		return m_text;
	}

	GEngine::Ref<GEngine::BatchRenderer> UIComponent::GetBatchRenderer() const
	{
		return s_ShapeFactory;
	}

	float UIComponent::GetTextWidth(const std::string& id)
	{
		return 0;
	}

	float UIComponent::GetTextHeight(const std::string& id)
	{
		return 0;
	}

	void UIComponent::Remove(ShapeID id)
	{	
		s_ShapeFactory->RemoveShape(id);
		m_objects.erase(id);	
	}

	void UIComponent::Remove(const std::string& hash)
	{
		RemoveText(hash);
	}

	void UIComponent::Clear()
	{
		for (const auto& p : m_objects) {
			s_ShapeFactory->RemoveShape(p.first);
		}

		m_text.clear();
		m_objects.clear();
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
