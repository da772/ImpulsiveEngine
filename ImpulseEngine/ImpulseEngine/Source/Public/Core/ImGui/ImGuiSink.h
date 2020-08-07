#pragma once

#include "spdlog/sinks/base_sink.h"
#include "Public/Core/ImGui/ImGuiLog.h"

namespace GEngine {
	template<typename Mutex>
	class ImGuiSink : public spdlog::sinks::base_sink <Mutex>
	{
	public:
		ImGuiSink(ImGuiAppLog& log)
			: log(log) {};
	protected:
		inline void sink_it_(const spdlog::details::log_msg& msg) override
		{

			// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
			// msg.raw contains pre formatted log
			
			// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
			fmt::memory_buffer formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			log.AddLog(fmt::to_string(formatted), msg.level);
			//std::cout << fmt::to_string(formatted);
			//log.AddLog(fmt::to_string(formatted));
		}

		void flush_() override
		{
			std::cout << std::flush;
		}
		ImGuiAppLog& log;
	};

};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using ImGui_sink_mt = GEngine::ImGuiSink<std::mutex>;
using ImGui_sink_st = GEngine::ImGuiSink<spdlog::details::null_mutex>;
