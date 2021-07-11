#pragma once
#ifndef GE_GRAPHICS_API_NONE
#include <imgui.h>

namespace GEngine{

	// Usage:
	//  static ExampleAppLog my_log;
	//  my_log.AddLog("Hello %d world\n", 123);
	//  my_log.Draw("title");
	class GE_API ImGuiAppLog
	{
    public:
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool                ScrollToBottom;
		ImVector<int>	LevelOffsets;
		ImVector<ImVec4>	Levels;


		ImGuiAppLog();

		void Clear();

		void AddLog(const char* fmt, ...) IM_FMTARGS(2);

		void AddLog(std::string fmt, int level);

		void Draw(const char* title, bool* p_open = NULL);

		std::string GetLastLog();
		int GetLastLevel();
	};


}

#endif
