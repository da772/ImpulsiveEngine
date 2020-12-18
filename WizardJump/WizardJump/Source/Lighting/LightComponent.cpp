#include "Lighting/LightComponent.h"


#include <glm/gtc/matrix_transform.hpp>

Ref < BatchRenderer >LightComponent::s_CircleShapeFactory = nullptr;
Ref < BatchRenderer >LightComponent::s_QuadShapeFactory = nullptr;


LightComponent::LightComponent() {
    bUpdates = false;
    if (s_CircleShapeFactory == nullptr) {
        std::string path = std::string("Content/shaders/CircleLight_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
        m_Shader = Ref<Shader>(Shader::Create(path));
        LightComponent::s_CircleShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Circle>(new Circle()),
            5000, m_Shader, "lighting"));

		LightComponent::s_QuadShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Quad>(new Quad()),
			5000, Shader::Create("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots()) + "Batch.glsl") , "lighting"));
    }
}


LightComponent::~LightComponent() {
    
}


PolygonLightRendererable::PolygonLightRendererable(const Vector3f& position, const std::vector<float>& _vertices, const std::vector<uint32_t>& _indices, Ref<BufferLayout> layout, const Vector4f& color)
{
    vertices = _vertices;
    indices = _indices;
    m_bufferLayout = layout;
    m_color = color;
    m_position = position;
    
    m_Priority = 10;

    m_Shader = Shader::Create("Content/shaders/PolygonLight.glsl");

    CreateGraphics();
}

PolygonLightRendererable::~PolygonLightRendererable()
{
    m_vertexArray = nullptr;
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
}

void PolygonLightRendererable::Render()
{
    //RenderCommand::BlendEquationSeparate(0x8006, 0x8007); // GL_FUNC_ADD, GL_MIN
    m_Shader->Bind();
    m_Shader->UploadUniformMat4("u_ViewProjection", SceneManager::GetCurrentViewProjectionMatrix());
    glm::mat4 pos = glm::translate(glm::mat4(1.f), glm::vec3(m_position.x, m_position.y, m_position.z));
    m_Shader->UploadUniformMat4("u_Transform", pos);
    m_Shader->UploadUniformFloat4("u_Color", m_color);
    m_vertexArray->Bind();
    RenderCommand::DrawIndexed(m_vertexArray);
    //RenderCommand::BlendEquationSeparate(0x8006, 0x8006); // GL_FUNC_ADD, GL_MIN
}


void PolygonLightRendererable::Unload()
{
	m_vertexArray = nullptr;
	m_indexBuffer = nullptr;
	m_vertexBuffer = nullptr;
}

void PolygonLightRendererable::Reload()
{
    CreateGraphics();
}

void PolygonLightRendererable::CreateGraphics()
{
	m_vertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create((float*)vertices.data(), (uint32_t)vertices.size() * sizeof(float)));
	m_indexBuffer = Ref<IndexBuffer>(IndexBuffer::Create((uint32_t*)indices.data(), (uint32_t)indices.size()));
	m_vertexBuffer->SetLayout(m_bufferLayout);

	m_vertexArray = Ref<VertexArray>(VertexArray::Create());

	m_vertexArray->AddVertexBuffer(m_vertexBuffer);
	m_vertexArray->SetIndexBuffer(m_indexBuffer);

}

void LightComponent::OnBegin()
{
    
}

const ShapeID LightComponent::AddCircleLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color) {
    Ref<Texture2D> t = nullptr;
    const ShapeID id = LightComponent::s_CircleShapeFactory->AddShape({ GetEntityPosition().x + position.x, GetEntityPosition().y+position.y, intensity}, 0, scale, color, t, scale, 4.f);
    m_Circleids.push_back(id);
    return id;
}

