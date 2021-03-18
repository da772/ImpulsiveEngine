#include "Reflection.map.generated.h"
#include "Logger.generated.h"
#include "TestScript.generated.h"


	void __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage) {
		Logger_Generated::Load(storage);
		TestScript_Generated::Load(storage);
	}
	void __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage) {
		Logger_Generated::Unload(storage);
		TestScript_Generated::Unload(storage);
	}

