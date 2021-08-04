#include "gepch.h"

#include "Public/Core/Application/GameObject.h"
#include "Public/Platform/Scripting/ScriptApi.h"

namespace GEngine {

	GameObject::GameObject(ObjectHash hash) : go_hash(hash)
	{ 
		Factory::AddHash(hash);
		s_map[go_hash] = this;
	};

	GameObject::~GameObject()
	{
		GE_CORE_DEBUG("REMOVED COMPONENT HASH: {0}", Factory::HashToString(go_hash));
		s_map.erase(go_hash);
		Factory::RemoveHash(go_hash);
		ScriptApi::RemoveGameObject(go_hash);
	}

	bool GameObject::IsComponent() const
	{
		return is_component;
	}

	std::unordered_map<ObjectHash, GameObject*> GameObject::s_map;

}