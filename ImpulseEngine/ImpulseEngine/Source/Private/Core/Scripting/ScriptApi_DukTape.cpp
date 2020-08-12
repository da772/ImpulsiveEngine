#include "gepch.h"
#include "Public/Core/Scripting/ScriptApi_DukTape.h"
#include "duk_logging.h"
#include <duk_module_duktape.h>

#include <duktape.h>

#include "Public/Core/Scripting/ScriptableComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Factory.h"
#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Application/Components/UI/UIComponent.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#include "Public/Core/Renderer/Graphics/Font.h"
#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_Texture.h"
#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_Font.h"
#include "Public/Core/Application/Components/SpriteAnimationComponent.h"
#include "Public/Core/Application/Components/QuadColliderComponent.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#include "Public/Core/FileSystem/FileSystem.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Application/Components/Graphics/ParticleSystem2DComponent.h"
#include "Public/Core/Controller/CameraController.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Components/ScriptComponent.h"
#include "Public/Core/Collision/CollisionDetection.h"


namespace GEngine {

	Ref<ScriptObject> ScriptApi_DukTape::_copyObj;

	ScriptApi_DukTape::ScriptApi_DukTape()
	{
		CreateContext();
		Setup();
	}

	ScriptApi_DukTape::~ScriptApi_DukTape()
	{
		DestroyContext();
		
	}

	void ScriptApi_DukTape::CreateContext()
	{
		GE_CORE_ASSERT(!m_ctx, "CONTEXT ALREDY CREATED");
		m_inputPtr = new ScriptInput();
		m_scenePtr = new ScriptScene();
		m_mathPtr = new ScriptMath();
		m_ctx = duk_create_heap_default();
		duk_module_duktape_init(m_ctx);
	}

	void ScriptApi_DukTape::DestroyContext()
	{
		ScriptApi_DukTape::_copyObj = nullptr;
		delete m_inputPtr;
		delete m_scenePtr;
		delete m_mathPtr;

		duk_context* v = m_ctx;

		if (Application::GetApp() != nullptr && Application::GetApp()->IsRunning()) {
			ThreadPool::AddEndFrameFunction([v]() {
				duk_destroy_heap(v);
				});
		}
		else {
			duk_destroy_heap(v);
		}
		m_ctx = nullptr;
	}

	DukValue CopyObject(DukValue obj) {
		return *(DukValue*)ScriptApi_DukTape::_copyObj->CallSelf<DukValue>(obj)->GetNativeObject();
	}

	Ref<Texture2D> CreateTexture2D(const char* path, int flags) {
		return Texture2D::Create(path, flags);
	}

	Ref<SubTexture2D> CreateSubTexture2D(Ref<Texture2D> texture, Ref<ScriptVector2> coords, Ref<ScriptVector2> cellSize, Ref<ScriptVector2> spriteSize = nullptr) {
		return SubTexture2D::CreateFromCoords(texture, coords->GetGlm(), cellSize->GetGlm(), (spriteSize != nullptr ? spriteSize->GetGlm() : glm::vec2(1,1) ) );
	}

	Ref<Font> _CreateFont(std::string path, int size) {
		Ref<Font> f = Font::Create(path, size);
		f->LoadCharactersEN();
		return f;
	}

	void ScriptApi_DukTape::SetupObject() {
		//const char* script = "var f = function(obj) { return JSON.parse(JSON.stringify(obj));   }; f; ";
		//ScriptApi_DukTape::_copyObj = CreateObject(script);
		//dukglue_register_function(m_ctx, CopyObject, "CreateObject");
	}


	Ref<SpriteComponent> CreateSpriteComponent() {
		return CreateGameObject<SpriteComponent>();
	}

	Ref<UIComponent> CreateUIComponent() {
		return CreateGameObject<UIComponent>();
	}

	Ref<QuadColliderComponent> CreateQuadColliderComponent(bool dyamic, Ref<ScriptVector2> position, Ref<ScriptVector2> scale, float rotation) {
		return GEngine::CreateGameObject<GEngine::QuadColliderComponent>(dyamic, position->GetGlm(), scale->GetGlm(), rotation);
	}

