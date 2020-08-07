#include "gepch.h"
#include "Public/Core/Scripting/ScriptManager.h"
#include "Public/Core/Scripting/ScriptApi.h"


namespace GEngine {

	ScriptApiType ScriptManager::s_scriptType;
	Ref<ScriptApi> ScriptManager::s_scriptApi;

	void ScriptManager::Init(ScriptApiType type)
	{
		ScriptManager::s_scriptType = type;
		ScriptManager::s_scriptApi = ScriptApi::Create();
	}

	void ScriptManager::Shutdown()
	{
		ScriptManager::s_scriptApi = nullptr;
	}

}