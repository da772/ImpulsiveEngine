#pragma once

#include <GEngine.h>
#include "Project/ProjectData.h"
#include "Modules/EditorModule.h"
#include "Events/EditorEvents.h"

namespace Editor {


	class ReloadModule;
	class SerializerModule;

	class EditorLayer : public GEngine::Layer {

	public:
		static EditorLayer* Create(Project::LocalProject* data);
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
		inline GEngine::Ref<T> AddModule(const std::string & id, bool isOpen, int flags, bool togglable, Args&& ... args) {
			std::shared_ptr<T> e = std::make_shared<T>(std::forward<Args>(args)...);
			modules[id] = {e, isOpen, flags, togglable};
			modules_order.push_back(id);
			return e;
		}

		inline EditorModuleData& GetModule(const std::string& id) {
			return modules[id];
		}

		static inline EditorDispatcher* GetDispatcher() { return &s_dispatcher; }
		static EditorLayer* GetEditor();

	private:
		static EditorDispatcher s_dispatcher;
		std::unordered_map<std::string, EditorModuleData> modules;
		std::vector<std::string> modules_order;
		EditorEventType editorTool = EditorEventType::None;

	private:
		Project::LocalProject m_projectData;
		EditorLayer(const std::string& name, Project::LocalProject* data);
		static EditorLayer* s_singleton;
		std::set<GEngine::ObjectHash> selectedGameObject;
		GEngine::Orthographic_CameraController* m_cameraController = nullptr;
		ReloadModule* reloadModule;
		SerializerModule* serializerModule;

	};

}