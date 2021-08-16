#include "EditorEvents.h"

namespace Editor {


	EditorDispatcher::EditorDispatcher()
	{

	}

	EditorDispatcher::~EditorDispatcher()
	{

	}

	
	uint64_t EditorDispatcher::SubscribeEvent(const EditorEventType& e, const std::function<void(const Editor::EditorEvent&)> f)
	{
		if (e == EditorEventType::AllEvents) {
			notify_all[(uint64_t)&f] = (f);
			return (uint64_t)&f;
		}
		notify[e][(uint64_t)&f] = (f);
		return (uint64_t)&f;
	}

	
	void EditorDispatcher::UnsubscribeEvent(const EditorEventType& e, const uint64_t& hash)
	{
		if (e == EditorEventType::AllEvents) {
			notify_all.erase(hash);
			return;
		}
		notify[e].erase(hash);
	}



}