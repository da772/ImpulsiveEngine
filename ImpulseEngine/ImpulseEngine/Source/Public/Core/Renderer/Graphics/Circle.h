#pragma once
#include "Public/Core/Renderer/Graphics/Shape.h"


namespace GEngine {

    struct CircleVertex {
        Vector3 position;
        Vector4 color;
        Vector2 texcoord;
        float texture;
        glm::vec2 textureScale;
        float alphaChannel;
        Vector2 startPos;
    };

    class Circle : public Shape {
    public:
        Circle();
        virtual ~Circle();

        virtual u32 GetVerticesRows() override;
        virtual u32 GetVerticesSize() override;

        virtual std::vector< u32 > GetIndices(u32 offset) override;
        virtual std::vector<float> GetVertices(Vector3 position, float rotation = 0.f, Vector3 scale = Vector3(1, 1, 1),
            Vector4 color = Vector4(1, 1, 1, 1), u32 texture = 0, glm::vec2 textureScale = { 1,1 }, const Vector2* textureCoords = nullptr, float alphaChannel = 4) override;
        virtual void SetZPosition(std::vector<float>& arr, float zPos);
        virtual void SetTextureSlot(std::vector<float>& arr, int texture);
    

    private:
        static Ref<BufferLayout> s_CircleBufferLayout;


    };


}


