#pragma once
#include "EditorModule.h"

namespace Editor {
	
	class InspectorModule : public EditorModule {

	public:
		InspectorModule();
		~InspectorModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	private:


	};


}
