#pragma once
#include "EditorModule.h"

namespace Editor {

	struct GameObjectPayload {
		GEngine::Entity* entity = nullptr;
	};

	class HierarchyModule : public EditorModule {
	public:
		HierarchyModule(uint64_t* selectedGameObject);
		~HierarchyModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;


	private:
		void AddEntity(const std::pair<uint64_t, GEngine::Entity*>& e, std::unordered_map<uint64_t, GEngine::Entity*>& entities);
		void AcceptPayload(const std::pair<uint64_t, GEngine::Entity*>& e);
	private:
		std::unordered_map<std::string, GEngine::Ref<GEngine::Texture2D>> m_textures;
		uint64_t* m_selectedObject;
	};

}