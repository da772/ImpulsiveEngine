#pragma once
namespace GEngine {

	enum class ScriptApiType {
		SCRIPT_NONE,
		SCRIPT_DUKTAPE,
	};

	class ScriptApi;

	class ScriptManager {

	public:
		static void Init(ScriptApiType type);
		static void Shutdown() ;
		static inline Ref<ScriptApi> GetApi() { return s_scriptApi; }


		static ScriptApiType GetType() { return s_scriptType; };

	private:
		static ScriptApiType s_scriptType;
		static Ref<ScriptApi> s_scriptApi;


	};


}
