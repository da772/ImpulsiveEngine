#pragma once

#include <GEngine.h>

class EditorLayer : public GEngine::Layer {

public:
	static EditorLayer* Create();
	static void Destroy();
	virtual ~EditorLayer();

	void Begin() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(GEngine::Timestep timeStep) override;
	void OnEvent(GEngine::Event& event) override;
	void OnImGuiRender() override;
	void OnDraw() override;
	void End() override;

	static EditorLayer* GetEditor();

private:
	
	EditorLayer(const std::string& name);
	static EditorLayer* s_singleton;

};