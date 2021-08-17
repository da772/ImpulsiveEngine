#pragma once

#include <GEngine.h>

namespace Editor {

	struct EventFunction;
	class EditorDispatcher;

	enum class EditorEventType : uint16_t
	{
		None = 0,
		AllEvents,
		ApplicationPlayEvent, ApplicationPauseEvent, ApplicationStopEvent, ApplicationResumeEvent, ApplicationSkipFrameEvent,
		EditorHideViewEvent, EditorShowViewEvent,
		SceneLoad,SceneSave,HistoryScene,UndoScene,RedoScene, HistorySceneValid,SceneCreateEntity, SceneDestroyEntity, SceneModifyEntity, SceneAddComponent, SceneDestroyComponent, SceneModifyComponent,
		EditorToolEvent, EditorToolEventDrag, EditorToolEventMove, EditorToolEventRotate, EditorToolEventScale, EditorToolEventUndo,
		EditorToolEventRedo
	};

	enum EditorEventCategory : uint32_t
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryUI = BIT(1),
		EventCategoryScene = BIT(2),
		EventCategoryGameObject = BIT(3),
		EventCategoryEntity = BIT(4),
		EventCategoryComponent = BIT(5),
		EventCategoryModification = BIT(6)
		
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



	class EditorDispatcher {

	public:
		EditorDispatcher();
		~EditorDispatcher();

		
		uint64_t SubscribeEvent(const EditorEventType& e, const std::function<void(const Editor::EditorEvent&)> f);


		
		void UnsubscribeEvent(const EditorEventType& e, const uint64_t& hash);

		template<class E, typename ... Args>
		inline void BroadcastEvent(Args&& ... args) {
			E event = E(std::forward<Args>(args)...);
			const std::unordered_map<uint64_t, std::function<void(const Editor::EditorEvent&)>> funcs = notify[event.GetEventType()];
			for (const auto& f : notify_all) {
				f.second(event);
			}
			for (const auto& f : funcs) {
				f.second(event);
			}
		}



	private:
		std::unordered_map<EditorEventType, std::unordered_map<uint64_t, std::function<void(const Editor::EditorEvent&)>>> notify;
		std::unordered_map<uint64_t, std::function<void(const Editor::EditorEvent&)>> notify_all;

	};



}
