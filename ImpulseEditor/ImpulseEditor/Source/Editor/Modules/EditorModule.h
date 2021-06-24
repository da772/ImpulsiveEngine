#pragma once
#include <GEngine.h>


namespace Editor {

	class EditorModule {

	public:
		inline const std::string& GetName() const { return moduleName; }


	protected:
		std::string moduleName;
		

	};


}
