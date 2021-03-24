#include "Public/Core/Application/Component.h"
#include "Public/Platform/Scripting/ScriptApi.h"

namespace GEngine {


	class NativeScriptComponent : public Component {

	public:
		NativeScriptComponent(Entity* e, const std::string& clazz);
		~NativeScriptComponent();
		
		void UnloadGraphics() override;
		void ReloadGraphics() override;

		inline constexpr NativeObject* GetNativeObject() { return &m_object; }

	protected:
		void OnBegin() override;
		void OnEnd() override;
		void OnUpdate(Timestep timestep) override;

	private:
		NativeObject m_object;
		Component* m_component;
        bool m_isValid = false;
		std::string m_clazz;

	};

}
