#pragma once

#include <GEngine.h>

namespace Editor {

	struct EventFunction;
	class EditorDispatcher;

	class EditorEvent {

	public:
		EditorEvent();
		virtual ~EditorEvent();

		virtual const std::string GetName() const = 0;

	protected:
		friend EditorDispatcher;
		std::string m_name = "Event";

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
