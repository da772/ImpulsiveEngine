#include "gepch.h"

#include "Public/Core/Application/GameObject.h"

namespace GEngine {
	std::unordered_map<uint64_t, GameObject*> GameObject::s_map;

}