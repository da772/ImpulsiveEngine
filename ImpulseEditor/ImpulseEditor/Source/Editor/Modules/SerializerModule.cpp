#include "SerializerModule.h"

#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorEvents.h"
#include "Editor/Events/EditorSceneEvents.h"

#include "CXML/CXML.hpp"

namespace Editor {

	SerializerModule::SerializerModule()
	{
		eventCallbacks.push_back(EditorLayer::GetDispatcher()->SubscribeEvent(EditorEventType::ApplicationPlayEvent, [this](const Editor::EditorEvent& e) {
			if (scene_name.size() > 0) {
				Save();
			}
		}));


		eventCallbacks.push_back(EditorLayer::GetDispatcher()->SubscribeEvent(EditorEventType::ApplicationStopEvent, [this](const Editor::EditorEvent& e) {
		if (scene_name.size() > 0) {
			GEngine::ThreadPool::AddMainThreadFunction([this]() {
				Load(scene_name, scene_location);
				});
		} }));

	}

	SerializerModule::~SerializerModule()
	{
		EditorLayer::GetDispatcher()->UnsubscribeEvent(EditorEventType::ApplicationPlayEvent, eventCallbacks[0]);
		EditorLayer::GetDispatcher()->UnsubscribeEvent(EditorEventType::ApplicationStopEvent, eventCallbacks[1]);
	}

	void SerializerModule::Load(const std::string& scene, const std::string& path)
	{
		
		scene_name = scene;
		scene_location = path;

		GEngine::Ref<GEngine::FileData> fd = GEngine::FileSystem::FileDataFromPath(path, false);
		Load(fd);
	}

	void SerializerModule::Load(GEngine::Ref<GEngine::FileData> fd)
	{

		using namespace GEngine;

		std::string str((char*)fd->GetData(), fd->GetDataSize());
		size_t pos = 0;
		CXML cxml;
		CXML::CXML_Node node = cxml.GetNext(str.c_str(), 0);
		std::unordered_map<ObjectHash, SerializedEntity> gameObjects;
		std::unordered_map<ObjectHash, ObjectHash> compMap;
		// Entity loop
		while (node.type.size() > 1) {

			ObjectHash hash(node.tags["id"].c_str());
			ObjectHash tId(node.tags["tId"].c_str());
			ObjectHash parent(node.tags["parent"].c_str());
			std::unordered_map<ObjectHash, SerializedComponent> components = {};
			CXML::CXML_Node component = cxml.GetNext(node.info.c_str());
			while (component.type.size() > 1) {
				ObjectHash _hash(component.tags["id"].c_str());
				ObjectHash _phash(component.tags["parent"].c_str());
				std::string _ctag = component.tags["tag"];
				const ComponentTypes& type = GetComponentType(_ctag);
				if (type == ComponentTypes::NativeScriptComponent) {
					CXML::CXML_Node n = cxml.GetNext(component.info.c_str());
					ObjectHash __childHash = ObjectHash(n.tags["id"].c_str());
					if (__childHash.isValid()) {
						components[__childHash] = { __childHash, hash, _hash, nullptr, n.type, component.info, true };
						compMap[__childHash] = hash;
					}
				}
				components[_hash] = { _hash, hash, _phash, nullptr, _ctag, component.info };
				compMap[_hash] = hash;
				component = cxml.GetNext(node.info.c_str(), component.endPos);
			}
			gameObjects[hash] = { hash, parent, tId, nullptr, node.tags["tag"], components };
			node = cxml.GetNext(str.c_str(), node);
		}

		Scene* sc = SceneManager::GetCurrentScene();
		sc->RemoveAllEntities();

		for (auto& p : gameObjects) {
			if (!p.second.ptr) {
				p.second.ptr = sc->CreateEntity<Entity>(p.first, p.second.tag, p.second.tId);
				ObjectHash& parentHash = p.second.parent;
				Entity* self = dynamic_cast<Entity*>(p.second.ptr);
				self->SetSerialize(true);
				self->GetTransform()->SetSerialize(true);
				while (parentHash.isValid()) {
					SerializedEntity& parent = gameObjects[parentHash];
					GameObject* pGo = parent.ptr;
					if (!pGo) {
						Entity* pE = sc->CreateEntity<Entity>(parent.hash, parent.tag, parent.tId);
						pGo = pE;
						pGo->SetSerialize(true);
						pE->GetTransform()->SetSerialize(true);
						parent.ptr = pGo;
					}
					Entity* _parent = dynamic_cast<Entity*>(pGo);
					if (_parent) {
						_parent->AddChild(self);
					}
					self = _parent;
					parentHash = parent.parent;
				}
			}
		}

		for (auto& p : gameObjects) {
			GE_CORE_ASSERT(p.second.ptr, "GameObject not created: \n"+std::string((const char*)fd->GetData()));

			if (p.second.ptr) {
				for (auto& c : p.second.components) {
					DeserializeComponent(gameObjects, compMap, c.second);
				}
			}

		}
		
	}

