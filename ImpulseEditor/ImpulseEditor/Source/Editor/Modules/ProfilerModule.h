#pragma once
#include "EditorModule.h"

namespace Editor {

	class ProfilerModule : public EditorModule
	{
	public:
		ProfilerModule();
		~ProfilerModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	protected:
		std::string moduleName;
	};


}

