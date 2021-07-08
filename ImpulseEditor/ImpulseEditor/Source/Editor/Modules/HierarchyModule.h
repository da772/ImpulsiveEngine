#pragma once
#include "EditorModule.h"

namespace Editor {

	class HierarchyModule : public EditorModule {
	public:
		HierarchyModule(GEngine::ObjectHash* selectedGameObject);
		~HierarchyModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;
	private:
		void AddEntity(const std::pair<GEngine::ObjectHash, GEngine::Entity*>& e, std::unordered_map<GEngine::ObjectHash, GEngine::Entity*>& entities, bool* hoveringObject);
		void AcceptPayload(const std::pair<GEngine::ObjectHash, GEngine::Entity*>& e, const GEngine::Vector2f& pos);
		void RightClickPopup(GEngine::Entity* e);

	private:
		bool openPopup = false;

	private:
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		GEngine::ObjectHash* m_selectedObject;
		GEngine::ObjectHash m_rightSelectedObject;

	};

}