const ShapeID LightComponent::AddQuadLight(const Vector2f& position, float intensity, const Vector2f& scale, const Vector4f& color, Ref<Texture2D> texture)
{
    const ShapeID id = s_QuadShapeFactory->AddShape({ GetEntityPosition().x + position.x, GetEntityPosition().y + position.y, intensity
        }, 0, scale, color, texture);
    m_Quadids.push_back(id);
    return id;
}

void LightComponent::EditCircleColor(const ShapeID id, const Vector4f& color)
{
    s_CircleShapeFactory->SetColor(id, color);
}

void LightComponent::EditCircleSize(const ShapeID id, const Vector2f& size)
{
    s_CircleShapeFactory->SetScale(id, size);
}

const ShapeID LightComponent::AddPolygonLight(const Vector3f& position, const std::vector<float>& vertices, const std::vector<uint32_t>& indices, Ref<BufferLayout> layout, const Vector4f& color)
{
    Ref<PolygonLightRendererable> l = make_shared<PolygonLightRendererable>(GetEntityPosition()+position, vertices, indices, layout, color);
    m_polygonLights.push_back(l);

    Renderer::GetPipeline("lighting")->Add(l);
    const ShapeID id = ++m_polygonLightCounter;
    m_polygonLightMap[id] = l;
    return id;
}

void LightComponent::RemoveQuadLight(const ShapeID id)
{
    m_Quadids.erase(std::find(m_Quadids.begin(), m_Quadids.end(), id));
    s_QuadShapeFactory->RemoveShape(id);
}

void LightComponent::RemovePolygonLight(const ShapeID id)
{
    Ref<PolygonLightRendererable> l = m_polygonLightMap[id].lock();
    m_polygonLightMap.erase(id);

    if (l) {
        m_polygonLights.erase(std::find(m_polygonLights.begin(), m_polygonLights.end(), l));
    }

    if (m_polygonLights.size() == 0)
        m_polygonLightCounter = 0;
}

void LightComponent::RemoveCircleLight(const ShapeID id)
{
    m_Circleids.erase(std::find (m_Circleids.begin(), m_Circleids.end(), id));
    s_CircleShapeFactory->RemoveShape(id);
}

void LightComponent::OnEnd()
{
    
    for (const ShapeID id : m_Circleids)
        LightComponent::s_CircleShapeFactory->RemoveShape(id);

    for (const ShapeID id : m_Quadids)
        s_QuadShapeFactory->RemoveShape(id);

    Ref<RenderPipeline> pipeline = Renderer::GetPipeline("lighting");
    for (Ref<PolygonLightRendererable> p : m_polygonLights) {
        pipeline->Remove(p);
    }

    m_polygonLights.end();
    
    
}

void LightComponent::OnUpdate(Timestep timestep)
{
	
}

void LightComponent::OnAttached(Ref<Entity> entity)
{
	entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
		if (IsInitialized()) {
			for (const ShapeID id : m_Circleids) {
				Vector3f pos = s_CircleShapeFactory->GetShapePosition(id);
				Vector3f nPos = pos - transData.position + transform->GetPosition();
				if (pos != nPos)
                    s_CircleShapeFactory->SetPosition(id, nPos);
				float rot = s_CircleShapeFactory->GetShapeRotation(id);
				float nRot = rot - transData.rotation.z + transform->GetRotation().z;
				if (rot != nRot)
                    s_CircleShapeFactory->SetRotation(id, nRot);
				Vector2f _scale = s_CircleShapeFactory->GetShapeScale(id);
				Vector3f scale(_scale.x, _scale.y, 1);
				Vector3f nScale = scale - transData.scale.z + transform->GetScale().z;
				if (scale != nScale)
                    s_CircleShapeFactory->SetScale(id, { nScale.x, nScale.y });
			}
			for (const ShapeID id : m_Quadids) {
				Vector3f pos = s_QuadShapeFactory->GetShapePosition(id);
				Vector3f nPos = pos - transData.position + transform->GetPosition();
				if (pos != nPos)
                    s_QuadShapeFactory->SetPosition(id, nPos);
				float rot = s_QuadShapeFactory->GetShapeRotation(id);
				float nRot = rot - transData.rotation.z + transform->GetRotation().z;
				if (rot != nRot)
                    s_QuadShapeFactory->SetRotation(id, nRot);
				Vector2f _scale = s_QuadShapeFactory->GetShapeScale(id);
				Vector3f scale(_scale.x, _scale.y, 1);
				Vector3f nScale = scale - transData.scale.z + transform->GetScale().z;
				if (scale != nScale)
                    s_QuadShapeFactory->SetScale(id, { nScale.x, nScale.y });
			}
		}
		});
}

