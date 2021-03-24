#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"
#include "Public/Core/Application/Entity.h"

namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), m_clazz(clazz)
	{
		m_entity->AddHash(Factory::NextHash());
		m_object = ScriptApi::GetReflector_Native()->CreateUClass(clazz, m_entity);
		m_component = (Component*)m_object.data();
        if (m_component) {
            m_isValid = true;
			bUpdates = m_component->GetDoesUpdate();
		}
		else {
			GE_CORE_ERROR("Native Script Component: Failed to Load {0}", clazz);
		}
		
	}

	NativeScriptComponent::~NativeScriptComponent()
	{
		ScriptApi::GetReflector_Native()->DestroyUClass(m_object);
	}

	void NativeScriptComponent::UnloadGraphics()
	{
        if (m_isValid)
            m_component->UnloadGraphics();
	}

	void NativeScriptComponent::ReloadGraphics()
	{
        if (m_isValid)
            m_component->ReloadGraphics();
	}

	void NativeScriptComponent::OnBegin()
	{
        if (m_isValid)
            m_component->Begin();
	}

	void NativeScriptComponent::OnEnd()
	{
        if (m_isValid) {
            m_component->End();
        }
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
        if (m_isValid)
            m_component->Update(timestep);
	}

}
