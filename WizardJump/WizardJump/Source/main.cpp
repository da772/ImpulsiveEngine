#include <EntryPoint.h>
#include "WizardJump.h"

GEngine::Application* GEngine::CreateApplication()
{
	return new WizardJump();
}
