#pragma once
#include <GEngine.h>

using namespace GEngine;


class LightComponent : public Component {

public:
	LightComponent(Entity* e);
	virtual ~LightComponent();
    
    const ShapeID AddCircleLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color);
    const ShapeID AddQuadLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color, Ref<Texture2D> texture = nullptr);
    void EditCircleColor(const ShapeID id, const Vector4f& color);
    void EditCircleSize(const ShapeID id, const Vector2f& size);

    void RemoveQuadLight(const ShapeID id);
    void RemoveCircleLight(const ShapeID id);


    const std::vector<ShapeID>& GetCircleLights() const { return m_Circleids; };
    const std::vector<ShapeID>& GetQuadLights() const { return m_Quadids; };
    Ref<BatchRenderer> GetCircleRenderer() const { return s_CircleShapeFactory; }
    Ref<BatchRenderer> GetQuadRenderer() const { return s_QuadShapeFactory; }

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
    void CreateGraphics();
    
  
};