	Ref<SpriteAnimationComponent> CreateSpriteAnimationComponent() {
		return CreateGameObject<SpriteAnimationComponent>();
	}

	Ref<ScriptVector4> CreateVector4(float x, float y, float z, float w) {
		return std::make_shared<ScriptVector4>(x, y, z, w);
	}

	Ref<ScriptVector3> CreateVector3(float x, float y, float z) {
		return std::make_shared<ScriptVector3>(x, y, z);
	}

	Ref<ScriptVector2> CreateVector2(float x, float y) {
		return std::make_shared<ScriptVector2>(x, y);
	}

	Ref<ScriptObject> CreateScriptObject(DukValue d) {
		return make_shared<DukTapeObject>(d);
	}

	Ref<ParticleSystem2DComponent> CreateParticleComponent(Ref<ParticleProps> props) {
		return CreateGameObject<ParticleSystem2DComponent>(props);
	}

	Ref<ParticleProps> CreateParticleProps() {
		return make_shared<ParticleProps>();
	}

	void SetCameraPosition(Ref<ScriptVector3> position) {
		GEngine::Application::GetApp()->GetTargetCameraController()->SetPosition(position->GetGlm());
	}

	void SetCameraZoom(float d) {
		GEngine::Application::GetApp()->GetTargetCameraController()->SetCameraZoom(d);
	}

	Ref<ScriptComponent> CreateScriptComponent(std::string name) {
		return CreateGameObject<ScriptComponent>(name.c_str());
	}

	Ref<Entity> CreateEntity() {
		Ref<Entity> e = CreateGameObject<Entity>();
		Ref<Scene> s = SceneManager::GetCurrentScene();
		if (s) {
			s->AddEntity(e);
			return e;
		}
		GE_CORE_ERROR("FAILED TO ADD ENTITY TO SCENE");
		return nullptr;
	}

	

