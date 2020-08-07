#pragma once
#include <GEngine.h>
#include "DebugLayer.h"

class ExampleLayer : public GEngine::Layer
{
public:

	ImFont* font2;

	ExampleLayer();
	~ExampleLayer();

	virtual void OnImGuiRender() override;

	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(GEngine::Timestep timeStep) override;

	virtual void OnDraw() override;

	virtual void OnEvent(GEngine::Event& event) override;

private:

	GEngine::Application* app;

};

class Sandbox : public GEngine::Application
{

public:
	Sandbox();

	ExampleLayer* m_ExampleLayer;
	GEngine::Layer* m_DebugLayer;

	virtual void OnCleanDirtyApi() override;

	virtual void OnUpdate(GEngine::Timestep timeStep) override;

	inline ~Sandbox() {};


};


