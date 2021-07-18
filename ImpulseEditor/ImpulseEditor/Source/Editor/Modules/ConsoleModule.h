#pragma once
#include "EditorModule.h"

namespace Editor {

	class ConsoleModule : public EditorModule {
	public:
		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;
		ConsoleModule();
		~ConsoleModule();
		

	};


}