	void ScriptApi_DukTape::Setup()
	{
		duk_logging_init(m_ctx, 0 /*flags*/);
		SetupLog();
		SetupRequire();
		SetupObject();

		/************************************************************************/
		/*                              HELPERS                                 */
		/************************************************************************/
		dukglue_register_function(m_ctx, CreateScriptObject, "toObject");
		dukglue_set_base_class<ScriptObject, DukTapeObject>(m_ctx);
		dukglue_register_function(m_ctx, SetCameraPosition, "SetCameraPosition");
		dukglue_register_function(m_ctx, SetCameraZoom, "SetCameraZoom");
		dukglue_register_function(m_ctx, &CollisionDetection::CheckPointComponent, "CheckPoint");
		

		/************************************************************************/
		/*                              VECTOR                                  */
		/************************************************************************/

		dukglue_register_function(m_ctx, CreateVector4, "Vector4");
		dukglue_register_property(m_ctx, &ScriptVector4::GetX, &ScriptVector4::SetX, "x");
		dukglue_register_property(m_ctx, &ScriptVector4::GetY, &ScriptVector4::SetY, "y");
		dukglue_register_property(m_ctx, &ScriptVector4::GetZ, &ScriptVector4::SetZ, "z");
		dukglue_register_property(m_ctx, &ScriptVector4::GetW, &ScriptVector4::SetZ, "w");
		dukglue_register_method(m_ctx, &ScriptVector4::Add, "Add");
		dukglue_register_method(m_ctx, &ScriptVector4::Subtract, "Subtract");
		dukglue_register_method(m_ctx, &ScriptVector4::Multiply, "Multiply");
		dukglue_register_method(m_ctx, &ScriptVector4::Divide, "Divide");

		dukglue_register_function(m_ctx, CreateVector3, "Vector3");
		dukglue_register_property(m_ctx, &ScriptVector3::GetX, &ScriptVector3::SetX, "x");
		dukglue_register_property(m_ctx, &ScriptVector3::GetY, &ScriptVector3::SetY, "y");
		dukglue_register_property(m_ctx, &ScriptVector3::GetZ, &ScriptVector3::SetZ, "z");
		dukglue_register_method(m_ctx, &ScriptVector3::Add, "Add");
		dukglue_register_method(m_ctx, &ScriptVector3::Subtract, "Subtract");
		dukglue_register_method(m_ctx, &ScriptVector3::Multiply, "Multiply");
		dukglue_register_method(m_ctx, &ScriptVector3::Divide, "Divide");
		dukglue_register_method(m_ctx, &ScriptVector3::Normalize, "Normalize");

		dukglue_register_function(m_ctx, CreateVector2, "Vector2");
		dukglue_register_property(m_ctx, &ScriptVector2::GetX, &ScriptVector2::SetX, "x");
		dukglue_register_property(m_ctx, &ScriptVector2::GetY, &ScriptVector2::SetY, "y");
		dukglue_register_method(m_ctx, &ScriptVector2::Add, "Add");
		dukglue_register_method(m_ctx, &ScriptVector2::Subtract, "Subtract");
		dukglue_register_method(m_ctx, &ScriptVector2::Multiply, "Multiply");
		dukglue_register_method(m_ctx, &ScriptVector2::Divide, "Divide");
		dukglue_register_method(m_ctx, &ScriptVector2::Normalize, "Normalize");

		/************************************************************************/
		/*                              Input                                  */
		/************************************************************************/


		dukglue_register_method(m_ctx, &FTouchInfo::GetID, "GetID");
		dukglue_register_method(m_ctx, &FTouchInfo::GetX, "GetX");
		dukglue_register_method(m_ctx, &FTouchInfo::GetY, "GetY");
		dukglue_register_method(m_ctx, &FTouchInfo::GetState, "GetState");
		dukglue_register_method(m_ctx, &FTouchInfo::GetForce, "GetForce");

		dukglue_register_global(m_ctx, m_inputPtr, "Input");
		dukglue_register_method(m_ctx, &ScriptInput::IsKeyPressed, "IsKeyPressed");
		dukglue_register_method(m_ctx, &ScriptInput::IsMouseButtonPressed, "IsMouseButtonPressed");
		dukglue_register_method(m_ctx, &ScriptInput::GetMouseX, "GetMouseX");
		dukglue_register_method(m_ctx, &ScriptInput::GetMouseY, "GetMouseY");
		dukglue_register_method(m_ctx, &ScriptInput::GetMousePosition, "GetMousePosition");
		dukglue_register_method(m_ctx, &ScriptInput::GetTouches, "GetTouches");
		dukglue_register_method(m_ctx, &ScriptInput::GetTouchCount, "GetTouchCount");


		/************************************************************************/
		/*                              Math                                    */
		/************************************************************************/
		dukglue_register_global(m_ctx, m_mathPtr, "GMath");
		dukglue_register_method(m_ctx, &ScriptMath::ScreenPosToWorldPos, "ScreenPosToWorldPos");

		/************************************************************************/
		/*                              Texture                                 */
		/************************************************************************/

		dukglue_register_function(m_ctx, CreateTexture2D, "CreateTexture2D");
		dukglue_register_function(m_ctx, _CreateFont, "CreateFont");
		dukglue_set_base_class<Font, OpenGL_Font>(m_ctx);
		dukglue_register_function(m_ctx, CreateSubTexture2D, "CreateTexture2DSub");
		dukglue_register_method(m_ctx, &Texture2D::GetHeight, "GetHeight");

		dukglue_set_base_class<Texture, Texture2D>(m_ctx);
		dukglue_set_base_class<Texture2D, OpenGL_Texture2D>(m_ctx);
		
		/************************************************************************/
		/*                              ECS                                     */
		/************************************************************************/
		//dukglue_register_constructor<Entity>(m_ctx, "Entity");
		dukglue_register_function(m_ctx, &CreateEntity, "Entity");
		dukglue_register_method(m_ctx, &Entity::AddComponent, "AddComponent");
		dukglue_register_method(m_ctx, &Entity::RemoveComponent, "RemoveComponent");
		dukglue_register_method(m_ctx, &Entity::RemoveComponent_ptr, "RemoveComponent");
		dukglue_register_method(m_ctx, &Entity::GetEntityPositionScript, "GetPosition");
		dukglue_register_method(m_ctx, &Entity::SetEntityPositionScript, "SetPosition");
		dukglue_register_method(m_ctx, &Entity::SetEntityRotationScript, "SetRotation");
		dukglue_register_method(m_ctx, &Entity::GetEntityRotationScript, "GetRotation");
		dukglue_register_method(m_ctx, &Entity::SetEntityScaleScript, "SetScale");
		dukglue_register_method(m_ctx, &Entity::GetEntityScaleScript, "GetScale");
		dukglue_register_method(m_ctx, &Entity::Destroy, "Destroy");
		dukglue_register_method(m_ctx, &Entity::GetComponentsByTag, "GetComponents");
		dukglue_register_property(m_ctx, &Entity::GetShouldUpdate, &Entity::SetShouldUpdate, "doesUpdate");
		
		dukglue_register_global(m_ctx, m_scenePtr, "SceneManager");
		dukglue_register_method(m_ctx, &ScriptScene::GetScene, "GetScene");
		dukglue_register_method(m_ctx, &ScriptScene::PauseScene, "PauseScene");
		dukglue_register_method(m_ctx, &ScriptScene::PlayScene, "PlayScene");
		dukglue_register_method(m_ctx, &ScriptScene::SetScene, "SetScene");


		/************************************************************************/
		/*                            COMPONENTS                                */
		/************************************************************************/

		dukglue_register_constructor<Component>(m_ctx, "Component");
		dukglue_register_property(m_ctx, &Component::GetSelf, &Component::SetSelf, "self");
		dukglue_register_property(m_ctx, &Component::GetEntityPtr, &Component::SetEntityPtr, "entity");
		dukglue_register_property(m_ctx, &Component::GetShouldUpdate, &Component::SetShouldUpdate, "doesUpdate");
		dukglue_register_property(m_ctx, &Component::GetTag, &Component::SetTag, "tag");

		dukglue_register_constructor<ScriptableComponent>(m_ctx, "Component");
		dukglue_register_property(m_ctx, &ScriptableComponent::GetParent, &ScriptableComponent::SetParent, "entity");
		dukglue_register_property(m_ctx, &ScriptableComponent::DoesUpdate, &ScriptableComponent::SetDoesUpdate, "doesUpdate");
		dukglue_set_base_class<Component, ScriptableComponent >(m_ctx);

		dukglue_register_function(m_ctx, CreateScriptComponent, "ScriptComponent");
		dukglue_register_method(m_ctx, &ScriptComponent::SetScriptProperty, "SetProperty");
		dukglue_set_base_class<Component, ScriptComponent >(m_ctx);

		dukglue_register_function(m_ctx, CreateSpriteComponent, "SpriteComponent");
		//dukglue_register_constructor<SpriteComponent>(m_ctx, "SpriteComponent");
		dukglue_register_method(m_ctx, &SpriteComponent::CreateQuadScript, "CreateQuad");
		dukglue_register_method(m_ctx, &SpriteComponent::SetQuadColorScript, "SetColor");
		dukglue_register_method(m_ctx, &SpriteComponent::SetSubTexture, "SetAtlasTexture");
		dukglue_register_method(m_ctx, &SpriteComponent::SetPositionScript, "SetPosition");
		dukglue_register_method(m_ctx, &SpriteComponent::SetZOrder, "SetZOrder");
		dukglue_register_method(m_ctx, &SpriteComponent::CreateSubTexturedQuadScript, "CreateQuadAtlas");
		dukglue_register_method(m_ctx, &SpriteComponent::RemoveQuad, "RemoveQuad");
		dukglue_register_method(m_ctx, &SpriteComponent::ClearQuads, "ClearQuads");
		dukglue_set_base_class<Component, SpriteComponent>(m_ctx);


		dukglue_register_function(m_ctx, CreateUIComponent, "UIComponent");
		//dukglue_register_constructor<UIComponent>(m_ctx, "UIComponent");
		dukglue_register_method(m_ctx, &UIComponent::CreateQuadScript, "CreateQuad");
		dukglue_register_method(m_ctx, &UIComponent::CreateSubTexturedQuadScript, "CreateQuadAtlas");
		dukglue_register_method(m_ctx, &UIComponent::CreateTextScript, "CreateText");
		dukglue_register_method(m_ctx, &UIComponent::SetPositionScript, "SetPosition");
		dukglue_register_method(m_ctx, &UIComponent::SetZOrder, "SetZOrder");
		dukglue_register_method(m_ctx, &UIComponent::Remove, "Remove");
		dukglue_register_method(m_ctx, &UIComponent::ClearQuads, "ClearQuads");
		dukglue_set_base_class<Component, UIComponent>(m_ctx);

		dukglue_register_function(m_ctx, CreateSpriteAnimationComponent, "SpriteAnimationComponent");
		dukglue_register_method(m_ctx, &SpriteAnimationComponent::SetFrameAnimation_Script, "SetFrameAnimation");
		dukglue_register_method(m_ctx, &SpriteAnimationComponent::RemoveFrameAnimation, "RemoveFrameAnimation");

		dukglue_register_method(m_ctx, &SpriteAnimationComponent::Stop, "Stop");
		dukglue_register_method(m_ctx, &SpriteAnimationComponent::Start, "Start");
		dukglue_set_base_class<Component, SpriteAnimationComponent >(m_ctx);



		dukglue_register_function(m_ctx, CreateQuadColliderComponent, "QuadColliderComponent");
		dukglue_register_method(m_ctx, &QuadColliderComponent::SetEndCollideFunction_Script, "SetOnCollideEndFunction");
		dukglue_register_method(m_ctx, &QuadColliderComponent::SetOnCollideFunction_Script, "SetOnCollideStartFunction");
		dukglue_register_method(m_ctx, &QuadColliderComponent::SetScale, "SetScale");
		dukglue_register_method(m_ctx, &QuadColliderComponent::SetPosition, "SetPosition");
		dukglue_register_method(m_ctx, &QuadColliderComponent::GetPositionScript, "GetPosition");
		dukglue_register_method(m_ctx, &QuadColliderComponent::GetScaleScript, "GetScale");
		dukglue_register_method(m_ctx, &QuadColliderComponent::RemoveOnCollideFunction, "RemoveOnCollideFunction");
		dukglue_register_method(m_ctx, &QuadColliderComponent::RemoveEndCollideFunction, "RemoveEndCollideFunction");
		dukglue_set_base_class<Component, QuadColliderComponent>(m_ctx);

		dukglue_register_function(m_ctx, CreateParticleComponent, "ParticleComponent");
		dukglue_register_method(m_ctx, &ParticleSystem2DComponent::EmitScript, "Emit");
		dukglue_register_method(m_ctx, &ParticleSystem2DComponent::ParticleCount, "Count");
		dukglue_set_base_class<Component, ParticleSystem2DComponent>(m_ctx);

		dukglue_register_function(m_ctx, CreateParticleProps, "ParticleProps");
		
		dukglue_register_property(m_ctx, &ParticleProps::GetVelocity, &ParticleProps::SetVelocity, "velocity");
		dukglue_register_property(m_ctx, &ParticleProps::GetVelocityVariation, &ParticleProps::SetVelocityVariation, "velocityVariation");
		dukglue_register_property(m_ctx, &ParticleProps::GetColorBegin, &ParticleProps::SetColorBegin, "colorBegin");
		dukglue_register_property(m_ctx, &ParticleProps::GetColorEnd, &ParticleProps::SetColorEnd, "colorEnd");
		dukglue_register_property(m_ctx, &ParticleProps::GetLifetime, &ParticleProps::SetLifeTime, "lifetime");
		dukglue_register_property(m_ctx, &ParticleProps::GetSizeBegin, &ParticleProps::SetSizeBegin, "sizeBegin");
		
		dukglue_register_property(m_ctx, &ParticleProps::GetSizeEnd, &ParticleProps::SetSizeEnd, "sizeEnd");
		dukglue_register_property(m_ctx, &ParticleProps::GetTexture, &ParticleProps::SetTexture, "texture");
		dukglue_register_property(m_ctx, &ParticleProps::GetSubTexture, &ParticleProps::SetSubTexture, "textureAtlas");
		dukglue_register_property(m_ctx, &ParticleProps::GetSizeVariation, &ParticleProps::SetSizeVariation, "sizeVariation");

	}


