#include "MainMenuModule.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorUIEvents.h"

#include "Editor/Modules/ReloadModule.h"
#include "Editor/Modules/SerializerModule.h"

namespace Editor {

	MainMenuModule::MainMenuModule(std::unordered_map<std::string, EditorModuleData>* modules, Project::LocalProject* proj, ReloadModule* reloadModule, SerializerModule* serializer) : m_modules(modules), m_project(proj), m_reloadModule(reloadModule),
		m_serialzierModule(serializer)
	{
		folderIcon = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/folderIcon172x172.png");
		m_msBuildLocation = m_project->compilerDir;
		memcpy(buildBuffer, &m_msBuildLocation[0], std::min((int)m_msBuildLocation.size() * sizeof(char), 2048 * sizeof(char)));
		LoadProjects();
		GEngine::Utility::sys::set_ms_build_location(m_msBuildLocation);
		for (int i = 0; m_projectData.size(); i++) {
			if (m_projectData[i] == *m_project) {
				project_pos = i;
				break;
			}
		}
		
	}

	MainMenuModule::~MainMenuModule()
	{

	}

	void MainMenuModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		if (m_generateModalStart) {
			ImGui::OpenPopup("Generate Project");
			m_generateModal = true;
			m_generateModalStart = false;
		}

		GenerateModal();


		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("New Scene", "CTRL+N", nullptr)) {

			}

			if (ImGui::MenuItem("Save Scene", "CTRL+S", nullptr)) {
				if (m_serialzierModule->Save()) {

				}
			}

			if (ImGui::MenuItem("Open Scene", "CTRL+O", nullptr)) {
				std::string path = "";
				GEngine::FileSystem::OpenFileDialog({ {"Scene", "scene"} }, path);

				if (path.size() > 0) {
					std::filesystem::directory_entry e(path);
					std::string name = e.path().filename().generic_string();
					m_serialzierModule->Load(name, path);
				}

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z", nullptr)) {
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", nullptr)) {
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "CTRL+C", nullptr)) {
			
			}
			if (ImGui::MenuItem("Cut", "CTRL+X", nullptr)) {

			}
			if (ImGui::MenuItem("Paste", "CTRL+V", nullptr)) {

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Build")) {
			
			
			if (ImGui::MenuItem("Reload Native Scripts", 0, nullptr, m_reloadModule->CanReload())) {
				m_reloadModule->Reload();
			}

			if (m_reloadModule->IsReloading()) {
				if (ImGui::MenuItem("Cancel Build", 0, nullptr)) {
					GEngine::Utility::sys::SetForceKillChild(true);
				}
			}

			if (ImGui::MenuItem("Build Settings", 0, nullptr, true)) {
				m_generateModalStart = true;
			}

			if (ImGui::MenuItem("Package", 0, nullptr, true)) {


			}

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



	bool MainMenuModule::GenerateProject(bool retry /*= true*/)
	{

		Project::ProjectData* d = &m_project->data;
		std::string path = GEngine::FileSystem::GetParentExecuteableDir(4);

		std::string flags = Generation::GenerateProject::GenerateCommand(static_cast<Generation::PlatformFlags>(m_project->platformFlags), static_cast<Generation::ProjectTypeFlags>(m_project->buildFlags), m_project->generateFlags);
		std::string cmd = "\"" + d->path + "/" + d->name + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\" " + flags + " --engine-source=" + path + " --target-name=\"" + d->name + "\"";
#ifdef GE_PLATFORM_WINDOWS
		std::replace(cmd.begin(), cmd.end(), '/', '\\');
#endif
		GE_CORE_DEBUG("CMD {0}", cmd);
		std::string re = GEngine::Utility::sys::exec_command(cmd);
#ifndef GE_PLATFORM_WINDOWS
		if (retry) {
			if (re.find("Permission denied") != std::string::npos) {
				std::string chmod = "chmod +x \"" + d->name + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\"";
				GE_CORE_DEBUG("CMD {0}", chmod);
				GEngine::Utility::sys::exec_command(chmod);
				return GenerateProject(false);
			}
		}
#endif
		return true;
		
	}

	bool MainMenuModule::GenerateModal()
	{
#ifdef GE_EDITOR
		if (m_generateModal) ImGui::SetNextWindowSize({ 400,0 });
		if (ImGui::BeginPopupModal("Generate Project", &m_generateModal, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("Platform: ");
			ImGui::SameLine();


			auto& d = Generation::GenerateProject::GetPlatformData();
			Generation::PlatformStructure platformStruct = d[static_cast<Generation::PlatformFlags>(m_project->platformFlags)];

			if (ImGui::BeginCombo("##Platform", Generation::GenerateProject::PlatformFlagToStr(static_cast<Generation::PlatformFlags>(m_project->platformFlags)).c_str())) {

				for (const auto& p : d) {

					bool is_selected = (uint32_t)p.first == m_project->platformFlags;
					if (ImGui::Selectable(Generation::GenerateProject::PlatformFlagToStr(p.first).c_str(), is_selected)) {
						m_project->platformFlags = (uint32_t)p.first;
						m_project->generateFlags = Generation::GenerateProject::GetDefaultGenerationFlags(static_cast<Generation::PlatformFlags>(m_project->platformFlags));
						platformStruct = d[static_cast<Generation::PlatformFlags>(m_project->platformFlags)];
						m_project->buildFlags = (uint32_t)Generation::GenerateProject::GetDefaultProjectType(static_cast<Generation::PlatformFlags>(m_project->platformFlags));
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();

					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Project Type: ");
			ImGui::SameLine();

			if (ImGui::BeginCombo("##Project", Generation::GenerateProject::ProjectFlagToStr(static_cast<Generation::ProjectTypeFlags>(m_project->buildFlags)).c_str())) {

				for (const auto& p : platformStruct.projectFlags) {

					bool is_selected = (uint32_t)p == m_project->buildFlags;
					if (ImGui::Selectable(Generation::GenerateProject::ProjectFlagToStr(p).c_str(), is_selected)) {
						m_project->buildFlags = (uint32_t)p;
					}

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (platformStruct.generationFlags.size() > 1) {


				ImGui::BeginChild("GenerateFlagsLeft", { ImGui::GetWindowWidth() / 2,  (platformStruct.generationFlags.size() / 2.f) * 40.f }, false);

				for (size_t i = platformStruct.generationFlags.size() <= 1 ? 2 : platformStruct.generationFlags.size() / 2; i < platformStruct.generationFlags.size(); i++) {
					ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[i]).c_str(), (unsigned int*)&m_project->generateFlags, (uint32_t)platformStruct.generationFlags[i]);
				}

				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("GenerateFlagsRight", { ImGui::GetWindowWidth() / 2, (platformStruct.generationFlags.size() / 2.f) * 40.f }, false);

				for (int i = 0; i < platformStruct.generationFlags.size() / 2; i++) {
					ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[i]).c_str(), (unsigned int*)&m_project->generateFlags, (uint32_t)platformStruct.generationFlags[i]);
				}

				ImGui::EndChild();

			}
			else {
				ImGui::CheckboxFlags(Generation::GenerateProject::GenerateFlagStr(platformStruct.generationFlags[0]).c_str(), (unsigned int*)&m_project->generateFlags, (uint32_t)platformStruct.generationFlags[0]);
			}

			if (m_project->platformFlags == (uint32_t)Generation::PlatformFlags::WINDOWS) {
				ImGui::Text("MSBuild Location");
				ImGui::PushItemWidth(350);
				ImGui::InputText("##msbuild", buildBuffer, 2048);
				
				m_msBuildLocation = std::string(buildBuffer);

				ImGui::SameLine(0,0);

				if (m_msBuildLocation.size() > 2) {
					if (m_msBuildLocation[0] == '"') {
						m_msBuildLocation.erase(m_msBuildLocation.begin());
					}
					if (m_msBuildLocation[m_msBuildLocation.size() - 1] == '"') {
						m_msBuildLocation.erase(m_msBuildLocation.begin()+m_msBuildLocation.size() - 1);
					}
				}


				if (ImGui::ImageButton((ImTextureID)(uintptr_t)folderIcon->GetRendererID(), { 17.f,17.f }, { .0f,1.f }, { 1.f, 0.f })) {
					std::string fileLoc;
					std::replace(m_msBuildLocation.begin(), m_msBuildLocation.end(), '\\', '/');
					GEngine::FileSystem::OpenFileDialog({}, fileLoc, m_msBuildLocation, true);
					if (fileLoc.size() > 0) {
						m_msBuildLocation = fileLoc;
						memset(buildBuffer, 0, sizeof(buildBuffer));
						memcpy(buildBuffer, m_msBuildLocation.data(), std::min(m_msBuildLocation.size(), sizeof(buildBuffer)));
					}
				}

				if (m_project->compilerDir != m_msBuildLocation) {
					if (m_msBuildLocation.size() > 2) {
						if (m_msBuildLocation[0] != '"') {
							m_msBuildLocation.insert(m_msBuildLocation.begin(), '"');
						}
						if (m_msBuildLocation[m_msBuildLocation.size() - 1] != '"') {
							m_msBuildLocation += '"';
						}
					}

					std::replace(m_msBuildLocation.begin(), m_msBuildLocation.end(), '\\', '/');

					memset(m_project->compilerDir, 0, sizeof(m_project->compilerDir));
					memcpy(m_project->compilerDir, m_msBuildLocation.c_str(), std::min((int)sizeof(m_project->compilerDir), (int)m_msBuildLocation.size()));
					GEngine::Utility::sys::set_ms_build_location(m_msBuildLocation);
				}
				
				

			}

			ImGui::Separator();
			if (ImGui::Button("Generate")) {
				SaveProjects();
				GenerateProject();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return true;
#endif
	}

	void MainMenuModule::LoadProjects()
	{
		std::string saveLoc = GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor/.projects";

		std::ifstream stream(saveLoc, std::ios::in | std::ios::binary);

		if (stream.is_open()) {
			char path[1008] = { 0 };
			stream.read(path, sizeof(char) * 1008);
			m_lastProjectDir = path;
			memset(path, 0, 1008 * sizeof(char));
			uint64_t prjCount = 0;
			stream.read((char*)&prjCount, sizeof(uint64_t));
			Project::LocalProject proj;
			for (int i = 0; i < prjCount; i++) {
				stream.read(path, sizeof(char) * 1008);
				stream.read((char*)&proj.generateFlags, sizeof(proj.generateFlags));
				stream.read((char*)&proj.buildFlags, sizeof(proj.buildFlags));
				stream.read((char*)&proj.platformFlags, sizeof(proj.platformFlags));
				stream.read((char*)proj.compilerDir, sizeof(proj.compilerDir));
				std::ifstream in(path, std::ios::in | std::ios::binary);
				in >> proj.data;
				in.close();
				if (proj.data.path.size() > 0)
					m_projectData.push_back(std::move(proj));
				memset(path, 0, 1008 * sizeof(char));
			}

			stream.close();
		}
	}

	void MainMenuModule::SaveProjects()
	{
		m_projectData[project_pos] = *m_project;
		std::string file = GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor/.projects";
		std::ofstream stream;
		stream.open(file, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!stream.is_open()) {
			GEngine::FileSystem::CreateDirectories(GEngine::FileSystem::GetDefaultLocation() + "/ImpulseEditor");
			stream = std::ofstream(file, std::ios::out | std::ios::binary | std::ios::trunc);
		}

		if (stream.is_open()) {
			char path[1008] = { 0 };
			memcpy(path, m_lastProjectDir.data(), sizeof(char) * m_lastProjectDir.size());
			stream.write(path, 1008 * sizeof(char));
			memset(path, 0, 1008 * sizeof(char));
			uint64_t prjCount = m_projectData.size();
			stream.write((char*)&prjCount, sizeof(uint64_t));
			for (Project::LocalProject& proj : m_projectData) {
				const Project::ProjectData& p = proj.data;
				std::string _path = p.path + "/" + p.name + "/" + p.name + ".proj";
				memcpy(path, _path.data(), _path.size() * sizeof(char));
				stream.write(path, 1008 * sizeof(char));
				stream.write((const char*)&proj.generateFlags, sizeof(proj.generateFlags));
				stream.write((const char*)&proj.buildFlags, sizeof(proj.buildFlags));
				stream.write((const char*)&proj.platformFlags, sizeof(proj.platformFlags));
				stream.write((const char*)proj.compilerDir, sizeof(proj.compilerDir));
				memset(path, 0, 1008 * sizeof(char));
			}
			stream.close();
		}
	}

}
