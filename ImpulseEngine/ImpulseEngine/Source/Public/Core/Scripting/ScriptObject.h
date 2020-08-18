#pragma once
#include "Public/Core/Scripting/ScriptManager.h"
#include <duktape.h>
#include <dukglue/dukglue.h>

namespace GEngine {


	struct FTouchInfo;
	struct ScriptVector2;
	namespace Script {
		enum class ObjectTypes {
			UNDEFINED = 0,
			NULLREF,
			BOOLEAN,
			NUMBER,
			STRING,
			OBJECT,
			BUFFER,
			POINTER,
			LIGHTFUNC,
			ARRAY,
			FUNCTION

		};
	}

	struct ScriptMath {
		Ref<ScriptVector2> ScreenPosToWorldPos(Ref<ScriptVector2> pos);
	};

	struct ScriptScene {
		void SetScene(std::string name);
		void PauseScene();
		void PlayScene();
		std::string GetScene();
	};

	struct ScriptInput {

		bool IsKeyPressed(std::string keyCode);
		bool IsMouseButtonPressed(int mouseButton);
		int GetMouseX();
		int GetMouseY();
		Ref<ScriptVector2> GetMousePosition();
		std::vector<Ref<FTouchInfo>> GetTouches();
		uint32_t GetTouchCount();

		static std::unordered_map<std::string, int> keycodes;

	};



	struct ScriptVector4 {
		inline ScriptVector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		inline ScriptVector4(glm::vec4 vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
		float x, y, z, w;

		inline void SetX(float n) { x = n; }
		inline void SetY(float n) { y = n; }
		inline void SetZ(float n) { z = n; }
		inline void SetW(float n) { w = n; }

		inline float GetX() { return x; }
		inline float GetY() { return y; }
		inline float GetZ() { return z; }
		inline float GetW() { return w; }

		inline Ref<ScriptVector4> Subtract(Ref<ScriptVector4> p) {
			return make_shared<ScriptVector4>(x - p->x, y - p->y, z - p->z, w-p->w);
		}
		inline Ref<ScriptVector4> Add(Ref<ScriptVector4> p) {
			return make_shared<ScriptVector4>(x + p->x, y + p->y, z + p->z, w+p->w);
		}
		inline Ref<ScriptVector4> Multiply(Ref<ScriptVector4> p) {
			return make_shared<ScriptVector4>(x * p->x, y * p->y, z * p->z, w*p->w);
		}
		inline Ref<ScriptVector4> Divide(Ref<ScriptVector4> p) {
			return make_shared<ScriptVector4>(x / p->x, y / p->y, z / p->z, w/p->w);
		}
		
		inline glm::vec4 GetGlm() {
			return glm::vec4(x, y, z, w);
		}


	};


	struct ScriptVector3 {
		inline ScriptVector3(float x, float y, float z) : x(x),y(y),z(z) {}
		inline ScriptVector3(glm::vec3 vector) : x(vector.x), y(vector.y), z(vector.z) {}

		float x, y, z;

		inline void SetX(float n) { x = n; }
		inline void SetY(float n) { y = n; }
		inline void SetZ(float n) { z = n; }

		inline float GetX() { return x; }
		inline float GetY() { return y; }
		inline float GetZ() { return z; }

		inline Ref<ScriptVector3> Subtract(Ref<ScriptVector3> p) {
			return make_shared<ScriptVector3>(x - p->x, y - p->y, z - p->z);
		}
		inline Ref<ScriptVector3> Add(Ref<ScriptVector3> p) {
			return make_shared<ScriptVector3>(x + p->x, y + p->y, z + p->z);
		}
		inline Ref<ScriptVector3> Multiply(Ref<ScriptVector3> p) {
			return make_shared<ScriptVector3>(x * p->x, y * p->y, z * p->z);
		}
		inline Ref<ScriptVector3> Divide(Ref<ScriptVector3> p) {
			return make_shared<ScriptVector3>(x / p->x, y / p->y, z / p->z);
		}

		inline Ref<ScriptVector3> Normalize() {
			float v = sqrt((x * x + y * y + z*z));
			return make_shared<ScriptVector3>(glm::vec3(x / v, y / v, z/v));
			
		}

		inline glm::vec3 GetGlm() {
			return glm::vec3(x, y, z);
		}


	};


	struct ScriptVector2 {
		inline ScriptVector2(float x, float y) : x(x), y(y) {};
		inline ScriptVector2(glm::vec2 vector) : x(vector.x), y(vector.y) {}
		float x, y;

