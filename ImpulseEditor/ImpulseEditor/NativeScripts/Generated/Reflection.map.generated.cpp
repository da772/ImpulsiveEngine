#include "Reflection.map.generated.h"
#include "ExampleScript.generated.h"
#include "Logger.generated.h"
#include "TestScript.generated.h"
#include "MyScript.generated.h"


	void __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage) {
		ExampleScript_Generated::Load(storage);
		Logger_Generated::Load(storage);
		TestScript_Generated::Load(storage);
		MyScript_Generated::Load(storage);
	}
	void __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage) {
		ExampleScript_Generated::Unload(storage);
		Logger_Generated::Unload(storage);
		TestScript_Generated::Unload(storage);
		MyScript_Generated::Unload(storage);
	}

