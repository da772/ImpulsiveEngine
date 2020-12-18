
#include "gepch.h"

#include "Public/Core/Renderer/Graphics/Circle.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GEngine {


    Ref<BufferLayout> Circle::s_CircleBufferLayout = Ref<BufferLayout>(new BufferLayout({
            {GEngine::ShaderDataName::Position },
            {GEngine::ShaderDataName::Color },
            {GEngine::ShaderDataName::TextureCoord},
            {GEngine::ShaderDataName::TextureSlot},
            {GEngine::ShaderDataName::TextureScale},
            {GEngine::ShaderDataName::AlphaChannel},
            {GEngine::ShaderDataName::Position2D}
        }));

    Circle::Circle() {
        m_BufferLayout = s_CircleBufferLayout;
        m_Indices = {
            1, 0, 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 6, 5, 0, 7, 6, 0, 8, 7, 0, 9, 8, 0, 10, 9, 0, 11, 10, 0, 12, 11, 0, 13, 12, 0, 14, 13, 0, 15, 14, 0, 16, 15, 0, 17, 16, 0, 18, 17, 0, 19, 18, 0, 20, 19, 0, 21, 20, 0, 22, 21, 0, 23, 22, 0, 24, 23, 0, 25, 24, 0, 26, 25, 0, 27, 26, 0, 28, 27, 0, 29, 28, 0, 30, 29, 0, 31, 30, 0, 32, 31, 0, 33, 32, 0, 34, 33, 0, 35, 34, 0, 1, 35, 0

        };
    }

static const Vector4f quadPos[] = {
    {0.000f, 0.000f, 0.0f, 1.0f},
    {0.174f, 0.985f, 0.0f, 1.0f},
    {0.342f, 0.940f, 0.0f, 1.0f},
    {0.500f, 0.866f, 0.0f, 1.0f},
    {0.643f, 0.766f, 0.0f, 1.0f},
    {0.766f, 0.643f, 0.0f, 1.0f},
    {0.866f, 0.500f, 0.0f, 1.0f},
    {0.940f, 0.342f, 0.0f, 1.0f},
    {0.985f, 0.174f, 0.0f, 1.0f},
    {1.000f, 0.000f, 0.0f, 1.0f},
    {0.985f, -0.174f, 0.0f, 1.0f},
    {0.940f, -0.342f, 0.0f, 1.0f},
    {0.866f, -0.500f, 0.0f, 1.0f},
    {0.766f, -0.643f, 0.0f, 1.0f},
    {0.643f, -0.766f, 0.0f, 1.0f},
    {0.500f, -0.866f, 0.0f, 1.0f},
    {0.342f, -0.940f, 0.0f, 1.0f},
    {0.174f, -0.985f, 0.0f, 1.0f},
    {0.000f, -1.000f, 0.0f, 1.0f},
    {-0.174f, -0.985f, 0.0f, 1.0f},
    {-0.342f, -0.940f, 0.0f, 1.0f},
    {-0.500f, -0.866f, 0.0f, 1.0f},
    {-0.643f, -0.766f, 0.0f, 1.0f},
    {-0.766f, -0.643f, 0.0f, 1.0f},
    {-0.866f, -0.500f, 0.0f, 1.0f},
    {-0.940f, -0.342f, 0.0f, 1.0f},
    {-0.985f, -0.174f, 0.0f, 1.0f},
    {-1.000f, -0.000f, 0.0f, 1.0f},
    {-0.985f, 0.174f, 0.0f, 1.0f},
    {-0.940f, 0.342f, 0.0f, 1.0f},
    {-0.866f, 0.500f, 0.0f, 1.0f},
    {-0.766f, 0.643f, 0.0f, 1.0f},
    {-0.643f, 0.766f, 0.0f, 1.0f},
    {-0.500f, 0.866f, 0.0f, 1.0f},
    {-0.342f, 0.940f, 0.0f, 1.0f},
    {-0.174f, 0.985f, 0.0f, 1.0f},
};
Circle::~Circle()
    {
        m_BufferLayout.reset();
    }

    u32 Circle::GetVerticesRows()
    {
        return sizeof(quadPos)/sizeof(Vector4f);
    }

    u32 Circle::GetVerticesSize()
    {
        // AMT * ROWS
        return m_BufferLayout.lock()->GetStride()/sizeof(float)*GetVerticesRows();
    }

    std::vector<u32> Circle::GetIndices(u32 offset)
    {
        std::vector<u32> ind = std::vector<u32>(m_Indices.size());
        
        for (int i = 0; i < m_Indices.size(); i++) {
            ind[i] = m_Indices[i] + offset;
        }

        return ind;
    }

    std::vector<float> Circle::GetVertices(Vector3f position, float rotation, Vector3f scale /*= Vector3f(1,1,1)*/,
        Vector4f color /*= Vector4f(1,1,1,1)*/, u32 texture /*= 0*/, Vector2f textureScale, const Vector2f* textureCoords, float alphaChannel)
    {
        
        std::vector<CircleVertex> vert;

        glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z))
            * glm::rotate(glm::mat4(1.f), glm::radians(rotation), { 0,0,1.f })
            * glm::scale(glm::mat4(1.0), { scale.x, scale.y, 1.f });
        
        const Vector2f texCoordPos[] = { {0,0},{1,0},{1,1},{0,1} };
        uint32_t quadLen = GetVerticesRows();
        {
            Vector4f _p = Vector4f(glm::value_ptr(transform * glm::vec4(quadPos[0].x, quadPos[0].y, quadPos[0].z, quadPos[0].z)));
			Vector4f _sp = Vector4f(glm::value_ptr(transform * glm::vec4(quadPos[0].x, quadPos[0].y, 0, 1)));
			vert.push_back({ Vector3f(_p.x,_p.y,_p.z),color, (textureCoords != nullptr ? Vector2f(textureCoords[0]) : texCoordPos[0]), (float)texture, textureScale,0, Vector2f(_sp.x,_sp.y) });
        }
        for (int i = 1; i < quadLen; i++) {
            Vector4f _p = Vector4f(glm::value_ptr(transform * glm::vec4(quadPos[i].x, quadPos[i].y, quadPos[i].z, quadPos[i].z)));
            Vector4f _sp = Vector4f(glm::value_ptr(transform * glm::vec4(quadPos[0].x, quadPos[0].y, 0, 1)));
            vert.push_back({ Vector3f(_p.x,_p.y,_p.z),color, (textureCoords != nullptr ? Vector2f(textureCoords[i]) : texCoordPos[i%4]), (float)texture, textureScale,1, Vector2f(_sp.x,_sp.y) });
        }

        std::vector<float> _f((float*)&vert[0], (float*)&vert[0]+GetVerticesSize());
        vert.clear();
        return _f;
    }


    void Circle::SetZPosition(std::vector<float>& arr, float zPos)
    {
        for (int i = 0; i < GetVerticesSize(); i += GetVerticesSize()/GetVerticesRows()) {
            arr[i + 2] = zPos;
        }
    }

    void Circle::SetTextureSlot(std::vector<float>& arr, int texture)
    {
        for (int i = 0; i < GetVerticesSize(); i += GetVerticesSize() / GetVerticesRows()) {
            arr[i + 9] = (float)texture;
        }
    }


};