		inline void SetX(float n) { x = n; }
		inline void SetY(float n) { y = n; }


		inline float GetX() { return x; }
		inline float GetY() { return y; }

		inline Ref<ScriptVector2> Subtract(Ref<ScriptVector2> p) {
			return make_shared<ScriptVector2>(x - p->x, y - p->y);
		}
		inline Ref<ScriptVector2> Add(Ref<ScriptVector2> p) {
			return make_shared<ScriptVector2>(x + p->x, y + p->y);
		}
		inline Ref<ScriptVector2> Multiply(Ref<ScriptVector2> p) {
			return make_shared<ScriptVector2>(x * p->x, y * p->y);
		}
		inline Ref<ScriptVector2> Divide(Ref<ScriptVector2> p) {
			return make_shared<ScriptVector2>(x / p->x, y / p->y);
		}

		inline Ref<ScriptVector2> Normalize() {
			float v = sqrt((x * x + y * y));
			return make_shared<ScriptVector2>(glm::vec2(x/v,y/v));
		}

		inline glm::vec2 GetGlm() {
			return glm::vec2(x, y);
		}

	};


	class ScriptObject {

	public:
		ScriptObject(const char* path) : m_path(path) {};
		virtual ~ScriptObject() {};

		template <typename... ArgTs>
		Ref<ScriptObject> CallMethod(const char* method_name, ArgTs... args);

		template <typename... ArgTs>
		Ref<ScriptObject> CallSelf(ArgTs... args);

		virtual float asFloat() { return NULL; };
		virtual int asInt() { return NULL; };
		virtual bool asBool() { return NULL; };
		virtual void* asPointer() { return nullptr; };
		virtual std::string asString() { return NULL; };
		virtual std::string asJSONString() { return NULL; };
		virtual std::vector<Ref<ScriptObject>> asVector() { return std::vector<Ref<ScriptObject>>(); }
		virtual std::unordered_map<std::string, Ref<ScriptObject>> asMap() { return std::unordered_map < std::string,  Ref<ScriptObject >> (); }
		virtual Ref<ScriptObject> GetProp(const char* name) { return nullptr; };
		void* GetNativeObject() { return m_nativeObj;  }
		virtual Script::ObjectTypes GetType() { return Script::ObjectTypes::UNDEFINED; };
		inline virtual Script::ObjectTypes GetTypeRaw() { return Script::ObjectTypes::UNDEFINED; }
		inline virtual void SetPath(const char* path) { m_path = path; }
		inline virtual const char* GetPath() { return m_path; }
		template <class T>
		bool isClass() {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				return (*(DukValue*)m_nativeObj).is_class<T>();
				break;
			}
			default:
				return false;
			}
		};

