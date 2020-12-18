#pragma once
#include <GEngine.h>

using namespace GEngine;


class PolygonLightRendererable : public Renderable {



public:
    PolygonLightRendererable(const Vector3f& position, const std::vector<float>& vertices,const std::vector<uint32_t>& indices, Ref<BufferLayout> layout, const Vector4f& color);
    ~PolygonLightRendererable();
    void Render() override;

    void Unload();
    void Reload();

    Vector3f m_position;

private:
    Ref<VertexArray> m_vertexArray;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
    Ref<BufferLayout> m_bufferLayout;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    Ref<Shader> m_Shader;
    Vector4f m_color;
   

    void CreateGraphics();
};


class LightComponent : public Component {

public:
	LightComponent();
	virtual ~LightComponent();
    
    const ShapeID AddCircleLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color);
    const ShapeID AddQuadLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color, Ref<Texture2D> texture = nullptr);
    void EditCircleColor(const ShapeID id, const Vector4f& color);
    void EditCircleSize(const ShapeID id, const Vector2f& size);

    const ShapeID AddPolygonLight(const Vector3f& position, const std::vector<float>& vertices, const std::vector<uint32_t>& indices, Ref<BufferLayout> layout, const Vector4f& color);

    void RemoveQuadLight(const ShapeID id);

    void RemovePolygonLight(const ShapeID id);
    void RemoveCircleLight(const ShapeID id);

	virtual void OnAttached(Ref<Entity> entity) override;
	virtual void DeAttached(Ref<Entity> entity) override;

protected:
    std::vector<ShapeID> m_Circleids;
    std::vector<ShapeID> m_Quadids;
	void OnBegin() override;
	void OnEnd() override;
	void OnUpdate(Timestep timestep) override;
    virtual void UnloadGraphics() override;
    virtual void ReloadGraphics() override;
    Ref<Shader> m_Shader;
    static Ref<BatchRenderer> s_CircleShapeFactory;
    static Ref<BatchRenderer> s_QuadShapeFactory;
    std::vector<Ref<PolygonLightRendererable>> m_polygonLights;
    void CreateGraphics();
    uint32_t m_polygonLightCounter = 0;
    std::unordered_map<ShapeID, Weak<PolygonLightRendererable>> m_polygonLightMap;
  
};
