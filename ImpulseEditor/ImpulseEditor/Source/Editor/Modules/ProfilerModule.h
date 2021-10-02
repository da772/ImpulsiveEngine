#ifdef GE_EDITOR
#pragma once
#include "EditorModule.h"

namespace Editor {

	struct ProfilerTimingGraph {

		std::string displayName;
		std::string dataName;
		GEngine::Vector4f color = { 0.f,0.f,0.f,1.f };
		std::vector<double> data = {};
	
	};

	class ProfilerModule : public EditorModule
	{
	public:
		ProfilerModule();
		~ProfilerModule();

		virtual void Create(const std::string& name, bool* is_open, uint32_t flags) override;

	protected:
		int maxHistory = 30;
		uint64_t lastUpdate = 0;
		uint64_t updateTime = 1000;
		std::string moduleName;
		std::vector<ProfilerTimingGraph> timingGraphs;
		std::vector<uint64_t> fpsHistory;
		std::vector<uint64_t> historyGraph;
		std::vector<double> timingHistoryGraph;
	};


}

#endif