#pragma once

#include <GEngine.h>


class DebugLayer : public GEngine::Layer {
public:

	DebugLayer();

	virtual void OnImGuiRender() override;

	virtual void OnEvent(GEngine::Event& event) override;

	void ShowDock(bool p_open);

	void CreateSceneHierarchy();

	void CreateViewPort();

	void CreateGraphicsDebuggger();

	GEngine::Vector3f m_sqColor= { .3f,.2f,.8f };
	bool showDock = true;
	static bool showLog;
	static bool showScene;
	static bool showGraphicsDebugger;



};