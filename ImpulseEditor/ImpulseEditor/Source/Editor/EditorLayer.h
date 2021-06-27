#pragma once

#include <GEngine.h>
#include "Project/ProjectData.h"
#include "Modules/EditorModule.h"


namespace Editor {

	struct EditorModuleData {
		GEngine::Ref<EditorModule> data;
		bool isOpen = true;
		int flags = 0;

		template<typename T = EditorModule>
		inline GEngine::Ref<T> DataAs() {
			return std::dynamic_pointer_cast<T>(data);
		}

	};

	class EditorLayer : public GEngine::Layer {

	public:
		static EditorLayer* Create(Project::ProjectData* data);
		static void Destroy();
		virtual ~EditorLayer();

		void Begin() override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(GEngine::Timestep timeStep) override;
		void OnEvent(GEngine::Event& event) override;
		void OnImGuiRender() override;
		void OnDraw() override;
		void End() override;

		template<typename T = EditorModule, typename ... Args>
		inline GEngine::Ref<T> AddModule(const std::string & id, bool isOpen, int flags, Args&& ... args) {
			std::shared_ptr<T> e = std::make_shared<T>(std::forward<Args>(args)...);
			modules[id] = {e, isOpen, flags};
			return e;
		}

		inline EditorModuleData& GetModule(const std::string& id) {
			return modules[id];
		}


		static EditorLayer* GetEditor();
	private:

		std::unordered_map<std::string, EditorModuleData> modules;

	private:
		Project::ProjectData m_projectData;
		EditorLayer(const std::string& name, Project::ProjectData* data);
		static EditorLayer* s_singleton;


	};

}