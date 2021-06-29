#include "MainMenuModule.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorUIEvents.h"

namespace Editor {

	MainMenuModule::MainMenuModule(std::unordered_map<std::string, EditorModuleData>* modules) : m_modules(modules)
	{

	}

	MainMenuModule::~MainMenuModule()
	{

	}

	void MainMenuModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Build")) {
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {

			for (auto& p : *m_modules) {
				if (p.first == name || !p.second.toggable) continue;
				if (ImGui::MenuItem(p.first.c_str(), 0, &p.second.isOpen, true)) {
					if (!p.second.isOpen) EditorLayer::GetDispatcher()->BroadcastEvent<EditorHideViewEvent>(p.first);
					else  EditorLayer::GetDispatcher()->BroadcastEvent<EditorShowViewEvent>(p.first);
				}

			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

}