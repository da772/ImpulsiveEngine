#include "gepch.h"

#include "Public/Core/Scripting/ScriptApi.h"
#include "Public/COre/Scripting/ScriptManager.h"
#include "Public/Core/Scripting/ScriptApi_DukTape.h"



namespace GEngine {
	Weak<ScriptApi> ScriptApi::s_api;

	GEngine::Ref<GEngine::ScriptApi> ScriptApi::Create()
	{
		Ref<ScriptApi> api;
		switch (ScriptManager::GetType()) {
	
		case SCRIPT_DUKTAPE: {
			api = Ref<ScriptApi>(new ScriptApi_DukTape());
			break;
		}
		default:
		case SCRIPT_NONE:
			GE_CORE_ASSERT(false, "INVALID SCRIPTING API");
			return nullptr;
		}
		ScriptApi::s_api = api;
		return api;
	}


}