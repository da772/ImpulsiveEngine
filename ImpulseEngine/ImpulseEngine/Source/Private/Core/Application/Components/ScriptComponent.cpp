
#include "gepch.h"

#include "Public/Core/Application/Components/ScriptComponent.h"
#include "Public/Core/Scripting/ScriptManager.h"
#include "Public/Core/Scripting/ScriptApi.h"
#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/Scripting/ScriptableComponent.h"
#include "Public/Core/FileSystem/FileSystem.h"
#include "Public/Core/Util/Factory.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Application/Components/UI/UIComponent.h"


namespace GEngine {

	ScriptComponent::ScriptComponent(const char* script) : m_filePath(script)
	{
		this->bUpdates = true;
		Ref<FileData> fd = FileSystem::FileDataFromPath(script);
		m_src = std::string((char*)fd->GetDataAsString());

		m_api = ScriptApi::Create();

		//ScriptManager::GetApi()->CheckScript(m_src);
		m_obj = m_api->CreateObject(m_src, m_filePath);

		if (m_obj == nullptr) {
			GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(m_filePath));
			return;
		}
		GE_CORE_ASSERT(m_obj->isClass<ScriptableComponent>(), "JAVASCRIPT COMPONENT IS NOT SCRIPT COMPONENT CLASS");
		m_scriptableComponent = m_obj->asClass<ScriptableComponent>();
		//m_update = m_obj->GetProp("OnUpdate");
		m_obj->SetProperty<bool>("shouldDestroy", true);
		
		//if (m_update == nullptr || m_update->GetType() != FUNCTION) {
		//	m_scriptableComponent->SetDoesUpdate(false);
		//}

		//m_begin = m_obj->GetProp("OnBegin");
		//m_end = m_obj->GetProp("OnEnd");


	}

	ScriptComponent::~ScriptComponent()
	{
		if (m_scriptableComponent != nullptr) {
			std::unordered_map<std::string, Ref<ScriptObject>> m = m_obj->asMap();
			for (std::pair<std::string, Ref<ScriptObject>> key : m) {
				if (key.second->GetType() == Script::BUFFER)
					m_obj->RemoveProperty(key.first.c_str());
			}
			delete m_scriptableComponent;
			
		}
		m_obj = nullptr;;
		m_scriptableComponent = nullptr;
		m_api = nullptr;
	}

	void ScriptComponent::SetEntity(Weak<Entity> e)
	{
		Component::SetEntity(e);
		if (m_scriptableComponent != nullptr) {
			m_scriptableComponent->SetParent(entity.lock().get());
			m_scriptableComponent->SetParentComponent(std::static_pointer_cast<Component>(self.lock()));
		}


	}

	void ScriptComponent::OnBegin()
	{
		//if (m_begin != nullptr && m_begin->GetType() == FUNCTION) {
			if (m_obj->CallMethod("OnBegin") == nullptr) {
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(m_filePath));
			}
		//}
	}

	void ScriptComponent::OnEnd()
	{
		//if (m_end != nullptr && m_end->GetType() == FUNCTION) {
			if (m_obj->CallMethod("OnEnd") == nullptr) {
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(m_filePath));
			}
		//}
	}


	void ScriptComponent::OnUpdate(Timestep timestep)
	{
		if (m_scriptableComponent->DoesUpdate() && !m_invalidUpdate) {
			if (m_obj->CallMethod<double>("OnUpdate", timestep.GetSeconds()) == nullptr) {
				m_invalidUpdate = true;
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(m_filePath));
			}
		}
	}

}