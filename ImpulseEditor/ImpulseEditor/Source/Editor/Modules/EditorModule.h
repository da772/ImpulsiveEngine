#pragma once
#include <GEngine.h>


namespace Editor {


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
