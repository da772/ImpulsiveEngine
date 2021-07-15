#include "gepch.h"

#include "Public/Core/Application/GameObject.h"
#include "Public/Platform/Scripting/ScriptApi.h"

namespace GEngine {

	GameObject::~GameObject()
	{
		s_map.erase(go_hash);
		ScriptApi::RemoveGameObject(go_hash);
	}

	std::unordered_map<ObjectHash, GameObject*> GameObject::s_map;

}