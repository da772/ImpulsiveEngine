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

#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/Util/Utility.h"

namespace GEngine {

	Ref<BatchRenderer> UIComponent::s_ShapeFactory = nullptr;


	UIComponent::UIComponent(Ref<Shader> shader)
	{
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = shader != nullptr ? shader : Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::UI, Ref<Quad>(new Quad()), 5000, m_Shader));
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
	}


	UIComponent::UIComponent() : Component()
	{
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::UI, Ref<Quad>(new Quad()), 5000, m_Shader));
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
		
	}

	UIComponent::~UIComponent()
	{

		
	}

	const long UIComponent::CreateQuad(const Vector3& _pos, const float rot /*= 0*/, const Vector3& scale /*= { 1,1,1 }*/, const Vector4& _color /*= { 1,1,1,1.f }*/, Ref<Texture2D> texture /*= nullptr*/, const glm::vec2& textureScale /*= 1*/, const float alphaChannel)
	{

		glm::vec3 _scale = scale;;
		int w = GEngine::Application::GetUIResolutionWidth();
		int h = GEngine::Application::GetUIResolutionHeight();
		if (w != 0 && h != 0) {
			
			if (w > h) {
				_scale.y = (w * scale.y) / (float)h;
			}
			else {
				_scale.x = (h * scale.x) / (float)w;
			}
		}

		const long id = s_ShapeFactory->AddShape(_pos+GetEntityPosition(), rot, _scale, _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}

	const long UIComponent::CreateQuadScript(Ref<ScriptVector3> _pos, const float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture)
	{
		return CreateQuad(_pos->GetGlm(), rot, scale->GetGlm(), _color->GetGlm(), texture);
	}


	const long UIComponent::CreateSubTexturedQuad(const Vector3& _pos, const float rot, const Vector3& scale, const Vector4& _color, Ref<SubTexture2D> texture, const glm::vec2& textureScale /*= 1.f*/, const float alphaChannel)
	{
		const long id = s_ShapeFactory->AddShape(_pos+GetEntityPosition(), rot, scale, _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}

	const std::string UIComponent::CreateText(const std::string& string, Ref<Font> font, const Vector3& pos, const Vector3& scale, const Vector4& color)
	{

		/* Should use Framebuffer size of pipeline */
		int width, height;
		width = 1080;//GEngine::Application::GetApp()->GetWindow()->GetWidth();
		height = 1920;//GEngine::Application::GetApp()->GetWindow()->GetHeight();

		char hash[16];
		Utility::GenerateHash(hash, 16);
		while (m_text.find(hash) != m_text.end()) {
			Utility::GenerateHash(hash, 16);
		}
		
		std::vector<CharacterData> data = font->DrawString(string, 2, width, height);
		std::vector<long> ids;
        
		for (CharacterData& d : data) {
			long id = CreateSubTexturedQuad(GetEntityPosition() + glm::vec3(d.position.x*scale.x + pos.x, d.position.y*scale.y + pos.y, pos.z), 0, { d.scale.x*scale.x , d.scale.y*scale.y , 1 }, color, d.texture, { 1,1 }, 1);
			ids.push_back(id);
		}
		m_text[hash] = ids;
		return hash;
	}

	void UIComponent::RemoveText(const std::string& id)
	{
		std::vector<long> ids = m_text[id];
		for (long l : ids) {
			s_ShapeFactory->RemoveShape(l);
			m_ids.erase(std::find(m_ids.begin(), m_ids.end(), l));
		}
		
		m_text.erase(id);
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

	void UIComponent::SetPosition(const long id, const glm::vec2& position)
	{
		if (id >= 0) {
			s_ShapeFactory->SetPosition(id, position);
		}
		else {
			GE_CORE_ASSERT(false, "SETUP MOVE TEXT");
		}
	}

	void UIComponent::SetPositionScript(long id, Ref<ScriptVector2> position)
	{
		SetPosition(id, position->GetGlm());
	}

	void UIComponent::SetZOrder(long id, float zOrder)
	{
		if (id >= 0) {
			s_ShapeFactory->SetZOrder(id, zOrder + entity.lock()->GetEntityPosition().z);
		}
	}

	void UIComponent::SetColor(const long id, const glm::vec4& color)
	{
		s_ShapeFactory->SetColor(id, color);
	}

	void UIComponent::Remove(long id)
	{
		if (id >= 0) {
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
		for (long id : m_ids) {
			s_ShapeFactory->RemoveShape(id);
		}
		for (std::pair<std::string, std::vector<long>> p : m_text) {
			for (long id : p.second) {
				s_ShapeFactory->RemoveShape(id);
			}
		}
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
		for (long id : m_ids) {
			s_ShapeFactory->RemoveShape(id);
		}
		m_ids.clear();
	}

	void UIComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
			//	GE_CORE_DEBUG("{0}, {1}, {2}", transData.position.x, transData.position.y, transData.position.z);
				for (long id : m_ids) {
					Vector3 pos = s_ShapeFactory->GetShapePosition(id);
					Vector3 nPos = pos - transData.position + transform->GetPosition();
					if (pos != nPos)
						s_ShapeFactory->SetPosition(id, nPos);
					float rot = s_ShapeFactory->GetShapeRotation(id);
					float nRot = rot - transData.rotation.z + transform->GetRotation().z;
					if (rot != nRot)
						s_ShapeFactory->SetRotation(id, nRot);
					Vector2 _scale = s_ShapeFactory->GetShapeScale(id);
					Vector3 scale(_scale.x, _scale.y, 1);
					Vector3 nScale = scale - transData.scale.z + transform->GetScale().z;
					if (scale != nScale)
						s_ShapeFactory->SetScale(id, { nScale.x, nScale.y });
				}
			}
			});
	}

	void UIComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

}
