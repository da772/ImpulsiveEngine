#include "DirectoryModule.h"

namespace Editor {

	DirectoryModule::DirectoryModule(const std::string& directoryBase) : m_currentEntry(directoryBase), m_directoryBase(directoryBase)
	{
		m_textures["folderIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderIcon172x172.png");
		m_textures["folderEmpty"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderEmpty160x160.png");
		m_textures["folderFull"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderFull160x160.png");
		m_textures["fileIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/document160x160.png");
		m_textures["undoIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/undo160x160.png"); 
	}

	std::string DirectoryModule::Create(bool* is_open, uint32_t flags)
	{
		ImGui::Begin(moduleName.c_str(), is_open, flags);

		DropDownViewPanel();
		ImGui::SameLine();
		FolderViewPanel();

		ImGui::End();

		return "";
	}

	void DirectoryModule::FolderViewPanel()
	{
		std::vector<DirectoryPath> directories = {};

		ImGui::BeginChild("IconFolderViewer", { ImGui::GetWindowWidth() * .85f, 0 }, true);

		if (depth > 0) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			if (ImGui::ImageButton((ImTextureID)m_textures["undoIcon"]->GetRendererID(), { 15, 15 }, { 0,1 }, { 1,0 })) {
				m_currentEntry = std::filesystem::directory_entry(m_currentEntry.path().parent_path());
				lastDepth = depth;
				depth--;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
		}
		if (depth == 0) {
			ImGui::Text("Content");
		}
		else {
			ImGui::Text(&m_currentEntry.path().generic_string().c_str()[m_directoryBase.path().generic_string().size() - m_directoryBase.path().parent_path().filename().generic_string().size() - 1]);
		}

		ImGui::Separator();

		for (const auto& entry : std::filesystem::directory_iterator(m_currentEntry)) {
			directories.push_back({ entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) });
		}

		std::sort(directories.begin(), directories.end(), [](const DirectoryPath& lhs, const DirectoryPath& rhs) {
			return lhs.name < rhs.name;
			});

		float width = ImGui::GetWindowWidth();
		float imageSize = 50;
		int cols = std::max((int)width / 100, 1);

		ImGui::Columns(cols, "DIRECTORYCOLS", false);
		bool open = true;

		float lineHeight = ImGui::CalcTextSize("A").y + 5;

		for (const auto& p : directories) {
			GEngine::Ref<GEngine::Texture2D> texture = nullptr;

			if (p.is_directory) {
				if (p.is_empty) {
					texture = m_textures["folderEmpty"];
				}
				else {
					texture = m_textures["folderFull"];
				}

				if (false) {
					m_currentEntry = std::filesystem::directory_entry(p.path);
					lastDepth = depth;
					depth++;
				}


			}
			else {
				if (p.ext == ".png" || p.ext == ".jpg" || p.ext == ".jpeg") {
					if (m_textures.find(p.path) == m_textures.end()) {
						GEngine::Ref<GEngine::Texture2D> tex = GEngine::Texture2D::Create(p.path);
						m_textures[p.path] = tex;
						texture = m_textures[p.path];
					}
					else {
						texture = m_textures[p.path];
					}
				}
				else {
					texture = m_textures["fileIcon"];
				}
			}

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 textSize = ImGui::CalcTextSize(p.name.c_str(), 0, true, ImGui::GetContentRegionAvailWidth());
			bool isMouseOver = ImGui::IsMouseHoveringRect({ pos.x, pos.y }, { pos.x + ImGui::GetContentRegionAvailWidth(), pos.y + +imageSize + textSize.y + lineHeight / 2 });
			if (m_selectedEntry == p.path || isMouseOver) {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + ImGui::GetContentRegionAvailWidth(), pos.y + imageSize + textSize.y + lineHeight / 2 }, IM_COL32(125, 125, 125, 100));
				if (isMouseOver && ImGui::IsMouseClicked(0)) {
					m_selectedEntry = p.path;
				}
				if (isMouseOver && p.is_directory && ImGui::IsMouseDoubleClicked(0)) {
					std::filesystem::directory_entry entry(p.path);
					m_currentEntry = entry;
					depth++;
					break;
				}
			}
			else {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + ImGui::GetContentRegionAvailWidth(), pos.y + 100 + textSize.y }, IM_COL32(255, 0, 0, 0));
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() / 2 - imageSize + imageSize / 2);

			ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize,imageSize }, { 0,1 }, { 1,0 });

			ImGui::PopStyleColor();

			//if (textSize.y <= lineHeight) {

			//}
			int startIndex = 0;
			for (int i = 0; i < p.name.size(); i++) {
				ImVec2 size = ImGui::CalcTextSize(&p.name[startIndex], &p.name[i], true);
				if (size.x > ImGui::GetContentRegionAvailWidth() - 10) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() / 2 -
						size.x + (size.x / 2));
					std::string s(&p.name[startIndex], &p.name[i]);
					ImGui::Text(s.c_str());
					startIndex = i;
					i -= 1;
				}
				else if (i == p.name.size() - 1) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() / 2 -
						size.x + (size.x / 2));
					std::string s(&p.name[startIndex]);
					ImGui::Text(s.c_str());
				}
			}

			//ImGui::TextWrapped(p.name.c_str());	


			ImGui::NextColumn();
		}
		ImGui::EndChild();
	}

	void DirectoryModule::DropDownViewPanel()
	{
		ImGui::BeginChild("DropDownViewer", { ImGui::GetWindowWidth() * .125f, 0 }, true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);

		ImGui::Text("Viewer");
		ImGui::Separator();
		DirectoryPath d = { m_directoryBase.path().generic_string(), m_directoryBase.path().filename().generic_string(), m_directoryBase.path().extension().generic_string(), m_directoryBase.is_directory(), std::filesystem::is_empty(m_directoryBase.path()) };
		ImGuiTreeNodeFlags fl = m_selectedViewEntry == d.path ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx("Content", ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
			SelectView(d, fl);
			for (const auto& entry : std::filesystem::directory_iterator(m_directoryBase)) {
				DirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
				fl = m_selectedViewEntry == d.path ? ImGuiTreeNodeFlags_Selected : 0;
				if (d.is_directory) {
					d.is_empty = true;
					for (const auto& _entry : std::filesystem::directory_iterator(entry)) {
						if (_entry.is_directory()) {
							d.is_empty = false;
						}
					}
					if (d.is_empty) {
						if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_Leaf | fl)) {
							SelectView(d, fl);
							ImGui::TreePop();
						}
						else {
							SelectView(d, fl);
						}
					}
					else {
						if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
							SelectView(d, fl);
							GetChildren(entry);
							ImGui::TreePop();
						}
						else
						{
							SelectView(d, fl);
						}
					}
				}
			}
			ImGui::TreePop();
		}

		ImGui::EndChild();
	}

	void DirectoryModule::GetChildren(const std::filesystem::directory_entry& _entry)
	{
		for (const auto& entry : std::filesystem::directory_iterator(_entry)) {
			DirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
			ImGuiTreeNodeFlags fl = m_selectedViewEntry == d.path ? ImGuiTreeNodeFlags_Selected : 0;
			if (d.is_directory) {
				d.is_empty = true;
				for (const auto& __entry : std::filesystem::directory_iterator(entry)) {
					if (__entry.is_directory()) {
						d.is_empty = false;
					}
				}
				if (d.is_empty) {
					if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_Leaf | fl)) {
						SelectView(d, fl);
						ImGui::TreePop();
					}
					else {
						SelectView(d, fl);
					}
				}
				else {
					if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
						SelectView(d, fl);
						GetChildren(entry);
						ImGui::TreePop();
					}
					else {
						SelectView(d, fl);
					}
					
				}
			}
		}
	}

	void DirectoryModule::SelectView(const DirectoryPath& d, int fl)
	{
		if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
		{
			m_selectedViewEntry = d.path;
			m_currentEntry = std::filesystem::directory_entry(m_selectedViewEntry);
			std::string s(&d.path[m_directoryBase.path().generic_string().size()]);
			if (s.size() <= 0) {
				depth = 0;
				return;
			}
			int d = 1;
			for (int i = 0; i < s.size()-1; i++) {
				if (s[i] == '/') d++;
			}
			depth = d;
		}
	}

}