	void SerializerModule::DeserializeComponent(std::unordered_map<GEngine::ObjectHash, SerializedEntity>& objects, std::unordered_map<GEngine::ObjectHash, GEngine::ObjectHash>& compMap, SerializedComponent& c)
	{
		using namespace GEngine;


		if (!c.wasCreated) {
			if (c.parentComponent.isValid()) {
				DeserializeComponent(objects, compMap, objects[compMap[c.parentComponent]].components[c.parentComponent]);
				if (c.wasCreated) return;
			}

			CXML cxml;
			const ComponentTypes& type = ComponentTypeMap.at(c.tag);
			Entity* ent = dynamic_cast<Entity*>(objects[c.parentEntity].ptr);
			GameObject* obj = nullptr;
			c.wasCreated = true;
			switch (type) {
			default:
			{
				break;
			}
			case ComponentTypes::TransformComponent: {
				obj = ent->GetTransform();
				CXML::CXML_Node node = cxml.GetNext(c.data.c_str());
				std::vector<float> data = {};
				while (node.type.size() > 1) {
					data.push_back(std::stof(node.info));
					node = cxml.GetNext(c.data.c_str(), node.endPos);
				}

				Vector3f pos(&data[0]);
				Vector3f rot(&data[3]);
				Vector3f scale(&data[6]);

				Transform* trans = dynamic_cast<Transform*>(obj);

				if (trans) {
					trans->SetLocalPosition(pos);
					trans->SetLocalRotation(rot);
					trans->SetLocalScale(scale);
				}
				c.ptr = trans;
				break;
			}
			case ComponentTypes::NativeScriptComponent: {
				CXML::CXML_Node node = cxml.GetNext(c.data.c_str());
				std::vector<std::pair<std::string, SerializedNativeField>> data = {};
				NativeScriptComponent* sc = ent->AddComponentHash<NativeScriptComponent>(c.hash, "");
				sc->SetSerialize(true);
				obj = sc;
				while (node.type.size() > 1) {
					SerializedNativeField d = { node.type, node.tags["name"], node.info };
					NativeTypes type = NativeTypeMap.at(node.type);
					try {
						switch (type) {

						case NativeTypes::CLASS: {
							ObjectHash objHash = ObjectHash(node.tags["id"].c_str());


							if (objHash.isValid()) {
								sc->SetAutoNative(false);
								sc->LoadClass(d.data, objHash);
							}
							else {
								objects[c.parentEntity].components[objHash].wasCreated = true;
								return;
							}
							objects[c.parentEntity].components[objHash].ptr = sc->GetComponent();
							objects[c.parentEntity].components[objHash].wasCreated = true;
							break;
						}
						case NativeTypes::INT:
							sc->GetNativeObject()->SetMember<int>(d.name, std::stoi(d.data));
							break;
						case NativeTypes::INT8:
							sc->GetNativeObject()->SetMember<int8_t>(d.name, (int8_t)std::stoi(d.data));
							break;
						case NativeTypes::INT16:
							sc->GetNativeObject()->SetMember<int16_t>(d.name, (int16_t)std::stoi(d.data));
							break;
						case NativeTypes::INT32:
							sc->GetNativeObject()->SetMember<int32_t>(d.name, (int32_t)std::stoi(d.data));
							break;
						case NativeTypes::INT64:
							sc->GetNativeObject()->SetMember<int64_t>(d.name, (int64_t)std::stoi(d.data));
							break;
						case NativeTypes::UINT:
							sc->GetNativeObject()->SetMember<uint32_t>(d.name, (uint32_t)std::stoi(d.data));
							break;
						case NativeTypes::UINT8:
							sc->GetNativeObject()->SetMember<uint8_t>(d.name, (uint8_t)std::stoi(d.data));
							break;
						case NativeTypes::UINT16:
							sc->GetNativeObject()->SetMember<uint16_t>(d.name, (uint16_t)std::stoi(d.data));
							break;
						case NativeTypes::UINT32:
							sc->GetNativeObject()->SetMember<uint32_t>(d.name, (uint32_t)std::stoi(d.data));
							break;
						case NativeTypes::UINT64:
							sc->GetNativeObject()->SetMember<uint64_t>(d.name, (uint64_t)std::stoi(d.data));
							break;
						case NativeTypes::DOUBLE:
							sc->GetNativeObject()->SetMember<double>(d.name, std::stod(d.data));
							break;
						case NativeTypes::FLOAT:
							sc->GetNativeObject()->SetMember<float>(d.name, std::stof(d.data));
							break;
						case NativeTypes::GAMEOBJECT:
						{
							ObjectHash gohash = ObjectHash(d.data.c_str());
							const int t = std::stoi(node.tags["component"]);
							// component
							if (t == 1) {

								const auto it = compMap.find(gohash);
								if (it != compMap.end()) {
									SerializedEntity& _ent = objects[it->second];
									const auto it2 = _ent.components.find(gohash);
									if (it2 != _ent.components.end()) {
										SerializedComponent& comp = _ent.components[gohash];

										if (!comp.wasCreated) {
											DeserializeComponent(objects, compMap, comp);
										}

										if (comp.ptr) {
											void* ptr = comp.ptr;
											sc->GetNativeObject()->SetMember<void*>(d.name, ptr);
										}
									}
								}
							}
							// entity
							else if (t == 0) {
								sc->GetNativeObject()->SetMember<void*>(d.name, objects[gohash].ptr);
							}
							break;
						}
						case NativeTypes::STRING:
							sc->GetNativeObject()->SetMember<std::string>(d.name, d.data);
							break;
						case NativeTypes::TEXTURE2D:
							break;
						default:
						case NativeTypes::NONE:
							break;

						}
					}
					catch (const std::exception& e) {
						GE_LOG_ERROR("SerializeException: {0} {1}:{2} \n{3}", e.what() ,d.name, d.type, d.data);
					}
					if (sc) {
						sc->SetNativePointerData();
					}

					node = cxml.GetNext(c.data.c_str(), node.endPos);
				}

				break;
			}

			}
		}
	}


