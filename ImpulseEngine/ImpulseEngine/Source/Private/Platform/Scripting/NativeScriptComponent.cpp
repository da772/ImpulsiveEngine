#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"
#include "Public/Core/Application/Entity.h"

namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), m_clazz(clazz)
	{
		go_tag = "Native Script Component";
		ScriptApi::AddNativeScript(this);
		if (clazz.size() > 0) {
			try {
				m_object = ScriptApi::GetReflector_Native()->CreateUClass(clazz, m_entity);
			}
			catch (const std::exception& e) {
				GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
			}
			m_component = (Component*)m_object.data();
			if (m_component) {
				m_isValid = true;
				bUpdates = m_component->GetDoesUpdate();
			}
			else {
				GE_CORE_ERROR("Native Script Component: Failed to Load Class<{0}>", clazz);
			}
		}
	}

	void NativeScriptComponent::LoadClass(const std::string& clazz)
	{
		m_clazz = clazz;
		if (clazz.size() >= 0) {
			try {
				m_object = ScriptApi::GetReflector_Native()->CreateUClass(clazz, m_entity);
			}
			catch (const std::exception& e) {
				GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
			}
			m_component = (Component*)m_object.data();
			if (m_component) {
				m_isValid = true;
				bUpdates = m_component->GetDoesUpdate();
				if (m_hasBegun)
					m_component->Begin();
			}
			else {
				GE_CORE_ERROR("Native Script Component: Failed to Load {0}", clazz);
			}
		}
	}


	std::string NativeScriptComponent::GetClass()
	{
		return m_clazz;
	}

	NativeScriptComponent::~NativeScriptComponent()
	{
		ScriptApi::RemoveNativeScript(this);
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
		if (m_isValid) {
			m_component->Begin();
		}
		m_hasBegun = true;
	}

	void NativeScriptComponent::OnEnd()
	{
        if (m_isValid) {
            m_component->End();
        }
		m_hasBegun = false;
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
        if (m_isValid)
            m_component->Update(timestep);
	}

}
