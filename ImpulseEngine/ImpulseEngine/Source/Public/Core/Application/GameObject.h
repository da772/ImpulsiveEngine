#pragma once

#ifndef GAMEOBJECT_DEF
#define GAMEOBJECT_DEF
#include "Public/Core/Core.h"


namespace GEngine {

	class GameObject {
	public:
		Weak<GameObject> self;
		uint64_t hash;



	};

}

#endif