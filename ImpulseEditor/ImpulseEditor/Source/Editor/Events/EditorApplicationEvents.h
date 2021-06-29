#pragma once

#include "EditorEvents.h"

namespace Editor {

	class ApplicationEvent : public EditorEvent  {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationEvent";

	};

	class ApplicationPlayEvent : public EditorEvent {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationPlayEvent";

	};

	class ApplicationPauseEvent : public EditorEvent {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationPauseEvent";

	};

	class ApplicationStopEvent : public EditorEvent {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationStopEvent";

	};

	class ApplicationResumeEvent : public EditorEvent {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationResumeEvent";

	};

	class ApplicationSkipFrameEvent : public EditorEvent {
	public:
		inline virtual const std::string GetName() const override {
			return m_name;
		};
	protected:
		std::string m_name = "ApplicationSkipFrameEvent";

	};
	

}
