#include "Public/Core/Application/Component.h"
#include "Public/Platform/Scripting/ScriptApi.h"

namespace GEngine {


	class GE_API NativeScriptComponent : public Component {

	public:
		NativeScriptComponent(Entity* e, const std::string& clazz);
		~NativeScriptComponent();
		



		void UnloadGraphics() override;
		void ReloadGraphics() override;

		void LoadClass(const std::string& clazz, const ObjectHash& hash = {});
		inline bool IsValid() const { return m_isValid; };
		const ObjectHash GetClassHash();
		Component* GetComponent() const;
		virtual std::string Serialize(int indent = 0) override;
		std::string GetClass();

		inline NativeObject* GetNativeObject() { return &m_object; }

	protected:
		void OnBegin() override;
		void OnEnd() override;
		void OnUpdate(Timestep timestep) override;

	private:
		bool CreateScript();

	private:
		NativeObject m_object;
		Component* m_component;
        bool m_isValid = false;
		bool m_hasBegun = false;
		std::string m_clazz;

	};

}
