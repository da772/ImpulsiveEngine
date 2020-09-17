#pragma once

#ifndef GAMEOBJECT_DEF
#define GAMEOBJECT_DEF
#include "Public/Core/Core.h"


namespace GEngine {

	class GameObject {
	public:
		Weak<GameObject> self;
		uint64_t hash;

		std::string m_tag = "GameObject";
		

	protected:
		inline std::string GetClassName() {
			return typeid(*self.lock().get()).name();
		}

	};

}

#endif