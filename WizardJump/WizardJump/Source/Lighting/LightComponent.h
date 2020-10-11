#pragma once
#include <GEngine.h>

using namespace GEngine;

class LightComponent : public Component {

public:
	LightComponent();
	~LightComponent();

protected:
	void OnBegin() override;
	void OnEnd() override;
	void OnUpdate(Timestep timestep) override;



};