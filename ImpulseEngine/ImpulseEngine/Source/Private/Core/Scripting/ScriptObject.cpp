#include "gepch.h"

#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/Application/Input.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#include "Public/Core/Util/KeyCodes.h"
#include "Public/Core/Util/Utility.h"

namespace GEngine {



	std::string ScriptObject::GetError()
	{
		std::string s = ScriptObject::s_ErrorStack.front();
		ScriptObject::s_ErrorStack.pop();
		return s;
	}

	void ScriptObject::AddError(std::string s)
	{
		ScriptObject::s_ErrorStack.push(s);
	}

	bool ScriptObject::HasError()
	{
		return ScriptObject::s_ErrorStack.size() > 0;
	}

	std::queue<std::string> ScriptObject::s_ErrorStack;

	bool ScriptInput::IsKeyPressed(std::string keyCode)
	{
		std::transform(keyCode.begin(), keyCode.end(), keyCode.begin(), ::toupper);
		if (keycodes.find(keyCode) != keycodes.end()) {
			return Input::IsKeyPressed(keycodes[keyCode]);
		}
		GE_CORE_ERROR("INVALID KEYCODE: {0}", keyCode);
		return false;
	}

	bool ScriptInput::IsMouseButtonPressed(int mouseButton)
	{
		return Input::IsMouseButtonPressed(mouseButton);
	}

	int ScriptInput::GetMouseX()
	{
		return Input::GetMouseX();
	}

	int ScriptInput::GetMouseY()
	{
		return Input::GetMouseY();
	}

	GEngine::Ref<GEngine::ScriptVector2> ScriptInput::GetMousePosition()
	{
		std::pair<float, float> pos = Input::GetMousePosition();
		return std::make_shared<ScriptVector2>(glm::vec2(pos.first, pos.second));
	}

	std::vector<Ref<FTouchInfo>> ScriptInput::GetTouches()
	{
		std::vector <Ref<FTouchInfo>> v;
		std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input::GetTouches();
		for (std::pair<uint64_t, FTouchInfo> pair : t) {
			v.push_back(make_shared<FTouchInfo>(pair.second));
		}
		return v;
	}

	uint32_t ScriptInput::GetTouchCount()
	{
		return Mobile_Input::GetTouchCount();
	}


	void ScriptScene::SetScene(std::string name)
	{
		std::string n = name;
		SceneManager::SetCurrentScene(n);
	}

	void ScriptScene::PauseScene()
	{
		SceneManager::GetCurrentScene()->Pause(true);
	}

	void ScriptScene::PlayScene()
	{
		SceneManager::GetCurrentScene()->Pause(false);
	}

	std::string ScriptScene::GetScene()
	{
		return SceneManager::GetCurrentScene()->GetId();
	}


	std::unordered_map<std::string, int> ScriptInput::keycodes =
	{
		{"`", GE_KEY_TILDE},
		{"F1", GE_KEY_F1},
		{"F2", GE_KEY_F2},
		{"F3", GE_KEY_F3},
		{"F4", GE_KEY_F4},
		{"F5", GE_KEY_F5},
		{"F6", GE_KEY_F6},
		{"F7", GE_KEY_F7},
		{"F8", GE_KEY_F8},
		{"F9", GE_KEY_F9},
		{"F10", GE_KEY_F10},
		{"F11", GE_KEY_F11},
		{"F12", GE_KEY_F12},
		{"1", GE_KEY_1},
		{"2", GE_KEY_2},
		{"3", GE_KEY_3},
		{"4", GE_KEY_4},
		{"5", GE_KEY_5},
		{"6", GE_KEY_6},
		{"7", GE_KEY_7},
		{"8", GE_KEY_8},
		{"9", GE_KEY_9},
		{"0", GE_KEY_0},
		{"-", GE_KEY_MINUS},
		{"=", GE_KEY_EQUAL},
		{"Q", GE_KEY_Q},
		{"W", GE_KEY_W},
		{"E", GE_KEY_E},
		{"R", GE_KEY_R},
		{"T", GE_KEY_T},
		{"T", GE_KEY_Y},
		{"U", GE_KEY_U},
		{"I", GE_KEY_I},
		{"O", GE_KEY_O},
		{"P", GE_KEY_P},
		{"[", GE_KEY_LEFT_BRACKET},
		{"]", GE_KEY_RIGHT_BRACKET},
		{"\\", GE_KEY_BACKSLASH},
		{"A", GE_KEY_A},
		{"S", GE_KEY_S},
		{"D", GE_KEY_D},
		{"F", GE_KEY_F},
		{"G", GE_KEY_G},
		{"H", GE_KEY_H},
		{"J", GE_KEY_J},
		{"K", GE_KEY_K},
		{"L", GE_KEY_L},
		{";", GE_KEY_SEMICOLON},
		{"'", GE_KEY_APOSTROPHE},
		{"Z", GE_KEY_Z},
		{"X", GE_KEY_X},
		{"C", GE_KEY_C},
		{"V", GE_KEY_V},
		{"B", GE_KEY_B},
		{"N", GE_KEY_N},
		{"M", GE_KEY_M},
		{",", GE_KEY_COMMA},
		{".", GE_KEY_PERIOD},
		{"/", GE_KEY_SLASH},
		{"CTRL", GE_KEY_LEFT_CONTROL},
		{"LCTRL", GE_KEY_LEFT_CONTROL},
		{"LALT", GE_KEY_LEFT_ALT},
		{"ALT", GE_KEY_LEFT_ALT},
		{"SHIFT", GE_KEY_LEFT_SHIFT},
		{"LSHIFT", GE_KEY_LEFT_SHIFT},
		{"TAB", GE_KEY_TAB},
		{"CAPS", GE_KEY_CAPS_LOCK},
		{"WINDOWS", GE_KEY_LEFT_SUPER},
		{"CMD", GE_KEY_LEFT_SUPER},
		{"LCMD", GE_KEY_LEFT_SUPER},
		{"SPACE", GE_KEY_SPACE},
		{" ", GE_KEY_SPACE},
		{"RALT", GE_KEY_RIGHT_ALT},
		{"RCTRL", GE_KEY_RIGHT_CONTROL},
		{"RSHIFT", GE_KEY_RIGHT_SHIFT},
		{"RCMD", GE_KEY_RIGHT_SUPER},
		{"ENTER", GE_KEY_ENTER},
		{"BACKSPACE", GE_KEY_BACKSPACE},
		{"BCKSPCE", GE_KEY_BACKSPACE},
		{"ESCAPE", GE_KEY_ESCAPE},
		{"ESC", GE_KEY_ESCAPE},
		{"LEFT", GE_KEY_LEFT},
		{"RIGHT", GE_KEY_RIGHT},
		{"UP", GE_KEY_UP},
		{"DOWN", GE_KEY_DOWN}

	};


	Ref<GEngine::ScriptVector2> ScriptMath::ScreenPosToWorldPos(Ref<ScriptVector2> pos)
	{
		glm::vec3 p = Utility::ScreenPosToWorldPos(pos->GetGlm());
		return make_shared<ScriptVector2>(glm::vec2(p.x ,p.y));
	}

}