/* CIRCLE POINT EVERY 10 DEGREES

 {
 {0.000f, 0.000f, 0.0f, 1.0f},
 {0.000f, 1.000f, 0.0f, 1.0f},
 {0.174f, 0.985f, 0.0f, 1.0f},
 {0.342f, 0.940f, 0.0f, 1.0f},
 {0.500f, 0.866f, 0.0f, 1.0f},
 {0.643f, 0.766f, 0.0f, 1.0f},
 {0.766f, 0.643f, 0.0f, 1.0f},
 {0.866f, 0.500f, 0.0f, 1.0f},
 {0.940f, 0.342f, 0.0f, 1.0f},
 {0.985f, 0.174f, 0.0f, 1.0f},
 {1.000f, 0.000f, 0.0f, 1.0f},
 {0.985f, -0.174f, 0.0f, 1.0f},
 {0.940f, -0.342f, 0.0f, 1.0f},
 {0.866f, -0.500f, 0.0f, 1.0f},
 {0.766f, -0.643f, 0.0f, 1.0f},
 {0.643f, -0.766f, 0.0f, 1.0f},
 {0.500f, -0.866f, 0.0f, 1.0f},
 {0.342f, -0.940f, 0.0f, 1.0f},
 {0.174f, -0.985f, 0.0f, 1.0f},
 {0.000f, -1.000f, 0.0f, 1.0f},
 {-0.174f, -0.985f, 0.0f, 1.0f},
 {-0.342f, -0.940f, 0.0f, 1.0f},
 {-0.500f, -0.866f, 0.0f, 1.0f},
 {-0.643f, -0.766f, 0.0f, 1.0f},
 {-0.766f, -0.643f, 0.0f, 1.0f},
 {-0.866f, -0.500f, 0.0f, 1.0f},
 {-0.940f, -0.342f, 0.0f, 1.0f},
 {-0.985f, -0.174f, 0.0f, 1.0f},
 {-1.000f, -0.000f, 0.0f, 1.0f},
 {-0.985f, 0.174f, 0.0f, 1.0f},
 {-0.940f, 0.342f, 0.0f, 1.0f},
 {-0.866f, 0.500f, 0.0f, 1.0f},
 {-0.766f, 0.643f, 0.0f, 1.0f},
 {-0.643f, 0.766f, 0.0f, 1.0f},
 {-0.500f, 0.866f, 0.0f, 1.0f},
 {-0.342f, 0.940f, 0.0f, 1.0f},
 {-0.174f, 0.985f, 0.0f, 1.0f},
 }
 0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 5, 4, 0, 6, 5, 0, 7, 6, 0, 8, 7, 0, 9, 8, 0, 10, 9, 0, 11, 10, 0, 12, 11, 0, 13, 12, 0, 14, 13, 0, 15, 14, 0, 16, 15, 0, 17, 16, 0, 18, 17, 0, 19, 18, 0, 20, 19, 0, 21, 20, 0, 22, 21, 0, 23, 22, 0, 24, 23, 0, 25, 24, 0, 26, 25, 0, 27, 26, 0, 28, 27, 0, 29, 28, 0, 30, 29, 0, 31, 30, 0, 32, 31, 0, 33, 32, 0, 34, 33, 0, 35, 34, 0, 36, 35, 0, 1, 36, 0,
 */