	Ref<ScriptObject> ScriptApi_DukTape::CreateObject(std::string script, std::string name)
	{
		try {
			uint32_t hash = std::hash<std::string>{}(script);
			DukValue obj = dukglue_peval<DukValue>(m_ctx, script.c_str());
			Ref<ScriptObject> scr = Ref<ScriptObject>((ScriptObject*)new DukTapeObject(obj));
			return scr;
		}
		catch (std::exception& e) {
			ScriptObject::AddError(e.what());
			return nullptr;
		}
	}

	const char* ScriptApi_DukTape::CheckScript(std::string script)
	{
		duk_push_string(m_ctx, script.c_str());
		if (duk_peval(m_ctx) != 0) {
			printf("eval failed: %s\n", duk_safe_to_string(m_ctx, -1));
			duk_uint_t flags = (duk_uint_t)duk_get_current_magic(m_ctx);
			duk_idx_t n = duk_get_top(m_ctx);
			duk_idx_t i;

			duk_get_global_string(m_ctx, "console");
			duk_get_prop_string(m_ctx, -1, "format");


			duk_push_error_object(m_ctx, DUK_ERR_ERROR, "%s", duk_require_string(m_ctx, -1));
			duk_push_string(m_ctx, "name");
			duk_push_string(m_ctx, "Compile Error");
			duk_def_prop(m_ctx, -3, DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE);
			duk_get_prop_string(m_ctx, -1, "stack");
			GE_CORE_ERROR("{0}", duk_to_string(m_ctx, -1));
			GE_CORE_ASSERT(false, "");
			duk_pop(m_ctx);
		}
		else {
			printf("result is: %s\n", duk_safe_to_string(m_ctx, -1));
		}
		duk_pop(m_ctx);

		return "";
	}