	struct EntityComp {
		bool operator()(const GEngine::Entity* a, const GEngine::Entity* b) {
			return a->GetHash() < b->GetHash();
		}
	};

	struct ComponentComp {
		bool operator()(const GEngine::Component* a, const GEngine::Component* b) {
			return a->GetHash() < b->GetHash();
		}
	};

	bool SerializerModule::Save()
	{
		if (scene_name.size() > 0) {

			GEngine::Ref<GEngine::FileData> d = SerializeCurrentScene();
			
			std::ofstream out(scene_location, std::ios::trunc);
			out.write((const char*)d->GetData(), d->GetDataSize());
			out.close();
			EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneSaveEvent>();
			return true;

		}
		else {
			GE_LOG_ERROR("Serailizer Module: Cannot save scene without specifiying name and path");
		}
		return false;
	}

	void SerializerModule::SaveAs(const std::string& name, const std::string& path)
	{
		scene_name = name;
		scene_location = path;
		Save();
	}

	GEngine::Ref<GEngine::FileData> SerializerModule::SerializeCurrentScene()
	{
		using namespace GEngine;

		Scene* scene = SceneManager::GetCurrentScene();

		const auto& entities = scene->GetEntities();

		std::string serial = "";

		std::priority_queue <Entity*, std::vector<Entity*>, EntityComp> sorted_ent;

		for (const auto& e : entities) {
			if (e.second->DoesSerialize())
				sorted_ent.push(e.second);
		}

		while (!sorted_ent.empty()) {

			Entity* e = sorted_ent.top();
			sorted_ent.pop();
			serial += "<Entity id=\"" + e->GetHash().ToString() + "\" tId=\"" + e->GetTransform()->GetHash().ToString() + "\" tag=\"" + e->GetTag() + "\" parent=\"" + (e->GetParent() ? e->GetParent()->GetHash().ToString() : "") + "\">";

			std::priority_queue <Component*, std::vector<Component*>, ComponentComp> sorted_comp;

			for (const auto& c : e->GetComponents()) {
				if (c.second->DoesSerialize())
					sorted_comp.push(c.second);
			}

			while (!sorted_comp.empty()) {
				Component* c = sorted_comp.top();
				sorted_comp.pop();
				serial += "\n\t<Component id=\"" + c->GetHash().ToString() + "\" tag=\"" + c->GetTag() + "\" parent=\"" + c->IsAttatched().ToString() + "\">\n";

				// Do something
				serial += c->Serialize(2);

				serial += "\n\t</Component>";

			}

			serial += "\n</Entity>\n";
		}

		char* data = (char*)calloc(serial.size()+1, sizeof(char));
		memcpy(data, &serial[0], serial.size());

		return std::make_shared<GEngine::FileData>(serial.size(), (unsigned char*)data);



	}

	bool SerializerModule::IsSceneLoaded() const
	{
		return scene_name.size() > 0 && scene_location.size() > 0;
	}

	const std::string* SerializerModule::GetName() const
	{
		return &scene_name;
	}

	const std::string* SerializerModule::GetPath() const
	{
		return &scene_location;
	}

	void SerializerModule::SetName(const std::string& name)
	{
		scene_name = name;
	}

	void SerializerModule::SetPath(const std::string& path)
	{
		scene_location = path;
	}

	}