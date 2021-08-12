#pragma once
#include "EditorEvents.h"

namespace Editor {

	class EditorToolEvent : public EditorEvent {
	public:
		EditorToolEvent(EditorEventType tool) :tool(tool) {};
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryUI);
		EDITOR_EVENT_CLASS_TYPE(EditorToolEvent);
		const EditorEventType& GetTool() const { return tool; }

	private:
		EditorEventType tool;
	};



}
