#include "Editor/Modules/ReloadModule.h"

namespace Editor {


	ReloadModule::ReloadModule(Project::LocalProject* prj) : m_project(prj)
	{
		LoadProjects();
		for (int i = 0; m_projectData.size(); i++) {
			if (m_projectData[i] == *m_project) {
				project_pos = i;
				break;
			}
		}
	}

	bool ReloadModule::CanReload()
	{
		bool isReloading = false;
		{
			std::lock_guard<std::mutex> guard(m_reloadMutex);
			isReloading = b_isReloading;
		}
		return !GEngine::SceneManager::HasBegun() && !isReloading;
	}

	bool ReloadModule::IsReloading()
	{
		bool isReloading = false;
		{
			std::lock_guard<std::mutex> guard(m_reloadMutex);
			isReloading = b_isReloading;
		}

		return isReloading;
	}

	void ReloadModule::Reload()
	{

		if (!CanReload()) return;

		GEngine::ThreadPool::AddJob([this]() {

			if (m_project->data.isNative()) {
				{
					std::lock_guard<std::mutex> guard(m_reloadMutex);
					b_isReloading = true;
				}
				std::string path = m_project->data.path + "/" + m_project->data.name;
				Project::ProjectData* d = &m_project->data;

				GEngine::ScriptApi::SetBuild_Native(path + "/" + d->name + "/" + d->name + "/NativeScripts/", "NativeScripts");
				GEngine::ScriptApi::SetMake_Native("", "", [this]() {
					return GenerateProject();
					});
				GEngine::ScriptApi::SetDLLDir_Native(path + "/" + d->name + "/Bin/" + GE_CONFIG + "-" + Generation::GenerateProject::PlatformFlagToStr(static_cast<Generation::PlatformFlags>(m_project->platformFlags)) + "-x86_64/" + d->name + "/");
				GEngine::ScriptApi::OutputDir_Native(path + "/" + d->name + "/" + d->name + "/NativeScripts/Generated/");
				GEngine::ScriptApi::SetRelativePath_Native("../Scripts/");
				GEngine::ScriptApi::Load(path + "/" + d->name + "/" + d->name + "/NativeScripts/Scripts/", ".h");
				{
					std::lock_guard<std::mutex> guard(m_reloadMutex);
					b_isReloading = false;
				}
			}
			});
	}


	bool ReloadModule::GenerateProject(bool retry /*= true*/)
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
				std::string chmod = "chmod +x \"" + selectedProject + "/" + d->name + "/Generate/GenerateProject." + GE_CMD_EXTENSION + "\"";
				GE_CORE_DEBUG("CMD {0}", chmod);
				GEngine::Utility::sys::exec_command(chmod);
				return GenerateProject(false);
			}
		}
#endif
		return true;

	}


	void ReloadModule::LoadProjects()
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

	void ReloadModule::SaveProjects()
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