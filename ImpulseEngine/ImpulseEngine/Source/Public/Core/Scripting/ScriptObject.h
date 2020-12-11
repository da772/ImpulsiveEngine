#pragma once
#include "Public/Core/Scripting/ScriptManager.h"


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
		inline ScriptVector4(Vector4f vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
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
		
		inline Vector4f GetGlm() {
			return Vector4f(x, y, z, w);
		}


	};


	struct ScriptVector3 {
		inline ScriptVector3(float x, float y, float z) : x(x),y(y),z(z) {}
		inline ScriptVector3(Vector3f vector) : x(vector.x), y(vector.y), z(vector.z) {}

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
			return make_shared<ScriptVector3>(Vector3f(x / v, y / v, z/v));
			
		}

		inline Vector3f GetGlm() {
			return Vector3f(x, y, z);
		}


	};


	struct ScriptVector2 {
		inline ScriptVector2(float x, float y) : x(x), y(y) {};
		inline ScriptVector2(Vector2f vector) : x(vector.x), y(vector.y) {}
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
			return make_shared<ScriptVector2>(Vector2f(x/v,y/v));
		}

		inline Vector2f GetGlm() {
			return Vector2f(x, y);
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
			default:
				return false;
			}
		};

		inline void SetPropertyNative(const char* name, Ref<ScriptObject> val) {
			switch (ScriptManager::GetType()) {
			default:
				return;
			}
		}

		template <class T>
		inline void SetProperty(const char* name, T val) {
			switch (ScriptManager::GetType()) {
			default:
				return;
			}
		}

		inline void RemoveProperty(const char* name) {
			switch (ScriptManager::GetType()) {
			default:
				return;
			}
		}

		inline void RemoveFunction(const char* name) {
			switch (ScriptManager::GetType()) {
			default:
				return;
			}
		}

		template <class T>
		inline void InvalidateProperty(const char* name) {
			switch (ScriptManager::GetType()) {
			default:
				return;
			}
		}

		template <class T>
		T* asClass() {
			switch (ScriptManager::GetType()) {
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

	template <typename... ArgTs>
	inline Ref<ScriptObject>
		GEngine::ScriptObject::CallSelf(ArgTs... args)
	{

		switch (ScriptManager::GetType()) {
		default:
			return NULL;
		}

	}

	template <typename... ArgTs>
	Ref<ScriptObject>
		GEngine::ScriptObject::CallMethod(const char* method_name, ArgTs... args)
	{
		switch (ScriptManager::GetType()) {
		default:
			return NULL;
		}
		return nullptr;
	}


}

