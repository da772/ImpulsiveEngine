#pragma once

#include <GEngine.h>

namespace Editor {

	struct EventFunction;
	class EditorDispatcher;

	enum class EditorEventType
	{
		None = 0,
		ApplicationPlayEvent, ApplicationPauseEvent, ApplicationStopEvent, ApplicationResumeEvent, ApplicationSkipFrameEvent,
		EditorHideViewEvent, EditorShowViewEvent,
		SceneCreateEntity, SceneDestroyEntity, SceneModifyEntity, SceneAddComponent, SceneDestroyComponent, SceneModifyComponent
	};

	enum EditorEventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryUI = BIT(1),
		EventCategoryScene = BIT(2),
		EventCategoryGameObject = BIT(3),
		EventCategoryEntity = BIT(4),
		EventCategoryComponent = BIT(5),
		EventCategoryModification = BIT(5),
		
	};

#if defined(GE_PLATFORM_WINDOWS) && !defined(__clang__) && !defined(__GNUC__)
#define EDITOR_EVENT_CLASS_TYPE(type) static EditorEventType GetStaticType() { return EditorEventType::##type; }\
								virtual EditorEventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#else
#define EDITOR_EVENT_CLASS_TYPE(type) static EditorEventType GetStaticType() { return EditorEventType::type; }\
								virtual EditorEventType GetEventType() const override { return GetStaticType(); }\
								virtual const char*  GetName() const override { return #type; }
#endif

#define EDITOR_EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class EditorEvent {

	public:
		virtual EditorEventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		inline bool IsCategory(int i) { return GetCategoryFlags() & i; }

	protected:
		friend EditorDispatcher;

	};

	struct EventFunction {
		const std::string& e;
		const std::function<void(const Editor::EditorEvent&)>& f;
	};


	class EditorDispatcher {

	public:
		EditorDispatcher();
		~EditorDispatcher();

		uint64_t SubscribeEvent(const std::string& e, const std::function<void(const Editor::EditorEvent&)> f);

		void UnsubscribeEvent(const Editor::EditorEvent& e, const uint64_t& hash);

		template<class E, typename ... Args>
		inline void BroadcastEvent(Args&& ... args) {
			E event = E(std::forward<Args>(args)...);
			const std::unordered_map<uint64_t, std::function<void(const Editor::EditorEvent&)>> funcs = notify[event.GetName()];

			for (const auto& f : funcs) {
				f.second(event);
			}
		}

		EditorDispatcher& operator+=(const EventFunction& e) {
			this->SubscribeEvent(e.e, e.f);
			return *this;
		}



	private:
		std::unordered_map<std::string, std::unordered_map<uint64_t, std::function<void(const Editor::EditorEvent&)>>> notify;

	};



}
