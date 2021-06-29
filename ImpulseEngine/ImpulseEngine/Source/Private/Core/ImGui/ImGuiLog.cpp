#include "gepch.h"
#include "Public/Core/ImGui/ImGuiLog.h"
#include "imgui/imgui_internal.h"
#ifndef GE_GRAPHICS_API_NONE
namespace GEngine {


	ImGuiAppLog::ImGuiAppLog()
	{
		Levels.push_back(ImVec4(0, 255, 0, 1)); // Info White
		Levels.push_back(ImVec4(0, 230, 255, 1)); // Debug Blue
		Levels.push_back(ImVec4(255, 255, 255, 1)); // Trace Green
		Levels.push_back(ImVec4(255, 255, 0, 1)); // Warning Yellow
		Levels.push_back(ImVec4(255, 0, 0, 1)); // Error Red

		ScrollToBottom = false;
		Clear();
	}


	void ImGuiAppLog::Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
		LevelOffsets.clear();
		LevelOffsets.push_back(0);
	}

	void ImGuiAppLog::AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
		ScrollToBottom = true;
	}

	void ImGuiAppLog::AddLog(std::string fmt, int level)
	{
		int old_size = Buf.size();
		Buf.append(fmt.c_str());
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
		{
			if (Buf[old_size] == '\n') {
				LineOffsets.push_back(old_size + 1);
				LevelOffsets.push_back(level);
			}

		}

		ScrollToBottom = true;
	}

	void ImGuiAppLog::Draw(const char* title, bool* p_open)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy)
		{
			ImGui::SetClipboardText(Buf.begin());
		}


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end)) {
					ImGui::PushStyleColor(ImGuiCol_Text, Levels[LevelOffsets[(line_no + 1 < LineOffsets.Size) ? line_no + 1 : LevelOffsets.Size - 1]]);
					ImGui::TextUnformatted(line_start, line_end);
					ImGui::PopStyleColor();
				}

			}
		}
		else
		{
			// The simplest and easy way to display the entire buffer:
			//   ImGui::TextUnformatted(buf_begin, buf_end);
			// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
			// Here we instead demonstrate using the clipper to only process lines that are within the visible area.
			// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
			// Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
			// both of which we can handle since we an array pointing to the beginning of each line of text.
			// When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
			// Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{

				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;

					ImGui::PushStyleColor(ImGuiCol_Text, Levels[LevelOffsets[(line_no + 1 < LineOffsets.Size) ? line_no + 1 : LevelOffsets.Size - 1]]);
					ImGui::TextUnformatted(line_start, line_end);
					ImGui::PopStyleColor();
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;
		ImGui::EndChild();
		ImGui::End();
	}



}

#endif