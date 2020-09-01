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
     Batch::Batch(Ref<Shape> shape, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, int maxShapes, int maxTextures) : 
        m_IndexBuffer(indexBuffer), m_Shader(shader) {

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
     }

     Batch::~Batch() {
        m_VertexBuffer = nullptr;
        m_IndexBuffer = nullptr;
        m_VertexArray = nullptr;
        m_Shader = nullptr;
		m_Vertices.clear();
		m_TextureIds.clear();
     }

	 void Batch::SetVertices(std::vector<float>& vertices, std::vector<int>& textures, int indexCount)
	 {
         m_Vertices = std::vector<float>(std::move(vertices));
		 m_TextureIds = std::vector<int>(std::move(textures));
		 m_IndexCount = indexCount;

		 m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
	 }

	 void Batch::RefreshVertices()
	 {
		 m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
	 }


	 void Batch::Render()
	 {
		 if (m_IndexCount > 0) {

			 m_Shader->Bind();
			 m_Shader->UploadUniformMat4("u_ViewProjection", SceneManager::GetCurrentViewProjectionMatrix());

			 for (int i = 0; i < m_TextureIds.size(); i++) {
				 if (m_TextureIds[i] != -1)
					 RenderCommand::BindTexture(m_TextureIds[i], i);
			 }

			 m_VertexArray->Bind();
			 RenderCommand::DrawIndexed(m_VertexArray, m_IndexCount);
		 }

	 }

	 BatchRenderer::BatchRenderer(ERenderType pipeline, Ref<Shape> shape, int maxShapes, Ref<Shader> shader) : m_MaxShapes(maxShapes), m_Shader(shader), m_Shape(shape),
		 m_PipelineId(pipeline == ERenderType::GAME ? "2d" : "ui"), m_renderType(pipeline)
	 {
		m_MaxVertices = m_MaxShapes * m_Shape->GetVerticesSize();
		m_MaxIndices = m_MaxShapes * m_Shape->GetIndicesSize();

		u32 offset = 0;
		m_Indices = std::vector<u32>(m_MaxIndices);
		for (u32 i = 0; i < m_MaxIndices; i += m_Shape->GetIndicesSize()) {
			std::vector<u32> ind = m_Shape->GetIndices(offset);
			for (u32 j = 0; j < m_Shape->GetIndicesSize(); j++) {
				m_Indices[i + j] = ind[j];
			};
			offset += m_Shape->GetVerticesRows();
		}
		

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
		 if (m_MaxTextures == 0) m_MaxTextures = RenderCommand::GetMaxTextureSlots();
		 if (m_BlankTexture == nullptr) {
			 m_BlankTexture = Texture2D::Create("batchBlank", 1, 1);
			 uint32_t whiteTextureData = 0xffffffff;
			 m_BlankTexture->SetData(&whiteTextureData, sizeof(u32));
		 }
		 if (m_Shader == nullptr)
			 m_Shader = Ref<Shader>(Shader::Create((std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl")));
		 if (m_IndexBuffer == nullptr) {
			 m_IndexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(m_Indices.data(), m_MaxIndices));
		 }

		 if (m_Pipeline == nullptr) {
			 m_Pipeline = Renderer::GetPipeline(m_PipelineId);
		 }
	 }



	 long BatchRenderer::AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture, const glm::vec2& textureScale, float alphaChannel /*= 4*/)
	 {
		 Setup();

		 if (m_renderType == ERenderType::UI) {
			 position.y = GEMath::MapRange(position.y, -1.f, 1.f, Application::GetSafeBottomUI()-1.f, 1.f - Application::GetSafeTopUI());
			 position.x = GEMath::MapRange(position.x, -1.f, 1.f, Application::GetSafeLeftUI()-1.f, 1.f - Application::GetSafeRightUI());
			// GE_CORE_DEBUG("UI: {0},{1},{2},{3}", Application::GetSafeTopUI(), Application::GetSafeBottom(), Application::GetSafeLeftUI(), Application::GetSafeRight());
		 }

		 BatchObjectData data = { position, rotation, scale, color, texture == nullptr ? m_BlankTexture : texture, textureScale, alphaChannel,
			 m_Shape->GetVertices(position, rotation, { scale.x, scale.y, 1 }, color, 0, textureScale, nullptr, alphaChannel) };
		

		 return AddShape(data);
	 }

	 long BatchRenderer::AddShape(glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, const glm::vec2& textureScale, float alphaChannel /*= 4*/)
	 {
		 if (m_renderType == ERenderType::UI) {
			 position.y = GEMath::MapRange(position.y, -1.f, 1.f, Application::GetSafeBottomUI() - 1.f, 1.f - Application::GetSafeTopUI());
			 position.x = GEMath::MapRange(position.x, -1.f, 1.f, Application::GetSafeLeftUI() - 1.f, 1.f - Application::GetSafeRightUI());
			// GE_CORE_DEBUG("UI: {0},{1},{2},{3}", Application::GetSafeTopUI(), Application::GetSafeBottom(), Application::GetSafeLeftUI(), Application::GetSafeRight());
		 }
		 Setup();

		 BatchObjectData data = { position, rotation, scale, color, texture->GetTexture(), textureScale, alphaChannel,
			m_Shape->GetVertices(position, rotation, { scale.x, scale.y, 1 }, color, 0 , textureScale,  texture->GetTexCoords(), alphaChannel), texture };
		 
		 return AddShape(data);

	 }

	 long BatchRenderer::AddShape(BatchObjectData& data)
	 {
		 data.time = Time::GetEpochTimeNS();
		 long id = ++counter;

		 m_SortedObjects.push_back({ id, data });


		 if (m_Sort) {
			 std::sort(m_SortedObjects.begin(), m_SortedObjects.end(), [](const std::pair<u32, BatchObjectData>& l, const std::pair<u32, BatchObjectData>& r) {
				 return l.second.position == r.second.position ? l.second.time < r.second.time : l.second.position.z < r.second.position.z;
				 });
		 }

		 ReCreateBatches();

		 return id;
	 }

	 void BatchRenderer::EditShape(long id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<Texture2D> texture, const glm::vec2& textureScale, float alphaChannel /*= 4*/)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
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

			 it->second.vertices = m_Shape->GetVertices(it->second.position, it->second.rotation, { it->second.scale.x, it->second.scale.y, 1 }, it->second.color,
				 it->second.textureId, textureScale, nullptr, alphaChannel);

			 memcpy(&vertices[m_Shape->GetVerticesSize() * it->second.batchPos], &it->second.vertices[0], it->second.vertices.size() * sizeof(float));

			 batch->RefreshVertices();
		 }
	 }

	 void BatchRenderer::EditShape(long id, glm::vec3 postiion, float rotation, glm::vec2 scale, glm::vec4 color, Ref<SubTexture2D> texture, 
		 const glm::vec2& textureScale, float alphaChannel /*= 4*/)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
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

			 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));

			 batch->RefreshVertices();
		 }
	 }


	 void BatchRenderer::SetColor(long id, glm::vec4 color)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.color = color;
		 ReCreateShapeVertices(&it->second);
		
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));
		 batch->RefreshVertices();


	 }

	 void BatchRenderer::ReCreateShapeVertices(BatchObjectData* data)
	 {
		 data->vertices = m_Shape->GetVertices(data->position, data->rotation, { data->scale.x, data->scale.y, 1 }, data->color, 
			 data->textureId, data->textureScale, data->subTexture == nullptr ? nullptr : data->subTexture->GetTexCoords(), data->alphaChannel);
	 }

	 void BatchRenderer::SetPosition(long id, glm::vec2 position)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.position = { position.x, position.y, it->second.position.z };

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));
		 batch->RefreshVertices();
	 }

	 void BatchRenderer::SetZOrder(long id, float zOrder)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });
		 float _zOrder = it->second.position.z;
		 it->second.position = { it->second.position.x,  it->second.position.y, zOrder };

		 ReCreateBatches();
	 }

	 void BatchRenderer::SetSubTexture(long id, Ref<SubTexture2D> texture)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
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

			 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));

			 batch->RefreshVertices();
		 }
		 else {
			 it->second.texture = texture != nullptr ? texture->GetTexture() : m_BlankTexture;
			 it->second.subTexture = texture;
			 ReCreateBatches();
		 }
	 }

	 void BatchRenderer::SetRotation(long id, float rotation)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.rotation= rotation;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));
		 batch->RefreshVertices();
	 }

	 void BatchRenderer::SetScale(long id, glm::vec2 scale)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 Ref<Batch> batch = m_Batches[it->second.batchId];
		 std::vector<float>& vertices = batch->GetVertices();
		 it->second.scale = scale;

		 ReCreateShapeVertices(&it->second);
		 std::copy(it->second.vertices.begin(), it->second.vertices.end(), vertices.begin() + (m_Shape->GetVerticesSize() * it->second.batchPos));
		 batch->RefreshVertices();

	 }



	 const GEngine::Vector3 BatchRenderer::GetShapePosition(long id)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.position;
	 }

	 const float BatchRenderer::GetShapeRotation(long id)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.rotation;
	 }

	 const GEngine::Vector2 BatchRenderer::GetShapeScale(long id)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			 });

		 return it->second.scale;
	 }

	 void BatchRenderer::RemoveShape(long id)
	 {
		 std::vector<std::pair<uint32_t, BatchObjectData>>::iterator it = std::find_if(m_SortedObjects.begin(), m_SortedObjects.end(), [id](const std::pair<u32, BatchObjectData>& e) {
			 return e.first == id;
			});

		 m_SortedObjects.erase(it);
		 ReCreateBatches();
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
		if (bUnloaded) {
            bUnloaded = false;
			Setup();
			ReCreateBatches();
		}
	 } 

	 void BatchRenderer::UnloadGraphics() {
		 if (!bUnloaded) {
			 for (int i = 0; i < m_Batches.size(); i++) {
				 m_Pipeline->Remove(m_Batches[i]);
			 }
			 m_Batches.clear();
			 m_IndexBuffer = nullptr;
			 m_BlankTexture = nullptr;
			 bUnloaded = true;
		 }
	 }

	 void BatchRenderer::ReCreateBatches()
	 {

         if (bUnloaded) return;
         
		 if (m_SortedObjects.size() <= 0 && m_Batches.size() == 1) {
			 std::vector<float> f;
			 std::vector<int> i;
			 m_Batches[0]->SetVertices(f, i, 0);
			 return;
		 }

		 if (m_Batches.size() <= 0) {
			 Ref<Batch> batch = Ref<Batch>(new Batch(m_Shape, m_IndexBuffer, m_Shader, m_MaxShapes, m_MaxTextures));
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

			 std::vector<int>::iterator it = std::find(textures.begin(), textures.end(), m_SortedObjects[i].second.texture->GetRendererID());
			 if  (objectCount +1 < m_MaxShapes && (textures.size() < m_MaxTextures || it != textures.end())) {
				 std::pair<uint32_t, BatchObjectData>& data = m_SortedObjects[i];
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
					 Ref<Batch> batch = Ref<Batch>(new Batch(m_Shape, m_IndexBuffer, m_Shader, m_MaxShapes, m_MaxTextures));
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
	 }

	 }
