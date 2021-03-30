#include "TestScript.h"
#include "Logger.h"

TestScript::TestScript(Entity* e) : Component(e)
{
	GE_LOG_DEBUG("TEST SCRIPT CREATED: "+Factory::HashToString(go_hash));
	bUpdates = true;
	GetEntity()->AddComponent<AudioListenerComponent>();
	auto audioComp = GetEntity()->AddComponent<AudioComponent>("Content/Audio/windLoop.ogg", true, true, false,.5f);
	auto refreshButton = GetEntity()->AddComponent<ButtonComponent>( Vector3f(-.5f, 0, 10), 0.f, Vector2f(.25f, .15f), Vector4f(0, 1, 0, 1.f));
	refreshButton->SetImageTexture(Texture2D::Create("Content/Textures/videoLife_button_7.png", 192));
	refreshButton->SetOnEvent([refreshButton, audioComp](const Event& e){
		if (e.GetEventType() == EventType::MouseButtonReleased || e.GetEventType() == EventType::TouchReleased) {
			if (audioComp->GetVolume() <= 0) {
				audioComp->SetVolume(.25f);
				refreshButton->SetImageColor({0,1.f,0,1});
			} else {
				audioComp->SetVolume(0.f);
				refreshButton->SetImageColor({0,.25f,0,1});
			}
		}
	});
}

TestScript::~TestScript()
{
	GE_LOG_DEBUG("TEST SCRIPT DELTEED");
}

void TestScript::UnloadGraphics()
{
    
}

void TestScript::ReloadGraphics()
{
    
}


void TestScript::OnBegin()
{

    GE_LOG_DEBUG("TEST SCRIPT BEGIN {0}:{1} YEEE {2}", "TEST THIS YEE ", 1231, 21231.2f);
	TestFunc();

	NewFunct(3213);
}


void TestScript::OnEnd()
{
	GE_LOG_WARN("TEST SCIPR END");
}

void TestScript::OnUpdate(Timestep timestep)
{
	//GE_LOG_DEBUG("HELLO WORLD: {0}", 25);
}

void TestScript::TestFunc()
{
	GE_LOG_TRACE("TESTFUNC YES NO MAYBE SO");
}

void TestScript::NewFunct(int i)
{
	GE_LOG_WARN("NEW FUNCT: {0}", i);
}

