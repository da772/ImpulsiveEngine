#include "DirectoryModule.h"

#include "imgui/imgui_internal.h"

namespace Editor {

	static void RenderArrow(ImDrawList* drawList, float fontSize, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale);

	DirectoryModule::DirectoryModule(const std::string& directoryBase, Project::ProjectData* projectData) : m_directoryBase(directoryBase+"/"), m_contentDirectoryBase(directoryBase + "/Content/"), m_scriptDirectoryBase(directoryBase +"/NativeScripts/Scripts/"), m_currentEntry(m_contentDirectoryBase)
		, m_projectData(projectData)
	{
		m_textures["folderIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderIcon172x172.png");
		m_textures["folderEmpty"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderEmpty160x160.png");
		m_textures["folderFull"] = GEngine::Texture2D::Create("Content/Textures/Icons/folderFull160x160.png");
		m_textures["fileIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/document160x160.png");
		m_textures["undoIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/undo160x160.png"); 
		m_textures["searchIcon"] = GEngine::Texture2D::Create("Content/Textures/Icons/searchIcon160x160.png", TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear);
	}

	void DirectoryModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags);
        // TODO: optimize direcroties
		
		DropDownViewPanel();
		ImGui::SameLine();
        ResizePanel();
        ImGui::SameLine();
		FolderViewPanel();
		// Dock flags
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

		dirWindowWidth = ImGui::GetWindowWidth();

