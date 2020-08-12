#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class ScriptObject;
	class ScriptableComponent;
	class ScriptApi;

	class ScriptComponent : public Component {
	public:
		ScriptComponent(const char* script);
		virtual ~ScriptComponent();
		virtual void SetEntity(Weak<Entity>  e) override;
		void SetScriptProperty(std::string name, Ref<ScriptObject> value);
		

	protected:
		void OnBegin() override;

		void OnEnd() override;

		void OnUpdate(Timestep timestep) override;


	private:
		std::string m_filePath;
		std::string m_src;
		Ref<ScriptObject> m_obj = nullptr;
		Ref<ScriptApi> m_api = nullptr;
		ScriptableComponent* m_scriptableComponent = nullptr;
		bool m_invalidUpdate = false;

	};



}