#pragma once

#ifndef GAMEOBJECT_DEF
#define GAMEOBJECT_DEF
#include "Public/Core/Core.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {
	class GE_API GameObject {
	public:
		inline GameObject(ObjectHash hash) : go_hash(hash) { s_map[go_hash] = this; };
		virtual ~GameObject() {
			s_map.erase(go_hash);
		};
		inline const ObjectHash GetHash() const { return go_hash; }
		inline const std::string GetTag() const { return go_tag; }
		inline void SetTag(const std::string& tag) { go_tag = tag; }

		inline static GameObject* GetObjectFromHash(const ObjectHash& hash) {
			std::unordered_map<ObjectHash, GameObject*>::iterator it = s_map.find(hash);
			if (it == s_map.end()) return nullptr;
			return it->second;
		}

	protected:
		ObjectHash go_hash = ObjectHash();
		std::string go_tag = "GameObject";
		inline std::string GetClassName() {
			return typeid(this).name();
		}
		bool is_component = false;

		static std::unordered_map<ObjectHash, GameObject*> s_map;

	};

}

#endif