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
        // TODO: optimize direcroties
	
		DropDownViewPanel();
		ImGui::SameLine();
        ResizePanel();
        ImGui::SameLine();
		FolderViewPanel();
		ImGui::End();


		return "";
	}

    void DirectoryModule::ResizePanel() {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float height = ImGui::GetContentRegionAvail().y;
        ImGui::BeginChild("Resizeable", {5, 0}, 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::GetForegroundDrawList()->AddRectFilled({pos.x, pos.y}, {pos.x+5, pos.y+height}, IM_COL32(125,125,125,125));
        if (ImGui::IsMouseHoveringRect({pos.x, pos.y}, {pos.x+5, pos.y+height})) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            if (ImGui::IsMouseDown(0)) {
                isDragging = true;
                lastX = ImGui::GetMousePos().x;
            } else {
                isDragging = false;
            }
        }
        if (!ImGui::IsMouseDown(0)) {
            isDragging = false;
        } else if (isDragging && !isDragAndDrop) {
            float xPos =ImGui::GetMousePos().x;
            float xDist = xPos - lastX;
            if (xDist != 0) {
                
                xDist /= 1000.f;
                
                dropDownPanelWidth += xDist;
                
                if (dropDownPanelWidth > .6f) {
                    dropDownPanelWidth = .6f;
                } else if (dropDownPanelWidth < .15f) {
                    dropDownPanelWidth = .15f;
                }
                
                lastX = xPos;
            }
        }
        ImGui::EndChild();
    }

	void DirectoryModule::SettingsModal()
	{
		bool b = true;
		if (ImGui::BeginPopup("DirectoryModal")) {

			if (m_rightClicked.is_directory) {
				if (ImGui::Button("Open")) {
					SelectView(m_rightClicked, 0);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Rename")) {
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete")) {
					GEngine::FileSystem::DeleteFile(m_rightClicked.path);
					ImGui::CloseCurrentPopup();
				}
			}
			else if (m_rightClicked.path == "NULL") {
				if (ImGui::Button("New Folder")) {
					GEngine::FileSystem::CreateDirectories(m_currentEntry.path().generic_string() + "NewFolder");
					ImGui::CloseCurrentPopup();
				}
			} 
			else  {
				if (ImGui::Button("Open")) {
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Rename")) {
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete")) {
					GEngine::FileSystem::DeleteFile(m_rightClicked.path);
					ImGui::CloseCurrentPopup();
				}
			}


			ImGui::EndPopup();
		}
	}

	void DirectoryModule::FolderViewPanel()
	{
		std::vector<DirectoryPath> directories = {};
		bool isFileHovered = false;
		isDragAndDrop = false;
        if (ImGui::GetWindowWidth()* (1-dropDownPanelWidth) < 100) return;
		ImGui::BeginChild("IconFolderViewer", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        
		if (depth > 0) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			if (ImGui::ImageButton((ImTextureID)m_textures["undoIcon"]->GetRendererID(), { 15, 15 }, { 0,1 }, { 1,0 })) {
				m_currentEntry = std::filesystem::directory_entry(m_currentEntry.path().parent_path());
				m_selectedViewEntry = m_currentEntry.path().generic_string();
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
#ifdef GE_PLATFORM_MACOSX
            if (entry.path().filename() == ".DS_Store") continue;
#endif
			directories.push_back({ entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) });
		}

		std::sort(directories.begin(), directories.end(), [](const DirectoryPath& lhs, const DirectoryPath& rhs) {
			return lhs.name < rhs.name;
			});

		float width = ImGui::GetWindowWidth();
		float imageSize = 50;
		int cols = std::max((int)width / 100, 1);

		ImGui::Columns(cols, "DIRECTORYCOLS", false);

		float lineHeight = ImGui::CalcTextSize("A").y;
        lineHeight += lineHeight/2;
		std::vector<DirectoryPath>::iterator& _p = directories.begin();
		int dirCounter = 1;
		while ( _p != directories.end() ) {
			GEngine::Ref<GEngine::Texture2D> texture = nullptr;
			DirectoryPath& p = *_p;
			if (p.is_directory) {
				if (p.is_empty) {
					texture = m_textures["folderEmpty"];
				}
				else {
					texture = m_textures["folderFull"];
				}

				if (false) {
					m_currentEntry = std::filesystem::directory_entry(p.path);
					m_selectedViewEntry = p.path;
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

			float availWidth = ImGui::GetContentRegionAvailWidth();
			ImVec2 textSize = ImGui::CalcTextSize(p.name.c_str(), 0, false, availWidth + 1);
			ImVec2 pos = ImGui::GetCursorScreenPos();

			ImGui::BeginChild(p.path.c_str(), { availWidth, imageSize + textSize.y + lineHeight });
			bool isMouseOver = ImGui::IsMouseHoveringRect({ pos.x, pos.y }, { pos.x + ImGui::GetContentRegionAvailWidth(), pos.y + +imageSize + textSize.y + lineHeight / 2 });
			if (isMouseOver)
				isFileHovered = isMouseOver; 
			if (m_selectedEntry == p.path || isMouseOver) {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + availWidth, pos.y + imageSize + textSize.y + lineHeight / 2 }, IM_COL32(125, 125, 125, 100));
				if (isMouseOver && ImGui::IsMouseClicked(0)) {
					m_selectedEntry = p.path;
				}
				if (isMouseOver && p.is_directory && ImGui::IsMouseDoubleClicked(0)) {
					std::filesystem::directory_entry entry(p.path);
					m_currentEntry = entry;
					m_selectedViewEntry = p.path;
					depth++;
					ImGui::EndChild();
					break;
				}
			}
			else {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + ImGui::GetContentRegionAvailWidth(), pos.y + 100 + textSize.y }, IM_COL32(255, 0, 0, 0));
			}

			ImGui::PushID(dirCounter);
			if (ImGui::BeginDragDropSource()) {
				DirectoryPayload* payload = new DirectoryPayload(p);
				isDragAndDrop = true;
				ImGui::SetDragDropPayload("DirectoryPath", (void*)payload, sizeof(DirectoryPayload));
				delete payload;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 - imageSize + imageSize / 2);
				ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize,imageSize }, { 0,1 }, { 1,0 });
				int startIndex = 0;
				for (int i = 0; i < p.name.size(); i++) {
					ImVec2 size = ImGui::CalcTextSize(&p.name[startIndex], &p.name[i], true);
					if (size.x > availWidth - 10) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 -
							size.x + (size.x / 2));
						std::string s(&p.name[startIndex], &p.name[i]);
						ImGui::Text(s.c_str());
						startIndex = i;
						i -= 1;
					}
					else if (i == p.name.size() - 1) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 -
							size.x + (size.x / 2));
						std::string s(&p.name[startIndex]);
						ImGui::Text(s.c_str());
					}
				}
				ImGui::EndDragDropSource();
			}
			ImGui::PopID();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 - imageSize + imageSize / 2);
			ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize,imageSize }, { 0,1 }, { 1,0 });

			//if (textSize.y <= lineHeight) {

			//}
			int startIndex = 0;
			for (int i = 0; i < p.name.size(); i++) {
				ImVec2 size = ImGui::CalcTextSize(&p.name[startIndex], &p.name[i], true);
				if (size.x > availWidth - 10) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 -
						size.x + (size.x / 2));
					std::string s(&p.name[startIndex], &p.name[i]);
					ImGui::Text(s.c_str());
					startIndex = i;
					i -= 1;
				}
				else if (i == p.name.size() - 1) {
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 -
						size.x + (size.x / 2));
					std::string s(&p.name[startIndex]);
					ImGui::Text(s.c_str());
				}
			}

			//ImGui::TextWrapped(p.name.c_str());	
			ImGui::EndChild();
			if (p.is_directory) {
				AcceptDirPayload(p);
			}
			if (isMouseOver && ImGui::IsMouseClicked(1)) {
				ShowSetingsModal(*_p);
			}

			ImGui::NextColumn();
			_p++;
			dirCounter++;
		}
		if (!isFileHovered && ImGui::IsWindowHovered()) {
			if (ImGui::IsMouseDown(1)) {
				m_selectedEntry = "";
				m_rightClicked = {"NULL"};
				ImGui::OpenPopup("DirectoryModal");
			}
			if (ImGui::IsMouseDown(0)) {
				m_selectedEntry = "";
			} 
		}
		SettingsModal();
		ImGui::EndChild();
	}

	void DirectoryModule::DropDownViewPanel()
	{
		ImGui::BeginChild("DropDownViewer", { ImGui::GetWindowWidth() * dropDownPanelWidth, 0 }, true, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Viewer");
		ImGui::Separator();
		DirectoryPath d = { m_directoryBase.path().generic_string(), m_directoryBase.path().filename().generic_string(), m_directoryBase.path().extension().generic_string(), m_directoryBase.is_directory(), std::filesystem::is_empty(m_directoryBase.path()) };
		ImGuiTreeNodeFlags fl = m_selectedViewEntry == d.path ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx("Content", ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
			if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
			AcceptDirPayload(d);
			for (const auto& entry : std::filesystem::directory_iterator(m_directoryBase)) {
				DirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
				fl = m_selectedViewEntry == d.path ? ImGuiTreeNodeFlags_Selected : 0;
				if (d.is_directory) {
					d.is_empty = true;
					for (const auto& _entry : std::filesystem::directory_iterator(entry)) {
						if (_entry.is_directory()) {
							d.is_empty = false;
							break;
						}
					}
					if (d.is_empty) {
						if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow |  fl)) {
							if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
							AcceptDirPayload(d);
							ImGui::TreePop();
						}
						else {
							if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
							AcceptDirPayload(d);
						}
						
					}
					else {
						if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
							if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
							AcceptDirPayload(d);
							GetChildren(entry);
							ImGui::TreePop();
						}
						else
						{
							if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
							AcceptDirPayload(d);
						}
					}
				}
			}
			ImGui::TreePop();
		}
		else {
			if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
			AcceptDirPayload(d);
		}

		ImGui::EndChild();
	}

	void DirectoryModule::GetChildren(const std::filesystem::directory_entry& _entry)
	{
		for (const auto& entry : std::filesystem::directory_iterator(_entry)) {
			DirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
			ImGuiTreeNodeFlags fl = (m_currentEntry == d.path && m_selectedViewEntry == d.path) ? ImGuiTreeNodeFlags_Selected : 0;
			if (d.is_directory) {
				d.is_empty = true;
				for (const auto& __entry : std::filesystem::directory_iterator(entry)) {
					if (__entry.is_directory()) {
						d.is_empty = false;
					}
				}
				if (d.is_empty) {
					if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow |  fl)) {
						if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
						AcceptDirPayload(d);
						ImGui::TreePop();
					}
					else {
						if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
						AcceptDirPayload(d);
					}
				}
				else {
					if (ImGui::TreeNodeEx(d.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
						if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
						AcceptDirPayload(d);
						GetChildren(entry);
						ImGui::TreePop();
					}
					else {
						if (!fl && ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing()) SelectView(d, fl);
						AcceptDirPayload(d);
					}
					
				}
			}
		}
	}

	void DirectoryModule::SelectView(const DirectoryPath& d, int fl)
	{
		
		m_selectedViewEntry = d.path;
		m_currentEntry = std::filesystem::directory_entry(m_selectedViewEntry);
		std::string s(&d.path[m_directoryBase.path().generic_string().size()]);
		if (s.size() <= 0) {
			depth = 0;
			return;
		}
		int de = 1;
		for (int i = 0; i < s.size() - 1; i++) {
			if (s[i] == '/') de++;
		}
		depth = de;
		m_selectedEntry = "";
		
	}

	void DirectoryModule::ShowSetingsModal(const DirectoryPath& flags)
	{
		if (m_rightClicked != flags)
			m_rightClicked = flags;
		ImGui::OpenPopup("DirectoryModal");

	}

	void DirectoryModule::AcceptDirPayload(const DirectoryPath& p)
	{
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DirectoryPath");
			if (payload) {
				GE_CORE_DEBUG("ACCEPTED PAYLOAD: {0}", payload->Data);
				DirectoryPayload payloadPath = *(DirectoryPayload*)payload->Data;
				if (payloadPath.name != p.name) {
					GEngine::ThreadPool::AddMainThreadFunction([payloadPath, p]() {
						GEngine::FileSystem::MoveFile(payloadPath.path, p.path + "/" + payloadPath.name);
						});
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

}