		ImGui::End();


	}

    void DirectoryModule::ResizePanel() {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float height = ImGui::GetContentRegionAvail().y;
        ImGui::BeginChild("Resizeable", {5, 0}, 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::GetWindowDrawList()->AddRectFilled({pos.x, pos.y}, {pos.x+5, pos.y+height}, IM_COL32(125,125,125,125));
        if (ImGui::IsMouseHoveringRect({pos.x, pos.y}, {pos.x+5, pos.y+height}) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
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
                
				xDist /= dirWindowWidth;
                
                dropDownPanelWidth += xDist;
                
                if (dropDownPanelWidth > .6f) {
                    dropDownPanelWidth = .6f;
                } else if (dropDownPanelWidth < .1f) {
                    dropDownPanelWidth = .1f;
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

			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			if (m_rightClicked.is_directory) {
				if (ImGui::Button("Open", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					SelectView(m_rightClicked);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Rename", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					rename = true;
					m_selectedEntry = m_rightClicked.path;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					if (m_rightClicked.is_directory)
						GEngine::FileSystem::RemoveAllFolders(m_rightClicked.path);
					else
						GEngine::FileSystem::DeleteFile(m_rightClicked.path);
					forceFolderViewRefresh = true;
					ImGui::CloseCurrentPopup();
				}
			}
			else if (m_rightClicked.path == "NULL") {
				if (ImGui::Button("New File", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("New Folder", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					std::string s = m_currentEntry.path().generic_string();
					if (s[s.size() - 1] == '/') {
						s += "NewFolder";
					}
					else {
						s += "/NewFolder";
					}

					int counter = 0;
					uint32_t mpath = 250;
					if (s.size() < mpath) {
						while (!GEngine::FileSystem::CreateDirectory(s)) {
							s = m_currentEntry.path().generic_string();
							if (s[s.size() - 1] != '/') {
								s += "/";
							}
							if (counter > 0) {
								s += "NewFolder_" + std::to_string(counter);
							}
							else {
								s += "NewFolder";
							}
							if (s.size() >= mpath) break;
							counter++;
						}
					}
					forceFolderViewRefresh = true;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Show", { ImGui::GetContentRegionAvailWidth(), 0 })) {
#ifdef GE_PLATFORM_WINDOWS
					std::string cmd = "explorer \"" + m_currentEntry.path().generic_string()+"\"";
					std::replace(cmd.begin(), cmd.end(), '/', '\\');
					GEngine::Utility::sys::exec_command(cmd);
#endif
#ifdef GE_PLATFORM_LINUX
					GEngine::Utility::sys::exec_command("xdg-open " + m_currentEntry.path().generic_string());
#endif
#ifdef GE_PLATFORM_MACOSX
					GEngine::Utility::sys::exec_command("open " + m_currentEntry.path().generic_string());
#endif
					ImGui::CloseCurrentPopup();
				}
			} 
			else  {
				if (ImGui::Button("Open", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Rename", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					rename = true;
					m_selectedEntry = m_rightClicked.path;
					forceFolderViewRefresh = true;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Delete", { ImGui::GetContentRegionAvailWidth(), 0 })) {
					GEngine::FileSystem::DeleteFile(m_rightClicked.path);
					forceFolderViewRefresh = true;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}
	}

	static bool filterChange = false;

	void DirectoryModule::Filterbar()
	{
		ImGui::BeginChild("DirectoryFilterBar", { 0, ImGui::GetTextLineHeight() * 1.5f }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SameLine();
		ImGui::BeginChild("filterBarSpacer", { ImGui::GetContentRegionAvailWidth() * .75f, ImGui::GetTextLineHeight() * 1.5f }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::Text("Filter:");
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * .7f);
	
		
		ImGui::InputText("##FilterInput", filterBuffer, 255, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) {
			filterChange = true;
			return 0;
			}) ;

		if (filterChange) {
			filterChange = false;
			forceFolderViewRefresh = true;
		}

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::SameLine(0,0);
		ImGui::SetCursorPos({ ImGui::GetCursorPosX()-30.f, ImGui::GetCursorPosY() + 5.f });
		ImGui::Image((ImTextureID)m_textures["searchIcon"]->GetRendererID(), { ImGui::GetTextLineHeight() * .85f, ImGui::GetTextLineHeight() * .85f }, { 0,1 }, { 1,0 });
		ImGui::EndChild();
	}

	void DirectoryModule::FolderViewPanel()
	{
		bool isFileHovered = false;
		isDragAndDrop = false;
        if (ImGui::GetWindowWidth()* (1-dropDownPanelWidth) < 100) return;
		ImGui::BeginChild("IconFolderViewer", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		ImGui::BeginChild("IconFolderNavigator", { 0, ImGui::GetFontSize() * 1.75f }, false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar);
		{
		ImVec2 startPos = ImGui::GetCursorScreenPos();
		float width = ImGui::GetContentRegionAvailWidth();
		if (depth == 0) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			std::string dir = m_contentDirectoryBase.path().generic_string();
			dir.erase(dir.size() - 1);
			std::string current_folder = m_currentEntry.path().generic_string().find(dir) != std::string::npos ? "Content" : "Scripts";
			ImGui::Button(current_folder.c_str());
			ImGui::PopStyleColor();
		}
		else {
			std::filesystem::directory_entry current_base = m_currentEntry.path().generic_string().find(m_contentDirectoryBase.path().generic_string()) != std::string::npos ? m_contentDirectoryBase: m_scriptDirectoryBase;
			std::vector<std::filesystem::path> dirs;
			std::filesystem::path curPath = m_currentEntry.path();
			while (curPath.generic_string() != current_base.path().parent_path().generic_string()) {
				dirs.push_back(curPath);
				curPath = curPath.parent_path();
				//ImGui::Text(&m_currentEntry.path().generic_string().c_str()[m_directoryBase.path().generic_string().size() - m_directoryBase.path().parent_path().filename().generic_string().size() - 1]);
			}
			dirs.push_back(curPath);

			for (int i = dirs.size() - 1; i >= 0; i--) {
				if (dirs.size() - 1 != i) {
					ImGui::SameLine();
					ImGui::Text(">");
					ImGui::SameLine();
				}
				ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
				if (ImGui::Button((dirs[i].filename().generic_string()).c_str())) {
					DirectoryPath d = { dirs[i].generic_string(), dirs[i].filename().generic_string(), dirs[i].extension().generic_string(), false, false };
					SelectView(d);
					depth = dirs.size()-1 - i;
				}
				ImGui::PopStyleColor();
			}
		}
		ImGui::GetWindowDrawList()->AddLine({ startPos.x, ImGui::GetCursorScreenPos().y }, { startPos.x + width, ImGui::GetCursorScreenPos().y }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)));
		if (width > 610) {
			ImGui::SameLine();
			Filterbar();
		}
		}
		ImGui::EndChild();

		uint64_t cTime = GEngine::Time::GetEpochTimeMS();
		
		if (cTime - lastFolderViewUpdate >= folderViewRefreshTime || lastFolderViewEntry != m_currentEntry || forceFolderViewRefresh) {
			folderViewdirectories.clear();
			lastFolderViewUpdate = cTime;
			lastFolderViewEntry = m_currentEntry;
			forceFolderViewRefresh = false;
			dropDownFolderForceRefresh = true;

			for (const auto& entry : std::filesystem::directory_iterator(m_currentEntry)) {
#ifdef GE_PLATFORM_MACOSX
				if (entry.path().filename() == ".DS_Store") continue;
#endif
				DirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
				if (strlen(filterBuffer) > 0) {
					std::string filter(filterBuffer);
					std::string nme = d.name;
					std::transform(filter.begin(), filter.end(), filter.begin(),
						[](unsigned char c) { return std::tolower(c); });
					std::transform(nme.begin(), nme.end(), nme.begin(),
						[](unsigned char c) { return std::tolower(c); });
					if (nme.find(filter) != std::string::npos) {
						folderViewdirectories.push_back(d);
					}

				}
				else {
					folderViewdirectories.push_back(d);
				}
			}

			std::sort(folderViewdirectories.begin(), folderViewdirectories.end(), [](const DirectoryPath& lhs, const DirectoryPath& rhs) {
				return lhs.name < rhs.name;
				});
		}

		float width = ImGui::GetWindowWidth();
		float imageSize = 50;
		int cols = std::max((int)width / 110, 1);

		ImGui::Columns(cols, "DIRECTORYCOLS", false);

		float lineHeight = ImGui::CalcTextSize("A").y;
        lineHeight += lineHeight/2;
		std::vector<DirectoryPath>::iterator _p = folderViewdirectories.begin();
		int dirCounter = 1;
		while ( _p != folderViewdirectories.end() ) {
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

			
			
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 textSize = ImGui::CalcTextSize(p.name.c_str(), 0, false, ImGui::GetContentRegionAvailWidth() -ImGui::GetFontSize() / 2.f);

			ImGui::BeginChild(p.path.c_str(), { ImGui::GetContentRegionAvailWidth(), imageSize + textSize.y + lineHeight } , false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			float availWidth = ImGui::GetContentRegionAvailWidth();
			
			bool isMouseOver = ImGui::IsMouseHoveringRect({ pos.x, pos.y }, { pos.x + availWidth, pos.y + +imageSize + textSize.y + lineHeight / 2 }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);
			if (isMouseOver)
				isFileHovered = isMouseOver; 
			if (m_selectedEntry == p.path || isMouseOver) {
				ImVec4 col = m_selectedEntry == p.path && !isMouseOver ? ImGui::GetStyleColorVec4(ImGuiCol_Header) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + availWidth, pos.y + imageSize + textSize.y + lineHeight / 2 }, ImGui::GetColorU32(col));
				if (isMouseOver && ImGui::IsMouseReleased(0)) {
					if (m_selectedEntry != p.path) {
						m_selectedEntry = p.path;
						rename = false;
					}
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
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + availWidth, pos.y + 100 + textSize.y }, IM_COL32(255, 0, 0, 0));
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				DirectoryPayload* payload = new DirectoryPayload(p);
				isDragAndDrop = true;
				ImGui::SetDragDropPayload("DirectoryPath", (void*)payload, sizeof(DirectoryPayload));
				delete payload;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 - imageSize + imageSize / 2);
				ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize,imageSize }, { 0,1 }, { 1,0 });
				int startIndex = 0;
				for (int i = 0; i < p.name.size(); i++) {
					ImVec2 size = ImGui::CalcTextSize(&p.name[startIndex], &p.name[i], true);
					if (size.x > availWidth) {
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

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth / 2 - imageSize + imageSize / 2);
			if (texture)
				ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize,imageSize }, { 0,1 }, { 1,0 });

			//if (textSize.y <= lineHeight) {

			//}
			int startIndex = 0;
			if (m_selectedEntry == p.path && rename) {
				memcpy(renameBuffer, p.name.c_str(), p.name.size());
				if (ImGui::InputTextEx("##input", "Rename", renameBuffer, 255, {availWidth, 0}, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
					std::string newPath(p.path.c_str(), p.path.size() - p.name.size());
					newPath += renameBuffer;
					memset(renameBuffer, 0, 255);
					rename = false;
					GEngine::FileSystem::MoveFile(p.path, newPath);
					forceFolderViewRefresh = true;
				}
				if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseReleased(0))
					ImGui::SetKeyboardFocusHere(0);
			}
			else {
				for (int i = 0; i < p.name.size(); i++) {
					ImVec2 size = ImGui::CalcTextSize(&p.name[startIndex], &p.name[i], true);
					if (size.x > availWidth - ImGui::GetFontSize()) {
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
			}

			//ImGui::TextWrapped(p.name.c_str());	
			ImGui::EndChild();
			if (p.is_directory) {
				ImRect r = { { pos.x, pos.y }, { pos.x + availWidth, pos.y + +imageSize + textSize.y + lineHeight / 2 } };
				AcceptDirPayloadFolder(p, &r);
			}
			if (isMouseOver && ImGui::IsMouseReleased(1)) {
				ShowSetingsModal(*_p);
			}

			ImGui::NextColumn();
			_p++;
			dirCounter++;
		}
		if (!isFileHovered && ImGui::IsWindowHovered()) {
			if (ImGui::IsMouseDown(1)) {
				m_selectedEntry = "";
				rename = false;
				m_rightClicked = {"NULL"};
				ImGui::OpenPopup("DirectoryModal");
			}
			if (ImGui::IsMouseDown(0)) {
				m_selectedEntry = "";
				rename = false;
			} 
		}
		SettingsModal();
		ImGui::EndChild();
	}

	void DirectoryModule::GetDirectoryChildren(IterativeDirectoryPath* root) {

		for (const auto& entry : std::filesystem::directory_iterator(root->path)) {
			IterativeDirectoryPath d = { entry.path().generic_string(), entry.path().filename().generic_string(), entry.path().extension().generic_string(), entry.is_directory(), std::filesystem::is_empty(entry.path()) };
			if (d.is_directory) {
				if (!d.is_empty)
					GetDirectoryChildren(&d);
				root->children.push_back(d);
			}
		}
	}

	void DirectoryModule::DropDownViewPanel()
	{
		ImGui::BeginChild("DropDownViewer", { ImGui::GetWindowWidth() * dropDownPanelWidth, 0 }, true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Viewer");
		ImGui::Separator();

		float fontSize = ImGui::GetFontSize();

		if (dropDownFolderForceRefresh) {
			dropDownFolderForceRefresh = false;
			IterativeDirectoryPath d = { m_contentDirectoryBase.path().generic_string(), m_contentDirectoryBase.path().parent_path().filename().generic_string(), m_contentDirectoryBase.path().extension().generic_string(), m_contentDirectoryBase.is_directory(), std::filesystem::is_empty(m_contentDirectoryBase.path()) };
			dropDownDirectories.clear();
			GetDirectoryChildren(&d);
			dropDownDirectories.push_back(d);
			if (m_projectData->isNative()) {
				d = { m_scriptDirectoryBase.path().generic_string(), m_scriptDirectoryBase.path().parent_path().filename().generic_string(), m_scriptDirectoryBase.path().extension().generic_string(), m_scriptDirectoryBase.is_directory(), std::filesystem::is_empty(m_scriptDirectoryBase.path()) };
				GetDirectoryChildren(&d);
				dropDownDirectories.push_back(d);
			}

		}
		for (const auto& dir : dropDownDirectories) {
			CreateFolderView(dir, fontSize);
		}

		ImGui::EndChild();
	}

	void DirectoryModule::CreateFolderView(const IterativeDirectoryPath& d, float fontSize)
	{
		float availWidth = ImGui::GetContentRegionAvailWidth();
		ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
		float offset = d.is_empty ? 0 : ImGui::GetTreeNodeToLabelSpacing();
		ImGuiTreeNodeFlags fl = d.path == m_currentEntry.path().generic_string() ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx(("##" + d.name).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			if (ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(col));
				if (!d.is_empty)
					RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize * 1.75f) / 2.f - (fontSize * 0.40f * .75f), pos.y + (fontSize * 0.40f * .75f) / 2.f }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Down, .75f);
				if (ImGui::IsMouseReleased(0) && !isDragAndDrop && !isDragging && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > offset) SelectView(d);
			}
			else if (d.is_empty) {
					ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));
			}

			ImGui::Image((ImTextureID)m_textures["folderEmpty"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			ImGui::Text(d.name.c_str());

			AcceptDirPayload(d, { &pos.x });
			if (!d.is_empty) {
				for (const auto& child : d.children)
					CreateFolderView(child, fontSize);
			}
			ImGui::TreePop();
		}
		else {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			if (ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(col));
				if (!d.is_empty)
					RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize*1.75f)/2.f- (fontSize * 0.40f * .85f), pos.y+ (fontSize * 0.40f * .85f)/2.f}, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Right, .75f);
				if (!fl && ImGui::IsMouseReleased(0) && !isDragAndDrop && !isDragging && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > offset) SelectView(d);
			}
			else if (d.is_empty) {
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));
			}
			
			ImGui::Image((ImTextureID)m_textures["folderIcon" ]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			ImGui::Text(d.name.c_str());
			AcceptDirPayload(d, {&pos.x});
		}
	}

	static void RenderArrow(ImDrawList* drawList,  float fontSize, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale)
	{
		const float h = fontSize * 1.00f;
		float r = h * 0.40f * scale;
		ImVec2 center = { pos.x + h * 0.50f, pos.y + h * 0.50f * scale };
		ImVec2 a, b, c;
		switch (dir)
		{
		case ImGuiDir_Up:
		case ImGuiDir_Down:
			if (dir == ImGuiDir_Up) r = -r;
			a = { +0.000f * r, +0.750f * r };
			b = ImVec2(-0.866f*r, -0.750f * r);
			c = ImVec2(+0.866f*r, -0.750f * r);
			break;
		case ImGuiDir_Left:
		case ImGuiDir_Right:
			if (dir == ImGuiDir_Left) r = -r;
			a = ImVec2(+0.750f*r, +0.000f * r);
			b = ImVec2(-0.750f*r, +0.866f* r);
			c = ImVec2(-0.750f*r, -0.866f * r);
			break;
		case ImGuiDir_None:
		case ImGuiDir_COUNT:
			IM_ASSERT(0);
			break;
		}
		drawList->AddTriangleFilled({ center.x + a.x , center.y + a.y }, { center.x + b.x , center.y + b.y }, { center.x + c.x , center.y + c.y }, col);
	}

	void DirectoryModule::SelectView(const DirectoryPath& d)
	{
		
		m_selectedViewEntry = d.path;
		m_currentEntry = std::filesystem::directory_entry(m_selectedViewEntry);
		std::filesystem::directory_entry current_base = m_currentEntry.path().generic_string().find(m_scriptDirectoryBase.path().generic_string()) != std::string::npos ? m_scriptDirectoryBase : m_contentDirectoryBase;
		if (m_currentEntry.path() != current_base.path().parent_path()) {
			std::string s(&d.path[current_base.path().generic_string().size()]);
			if (s.size() <= 0) {
				depth = 0;
				return;
			}
			int de = 1;
			for (int i = 0; i < s.size() - 1; i++) {
				if (s[i] == '/') de++;
			}
			depth = de;
			
		}
		else {
			depth = 0;
		}
		m_selectedEntry = "";
		rename = false;
		
	}

	void DirectoryModule::ShowSetingsModal(const DirectoryPath& flags)
	{
		if (m_rightClicked != flags)
			m_rightClicked = flags;
		ImGui::OpenPopup("DirectoryModal");

	}

	void DirectoryModule::AcceptDirPayload(const DirectoryPath& p, const GEngine::Vector2f& pos)
	{
		
		if (ImGui::BeginDragDropTarget()) {

			const ImGuiPayload* _p = ImGui::GetDragDropPayload();
			if (_p && strcmp(_p->DataType, "DirectoryPath") == 0) {
				float availWidth = ImGui::GetContentRegionAvailWidth();
				float fontSize = ImGui::GetFontSize();
				ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				col.w = .25f;
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(col));
			}

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DirectoryPath");
			if (payload) {
				GE_CORE_DEBUG("ACCEPTED PAYLOAD: {0}", payload->Data);
				DirectoryPayload payloadPath = *(DirectoryPayload*)payload->Data;
				if (payloadPath.name != p.name) {
					GEngine::ThreadPool::AddMainThreadFunction([payloadPath, p, this]() {
						GEngine::FileSystem::MoveFile(payloadPath.path, p.path + "/" + payloadPath.name);
						forceFolderViewRefresh = true;
						});
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void DirectoryModule::AcceptDirPayloadFolder(const DirectoryPath& p, ImRect* rect)
	{

		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* _p = ImGui::GetDragDropPayload();
			if (_p && strcmp(_p->DataType, "DirectoryPath") == 0) {
				float availWidth = ImGui::GetContentRegionAvailWidth();
				float fontSize = ImGui::GetFontSize();
				ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				col.w = .25f;
				ImGui::GetWindowDrawList()->AddRectFilled(rect->Min, rect->Max, ImGui::GetColorU32(col));
			}
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DirectoryPath");
			if (payload) {
				GE_CORE_DEBUG("ACCEPTED PAYLOAD: {0}", payload->Data);
				DirectoryPayload payloadPath = *(DirectoryPayload*)payload->Data;
				if (payloadPath.name != p.name) {
					GEngine::ThreadPool::AddMainThreadFunction([payloadPath, p, this]() {
						GEngine::FileSystem::MoveFile(payloadPath.path, p.path + "/" + payloadPath.name);
						forceFolderViewRefresh = true;
						});
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	}
