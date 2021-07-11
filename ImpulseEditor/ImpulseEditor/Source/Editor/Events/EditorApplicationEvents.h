#pragma once

#include "EditorEvents.h"

namespace Editor {

	class ApplicationEvent : public EditorEvent  {
	public:
		EDITOR_EVENT_CLASS_CATEGORY(EventCategoryApplication);


	};

	class ApplicationPlayEvent : public ApplicationEvent {
	public:
		EDITOR_EVENT_CLASS_TYPE(ApplicationPlayEvent);
	
	};

	class ApplicationPauseEvent : public ApplicationEvent {
	public:
		EDITOR_EVENT_CLASS_TYPE(ApplicationPauseEvent);

	};

	class ApplicationStopEvent : public ApplicationEvent {
	public:
		EDITOR_EVENT_CLASS_TYPE(ApplicationStopEvent);

	};

	class ApplicationResumeEvent : public ApplicationEvent {
	public:
		EDITOR_EVENT_CLASS_TYPE(ApplicationResumeEvent);

	};

	class ApplicationSkipFrameEvent : public ApplicationEvent {
	public:
		EDITOR_EVENT_CLASS_TYPE(ApplicationSkipFrameEvent);

	};
	

}
