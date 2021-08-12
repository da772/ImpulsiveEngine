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
		notify[e][(uint64_t)&f] = (f);
		return (uint64_t)&f;
	}

	
	void EditorDispatcher::UnsubscribeEvent(const EditorEventType& e, const uint64_t& hash)
	{
		notify[e].erase(hash);
	}



}