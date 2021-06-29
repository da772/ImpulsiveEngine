#pragma once
#include "EditorEvents.h"

namespace Editor {

	class EditorUIEvent : public EditorEvent {
	protected:
		std::string m_name = "EditorUIEvent";

	};

	class EditorHideViewEvent : public EditorEvent {
	public:
		inline EditorHideViewEvent(const std::string& view) : view(view) {};
		std::string view;
		inline virtual const std::string GetName() const override {
			return m_name;
		};

	protected:
		std::string m_name = "EditorHideViewEvent";

	};


	class EditorShowViewEvent : public EditorEvent {
	public:
		inline EditorShowViewEvent(const std::string& view) : view(view) {};
		std::string view;
		inline virtual const std::string GetName() const override {
			return m_name;
		};

	protected:
		std::string m_name = "EditorShowViewEvent";

	};

}
