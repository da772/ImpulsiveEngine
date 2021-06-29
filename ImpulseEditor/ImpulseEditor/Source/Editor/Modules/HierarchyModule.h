#pragma once


#include "EditorModule.h"


namespace Editor {

	class HierarchyModule : public EditorModule {
	public:
		HierarchyModule();
		~HierarchyModule();

		void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	};

}