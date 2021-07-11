#pragma once
#include "EditorEvents.h"

namespace Editor {

	class EditorUIEvent : public EditorEvent {
	public:
		EditorUIEvent(const std::string& view) : m_view(view) {};
		inline const std::string& GetView() { return m_view; }
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryUI);
		
	private:
		std::string m_view;
	};

	class EditorHideViewEvent : public EditorUIEvent {
	public:
		inline EditorHideViewEvent(const std::string& view) : EditorUIEvent(view) {};
		EDITOR_EVENT_CLASS_TYPE(EditorHideViewEvent);

	};


	class EditorShowViewEvent : public EditorUIEvent {
	public:
		inline EditorShowViewEvent(const std::string& view) : EditorUIEvent(view) {};
		EDITOR_EVENT_CLASS_TYPE(EditorShowViewEvent);
	
	};

}
