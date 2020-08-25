#include "gepch.h"

#include "Public/Core/Scripting/ScriptApi.h"
#include "Public/COre/Scripting/ScriptManager.h"



namespace GEngine {
	Weak<ScriptApi> ScriptApi::s_api;

	GEngine::Ref<GEngine::ScriptApi> ScriptApi::Create()
	{
		Ref<ScriptApi> api;
		switch (ScriptManager::GetType()) {
	
		default:
		case ScriptApiType::SCRIPT_NONE:
			GE_CORE_WARN("NO SCRIPT API SELECTED");
			return nullptr;
		}
		ScriptApi::s_api = api;
		return api;
	}


}