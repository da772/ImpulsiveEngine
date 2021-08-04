#pragma once
#include "EditorModule.h"

namespace Editor {


	struct SerializedComponent {
		GEngine::ObjectHash hash;
		GEngine::ObjectHash parentEntity;
		GEngine::ObjectHash parentComponent;
		GEngine::GameObject* ptr;
		std::string tag;
		std::string data;
		bool nsChild = false;
		bool wasCreated = false;
	};


	struct SerializedEntity {
		GEngine::ObjectHash hash;
		GEngine::ObjectHash parent;
		GEngine::ObjectHash tId;
		GEngine::GameObject* ptr;
		std::string tag;
		std::unordered_map<GEngine::ObjectHash, SerializedComponent> components;
	};

	struct SerializedNativeField {
		std::string type;
		std::string name;
		std::string data;
	};

	enum class ComponentTypes : uint8_t {
		NONE = 0, TransformComponent, SpriteComponent, UIComponent, AudioComponent, NativeScriptComponent
	};


	enum class NativeTypes : uint8_t {
		NONE = 0, CLASS, INT, INT8, INT16, INT32, INT64, UINT, UINT8, UINT16, UINT32, UINT64, DOUBLE, FLOAT, GAMEOBJECT, STRING, TEXTURE2D,
	};
	
	const std::unordered_map<std::string, NativeTypes> NativeTypeMap = { {"class", NativeTypes::CLASS}, {"int", NativeTypes::INT}, {"int8", NativeTypes::INT8},
	{"int16", NativeTypes::INT16}, {"int32", NativeTypes::INT32}, {"int64", NativeTypes::INT64}, {"uint", NativeTypes::UINT}, {"uint8", NativeTypes::UINT8},
	{"uint16", NativeTypes::UINT16}, {"uint32", NativeTypes::UINT32}, {"uint64", NativeTypes::UINT64}, {"string", NativeTypes::STRING},  {"GameObject", NativeTypes::GAMEOBJECT},
	{"Texture2D", NativeTypes::TEXTURE2D},{"float", NativeTypes::FLOAT}, {"double", NativeTypes::DOUBLE} };

	const std::unordered_map<std::string, ComponentTypes> ComponentTypeMap = { {"None", ComponentTypes::NONE }, {"Transform Component", ComponentTypes::TransformComponent},
		{"Sprite Component", ComponentTypes::SpriteComponent },
		{"UI Component", ComponentTypes::UIComponent },
		{"Audio Component", ComponentTypes::AudioComponent },
		{"Native Script Component", ComponentTypes::NativeScriptComponent }

	};

	static ComponentTypes GetComponentType(const std::string& type) {
		auto it = ComponentTypeMap.find(type);

		if (it != ComponentTypeMap.end()) {
			return it->second;
		}
		return ComponentTypes::NONE;
	}




	class SerializerModule : public EditorModule {
	public:
		SerializerModule();
		~SerializerModule();
		inline void Create(const std::string& name, bool* is_open, uint32_t flags) override {};
		void Load(const std::string& scene, const std::string& path);
		bool Save();
		void SaveAs(const std::string& name, const std::string& path);

		const std::string& GetName() const;
		const std::string& GetPath() const;
		void SetName(const std::string& name);
		void SetPath(const std::string& path);

	private:
		void DeserializeComponent(std::unordered_map<GEngine::ObjectHash, SerializedEntity>& objects, std::unordered_map<GEngine::ObjectHash, GEngine::ObjectHash>& compMap, SerializedComponent& c);
		std::vector<uint64_t> eventCallbacks;

	private:
		std::string scene_name = "";
		std::string scene_location = "";


	};









}