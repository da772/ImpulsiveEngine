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
		int maxHistory = 30;
		int fpsMax = 0;
		uint64_t lastUpdate = 0;
		uint64_t updateTime = 1000;
		std::string moduleName;
		std::vector<uint64_t> fpsHistory;
		std::vector<uint64_t> historyGraph;
	};


}