		inline void SetPropertyNative(const char* name, Ref<ScriptObject> val) {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				duk_context* c = (*(DukValue*)m_nativeObj).context();
				(*(DukValue*)m_nativeObj).push();
				(*(DukValue*)m_nativeObj).prop(name).push();
				//dukglue_push((*(DukValue*)m_nativeObj).context(), (*(DukValue*)val->GetNativeObject()).copy_from_stack);
				duk_put_prop_string(c, 0, name);
				duk_remove(c, -1);
				duk_remove(c, -1);
				break;
			}
			default:
				return;
			}
		}

		template <class T>
		inline void SetProperty(const char* name, T val) {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				duk_context* c = (*(DukValue*)m_nativeObj).context();
				(*(DukValue*)m_nativeObj).push();
				(*(DukValue*)m_nativeObj).prop(name).push();
				dukglue_push((*(DukValue*)m_nativeObj).context(), val);
				duk_put_prop_string(c, 0, name);
				duk_remove(c, -1);
				duk_remove(c, -1);
				break;
			}
			default:
				return;
			}
		}

		inline void RemoveProperty(const char* name) {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				duk_context* c = (*(DukValue*)m_nativeObj).context();
				(*(DukValue*)m_nativeObj).push();
				duk_del_prop_string(c, -1, name);
				duk_pop(c);
				break;
			}
			default:
				return;
			}
		}

		inline void RemoveFunction(const char* name) {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				duk_context* c = (*(DukValue*)m_nativeObj).context();
				int prev_top = duk_get_top(c);
				(*(DukValue*)m_nativeObj).prop(name).push();
				duk_get_prop_string(c, -1, "meat");
				GE_CORE_WARN("STACK: {0}", duk_safe_to_string(c,-1));
				break;
			}
			default:
				return;
			}
		}

		template <class T>
		inline void InvalidateProperty(const char* name) {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				duk_context* c = (*(DukValue*)m_nativeObj).context();
				dukglue_invalidate_object(c, GetProp(name)->asClass<T>());
				break;
			}
			default:
				return;
			}
		}

		template <class T>
		T* asClass() {
			switch (ScriptManager::GetType()) {
			case ScriptApiType::SCRIPT_DUKTAPE: {
				DukValue val = *(DukValue*)m_nativeObj;
				bool isClass = val.is_class<T>();

				if (isClass) {
					return val.as_object_pointer<T>();
				}
				else {
					GE_CORE_ERROR("Object is not class of that type");
					return nullptr;
				}
			}
			default:
				return nullptr;
			}
		};

		static std::string GetError();
		static void AddError(std::string s);
		static bool HasError();

		//virtual ArgumentType GetType() { return ArgumentType::ARG_FLOAT; };
		//virtual std::map<std::string, ArgumentType> GetVars() { return std::map<std::string, ArgumentType>(); };
		//virtual Ref<ScriptObject> GetArgumentObject(const char* name) { return nullptr; };
		

	protected:
		void* m_nativeObj;
		static std::queue<std::string> s_ErrorStack;
		const char* m_path;


	};


	class DukTapeObject : public ScriptObject {

	public:
		DukTapeObject(const char* path, DukValue val);
		virtual ~DukTapeObject();

		inline virtual std::map<std::string, Script::ObjectTypes> GetVars() { return std::map<std::string, Script::ObjectTypes>(); };
		inline virtual Ref<ScriptObject> GetArgumentObject(const char* name) { return nullptr; };
		inline virtual float asFloat() override  { return m_value.as_float(); } ;
		virtual void* asPointer() { return m_value.as_pointer(); };
		inline virtual int asInt() override { return m_value.as_int(); };
		inline virtual bool asBool() override { return m_value.as_bool(); };
		inline virtual std::string asString() override { return m_value.as_string(); };
		inline virtual std::string asJSONString() override { return m_value.as_json_string(); };
		inline virtual Ref<ScriptObject> GetProp(const char* name) { return Ref<ScriptObject>((ScriptObject*)new DukTapeObject(m_path, m_value.prop(name)));  };
		inline virtual Script::ObjectTypes GetType() override { return m_value.is_function() ? Script::ObjectTypes::FUNCTION : m_value.is_vector() ? Script::ObjectTypes::ARRAY : (Script::ObjectTypes)m_value.type(); }
		inline virtual Script::ObjectTypes GetTypeRaw() override { return (Script::ObjectTypes)m_value.type(); }
		inline virtual std::unordered_map<std::string, Ref<ScriptObject>> asMap() override {
			std::map<std::string, DukValue> m = m_value.as_map();
			std::unordered_map<std::string, Ref<ScriptObject>> map;
			for (std::pair<std::string, DukValue> pair : m) {
				map[pair.first] = Ref<ScriptObject>((ScriptObject*)new DukTapeObject(m_path, pair.second));
			}
			return map;
		};
		

	protected:
		DukValue m_value;


	};

	template <typename... ArgTs>
	inline Ref<ScriptObject>
		GEngine::ScriptObject::CallSelf(ArgTs... args)
	{

		switch (ScriptManager::GetType()) {
		case ScriptApiType::SCRIPT_DUKTAPE: {
			DukValue val = *(DukValue*)m_nativeObj;
			try {
				DukValue ret = dukglue_pcall<DukValue>(val.context(), val, std::forward<ArgTs>(args)...);
				return Ref<ScriptObject>((ScriptObject*)new DukTapeObject(m_path, ret));
			}
			catch (std::exception& e) {
				ScriptObject::AddError(e.what());
				return nullptr;
			}
		}
		default:
			return NULL;
		}

	}

	template <typename... ArgTs>
	Ref<ScriptObject>
		GEngine::ScriptObject::CallMethod(const char* method_name, ArgTs... args)
	{
		switch (ScriptManager::GetType()) {
		case ScriptApiType::SCRIPT_DUKTAPE: {
			try {
				DukValue val = *(DukValue*)m_nativeObj;
				DukValue ret = dukglue_pcall_method<DukValue>(val.context(), val, method_name, std::forward<ArgTs>(args)...);
				Ref<ScriptObject> o = Ref<ScriptObject>((ScriptObject*)new DukTapeObject(m_path, ret));
				return o;
			}
			catch (std::exception& e) {
				ScriptObject::AddError(e.what());
				return nullptr;
			}
			break;
		}
		default:
			return NULL;
		}
		return nullptr;
	}


}

