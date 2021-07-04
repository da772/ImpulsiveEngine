#pragma once
#define GE_DEBUG_TOOLS
#include <GEngine.h>

class ExampleLayer : public GEngine::Layer
{
public:

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

class ImpulseEditor : public GEngine::Application
{

public:
	ImpulseEditor();
	
	virtual ~ImpulseEditor();

	ExampleLayer* m_ExampleLayer;
	GEngine::Layer* m_ProjectSelectLayer;

	virtual void OnCleanDirtyApi() override;
	virtual void OnImGuiSetup() override;

	virtual void OnUpdate(GEngine::Timestep timeStep) override;

#ifdef GE_EDITOR
	ImFont* smallFont;
	ImFont* largeFont;
	ImFont* mainFont;
	GEngine::Ref<GEngine::FileData> fd = nullptr;
#endif
};


