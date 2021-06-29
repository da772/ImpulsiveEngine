#pragma once

#include <GEngine.h>

namespace Editor {



	class Event {

	public:
		Event();
		virtual ~Event();

		inline const std::string GetName() const { return m_name; }

	protected:
		std::string m_name = "Event";

	};


	class Dispatcher {

	public:
		Dispatcher();
		~Dispatcher();


		void RegisterEvent(const Editor::Event& e, const std::function<void(const Editor::Event&)> f) {
			notify[e.GetName()].push_back(f);
		}

		void BroadcastEvent(const Editor::Event& event) {
			const std::vector<std::function<void(const Editor::Event&)>> funcs = notify[event.GetName()];

			for (const auto& f : funcs) {
				f(event);
			}
		}


	private:
		std::unordered_map<std::string, std::vector<std::function<void(const Editor::Event&)>>> notify;

	};

}
