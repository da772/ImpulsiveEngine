#pragma once

#include <GEngine.h>


class DebugLayer : public GEngine::Layer {
public:

	DebugLayer();

	virtual void OnImGuiRender() override;

	virtual void OnEvent(GEngine::Event& event) override;

	void ShowDock(bool p_open);

	glm::vec3 m_sqColor = { .3f,.2f,.8f };
	bool showDock = true;
	static bool showLog;



};