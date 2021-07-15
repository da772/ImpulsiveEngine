#include "gepch.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"
#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Shape.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Util/Time.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Util/GEMath.h"

namespace GEngine {

	int Batch::s_BatchCount = 0;

	Batch::Batch(Ref<Shape> shape, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, int maxShapes, int maxTextures, const std::function<void()>& shaderFunction) :
		m_IndexBuffer(indexBuffer), m_Shader(shader), m_shaderFunction(shaderFunction) {
#ifndef GE_GRAPHICS_API_NONE
        m_VertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(shape->GetVerticesSize()*maxShapes *sizeof(float)));
        m_VertexBuffer->SetLayout(shape->GetBufferLayout());

        m_VertexArray = Ref<VertexArray>(VertexArray::Create());

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        m_Shader->Bind();
		std::vector<int> samplers;
		for (int i = 0; i < maxTextures; i++) {
			samplers.push_back(i);
		}
		m_Shader->UploadUniformIntArray("u_Textures", samplers.data(), maxTextures);
		s_BatchCount++;
		#endif
     }

     Batch::~Batch() {
        m_VertexBuffer = nullptr;
        m_IndexBuffer = nullptr;
        m_VertexArray = nullptr;
        m_Shader = nullptr;
		m_Vertices.clear();
		m_TextureIds.clear();
		s_BatchCount--;
     }

	 void Batch::SetVertices(std::vector<float>& vertices, std::vector<int>& textures, int indexCount)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
         m_Vertices = std::vector<float>(std::move(vertices));
		 m_TextureIds = std::vector<int>(std::move(textures));
		 m_IndexCount = indexCount;

		 m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
		 #endif
	 }

	 void Batch::RefreshVertices()
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
		 #endif
	 }


	 void Batch::Render()
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 if (m_IndexCount > 0) {
			 m_Shader->Bind();
			 if (m_camera) m_Shader->UploadUniformMat4("u_ViewProjection", m_camera->GetViewProjectionMatrix());
			 if (m_shaderFunction) m_shaderFunction();
			 for (int i = 0; i < m_TextureIds.size(); i++) {
				 if (m_TextureIds[i] != -1)
					 RenderCommand::BindTexture(m_TextureIds[i], i);
			 }

			 m_VertexArray->Bind();
			 RenderCommand::DrawIndexed(m_VertexArray, m_IndexCount);
		 }
		#endif
	 }

	 BatchRenderer::BatchRenderer(ERenderType pipeline, Ref<Shape> shape, int maxShapes, Ref<Shader> shader, const char* pipelineId, const std::function<void()>& shaderFunc) : m_MaxShapes(maxShapes), m_Shader(shader), m_Shape(shape),
		 m_PipelineId(pipelineId == nullptr ? pipeline == ERenderType::GAME ? "2d" : "ui" : pipelineId), m_renderType(pipeline), m_shaderfunction(shaderFunc)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		m_MaxVertices = m_MaxShapes * m_Shape->GetVerticesSize();
		m_MaxIndices = m_MaxShapes * m_Shape->GetIndicesSize();

		u32 offset = 0;
		m_Indices = std::vector<u32>(m_MaxIndices);
		for (u32 i = 0; i < m_MaxIndices; i += m_Shape->GetIndicesSize()) {
			std::vector<u32> ind = m_Shape->GetIndices(offset);
			for (u32 j = 0; j < m_Shape->GetIndicesSize(); j++) {
				m_Indices[(uint64_t)i + (uint64_t)j] = ind[j];
			};
			offset += m_Shape->GetVerticesRows();
		}
		
		#endif
	 }

	 BatchRenderer::~BatchRenderer()
	 {
		 m_SortedObjects.clear();
		 m_Shape = nullptr;
		 m_Shader = nullptr;
		 m_IndexBuffer = nullptr;
		 m_BlankTexture = nullptr;
		 for (Ref<Batch>& b : m_Batches) {
			 m_Pipeline->Remove(b);
			 b = nullptr;
		 }
		 m_Batches.clear();
		 m_Pipeline = nullptr;
	 }

	 void BatchRenderer::Setup()
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 if (m_MaxTextures == 0) m_MaxTextures = RenderCommand::GetMaxTextureSlots();
		 if (m_BlankTexture == nullptr) {
			 m_BlankTexture = Texture2D::Create("batchBlank", 1, 1);
			 uint32_t whiteTextureData = 0xffffffff;
			 m_BlankTexture->SetData(&whiteTextureData, sizeof(u32));
		 }
		 if (m_Shader == nullptr)
			 m_Shader = Ref<Shader>(Shader::Create((std::string("EngineContent/shaders//TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl")));
		 if (m_IndexBuffer == nullptr) {
			 m_IndexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(m_Indices.data(), m_MaxIndices));
		 }

		 if (m_Pipeline == nullptr) {
			 m_Pipeline = Renderer::GetPipeline(m_PipelineId.c_str());
		 }
		 #endif
	 }



	 const uint64_t BatchRenderer::AddShape(const Vector3f& _position, float rotation,const Vector2f& scale,const Vector4f& color, Ref<Texture2D> texture, const Vector2f& textureScale, float alphaChannel /*= 4*/)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 Vector3f position = _position;
		 Setup();

		 if (m_renderType == ERenderType::UI) {
			 position.y = GEMath::MapRange(position.y, -1.f, 1.f, Application::GetSafeBottomUI()-1.f, 1.f - Application::GetSafeTopUI());
			 position.x = GEMath::MapRange(position.x, -1.f, 1.f, Application::GetSafeLeftUI()-1.f, 1.f - Application::GetSafeRightUI());
			// GE_CORE_DEBUG("UI: {0},{1},{2},{3}", Application::GetSafeTopUI(), Application::GetSafeBottom(), Application::GetSafeLeftUI(), Application::GetSafeRight());
		 }

		 BatchObjectData data = { position, rotation, scale, color, texture == nullptr ? m_BlankTexture : texture, textureScale, alphaChannel,
             m_Shape->GetVertices(m_renderType == ERenderType::UI ? Vector3f(position.x, position.y, 0) : position, rotation, { scale.x, scale.y, 1 }, color, 0, textureScale, nullptr, alphaChannel) };
		

		 return AddShape(data);
		 #else
		 return 0;
		 #endif
	 }

	 const uint64_t BatchRenderer::AddShape(const Vector3f& _position, float rotation,const Vector2f& scale, const Vector4f& color, Ref<SubTexture2D> texture, const Vector2f& textureScale, float alphaChannel /*= 4*/)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 Vector3f position = _position;
		 if (m_renderType == ERenderType::UI) {
			 position.y = GEMath::MapRange(position.y, -1.f, 1.f, Application::GetSafeBottomUI() - 1.f, 1.f - Application::GetSafeTopUI());
			 position.x = GEMath::MapRange(position.x, -1.f, 1.f, Application::GetSafeLeftUI() - 1.f, 1.f - Application::GetSafeRightUI());
			// GE_CORE_DEBUG("UI: {0},{1},{2},{3}", Application::GetSafeTopUI(), Application::GetSafeBottom(), Application::GetSafeLeftUI(), Application::GetSafeRight());
		 }
		 Setup();

		 BatchObjectData data = { position, rotation, scale, color, texture->GetTexture(), textureScale, alphaChannel,
			m_Shape->GetVertices(m_renderType == ERenderType::UI ? Vector3f(position.x, position.y, 0) : position, rotation, { scale.x, scale.y, 1 }, color, 0 , textureScale,  texture->GetTexCoords(), alphaChannel), texture };
		 
		 return AddShape(data);
		 #else
		 return 0;
		 #endif

	 }

	 const uint64_t BatchRenderer::AddShape(BatchObjectData& data)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 data.time = Time::GetEpochTimeNS();
		 uint64_t id = data.time;

		 m_SortedObjects.push_back({ id, data });


		 if (m_Sort) {
			 std::sort(m_SortedObjects.begin(), m_SortedObjects.end(), [](const std::pair<u64, BatchObjectData>& l, const std::pair<u64, BatchObjectData>& r) {
				 return l.second.position == r.second.position ? l.second.time < r.second.time : l.second.position.z < r.second.position.z;
				 });
		 }

		 ReCreateBatches();

		 return id;
		 #else
		 return 0;
		 #endif
	 }

	 void BatchRenderer::EditShape(const uint64_t id, Vector3f postiion, float rotation, Vector2f scale, Vector4f color, Ref<Texture2D> texture, const Vector2f& textureScale, float alphaChannel /*= 4*/)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<int>& textures = batch->GetTextures();
		 std::vector<int>::iterator tIt = std::find(textures.begin(), textures.end(), texture != nullptr ? texture->GetRendererID() : m_BlankTexture->GetRendererID());

		 if (it->second.position.z == postiion.z && tIt != textures.end()) {

			 std::vector<float>& vertices = batch->GetVertices();

			 it->second.alphaChannel = alphaChannel;
			 it->second.position = postiion;
			 it->second.color = color;
			 it->second.scale = scale;
			 it->second.texture = texture != nullptr ? texture : m_BlankTexture;
			 it->second.textureId = tIt - textures.begin();

			 it->second.vertices = m_Shape->GetVertices(m_renderType == ERenderType::UI ? Vector3f(it->second.position.x, it->second.position.y, 0) : it->second.position, it->second.rotation, { it->second.scale.x, it->second.scale.y, 1 }, it->second.color,
				 it->second.textureId, textureScale, nullptr, alphaChannel);

			 memcpy(&vertices[(uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos], &it->second.vertices[0], it->second.vertices.size() * sizeof(float));

			 batch->RefreshVertices();
		 }
		 #endif
	 }

	 void BatchRenderer::EditShape(const uint64_t id, Vector3f postiion, float rotation, Vector2f scale, Vector4f color, Ref<SubTexture2D> texture,
		 const Vector2f& textureScale, float alphaChannel /*= 4*/)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<int>& textures = batch->GetTextures();
		 std::vector<int>::iterator tIt = std::find(textures.begin(), textures.end(), texture != nullptr ? texture->GetTexture()->GetRendererID() : m_BlankTexture->GetRendererID());


		 if (it->second.position.z == postiion.z && tIt != textures.end()) {

			 std::vector<float>& vertices = batch->GetVertices();

			 it->second.alphaChannel = alphaChannel;
			 it->second.position = postiion;
			 it->second.color = color;
			 it->second.scale = scale;
			 it->second.texture = texture != nullptr ? texture->GetTexture() : m_BlankTexture;
			 it->second.subTexture = texture;

			 ReCreateShapeVertices(&it->second);

			 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));

			 batch->RefreshVertices();
		 }
		 #endif
	 }


	 void BatchRenderer::SetColor(const uint64_t id, Vector4f color)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.color = color;
		 ReCreateShapeVertices(&it->second);
		
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif

	 }

	 void BatchRenderer::ReCreateShapeVertices(BatchObjectData* data)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 data->vertices = m_Shape->GetVertices(m_renderType == ERenderType::UI ? Vector3f(data->position.x, data->position.y, 0) : data->position, data->rotation, { data->scale.x, data->scale.y, 1 }, data->color,
			 data->textureId, data->textureScale, data->subTexture == nullptr ? nullptr : data->subTexture->GetTexCoords(), data->alphaChannel);
		#endif
	 }

	 void BatchRenderer::SetPosition(const uint64_t id, Vector2f position)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.position = { position.x, position.y, it->second.position.z };

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif
	 }

	 void BatchRenderer::SetTextureScale(const uint64_t id, const Vector2f& scale)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.textureScale = scale;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif
	 }

	 void BatchRenderer::SetSafeParams(const uint64_t& id, const Vector2f& pos, const float& rot, const Vector2f& scale, const Vector4f& color)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.scale = scale;
		 it->second.position = { pos, it->second.position.z };
		 it->second.color = color;
		 it->second.rotation = rot;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif
	 }

	 void BatchRenderer::SetZOrder(const uint64_t id, float zOrder)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });
		 float _zOrder = it->second.position.z;
		 it->second.position = { it->second.position.x,  it->second.position.y, zOrder };

		 if (m_Sort) {
			 std::sort(m_SortedObjects.begin(), m_SortedObjects.end(), [](const std::pair<u64, BatchObjectData>& l, const std::pair<u64, BatchObjectData>& r) {
				 return l.second.position == r.second.position ? l.second.time < r.second.time : l.second.position.z < r.second.position.z;
				 });
		 }

		 ReCreateBatches();
		#endif
	 }

	 void BatchRenderer::SetSubTexture(const uint64_t id, Ref<SubTexture2D> texture)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<int>& textures = batch->GetTextures();
		 std::vector<int>::iterator tIt = std::find(textures.begin(), textures.end(), texture != nullptr ? texture->GetTexture()->GetRendererID() : m_BlankTexture->GetRendererID());


		 if (tIt != textures.end()) {

			 std::vector<float>& vertices = batch->GetVertices();

			 it->second.texture = texture != nullptr ? texture->GetTexture() : m_BlankTexture;
			 it->second.subTexture = texture;

			 ReCreateShapeVertices(&it->second);

			 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));

			 batch->RefreshVertices();

			 //ReCreateBatches();
		 }
		 else {
			 it->second.texture = texture != nullptr ? texture->GetTexture() : m_BlankTexture;
			 it->second.subTexture = texture;
			 ReCreateBatches();
		 }
		#endif
	 }


	 void BatchRenderer::SetTexture(const uint64_t id, Ref<Texture2D> texture)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<int>& textures = batch->GetTextures();
		 std::vector<int>::iterator tIt = std::find(textures.begin(), textures.end(), texture != nullptr ? texture->GetRendererID() : m_BlankTexture->GetRendererID());


		 if (tIt != textures.end()) {

			 std::vector<float>& vertices = batch->GetVertices();

			 it->second.texture = texture;
			 it->second.subTexture = nullptr;

			 ReCreateShapeVertices(&it->second);

			 std::copy(it->second.vertices.begin(), it->second.vertices.end(),(vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos)));

			 batch->RefreshVertices();
			 ReCreateBatches();
		 }
		 else {
			 it->second.texture = texture;
			 it->second.subTexture = nullptr;
			 ReCreateBatches();
		 }
		#endif
	 }



	 void BatchRenderer::SetRotation(const uint64_t id, float rotation)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.rotation= rotation;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif
	 }

	 void BatchRenderer::SetScale(const uint64_t id, Vector2f scale)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.scale = scale;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + ((uint64_t)m_Shape->GetVerticesSize() * (uint64_t)it->second.batchPos));
		 batch->RefreshVertices();
		#endif
	 }




	 const GEngine::Vector3f BatchRenderer::GetShapePosition(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.position;
		#else
		return Vector3f(0.f);
		#endif
	 }

	 const GEngine::Ref<GEngine::Texture2D> BatchRenderer::GetShapeTexture(const uint64_t id)
	 {
		 #ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.texture;
		 #else
		 return nullptr;
		 #endif
	 }

	 const GEngine::Ref<GEngine::SubTexture2D> BatchRenderer::GetShapeSubTexture(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.subTexture;
		#else
		return nullptr;
		#endif
	 }

	 const float BatchRenderer::GetShapeRotation(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.rotation;
		#else
		return 0.f;
		#endif
	 }

	 const GEngine::Vector2f BatchRenderer::GetShapeScale(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.scale;
		#else
			return Vector2f(0.f);
		#endif
	 }

	 const GEngine::Vector2f BatchRenderer::GetTextureScale(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.textureScale;
		#else
			return Vector2f(0.f);
		#endif
	 }

	 const GEngine::Vector4f BatchRenderer::GetShapeColor(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });

		return it->second.color;
		#else
		return Vector4f(0.f);
		#endif
	 }

	 void BatchRenderer::RemoveShape(const uint64_t id)
	 {
		#ifndef GE_GRAPHICS_API_NONE
		 std::vector<std::pair<uint64_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u64, BatchObjectData>& e) {
			 return e.first == id;
			 });
		 m_SortedObjects.erase(it);
		 ReCreateBatches();
		 #endif
	 }


	 int BatchRenderer::UpdateCount(int i)
	 {
		 m_RefCount += i;
		 return m_RefCount;
	 }

	 void BatchRenderer::SetSort(bool b)
	 {
		 m_Sort = b;
	 }

	 void BatchRenderer::ReloadGraphics() {
		#ifndef GE_GRAPHICS_API_NONE
		if (bUnloaded) {
            bUnloaded = false;
			Setup();
			ReCreateBatches();
		}
		#endif
	 } 

	 void BatchRenderer::UnloadGraphics() {
		#ifndef GE_GRAPHICS_API_NONE
		 if (!bUnloaded) {
			 for (int i = 0; i < m_Batches.size(); i++) {
				 m_Pipeline->Remove(m_Batches[i]);
			 }
			 m_Batches.clear();
			 m_IndexBuffer = nullptr;
			 m_BlankTexture = nullptr;
			 bUnloaded = true;
		 }
		#endif
	 }

	 void BatchRenderer::ReCreateBatches()
	 {
		#ifndef GE_GRAPHICS_API_NONE
         if (bUnloaded) return;
         
		 if (m_SortedObjects.size() <= 0 && m_Batches.size() == 1) {
			 std::vector<float> f;
			 std::vector<int> i;
			 m_Batches[0]->SetVertices(f, i, 0);
			 return;
		 }

		 if (m_Batches.size() <= 0) {
			 Ref<Batch> batch = Ref<Batch>(new Batch(m_Shape, m_IndexBuffer, m_Shader, m_MaxShapes, m_MaxTextures, m_shaderfunction));
			 m_Batches.push_back(batch);
			 m_Pipeline->Add(batch);
		 }

		 std::vector<float> vertices;
		 std::vector<int> textures;
		 textures.push_back(m_BlankTexture->GetRendererID());
		 int objectCount = 0;
		 int batchCount = 0;
		 Ref<Batch> batch = m_Batches[batchCount];
		 for (int i = 0; i < m_SortedObjects.size(); i++) {

			 std::vector<int>::iterator it = std::find(textures.begin(), textures.end(), !m_SortedObjects[i].second.texture ? m_BlankTexture->GetRendererID()  : m_SortedObjects[i].second.texture->GetRendererID());
			 if  (objectCount +1 < m_MaxShapes && (textures.size() < m_MaxTextures || it != textures.end())) {
				 std::pair<uint64_t, BatchObjectData>& data = m_SortedObjects[i];
				 if (objectCount == 0) {
					 batch->SetPriority(floor(data.second.position.z));
					 batch->SetTime(Time::GetEpochTimeNS());
					 m_Pipeline->Sort();
				 }
				data.second.batchId = batchCount;
				data.second.batchPos = objectCount;
				objectCount++;
				int texId = it == textures.end() ? textures.size() : it - textures.begin();
				data.second.textureId = texId;
				m_Shape->SetTextureSlot(data.second.vertices, texId);
				if (it == textures.end()) {
					textures.push_back(data.second.texture->GetRendererID());
				}
				vertices.insert(vertices.end(), data.second.vertices.begin(), data.second.vertices.end());
			 }
			 else {
				 batch->SetVertices(vertices, textures, objectCount * m_Shape->GetIndicesSize());
				 i--;
				 if (batchCount >= m_Batches.size()-1) {
					 Ref<Batch> batch = Ref<Batch>(new Batch(m_Shape, m_IndexBuffer, m_Shader, m_MaxShapes, m_MaxTextures, m_shaderfunction));
					 m_Batches.push_back(batch);
					 m_Pipeline->Add(batch);
				 }
				 batch = m_Batches[++batchCount];
				 vertices.clear();
				 textures.clear();
				 textures.push_back(m_BlankTexture->GetRendererID());
				 objectCount = 0;
				 continue;
			 }
		 }

		 if (vertices.size() > 0) {
			 batch->SetVertices(vertices, textures, objectCount * m_Shape->GetIndicesSize());
		 }

		 if (batchCount < m_Batches.size() - 1) {
			 for (int i = batchCount + 1; i < m_Batches.size(); i++) {
				 m_Pipeline->Remove(m_Batches[i]);
			 }
			 m_Batches.erase(m_Batches.begin() + batchCount + 1, m_Batches.end());
		 }
		 #endif
	 }

}
	 	