#pragma once
#include <GEngine.h>

using namespace GEngine;


class LightComponent : public Component {

public:
	LightComponent();
	virtual ~LightComponent();
    
    long AddCircleLight(const glm::vec2& position, float intensity, const glm::vec2& scale, const glm::vec4& color);
    void EditCircleColor(long id, const glm::vec4& color);
    void RemoveCircleLight(long id);

protected:
    std::vector<long> m_ids;
	void OnBegin() override;
	void OnEnd() override;
	void OnUpdate(Timestep timestep) override;
    virtual void UnloadGraphics() override;
    virtual void ReloadGraphics() override;
    Ref<Shader> m_Shader;
    static Ref<BatchRenderer> s_ShapeFactory;
    
    void CreateGraphics();
  
};
