#include "Public/Core/Application/Component.h"
#include "Public/Platform/Scripting/ScriptApi.h"

namespace GEngine {


	class NativeScriptComponent : public Component {

	public:
		NativeScriptComponent(Entity* e, const std::string& clazz);
		~NativeScriptComponent();
		

		void UnloadGraphics() override;


		void ReloadGraphics() override;

	protected:


		void OnBegin() override;


		void OnEnd() override;


		void OnUpdate(Timestep timestep) override;

	private:
		NativeObject object;
		Component* component;

	};

}