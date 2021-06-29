#include "EditorEvents.h"

namespace Editor {


	EditorDispatcher::EditorDispatcher()
	{

	}

	EditorDispatcher::~EditorDispatcher()
	{

	}

	uint64_t EditorDispatcher::SubscribeEvent(const std::string& e, const std::function<void(const Editor::EditorEvent&)> f)
	{
		notify[e][(uint64_t)&f] = (f);
		return (uint64_t)&f;
	}

	void EditorDispatcher::UnsubscribeEvent(const Editor::EditorEvent& e, const uint64_t& hash)
	{
		notify[e.GetName()].erase(hash);
	}



	EditorEvent::EditorEvent()
	{

	}

	EditorEvent::~EditorEvent()
	{

	}

}