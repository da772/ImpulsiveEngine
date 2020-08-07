#include <EntryPoint.h>

#include "Sandbox.h"

GEngine::Application* GEngine::CreateApplication()
{
	return new Sandbox();
}
