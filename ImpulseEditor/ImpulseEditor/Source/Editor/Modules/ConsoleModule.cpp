#ifdef GE_EDITOR
#include "ConsoleModule.h"

namespace Editor {


	void ConsoleModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		GEngine::Log::GetImGuiLog()->Draw(name.c_str(), is_open);
	}

	ConsoleModule::~ConsoleModule()
	{

	}

	ConsoleModule::ConsoleModule()
	{

	}

}
#endif