#pragma once
#include <GEngine.h>


namespace Editor {

	struct GameObjectPayload {
		GEngine::Entity* entity = nullptr;
	};

	struct DirectoryPath {
		std::string path;
		std::string name;
		std::string ext;
		bool is_directory;
		bool is_empty;

		inline bool operator ==(const DirectoryPath& other) {
			return other.path == path;
		}

		inline bool operator ==(const std::string& other) {
			return other == path;
		}
		inline bool operator !=(const DirectoryPath& other) {
			return other.path != path;
		}

	};

	struct DirectoryPayload {

		inline DirectoryPayload(const DirectoryPath& p) {
			memcpy(path, p.path.c_str(), p.path.size());
			memcpy(name, p.name.c_str(), p.name.size());
			memcpy(ext, p.ext.c_str(), p.ext.size());
			is_directory = p.is_directory;
			is_empty = is_empty;
		}

		char path[2048] = { '\0' };
		char name[256] = { '\0' };
		char ext[64] = { '\0' };
		bool is_directory;
		bool is_empty;

	};



	struct DirectoryRecurse {
		DirectoryPath self;
		std::vector<DirectoryRecurse> children;
	};




	class EditorModule {

	public:
		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) = 0;
		inline const std::string& GetName() const { return moduleName; }


	protected:
		std::string moduleName;
		bool isOpen = true;

	};


	struct EditorModuleData {
		GEngine::Ref<EditorModule> data;
		bool isOpen = true;
		int flags = 0;
		bool toggable = true;

		template<typename T = EditorModule>
		inline GEngine::Ref<T> DataAs() {
			return std::dynamic_pointer_cast<T>(data);
		}

	};


}
