#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/.h"
class _Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("",{"",{
}, {
}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("");
	}
};
