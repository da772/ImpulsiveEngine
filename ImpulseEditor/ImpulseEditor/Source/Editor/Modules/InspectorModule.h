#pragma once
#include "EditorModule.h"

namespace Editor {
	
	class InspectorModule : public EditorModule {

	public:
		InspectorModule(uint64_t* selectedGameObject);
		~InspectorModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;

		static void Inspect_TransformComponent(GEngine::Component* c);

	private:
		void AddComponentModal();

		

	private:
		uint64_t* m_selectedGameObject;


	};


}
