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

namespace GEngine {

	Ref<BatchRenderer> UIComponent::s_ShapeFactory = nullptr;


	UIComponent::UIComponent(Ref<Shader> shader)
	{
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = shader != nullptr ? shader : Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer("ui", Ref<Quad>(new Quad()), 5000, m_Shader));
		}
	}


	UIComponent::UIComponent() : Component()
	{
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			UIComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer("ui", Ref<Quad>(new Quad()), 5000, m_Shader));
		}
		
	}

	UIComponent::~UIComponent()
	{

		
	}

	long UIComponent::CreateQuad(Vector3 _pos, float rot /*= 0*/, Vector3 scale /*= { 1,1,1 }*/, Vector4 _color /*= { 1,1,1,1.f }*/, Ref<Texture2D> texture /*= nullptr*/, float textureScale /*= 1*/, float alphaChannel)
	{
		long id = s_ShapeFactory->AddShape(_pos+GetEntityPosition(), rot, scale, _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}

	long UIComponent::CreateQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture)
	{
		return CreateQuad(_pos->GetGlm(), rot, scale->GetGlm(), _color->GetGlm(), texture);
	}

	long UIComponent::CreateSubTexturedQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<SubTexture2D> texture, float textureScale)
	{
		return CreateSubTexturedQuad(_pos->GetGlm(), rot, scale->GetGlm(), _color->GetGlm(), texture, textureScale);
	}

	long UIComponent::CreateSubTexturedQuad(Vector3 _pos, float rot, Vector3 scale, Vector4 _color, Ref<SubTexture2D> texture, float textureScale /*= 1.f*/, float alphaChannel)
	{
		long id = s_ShapeFactory->AddShape(_pos+GetEntityPosition(), rot, scale, _color, texture, textureScale, alphaChannel);
		m_ids.push_back(id);
		return id;
	}

	long UIComponent::CreateText(std::string string, Ref<Font> font, Vector3 pos, Vector3 scale, Vector4 color)
	{

		/* Should use Framebuffer size of pipeline */
		int width, height;
        width  = GEngine::Application::GetApp()->GetWindow()->GetWidth();
        height = GEngine::Application::GetApp()->GetWindow()->GetHeight();


		std::vector<CharacterData> data = font->DrawString(string, 2, width, height);
		std::vector<long> ids;
		for (CharacterData& d : data) {
			long id = CreateSubTexturedQuad(GetEntityPosition()+glm::vec3( d.position.x + pos.x, d.position.y + pos.y, pos.z), 0, { d.scale.x , d.scale.y , 1 }, color, d.texture, 1, 1);
			ids.push_back(id);
		}

		long id = -(++m_textCounter);
		m_text[id] = ids;
		return id;
	}

	long UIComponent::CreateTextScript(std::string string, Ref<Font> font, Ref<ScriptVector3> pos, Ref<ScriptVector3> scale, Ref<ScriptVector4> color)
	{
		return CreateText(string, font, pos->GetGlm(), scale->GetGlm(), color->GetGlm());
	}

	void UIComponent::RemoveText(long id)
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

	void UIComponent::SetPosition(long id, glm::vec2 position)
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
		else {
			std::vector<long> ids = m_text[id];
			for (long l : ids) {
				s_ShapeFactory->SetZOrder(l , zOrder + entity.lock()->GetEntityPosition().z);
			}
		}
	}

	void UIComponent::SetColor(long id, glm::vec4 color)
	{
		s_ShapeFactory->SetColor(id, color);
	}

	void UIComponent::Remove(long id)
	{
		if (id >= 0) {
			s_ShapeFactory->RemoveShape(id);
			m_ids.erase(std::find(m_ids.begin(), m_ids.end(), id));
		}
		else {
			RemoveText(id);
		}
	}

	void UIComponent::ClearQuads()
	{
		for (long id : m_ids) {
			s_ShapeFactory->RemoveShape(id);
		}
		for (std::pair<long, std::vector<long>> p : m_text) {
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
					s_ShapeFactory->SetPosition(id, pos - transData.position + transform->GetPosition());
				}
			}
			});
	}

	void UIComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

	void UIComponent::RemoveQuads(u32 id)
	{

	}

}
