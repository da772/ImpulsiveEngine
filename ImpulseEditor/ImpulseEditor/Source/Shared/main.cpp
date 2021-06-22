#include <EntryPoint.h>

#include "ImpulseEditor.h"

GEngine::Application* GEngine::CreateApplication()
{
	return new ImpulseEditor();
}
