#include "HierarchyModule.h"
#include "imgui/imgui_internal.h"

#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorSceneEvents.h"

namespace Editor {


	static void RenderArrow(ImDrawList* drawList, float fontSize, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale);

	HierarchyModule::HierarchyModule(std::set<GEngine::ObjectHash>* selectedGameObject, std::string* sceneName) : m_selectedObject(selectedGameObject), m_sceneName(sceneName)
	{
		m_textures["scene"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/cubeStacked172x172.png");
		m_textures["gameObjectChildren"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/cubeStacked172x172.png");
		m_textures["gameObject"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/cube172x172.png");
		m_textures["gameObjectAdd"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/cubeAdd172x172.png");
		m_textures["gameObjectRemove"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/cubeRemove172x172.png");


		m_dispatchId = EditorLayer::GetDispatcher()->SubscribeEvent(EditorEventType::AllEvents, [this](const EditorEvent& e) {

			if (e.GetCategoryFlags() & EditorEventCategory::EventCategoryModification) {
				if (e.GetEventType() == EditorEventType::SceneLoad) {
					m_saved = true;
					return;
				}
				m_saved = false;
			} else if (e.GetEventType() == EditorEventType::SceneSave) {
				m_saved = true;
			}
		});
		
	}

	HierarchyModule::~HierarchyModule()
	{
		EditorLayer::GetDispatcher()->UnsubscribeEvent(EditorEventType::AllEvents, m_dispatchId);
	}

	void HierarchyModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		std::unordered_map<GEngine::ObjectHash, GEngine::Entity*> entities;
		if (!ImGui::Begin(name.c_str(), is_open, flags | ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::End();
			return;
		}

		GEngine::Scene* scene = GEngine::SceneManager::GetCurrentScene();
		bool hovering = false;
		for (const auto& e : scene->GetEntities()) {
			if (e.second->GetParent() == nullptr) {
				entities[e.first] = e.second;
			}
		}

		float fontSize = ImGui::GetFontSize();
		float availWidth = ImGui::GetContentRegionAvailWidth();
		ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
		bool hasChildren = entities.size();
		float offset = !hasChildren ? 0 : ImGui::GetTreeNodeToLabelSpacing();
		ImGuiTreeNodeFlags fl = 0;
		if (ImGui::ImageButton((ImTextureID)(intptr_t)m_textures["gameObjectAdd"]->GetRendererID(), { ImGui::GetFontSize(), ImGui::GetFontSize() }, { 0,1 }, { 1,0 })) {
			GEngine::Scene* sc = GEngine::SceneManager::GetCurrentScene();
			if (sc) {
				GEngine::Entity* e = sc->CreateEntity<GEngine::Entity>();
				e->SetSerialize(true);
				e->GetTransform()->SetSerialize(true);
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneAddEntity>(e->GetHash());
			}
		}
		ImGui::SameLine();
		char ch[255] = { 0 };
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * .85f);
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_Text));
		ImGui::InputTextWithHint("##filerHierachy", "Search" ,ch, 255);
		ImGui::PopStyleColor();
		if (m_selectedObject->size() > 0) {
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(intptr_t)m_textures["gameObjectRemove"]->GetRendererID(), { ImGui::GetFontSize(), ImGui::GetFontSize() }, { 0,1 }, { 1,0 })) {
				std::vector<GEngine::Entity*> e;
				for (const auto& p : *m_selectedObject) {
					GEngine::Entity* go = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(p.hash));
					GEngine::Entity* parent = dynamic_cast<GEngine::Entity*>(go->GetParent());
					bool skip = false;
					while (parent) {
						if (m_selectedObject->find(parent->GetHash()) != m_selectedObject->end()) {
							skip = true;
							break;
						}
						parent = parent->GetParent();
					}
					if (skip) continue;
					e.push_back(go);
				}
				
				
				GEngine::ThreadPool::AddMainThreadFunction([e,this]() {
					for (const auto& go : e) {
						if (go) {
							go->Destroy();
						}
					}

					EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneDestroyEntity>(GEngine::ObjectHash());
				});
				
				m_selectedObject->clear();
				
			}
		}
		ImGui::Separator();
		if (ImGui::TreeNodeEx("##Scene", ImGuiTreeNodeFlags_OpenOnArrow | fl | ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			bool isHovering = ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);
			ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, isHovering ? ImGui::GetColorU32(col) : ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));

			if (hasChildren) {
				RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize * 1.75f) / 2.f - (fontSize * 0.40f * .75f), pos.y + (fontSize * 0.40f * .75f) / 2.f }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Down, .75f);
			}

			ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			if (m_sceneName->size() > 0) {
				if (!m_saved) {
					std::string n = *m_sceneName + "*";
					ImGui::Text(n.c_str());
				}
				else ImGui::Text(m_sceneName->c_str());
			}
			else {
				ImGui::Text("Untitled Scene");
			}
			AcceptPayload({ 0, nullptr }, {pos.x, pos.y});
			if (hasChildren) {
				for (const auto& e : entities) {
					AddEntity(e, entities, &hovering);
				}
			}

			ImGui::TreePop();
		}
		else {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			bool isHovering = ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);
			ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, isHovering ? ImGui::GetColorU32(col) : ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));

			if (hasChildren)
				RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize * 1.75f) / 2.f - (fontSize * 0.40f * .85f), pos.y + (fontSize * 0.40f * .85f) / 2.f }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Right, .75f);

			ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			if (m_sceneName->size() > 0) {
				if (!m_saved) {
					std::string n = *m_sceneName + "*";
					ImGui::Text(n.c_str());
				}
				else ImGui::Text(m_sceneName->c_str());
			}
			else {
				ImGui::Text("Untitled Scene");
			}
			
			AcceptPayload({ 0, nullptr }, { pos.x, pos.y });
		}

		if (!hovering) {
			if (ImGui::IsMouseReleased(0) && ImGui::IsWindowHovered()) {
				m_selectedObject->clear();
				m_rightSelectedObject = {};

			}
		}

		RightClickPopup(static_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(m_rightSelectedObject)));

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
	}

	void HierarchyModule::AddEntity(const std::pair<GEngine::ObjectHash, GEngine::Entity*>& e, std::unordered_map<GEngine::ObjectHash, GEngine::Entity*>& entities, bool* hoveringObject)
	{
		float fontSize = ImGui::GetFontSize();
		float availWidth = ImGui::GetContentRegionAvailWidth();
		ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
		bool hasChildren = e.second->GetChildren().size() > 0;
		float offset = !hasChildren ? 0 : ImGui::GetTreeNodeToLabelSpacing();
		const auto& it = m_selectedObject->find(e.first);
		ImGuiTreeNodeFlags fl = it != m_selectedObject->end() ? ImGuiTreeNodeFlags_Selected : 0;
		if (ImGui::TreeNodeEx(("##" + e.first.ToString()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | fl)) {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			bool isHovering = ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);
			ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, isHovering ? ImGui::GetColorU32(col) : ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));

			if (hasChildren) {
				RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize * 1.75f) / 2.f - (fontSize * 0.40f * .75f), pos.y + (fontSize * 0.40f * .75f) / 2.f }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Down, .75f);
			}

			if (isHovering) {

				if ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > offset) {

					if (ImGui::IsMouseReleased(0)) {
						if (m_selectedObject->size() >= 1) {
							if (ImGui::IsKeyDown(GE_KEY_LEFT_CONTROL)) {
								if (it == m_selectedObject->end())
									m_selectedObject->insert(e.first);
								else m_selectedObject->erase(it);
							}
							else if (GEngine::Input::IsKeyDown(GE_KEY_LEFT_SHIFT)) {
								if (m_selectedObject->size() == 1) {

								}
								else {
									m_selectedObject->clear();
									m_selectedObject->insert(e.first);
								}
							}
							else {
								m_selectedObject->clear();
								m_selectedObject->insert(e.first);
							}
						}
						else {
							m_selectedObject->insert(e.first);
						}
					}
					if (ImGui::IsMouseReleased(1)) {
						openPopup = true;
						m_rightSelectedObject = e.first;
					}

				}
				*hoveringObject = true;
			}

			ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			ImGui::Text(e.second->GetTag().c_str());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				EntityPayload payload = { e.second };
				ImGui::SetDragDropPayload(EntityPayload::GetName(), (void*)&payload, sizeof(EntityPayload));
				ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				ImGui::Text(e.second->GetTag().c_str());
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
				ImGui::Text(("(" + e.first.ToString() + ")").c_str());
				ImGui::PopStyleColor();
				ImGui::EndDragDropSource();


			}
			AcceptPayload(e, { pos.x,pos.y });

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			ImGui::Text(("(" + e.first.ToString() + ")").c_str());
			ImGui::PopStyleColor();
			
			if (hasChildren) {
				for (const auto& p : e.second->GetChildren()) {
					AddEntity(p, entities, hoveringObject);
				}
			}

			ImGui::TreePop();
		}
		else {
			ImGui::SameLine();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			bool isHovering = ImGui::IsMouseHoveringRect({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);
			ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, isHovering ? ImGui::GetColorU32(col) : ImGui::GetColorU32(ImGui::GetStyleColorVec4((fl & ImGuiTreeNodeFlags_Selected) ? ImGuiCol_Header : ImGuiCol_WindowBg)));
			
			if (hasChildren)
				RenderArrow(ImGui::GetWindowDrawList(), fontSize, { pos.x - (fontSize * 1.75f) / 2.f - (fontSize * 0.40f * .85f), pos.y + (fontSize * 0.40f * .85f) / 2.f }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)), ImGuiDir_Right, .75f);

			if (isHovering) {
				if ((ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > offset) {
					if (ImGui::IsMouseReleased(0)) {
						if (m_selectedObject->size() >= 1) {
							
							if (ImGui::IsKeyDown(GE_KEY_LEFT_CONTROL)) {
								if (it == m_selectedObject->end()) m_selectedObject->insert(e.first);
								else m_selectedObject->erase(it);
							}
							else {
								m_selectedObject->clear();
								m_selectedObject->insert(e.first);
							}
						}
						else {
							m_selectedObject->insert(e.first);
						}
					}
					if (ImGui::IsMouseReleased(1)) {
						openPopup = true;
						m_rightSelectedObject = e.first;
					}
				}
				*hoveringObject = true;
			}
			
			ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
			ImGui::SameLine();
			ImGui::Text(e.second->GetTag().c_str());
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
				EntityPayload payload = { e.second };
				ImGui::SetDragDropPayload(EntityPayload::GetName(), (void*)&payload, sizeof(EntityPayload));
				ImGui::Image((ImTextureID)(intptr_t)m_textures[hasChildren ? "gameObjectChildren" : "gameObject"]->GetRendererID(), { fontSize,fontSize }, { 0,1 }, { 1,0 });
				ImGui::SameLine();
				ImGui::Text(e.second->GetTag().c_str());
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
				ImGui::Text(("(" + e.first.ToString() + ")").c_str());
				ImGui::PopStyleColor();
				ImGui::EndDragDropSource();
			}

			AcceptPayload(e, { pos.x,pos.y });

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			ImGui::Text(("(" + e.first.ToString() + ")").c_str());
			ImGui::PopStyleColor();

		}

	}

	void HierarchyModule::AcceptPayload(const std::pair<GEngine::ObjectHash, GEngine::Entity*>& e, const GEngine::Vector2f& pos)
	{
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* p = ImGui::GetDragDropPayload();
			if (p && strcmp(p->DataType, EntityPayload::GetName()) == 0) {
				float availWidth = ImGui::GetContentRegionAvailWidth();
				float fontSize = ImGui::GetFontSize();
				ImVec4 col = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
				col.w = .25f;
				ImGui::GetWindowDrawList()->AddRectFilled({ pos.x - ImGui::GetTreeNodeToLabelSpacing(), pos.y }, { pos.x + availWidth, pos.y + fontSize }, ImGui::GetColorU32(col));
			}

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EntityPayload::GetName());
			if (payload) {
				
				EntityPayload payloadObj = *(EntityPayload*)payload->Data;
				if (payloadObj.entity != e.second) {
					GEngine::Entity* child = payloadObj.entity;
					if (child) {
						if (child->GetParent() == e.second) {
							child->SetParent(e.second->GetParent());
						}
						else {
							child->SetParent(e.second);
						}
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(e.second->GetHash(), GameObjectModifications::ADD_CHILD);
						
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	void HierarchyModule::RightClickPopup(GEngine::Entity* e)
	{
		if (openPopup) {
			openPopup = false;
			ImGui::OpenPopup("HierarchyPopup");
		}

		if (ImGui::BeginPopup("HierarchyPopup", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove)) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			
			ImGui::Button("Duplicate");
			float yPos = ImGui::GetCursorScreenPos().y;
			if (ImGui::Button("Components >")) {
				ImGui::OpenPopup("HierarchyComponentPopup");
				ImVec2 pos = ImGui::GetWindowPos();
				ImGui::SetNextWindowPos({ pos.x + ImGui::GetWindowWidth(), yPos });
			}

			if (ImGui::BeginPopup("HierarchyComponentPopup", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove)) {
				float fontSize = ImGui::GetFontSize();
				float width = ImGui::GetContentRegionAvailWidth();
				for (const auto& c : e->GetComponents()) {
					ImVec2 pos = ImGui::GetCursorScreenPos();
					if (ImGui::IsMouseHoveringRect({ pos.x, pos.y }, { pos.x + width, pos.y + fontSize }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow)) {
						ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + width, pos.y + fontSize }, ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered)));
					}
					ImGui::Text((c.second->GetTag() + " (" + c.first.ToString() + ")").c_str());
					
					if (c.second->GetTag() == "Native Script Component") {
						GEngine::NativeScriptComponent* comp = dynamic_cast<GEngine::NativeScriptComponent*>(c.second);

						if (comp) {
							if (comp->IsValid()) {
								GEngine::Component* sc = comp->GetComponent();
								if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
									ComponentPayload payload = { sc};
									ImGui::SetDragDropPayload(ComponentPayload::GetName(), (void*)&payload, sizeof(EntityPayload));
									ImGui::Text((c.second->GetTag() + " (" + c.first.ToString() + ")").c_str());
									ImGui::EndDragDropSource();
								}
							}
						}
					}
					else if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ComponentPayload payload = { c.second };
						ImGui::SetDragDropPayload(ComponentPayload::GetName(), (void*)&payload, sizeof(EntityPayload));
						ImGui::Text((c.second->GetTag() + " (" + c.first.ToString() + ")").c_str());
						ImGui::EndDragDropSource();
					}
					
				}

				ImGui::EndPopup();
			}

			if (ImGui::Button("Delete")) {

				GEngine::ThreadPool::AddMainThreadFunction([e, this]() {
					if (e) {
						const auto& it = m_selectedObject->find(e->GetHash());
						if (it != m_selectedObject->end()) {
							m_selectedObject->erase(it);
						}
						e->Destroy();
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneDestroyEntity>(e->GetHash());
					}
				});
				ImGui::CloseCurrentPopup();
			}
			
			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}
	
	

	}

	static void RenderArrow(ImDrawList* drawList, float fontSize, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale)
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
			b = ImVec2(-0.866f * r, -0.750f * r);
			c = ImVec2(+0.866f * r, -0.750f * r);
			break;
		case ImGuiDir_Left:
		case ImGuiDir_Right:
			if (dir == ImGuiDir_Left) r = -r;
			a = ImVec2(+0.750f * r, +0.000f * r);
			b = ImVec2(-0.750f * r, +0.866f * r);
			c = ImVec2(-0.750f * r, -0.866f * r);
			break;
		case ImGuiDir_None:
		case ImGuiDir_COUNT:
			IM_ASSERT(0);
			break;
		}
		drawList->AddTriangleFilled({ center.x + a.x , center.y + a.y }, { center.x + b.x , center.y + b.y }, { center.x + c.x , center.y + c.y }, col);
	}


}