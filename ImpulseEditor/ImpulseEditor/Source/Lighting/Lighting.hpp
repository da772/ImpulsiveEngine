#pragma once

#include <GEngine.h>

using namespace GEngine;

class Lighting {


public:

	static void Initialize();




private:
	static Ref<RenderPipeline> m_pipeline;
};