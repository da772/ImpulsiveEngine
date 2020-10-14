#pragma once
#include <GEngine.h>

using namespace GEngine;


class PolygonLightRendererable : public Renderable {



public:
    PolygonLightRendererable(const std::vector<float>& vertices,const std::vector<uint32_t>& indices, Ref<BufferLayout> layout);
    ~PolygonLightRendererable();
    void Render() override;

private:
    Ref<VertexArray> m_vertexArray;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    Ref<Shader> m_Shader;
};


class LightComponent : public Component {

public:
	LightComponent();
	virtual ~LightComponent();
    
    long AddCircleLight(const glm::vec2& position, float intensity, const glm::vec2& scale, const glm::vec4& color);
    void EditCircleColor(long id, const glm::vec4& color);
    void EditCircleSize(long id, const glm::vec2& size);

    long AddPolygonLight(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, Ref<BufferLayout> layout);


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
    std::vector<Ref<PolygonLightRendererable>> m_polygonLights;
    void CreateGraphics();
  
};
