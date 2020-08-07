#pragma once

namespace GEngine {

	class ScriptObject;

	
	class ScriptApi {
	public:

		inline ScriptApi() {};
		inline virtual ~ScriptApi() {};
		static Ref<ScriptApi> Create();

		virtual void CreateContext() = 0;
		virtual void DestroyContext() = 0;
		virtual void* GetNativePointer() = 0;
		virtual const char* CheckScript(std::string script) = 0;
		virtual Ref<ScriptObject> CreateObject(std::string script, std::string = "") = 0;



		static Weak<ScriptApi> s_api;

	};



	class TestData {
	public:

		inline TestData(int _id) : id(_id + 1) {};

		inline TestData(int _id, void* parent)
			: par(parent), id(_id + 1)
		{};
		inline ~TestData() { GE_CORE_DEBUG("TEST DATA DELTED {0}", id); }

		int id = 0;

		inline int getId() const {
			return id;
		}
		inline void setId(int v) {
			id = v;
		}

		void destroy();

		void* par;
	};

	class ScriptTestClass {

	public:
		inline ScriptTestClass(int id) :m_id(id) { GE_CORE_DEBUG("CREATING SCRIPT TEST CLASS {0}", id); m_testClass = Scope<TestData>(new TestData(id, (void*)this)); };
		inline ~ScriptTestClass() { GE_CORE_DEBUG("DESTROYING SCRIPT TEST CLASS {0}", m_id); };

		Scope<TestData> m_testClass;

		TestData* getTestClass() {
			return m_testClass.get();
		}

		bool checkTestClass() {
			return m_testClass != nullptr;
		}

		void destroyTestClass() {
			m_testClass = nullptr;
		}


		void createTestClass() {
			m_testClass = Scope<TestData>(new TestData(m_id, (void*)this));
		}

		void setTestClass(TestData* t) {
			if (t == nullptr) {
				m_testClass = nullptr;
				return;
			}
			m_testClass = Scope<TestData>(t);

		}

		int getId() const {
			return m_id;
		}
		void setId(int v) {
			m_id = v;
		}

		int m_id;

	};
	


}