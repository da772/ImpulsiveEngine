#include "TestScript.h"

TestScript::TestScript(Entity* e) : Component(e)
{

}

TestScript::~TestScript()
{

}

void TestScript::UnloadGraphics()
{

}

void TestScript::ReloadGraphics()
{

}

void TestScript::OnBegin()
{
	GE_LOG_DEBUG("TEST SCRIPT BEGIN");
}

void TestScript::OnEnd()
{
	GE_LOG_DEBUG("TEST SCRIPT END");
}

void TestScript::OnUpdate(Timestep timestep)
{

}

