#pragma once

#include <GEngine.h>
#include "Project/ProjectData.h"
#include "Modules/DirectoryModule.h"

namespace Editor {

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

		static EditorLayer* GetEditor();
	private:
		DirectoryModule* directoryModule;

	private:
		Project::ProjectData m_projectData;
		EditorLayer(const std::string& name, Project::ProjectData* data);
		static EditorLayer* s_singleton;


	};

}