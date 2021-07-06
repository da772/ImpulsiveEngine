#pragma once
#include "EditorModule.h"

namespace Editor {
	
	class ReloadModule;

	class InspectorModule : public EditorModule {

	public:
		InspectorModule(GEngine::ObjectHash* selectedGameObject, ReloadModule* reloadModule);
		~InspectorModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;

		void Inspect_TransformComponent(GEngine::Component* c);
		void Inspect_NativeComponent(GEngine::Component* c);

	private:
		void AddComponentModal();
		void ResizePanel(uint64_t id , bool& isDragging, float& leftWidth, float leftWidthMax, float leftWidthMin);

		

	private:
		GEngine::ObjectHash* m_selectedGameObject = nullptr;
		std::unordered_map < std::string, std::function<void(GEngine::Component*)>> s_ComponentMap;

		float lastX = 0;
		float windowWidth = 1;

		bool isDragAndDrop = false;
		bool isDragging = false;

		bool isDragging_transform = false;
		float leftWidth_transform = .15f;
		float leftWidthMax_transform = .8f;
		float leftWidthMin_transform = .115f;

		ReloadModule* reloadModule;

		std::string nativeScriptComponentClass = "";


	};


}
