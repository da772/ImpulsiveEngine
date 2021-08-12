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

	void NativeScriptComponent::LoadClass(const std::string& clazz, const ObjectHash& hash)
	{
		m_clazz = clazz;
		if (hash.isValid()) m_entity->AddHash(hash);
		if (!CreateScript()) {
			m_entity->PopHash();
		}
	}


	const GEngine::ObjectHash NativeScriptComponent::GetClassHash()
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

	std::string NativeScriptComponent::Serialize(int indent /*= 0*/)
	{
		std::string res = "";

		auto& p = ScriptApi::GetReflector_Native()->GetStorage()->get_map().at(m_clazz).property_map;
		res += SerializeIndent(indent) + "<class id=\""+GetClassHash().ToString()+"\">" + m_clazz + "</class>";
		for (auto& d : p) {
			switch (d.second.type) {

			case refl::store::uproperty_type::NONE:
				break;
			case refl::store::uproperty_type::uclass:
				if (d.second.type_name == "std::string") {
					res += "\n" + SerializeIndent(indent) + "<string name=\"" + d.second.name + "\">" + GetNativeObject()->GetMember<std::string>(d.second.name) + "</string>";
				}
				break;
			case refl::store::uproperty_type::uclass_ptr:
			{
				std::string typeN = d.second.type_name.substr(0, d.second.type_name.size() - 1);
				std::vector<std::string> types = { typeN };
				const auto& map = ScriptApi::GetReflector_Native()->GetStorage()->get_map();
				const auto& it = map.find(typeN);

				if (it != map.end()) {
					const auto& parents = map.at(typeN).parent_list;
					types.insert(types.end(), parents.begin(), parents.end());
				}

				std::vector<std::vector<std::string>> findTypes = { {"GEngine::Component*", "Component*", "Component", "GEngine::Component" }, {"GEngine::Entity*", "Entity*", "Entity", "GEngine::Entity"}, {"GEngine::NativeScript*", "NativeScript*", "GEngine::NativeScript", "NativeScript"} };
				std::string ftype = "";

				for (int i = 0; i < findTypes.size(); i++) {
					for (int j = 0; j < findTypes[i].size(); j++) {
						const auto& it = std::find(types.begin(), types.end(), findTypes[i][j]);
						if (it != types.end()) {
							ftype = *it;
							break;
						}
					}
					if (ftype.size() > 0) {
						break;
					}
				}

				if (ftype.size() > 0) {
					GameObject* go = GetNativeObject()->GetMember<GameObject*>(d.second.name);

					if (go) {
						res += "\n" + SerializeIndent(indent) + "<GameObject name=\"" + d.second.name + "\" component=\""+(go->IsComponent() ? "1" : "0")+"\">" + go->GetHash().ToString() + "</GameObject>";
					}
					else {
						res += "\n" + SerializeIndent(indent) + "<GameObject name=\"" + d.second.name + "\" component=\"" + ("0") + "\">nullptr</GameObject>";
					}
				}

				break;
				}
			case refl::store::uproperty_type::_void:
				break;
			case refl::store::uproperty_type::_bool:
				res += "\n"+SerializeIndent(indent) + "<bool name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<bool>(d.second.name)) + "</bool>";
				break;
			case refl::store::uproperty_type::_char:
				res += "\n"+SerializeIndent(indent) + "<char name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<char>(d.second.name)) + "</char>";
				break;
			case refl::store::uproperty_type::_uchar:
				res += "\n"+SerializeIndent(indent) + "<uchar name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<unsigned char>(d.second.name)) + "</uchar>";
				break;
			case refl::store::uproperty_type::_ptr:
				break;
			case refl::store::uproperty_type::_int:
				res += "\n"+SerializeIndent(indent) + "<int name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<int>(d.second.name)) + "</int>";
				break;
			case refl::store::uproperty_type::_int8_t:
				res += "\n"+SerializeIndent(indent) + "<int8 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<int8_t>(d.second.name)) + "</int8>";
				break;
			case refl::store::uproperty_type::_int16_t:
				res += "\n"+SerializeIndent(indent) + "<int16 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<int16_t>(d.second.name)) + "</int16>";
				break;
			case refl::store::uproperty_type::_int32_t:
				res += "\n"+SerializeIndent(indent) + "<int32 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<int32_t>(d.second.name)) + "</int32>";
				break;
			case refl::store::uproperty_type::_int64_t:
				res += "\n"+SerializeIndent(indent) + "<int64 name=\"" + d.second.name + "\">"+std::to_string(GetNativeObject()->GetMember<int64_t>(d.second.name))+ "</int64>";
				break;
			case refl::store::uproperty_type::_uint:
				res += "\n"+SerializeIndent(indent) + "<uint name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<unsigned int>(d.second.name)) + "</uint>";
				break;
			case refl::store::uproperty_type::_uint8_t:
				res += "\n"+SerializeIndent(indent) + "<uint8 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<uint8_t>(d.second.name)) + "</uint8>";
				break;
			case refl::store::uproperty_type::_uint16_t:
				res += "\n"+SerializeIndent(indent) + "<uint16 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<uint16_t>(d.second.name)) + "</uint16>";
				break;
			case refl::store::uproperty_type::_uint32_t:
				res += "\n"+SerializeIndent(indent) + "<uint32 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<uint32_t>(d.second.name)) + "</uint32>";
				break;
			case refl::store::uproperty_type::_uint64_t:
				res += "\n"+SerializeIndent(indent) + "<uint64 name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<uint64_t>(d.second.name)) + "</uint64>";
				break;
			case refl::store::uproperty_type::_float:
				res += "\n"+SerializeIndent(indent) + "<float name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<float>(d.second.name)) + "</float>";
				break;
			case refl::store::uproperty_type::_double:
				res += "\n"+SerializeIndent(indent) + "<double name=\"" + d.second.name + "\">" + std::to_string(GetNativeObject()->GetMember<double>(d.second.name)) + "</double>";
				break;
			case refl::store::uproperty_type::constructor:
				break;
			case refl::store::uproperty_type::uclass_ref:
				break;
			default:
				break;

			}
			
		}

		

		return res;
	}

	std::string NativeScriptComponent::GetClass()
	{
		return m_clazz;
	}

	void NativeScriptComponent::SetNativePointerData()
	{
		if (m_component && &m_object)
			ScriptApi::SetNativeScriptPtrs((void*)m_component, (void*)&m_object);
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

	bool NativeScriptComponent::CreateScript()
	{
		if (m_clazz.size() > 0) {
			try {
				m_object = ScriptApi::GetReflector_Native()->CreateUClass(m_clazz, m_entity);
				m_component = (Component*)m_object.data();
				if (m_component) {
					m_isValid = true;
					bUpdates = m_component->GetDoesUpdate();
					if (m_autoNativize)
						ScriptApi::SetNativeScriptPtrs((void*)m_component, (void*)&m_object);
					return true;
				}
				else {
					if (m_object.ptr) {
						delete m_object.ptr;
						m_object.ptr = nullptr;
					}
					m_component = nullptr;
					GE_CORE_ERROR("Native Script Component: Failed to Load Class<{0}>", m_clazz);
					return false;
				}
			}
			catch (const std::exception& e) {
				GE_CORE_ERROR("Native Script {0}: {1}", m_clazz, e.what());
				return false;
			}
		}
	}

}
