#pragma once

#include "Event.h"

namespace GEngine {


	class GE_API TouchEvent : public Event {
	public:
		TouchEvent(uint64_t id, float x, float y)
			: m_TouchX(x), m_TouchY(y), m_Id(id) {}
		inline float GetX() const { return m_TouchX; }
		inline float GetY() const { return m_TouchY; }
		inline uint64_t GetId() const { return m_Id; }

	protected:
		float m_TouchX, m_TouchY;
		uint64_t m_Id;
	};


	class GE_API TouchPressedEvent : public TouchEvent
	{
	public:
		TouchPressedEvent(uint64_t id, float x, float y)
			: TouchEvent(id, x, y) {};

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "TouchPressedEvent: " << m_TouchX << ", " << m_TouchY;
			std::string s = ss.str();
			return s;
		}

		EVENT_CLASS_TYPE(TouchPressed)
			EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	
	};


	class GE_API TouchHeldEvent : public TouchEvent
	{
	public:
		TouchHeldEvent(uint64_t id, float x, float y)
			: TouchEvent(id, x, y) {};

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "TouchHeldEvent: " << m_TouchX << ", " << m_TouchY;
			std::string s = ss.str();
			return s;
		}

		EVENT_CLASS_TYPE(TouchHeld)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)
	};

	class GE_API TouchReleasedEvent : public TouchEvent
	{
	public:
		TouchReleasedEvent(uint64_t id, float x, float y)
			: TouchEvent(id, x, y) {};


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "TouchReleasedEvent: " << m_TouchX << ", " << m_TouchY;
			std::string s = ss.str();
			return s;
		}

		EVENT_CLASS_TYPE(TouchReleased)
		EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput)

	};


}
