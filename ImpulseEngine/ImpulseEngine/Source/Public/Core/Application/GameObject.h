#pragma once

#ifndef GAMEOBJECT_DEF
#define GAMEOBJECT_DEF
#include "Public/Core/Core.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {

	class GE_API GameObject {
	public:
		inline const uint32_t GetHash() const { return go_hash; }
		inline const std::string GetTag() const { return go_tag; }
		inline void SetTag(const std::string& tag) { go_tag = tag; }
	protected:
		uint64_t go_hash = 0;
		std::string go_tag = "GameObject";
		inline std::string GetClassName() {
			return typeid(this).name();
		}

	};

}

#endif