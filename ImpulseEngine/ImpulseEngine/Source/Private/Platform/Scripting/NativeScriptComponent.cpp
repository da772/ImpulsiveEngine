#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"
#include "Public/Core/Application/Entity.h"

namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), m_clazz(clazz)
	{
		go_tag = "Native Script Component";
		ScriptApi::AddNativeScript(this);
		CreateScript();
	}

	void NativeScriptComponent::LoadClass(const std::string& clazz)
	{
		m_clazz = clazz;
		CreateScript();
	}


	const GEngine::ObjectHash& NativeScriptComponent::GetClassHash()
	{
		if (m_isValid) {
			return m_component->GetHash();
		}

		return {};
	}

	GEngine::Component* NativeScriptComponent::GetComponent() const
	{
		if (m_isValid)
			return m_component;
		return nullptr;
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
		try {
			if (m_isValid)
				m_component->ReloadGraphics();
		}
		catch (const std::exception& e) {
			GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
		}
        
	}


	void NativeScriptComponent::OnBegin()
	{
		try {
			if (m_isValid)
				m_component->Begin();
		}
		catch (const std::exception& e) {
			GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
		}
		m_hasBegun = true;
	}

	void NativeScriptComponent::OnEnd()
	{
		try {
			if (m_isValid)
				m_component->End();
		}
		catch (const std::exception& e) {
			GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
		}
            
		m_hasBegun = false;
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
		try {
			if (m_isValid)
				m_component->Update(timestep);
		}
		catch (const std::exception& e) {
			GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
		}
	}

	void NativeScriptComponent::CreateScript()
	{
		if (m_clazz.size() > 0) {
			try {
				m_object = ScriptApi::GetReflector_Native()->CreateUClass(m_clazz, m_entity);
				m_component = (Component*)m_object.data();
				if (m_component) {
					m_isValid = true;
					bUpdates = m_component->GetDoesUpdate();

					ScriptApi::SetNativeScriptPtrs((void*)m_component, (void*)&m_object);

				}
				else {
					GE_CORE_ERROR("Native Script Component: Failed to Load Class<{0}>", m_clazz);
				}
			}
			catch (const std::exception& e) {
				GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
			}
		}
	}

}
