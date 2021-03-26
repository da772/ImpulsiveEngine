#pragma once
#include "Public/Core/Renderer/Graphics/Shape.h"


namespace GEngine {

    struct CircleVertex {
        Vector3f position;
        Vector4f color;
        Vector2f texcoord;
        float texture;
        Vector2f textureScale;
        float alphaChannel;
        Vector2f startPos;
    };

    class Circle : public Shape {
    public:
        Circle();
        virtual ~Circle();

        virtual u32 GetVerticesRows() override;
        virtual u32 GetVerticesSize() override;

        virtual std::vector< u32 > GetIndices(u32 offset) override;
        virtual std::vector<float> GetVertices(Vector3f position, float rotation = 0.f, Vector3f scale = Vector3f(1, 1, 1),
            Vector4f color = Vector4f(1, 1, 1, 1), u32 texture = 0, Vector2f textureScale = { 1,1 }, const Vector2f* textureCoords = nullptr, float alphaChannel = 4) override;
        virtual void SetZPosition(std::vector<float>& arr, float zPos) override;
        virtual void SetTextureSlot(std::vector<float>& arr, int texture) override;
    

    private:
        static Ref<BufferLayout> s_CircleBufferLayout;


    };


}