void LightComponent::DeAttached(Ref<Entity> entity)
{
	entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
}


void LightComponent::CreateGraphics() {
    
}

void LightComponent::ReloadGraphics() {
    if (s_CircleShapeFactory) {
        s_CircleShapeFactory->ReloadGraphics();
    }
    if (s_QuadShapeFactory) {
        s_QuadShapeFactory->ReloadGraphics();
    }
	for (const Ref<PolygonLightRendererable>& p : m_polygonLights) {
		p->Reload();
	}
}

void LightComponent::UnloadGraphics() {
    if (s_CircleShapeFactory) {
        s_CircleShapeFactory->UnloadGraphics();
    }

	if (s_QuadShapeFactory) {
        s_QuadShapeFactory->UnloadGraphics();
	}

    for (const Ref<PolygonLightRendererable>& p : m_polygonLights) {
        p->Unload();
    }
}

/*

static const float circleVertices360[] = { 0, 0, 1, 1, 0.540302, 0.841471, 1, 1, -0.416147, 0.909297, 1, 1, -0.653644, -0.756802, 1, 1, 0.753902, 0.656987, 1, 1, -0.839072, -0.544021, 1, 1, 0.907447, 0.420167, 1, 1, -0.957659, -0.287903, 1, 1, 0.988705, 0.149877, 1, 1, -0.999961, -0.00885131, 1, 1, 0.991203, -0.132352, 1, 1, -0.962606, 0.270906, 1, 1, 0.914742, -0.404038, 1, 1, -0.84857, 0.529083, 1, 1, 0.765414, -0.643538, 1, 1, -0.666938, 0.745113, 1, 1, 0.555113, -0.831775, 1, 1, -0.432178, 0.901788, 1, 1, 0.300593, -0.953753, 1, 1, -0.162991, 0.986628, 1, 1, 0.0221268, -0.999755, 1, 1, 0.11918, 0.992873, 1, 1, -0.258102, -0.966118, 1, 1, 0.391857, 0.920026, 1, 1, -0.51777, -0.85552, 1, 1, 0.633319, 0.773891, 1, 1, -0.736193, -0.676772, 1, 1, 0.824331, 0.566108, 1, 1, -0.895971, -0.444113, 1, 1, 0.949678, 0.313229, 1, 1, -0.984377, -0.176076, 1, 1, 0.999373, 0.0353983, 1, 1, -0.994367, 0.105988, 1, 1, 0.969459, -0.245252, 1, 1, -0.925148, 0.379608, 1, 1, 0.862319, -0.506366, 1, 1, -0.782231, 0.622989, 1, 1, 0.686487, -0.727143, 1, 1, -0.577002, 0.816743, 1, 1, 0.455969, -0.889996, 1, 1, -0.32581, 0.945435, 1, 1, 0.189129, -0.981952, 1, 1, -0.0486636, 0.998815, 1, 1, -0.0927762, -0.995687, 1, 1, 0.232359, 0.97263, 1, 1, -0.367291, -0.930106, 1, 1, 0.494872, 0.868966, 1, 1, -0.612548, -0.790433, 1, 1, 0.717964, 0.69608, 1, 1, -0.80901, -0.587795, 1, 1, 0.883863, 0.467745, 1, 1, -0.941026, -0.338333, 1, 1, 0.979355, 0.20215, 1, 1, -0.998081, -0.0619203, 1, 1, 0.996831, -0.0795485, 1, 1, -0.975629, 0.219425, 1, 1, 0.9349, -0.35491, 1, 1, -0.875459, 0.483292, 1, 1, 0.798496, -0.602, 1, 1, -0.705551, 0.708659, 1, 1, 0.598484, -0.801135, 1, 1, -0.479439, 0.877575, 1, 1, 0.350797, -0.936451, 1, 1, -0.215135, 0.976584, 1, 1, 0.0751662, -0.997171, 1, 1, 0.0663069, 0.997799, 1, 1, -0.206453, -0.978457, 1, 1, 0.342466, 0.93953, 1, 1, -0.471626, -0.881799, 1, 1, 0.591345, 0.806418, 1, 1, -0.699229, -0.714898, 1, 1, 0.793118, 0.609068, 1, 1, -0.871133, -0.491048, 1, 1, 0.931711, 0.363199, 1, 1, -0.973642, -0.228082, 1, 1, 0.996085, 0.0883987, 1, 1, -0.998592, 0.0530535, 1, 1, 0.981111, -0.193444, 1, 1, -0.943994, 0.329962, 1, 1, 0.887983, -0.459877, 1, 1, -0.814198, 0.580587, 1, 1, 0.724118, -0.689676, 1, 1, -0.619544, 0.784962, 1, 1, 0.50257, -0.864536, 1, 1, -0.375538, 0.926807, 1, 1, 0.240988, -0.970528, 1, 1, -0.101616, 0.994824, 1, 1, -0.0397908, -0.999208, 1, 1, 0.180401, 0.983593, 1, 1, -0.3174, -0.948292, 1, 1, 0.448047, 0.89401, 1, 1, -0.569726, -0.821835, 1, 1, 0.680001, 0.733211, 1, 1, -0.776667, -0.629911, 1, 1, 0.857788, 0.514004, 1, 1, -0.92174, -0.387809, 1, 1, 0.967243, 0.253853, 1, 1, -0.993387, -0.114815, 1, 1, 0.999648, -0.026521, 1, 1, -0.985902, 0.167326, 1, 1, 0.952422, -0.304782, 1, 1, -0.89988, 0.436138, 1, 1, 0.829327, -0.558764, 1, 1, -0.742174, 0.670207, 1, 1, 0.640167, -0.768235, 1, 1, -0.525348, 0.850888, 1, 1, 0.400013, -0.91651, 1, 1, -0.266672, 0.963787, 1, 1, 0.127994, -0.991775, 1, 1, 0.0132466, 0.999912, 1, 1, -0.154222, -0.988036, 1, 1, 0.29211, 0.956385, 1, 1, -0.424152, -0.905591, 1, 1, 0.547704, 0.836672, 1, 1, -0.660294, -0.751007, 1, 1, 0.759668, 0.650311, 1, 1, -0.843838, -0.536598, 1, 1, 0.911118, 0.412146, 1, 1, -0.960162, -0.279444, 1, 1, 0.989988, 0.14115, 1, 1, -1, -3.01444e-05, 1, 1, 0.989997, -0.14109, 1, 1 };

static const int circleIndices360[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 13, 0, 13, 14, 0, 14, 15, 0, 15, 16, 0, 16, 17, 0, 17, 18, 0, 18, 19, 0, 19, 20, 0, 20, 21, 0, 21, 22, 0, 22, 23, 0, 23, 24, 0, 24, 25, 0, 25, 26, 0, 26, 27, 0, 27, 28, 0, 28, 29, 0, 29, 30, 0, 30, 31, 0, 31, 32, 0, 32, 33, 0, 33, 34, 0, 34, 35, 0, 35, 36, 0, 36, 37, 0, 37, 38, 0, 38, 39, 0, 39, 40, 0, 40, 41, 0, 41, 42, 0, 42, 43, 0, 43, 44, 0, 44, 45, 0, 45, 46, 0, 46, 47, 0, 47, 48, 0, 48, 49, 0, 49, 50, 0, 50, 51, 0, 51, 52, 0, 52, 53, 0, 53, 54, 0, 54, 55, 0, 55, 56, 0, 56, 57, 0, 57, 58, 0, 58, 59, 0, 59, 60, 0, 60, 61, 0, 61, 62, 0, 62, 63, 0, 63, 64, 0, 64, 65, 0, 65, 66, 0, 66, 67, 0, 67, 68, 0, 68, 69, 0, 69, 70, 0, 70, 71, 0, 71, 72, 0, 72, 73, 0, 73, 74, 0, 74, 75, 0, 75, 76, 0, 76, 77, 0, 77, 78, 0, 78, 79, 0, 79, 80, 0, 80, 81, 0, 81, 82, 0, 82, 83, 0, 83, 84, 0, 84, 85, 0, 85, 86, 0, 86, 87, 0, 87, 88, 0, 88, 89, 0, 89, 90, 0, 90, 91, 0, 91, 92, 0, 92, 93, 0, 93, 94, 0, 94, 95, 0, 95, 96, 0, 96, 97, 0, 97, 98, 0, 98, 99, 0, 99, 100, 0, 100, 101, 0, 101, 102, 0, 102, 103, 0, 103, 104, 0, 104, 105, 0, 105, 106, 0, 106, 107, 0, 107, 108, 0, 108, 109, 0, 109, 110, 0, 110, 111, 0, 111, 112, 0, 112, 113, 0, 113, 114, 0, 114, 115, 0, 115, 116, 0, 116, 117, 0, 117, 118, 0, 118, 119, 0, 119, 120, 0, 120, 121, 0, 121, 1, 0};



static const float circleVertices5[] = { 0, 0, 1, 1, 0.283662, -0.958924, 1, 1, -0.839072, -0.544021, 1, 1, -0.1455, 0.989358, 1, 1, -0.532833, -0.84622, 1, 1, 0.955074, 0.296369, 1, 1, -0.918283, 0.395925, 1, 1, 0.440143, -0.897928, 1, 1, 0.24954, 0.968364, 1, 1, -0.819288, -0.573382, 1, 1, 0.995267, -0.0971819, 1, 1, -0.692896, 0.721038, 1, 1, 0.0575025, -0.998345, 1, 1, 0.605528, 0.795824, 1, 1, -0.977527, -0.210811, 1, 1, 0.879703, -0.475524, 1, 1, -0.359072, 0.93331, 1, 1, -0.334137, -0.942525, 1, 1, 0.866752, 0.498739, 1, 1, -0.982785, 0.184752, 1, 1, 0.626468, -0.779447, 1, 1, 0.0309449, 0.999521, 1, 1, -0.673485, -0.739201, 1, 1, 0.992332, 0.123603, 1, 1, -0.83424, 0.551402, 1, 1, 0.275192, -0.961389, 1, 1, 0.416119, 0.90931, 1, 1 };

static const int circleIndices5[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 13, 0, 13, 14, 0, 14, 15, 0, 15, 16, 0, 16, 17, 0, 17, 18, 0, 18, 19, 0, 19, 20, 0, 20, 21, 0, 21, 22, 0, 22, 23, 0, 23, 24, 0, 24, 25, 0, 25, 26, 0, 26, 1, 0 };


static const float circleVertices20[] = { 0, 0, 1, 1, 0.408082, 0.912945, 1, 1, -0.666938, 0.745113, 1, 1, -0.532833, -0.84622, 1, 1, 0.24954, 0.968364, 1, 1, 0.0575025, -0.998345, 1, 1, -0.359072, 0.93331, 1, 1, 0.626468, -0.779447, 1, 1, -0.83424, 0.551402, 1, 1 };

static const int circleIndices20[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 1, 0 };
*/

