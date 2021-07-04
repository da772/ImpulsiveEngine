#pragma once

#ifndef GAMEOBJECT_DEF
#define GAMEOBJECT_DEF
#include "Public/Core/Core.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {

	class GE_API GameObject {
	public:
		inline GameObject(uint64_t hash) : go_hash(hash) { s_map[go_hash] = this; };
		virtual ~GameObject() { s_map.erase(go_hash); };
		inline const uint64_t GetHash() const { return go_hash; }
		inline const std::string GetTag() const { return go_tag; }
		inline void SetTag(const std::string& tag) { go_tag = tag; }

		inline static GameObject* GetObjectFromHash(const uint64_t& hash) {

			std::unordered_map<uint64_t, GameObject*>::iterator it = s_map.find(hash);
			if (it == s_map.end()) return nullptr;
			return it->second;
		}

	protected:
		uint64_t go_hash = 0;
		std::string go_tag = "GameObject";
		inline std::string GetClassName() {
			return typeid(this).name();
		}
		bool is_component = false;

		static std::unordered_map<uint64_t, GameObject*> s_map;

	};

}

#endif