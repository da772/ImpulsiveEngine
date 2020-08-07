#pragma once
#ifndef API_DUKTAPE
#define API_DUKTAPE
#include "Public/Core/Scripting/ScriptApi.h"
#include "Public/Core/Scripting/ScriptObject.h"

#include "duktape.h"

namespace GEngine {

	class ScriptApi_DukTape : public ScriptApi {


	public:
		ScriptApi_DukTape();
		virtual ~ScriptApi_DukTape();


		virtual void CreateContext() override;
		virtual void DestroyContext() override;
		virtual Ref<ScriptObject> CreateObject(std::string script, std::string name = "") override;
		virtual const char* CheckScript(std::string script) override;
		virtual void* GetNativePointer()  override { return (void*)m_ctx; };


		template <class T>
		T* ObjectToClass(void* obj) {
			DukValue val = *(DukValue*)obj;
			bool isClass = val.is_class<T>();

			if (isClass) {
				return val.as_object_pointer<T>();
			}
			else {
				GE_CORE_ERROR("Object is not class of that type");
				return nullptr;
			}
		}

		template <typename... ArgTs>
		Ref<ScriptObject> ObjectCallMethod(void* obj, const char* method_name, ArgTs... args) {
			DukValue val = *(DukValue*)obj;
			DukValue ret = dukglue_pcall_method<DukValue>(m_ctx, val, method_name, std::forward<ArgTs>(args)...);

			return Ref<ScriptObject>((ScriptObject*)new DukTapeObject(ret));
		}

		template <typename... ArgTs>
		Ref<ScriptObject> ObjectCallSelf(void* obj, ArgTs... args) {
			DukValue val = *(DukValue*)obj;
			DukValue ret = dukglue_pcall<DukValue>(m_ctx, val, std::forward<ArgTs>(args)...);
			return Ref<ScriptObject>((ScriptObject*)new DukTapeObject(ret));
		}

	

		
		static Ref<ScriptObject> _copyObj;
	private:
		duk_context* m_ctx = nullptr;
		void Setup();

		
		void SetupObject();
		void SetupRequire();
		void SetupLog();
		ScriptInput* m_inputPtr;
		ScriptScene* m_scenePtr;
		ScriptMath* m_mathPtr;


	};

}

#endif
