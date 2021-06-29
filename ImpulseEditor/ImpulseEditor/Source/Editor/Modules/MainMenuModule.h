#pragma once


#include "EditorModule.h"

namespace Editor {

	class MainMenuModule : public EditorModule {
	public:
		MainMenuModule(std::unordered_map<std::string, EditorModuleData>* modules);
		~MainMenuModule();
		void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	private:
		std::unordered_map<std::string, EditorModuleData>* m_modules;



	};



}
