#include "gepch.h"

#include "Public/Core/Application/GameObject.h"

namespace GEngine {
	std::unordered_map<ObjectHash, GameObject*> GameObject::s_map;

}