	duk_ret_t mod_search(duk_context* ctx) {
		/* Nargs was given as 4 and we get the following stack arguments:
		 *   index 0: id
		 *   index 1: require
		 *   index 2: exports
		 *   index 3: module
		 */


		// Pull Arguments
		const char* id = duk_require_string(ctx, 0);
		Ref<FileData> filedata = FileSystem::FileDataFromPath(id);

		if (filedata->GetDataSize() > 0) {

			std::string str((char*)filedata->GetDataAsString());
			duk_push_string(ctx, str.c_str());
			//duk_pop(ctx);
			return 1;

		}
		return -1;
	}

	void ScriptApi_DukTape::SetupRequire()
	{
		duk_get_global_string(m_ctx, "Duktape");
		duk_push_c_function(m_ctx, mod_search, 4 /*nargs*/);
		duk_put_prop_string(m_ctx, -2, "modSearch");
		duk_pop(m_ctx);
	}

	void duk__console_reg_vararg_func(duk_context* ctx, duk_c_function func, const char* name, duk_uint_t flags) {
		duk_push_c_function(ctx, func, DUK_VARARGS);
		duk_push_string(ctx, "name");
		duk_push_string(ctx, name);
		duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_FORCE);  /* Improve stacktraces by displaying function name */
		duk_set_magic(ctx, -1, (duk_int_t)flags);
		duk_put_prop_string(ctx, -2, name);
	}

	duk_ret_t duk__console_log_helper(duk_context* ctx, const char* error_name) {
		duk_uint_t flags = (duk_uint_t)duk_get_current_magic(ctx);
		duk_idx_t n = duk_get_top(ctx);
		duk_idx_t i;

		duk_get_global_string(ctx, "console");
		duk_get_prop_string(ctx, -1, "format");

		for (i = 0; i < n; i++) {
			if (duk_check_type_mask(ctx, i, DUK_TYPE_MASK_OBJECT)) {
				/* Slow path formatting. */
				duk_dup(ctx, -1);  /* console.format */
				duk_dup(ctx, i);
				duk_call(ctx, 1);
				duk_replace(ctx, i);  /* arg[i] = console.format(arg[i]); */
			}
		}


		duk_pop_2(ctx);

		duk_push_string(ctx, " ");
		duk_insert(ctx, 0);
		duk_join(ctx, n);


#ifdef GE_CORE_DEBUG
		if (error_name) {
			duk_push_error_object(ctx, DUK_ERR_ERROR, "%s", duk_require_string(ctx, -1));
			duk_push_string(ctx, "name");
			duk_push_string(ctx, error_name);
			duk_def_prop(ctx, -3, DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE);  
			duk_get_prop_string(ctx, -1, "stack");
		}
#endif

		if (error_name == "Trace") {
			GE_LOG_TRACE("{0}", duk_to_string(ctx, -1));
		} else if (error_name == "warn") {
			GE_LOG_WARN("{0}", duk_to_string(ctx, -1));
		}
		else if (error_name == "Error") {
			GE_LOG_ERROR("{0}", duk_to_string(ctx, -1));
		}
		else if (error_name == "debug") {
			GE_LOG_DEBUG("{0}", duk_to_string(ctx, -1));
		}
		else if (error_name == "assert") {
			GE_CORE_ASSERT(false, duk_to_string(ctx, -1));
		}
		else {
			GE_LOG_INFO("{0}", duk_to_string(ctx, -1));
		}


		return 0;
	}

	duk_ret_t duk__console_log(duk_context* ctx) {
		return duk__console_log_helper(ctx, NULL);
	}

	duk_ret_t duk__console_debug(duk_context* ctx) {
		return duk__console_log_helper(ctx, "debug");
	}

	duk_ret_t duk__console_trace(duk_context* ctx) {
		return duk__console_log_helper(ctx, "Trace");
	}

	duk_ret_t duk__console_info(duk_context* ctx) {
		return duk__console_log_helper(ctx, "info");
	}

	duk_ret_t duk__console_warn(duk_context* ctx) {
		return duk__console_log_helper(ctx, "warn");
	}

	duk_ret_t duk__console_error(duk_context* ctx) {
		return duk__console_log_helper(ctx, "Error");
	}

	duk_ret_t duk__console_dir(duk_context* ctx) {
		/* For now, just share the formatting of .log() */
		return duk__console_log_helper(ctx, 0);
	}

	static duk_ret_t duk__console_assert(duk_context* ctx) {
		if (duk_to_boolean(ctx, 0)) {
			return 0;
		}
		duk_remove(ctx, 0);

		return duk__console_log_helper(ctx, "assert");
	}

	void ScriptApi_DukTape::SetupLog()
	{
		duk_uint_t flags_orig;

		uint32_t flags = (1U << 2);
	

		duk_push_object(m_ctx);

		/* Custom function to format objects; user can replace.
		 * For now, try JX-formatting and if that fails, fall back
		 * to ToString(v).
		 */
		duk_eval_string(m_ctx,
			"(function (E) {"
			"return function format(v){"
			"try{"
			"return E('jx',v);"
			"}catch(e){"
			"return String(v);"  /* String() allows symbols, ToString() internal algorithm doesn't. */
			"}"
			"};"
			"})(Duktape.enc)");
		duk_put_prop_string(m_ctx, -2, "format");
			
		duk__console_reg_vararg_func(m_ctx, duk__console_assert, "assert", flags);
		duk__console_reg_vararg_func(m_ctx, duk__console_log, "log", flags);
		duk__console_reg_vararg_func(m_ctx, duk__console_debug, "debug", flags);  /* alias to console.log */
		duk__console_reg_vararg_func(m_ctx, duk__console_trace, "trace", flags);
		duk__console_reg_vararg_func(m_ctx, duk__console_info, "info", flags);


		duk__console_reg_vararg_func(m_ctx, duk__console_warn, "warn", flags);
		duk__console_reg_vararg_func(m_ctx, duk__console_error, "error", flags);
		duk__console_reg_vararg_func(m_ctx, duk__console_error, "exception", flags);  /* alias to console.error */
		duk__console_reg_vararg_func(m_ctx, duk__console_dir, "dir", flags);
	
		duk_put_global_string(m_ctx, "console");
	}

	void TestData::destroy()
	{
			//ScriptApi::s_api.lock()->InvalidateObject(self.lock());
		((ScriptTestClass*)par)->destroyTestClass();
			
	}

	}

