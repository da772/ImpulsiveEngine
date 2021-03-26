#include "TestScript.h"
#include "Logger.h"

TestScript::TestScript(Entity* e) : Component(e)
{
	GE_LOG_DEBUG("TEST SCRIPT CREATED: "+Factory::HashToString(go_hash));
	bUpdates = true;
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
	GE_LOG_TRACE("TESTFUNC CALLED");
}

