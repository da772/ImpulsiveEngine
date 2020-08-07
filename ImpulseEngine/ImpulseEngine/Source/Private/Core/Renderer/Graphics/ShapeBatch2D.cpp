#include "gepch.h"

#include "Public/Core/Renderer/Graphics/ShapeBatch2D.h"
#include "Public/Core/Renderer/Graphics/Shape.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"


#include "Public/Core/Application/SceneManager.h"



namespace GEngine {
	
	
	BatchData::BatchData(u32 maxShapes, u32 maxTextures,Ref<Shape> shape, Ref<Shader> shader)
	{
		m_Shape = shape;
		m_MaxShapes = maxShapes;
		m_MaxIndices = m_Shape->GetIndicesSize() * m_MaxShapes;
		m_MaxVertices = m_Shape->GetVerticesSize() * m_MaxShapes,
		m_MaxTextures = maxTextures;
		m_Indices = std::vector<u32>(m_MaxIndices);
		u32 offset = 0;
		for (u32 i = 0; i < m_MaxIndices; i += m_Shape->GetIndicesSize()) {
			std::vector<u32> ind = m_Shape->GetIndices(offset);
			for (u32 j = 0; j < m_Shape->GetIndicesSize(); j++) {
				m_Indices[i+ j] = ind[j];
			};
			offset += m_Shape->GetVerticesRows();
		}

		m_IndexBuffer = Ref<IndexBuffer>(IndexBuffer::Create(m_Indices.data(), m_MaxIndices));
		m_VertexBuffer = Ref<VertexBuffer>(VertexBuffer::Create(m_MaxVertices*sizeof(float)));
		m_VertexBuffer->SetLayout(shape->GetBufferLayout());
		m_VertexArray = Ref<VertexArray>(VertexArray::Create());


		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_Shader = shader;
		m_Shader->Bind();
		std::vector<int> samplers;
		for (int i = 0; i < m_MaxTextures; i++) {
			samplers.push_back(i);
		}
		m_Shader->UploadUniformIntArray("u_Textures", samplers.data(), m_MaxTextures);
		AddTexture(Texture2D::Create("shapeFactoryTexture", 0, 0));
	}

	
	BatchData::~BatchData()
	{
		Clear();
		m_Shape = nullptr;
		m_VertexArray = nullptr;
		m_IndexBuffer = nullptr;
		m_VertexBuffer = nullptr;
		m_Vertices.clear();
		m_Indices.clear();
		m_Textures.clear();
		std::queue<uint32_t>().swap(m_TexturePositions);
		std::queue<uint32_t>().swap(m_VertexPositions);
	}

	int BatchData::AddTexture(Ref<Texture2D> texture)
	{
		int id = FindTextureSlot(texture);

		if (id != -1) {
			m_Textures[id].refCount++;
			return id;
		}

		if (m_TextureCount + 1 <= m_MaxTextures || !m_TexturePositions.empty()) {

			if (!m_TexturePositions.empty()) {
				id = m_TexturePositions.front();
				m_TexturePositions.pop();
				m_Textures[id] = { texture, 1 };
				return id;
			}
			else {
				id = m_TextureCount;
				m_Textures[id] = { texture, 1 };
				m_TextureCount++;
				return id;
			}
		}
		else {
			return -1;
		}

	}

	void BatchData::RemoveTextureSlot(u32 id)
	{
		if (id > 0) {
			m_Textures[id].refCount--;
			if (m_Textures[id].refCount <= 0) {
				if (id >= m_TextureCount - 1) {
					//Texture2D::Destroy(m_Textures[id].texture);
					m_Textures[id].texture = nullptr;
					m_Textures.erase(id);
                    m_TextureCount--;
				}
				else {
					//Texture2D::Destroy(m_Textures[id].texture);
					m_Textures[id].texture = nullptr;
					m_Textures[id].refCount = 0;
					m_TexturePositions.push(id);
					
				}
			}
			if (m_Textures.size() <= 0)
				std::queue<uint32_t>().swap(m_TexturePositions);
		}
	}

	void BatchData::ReCalculateVertices()
	{
		std::sort(m_SortedVertices.begin(), m_SortedVertices.end(), [](const std::pair<u32, VertexData>& l, const std::pair<u32, VertexData>& r) {
			return l.second.pos.z < r.second.pos.z;
		});

		m_Vertices.clear();
		for (int i = 0; i < m_SortedVertices.size(); i++) {
			m_Vertices.insert(m_Vertices.end(), m_SortedVertices[i].second.vertices.begin(), m_SortedVertices[i].second.vertices.end());
		}
	}

	void BatchData::SetShapeRotation(u32 id, float rotation)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		it->second.vertices = m_Shape->GetVertices(it->second.pos, rotation, it->second.scale, it->second.color, it->second.texture,
			it->second.textureScale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.rot = rotation;

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ((it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));
		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
	}

	void BatchData::SetShapeScale(u32 id, Vector3 scale)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		it->second.vertices = m_Shape->GetVertices(it->second.pos, it->second.rot, scale, it->second.color, it->second.texture,
			it->second.textureScale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.scale = scale;

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ((it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));
		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
	}

	void BatchData::SetShapeColor(u32 id, Vector4 color)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		it->second.vertices = m_Shape->GetVertices(it->second.pos, it->second.rot, it->second.scale, color, it->second.texture,
			it->second.textureScale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.color = color;

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ( (it-m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));
		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
	}


	void BatchData::EditShape(long id, Vector3 position, float rotation /*= 0*/, Vector3 scale /*= Vector3(1, 1, 1)*/, Vector4 color /*= Vector4(1, 1, 1, 1)*/)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		it->second.vertices = m_Shape->GetVertices(position, rotation, scale, color, it->second.texture,
			it->second.textureScale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.scale = scale;
		it->second.pos = position;
		it->second.color = color;
		it->second.rot = rotation;

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ((it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));

		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
	}

	void BatchData::SetShapeTextureScale(u32 id, float scale)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		it->second.vertices = m_Shape->GetVertices(it->second.pos, it->second.rot, it->second.scale, it->second.color, it->second.texture,
			scale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.textureScale = scale;

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ((it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));

		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
	}

	GEngine::VertexData& BatchData::GetVertexData(u32 id)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		return it->second;
	}

	bool BatchData::SetShapePosition(u32 id, Vector3 position)
	{
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});
        int sId = FindTextureSlot(it->second._texture);
		it->second.vertices = m_Shape->GetVertices(position, it->second.rot, it->second.scale, it->second.color, sId,
			it->second.textureScale, it->second.textCoords.size() > 0 ? it->second.textCoords.data() : nullptr);
		it->second.pos = position;

		ReCalculateVertices();

		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
		return true;
	}

	bool BatchData::SetShapeTexture(u32 id, Ref<Texture2D> texture)
	{
		bool bFound = FindTextureSlot(texture) != -1;
		int tSlot = AddTexture(texture);

		if (tSlot == -1)
			return false;

		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
				});

		int slotOffset = m_Shape->GetTextureSlotOffset();
		if (bFound && slotOffset != -1) {
			int textureSlot = it->second.vertices[slotOffset];
			RemoveTextureSlot(textureSlot);
		}
		it->second.vertices = m_Shape->GetVertices(it->second.pos, it->second.rot, it->second.scale, it->second.color, tSlot,
			it->second.textureScale, nullptr);
		it->second.texture = tSlot;
		it->second._texture = texture;
		it->second._subTexture = nullptr;
		it->second.textCoords = {};

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ( (it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));
		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
		return true;
	}

	bool BatchData::SetShapeSubtexture(u32 id, Ref<SubTexture2D> texture)
	{
		bool bFound = FindTextureSlot(texture->GetTexture()) != -1;
		int tSlot = AddTexture(texture->GetTexture());

		if (tSlot == -1)
			return false;
		
		std::vector<std::pair<u32, VertexData>>::iterator it =
			std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
			return e.first == id;
		});

		int slotOffset = m_Shape->GetTextureSlotOffset();
		if (bFound && slotOffset != -1) {
			int textureSlot = it->second.vertices[slotOffset];
			RemoveTextureSlot(textureSlot);
		}

		it->second.vertices = m_Shape->GetVertices(it->second.pos, it->second.rot, it->second.scale, it->second.color, tSlot,
			it->second.textureScale, texture->GetTexCoords());
		it->second.texture = tSlot;
		it->second._texture = texture->GetTexture();
		it->second._subTexture = texture;
		it->second.textCoords = std::vector<Vector2>(texture->GetTexCoords(), texture->GetTexCoords() + m_Shape->GetVerticesRows());

		std::copy(it->second.vertices.begin(), it->second.vertices.end(), m_Vertices.begin() + ((it - m_SortedVertices.begin()) * m_Shape->GetVerticesSize()));
		m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
		return true;
	}

	long BatchData::AddShape(Vector3 position, float rotation /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/, Vector4 color /*= Vector4(1,1,1,1)*/, 
		Ref<Texture2D> texture /*= 0*/, float textureScale, float alphaChannel)
	{
		int tSlot = texture != nullptr ? AddTexture(texture) : 0;

		if (tSlot < 0) {
			return -1;
		}

		if (m_Vertices.size() + 1 <= m_MaxVertices || m_VertexPositions.size() > 0) {
			
			if (m_VertexPositions.size() > 0) {
				int id = m_VertexPositions.front();
				m_VertexPositions.pop();
				std::vector<float> vert = m_Shape->GetVertices(position, rotation, scale, color, tSlot, textureScale, nullptr, alphaChannel);

				m_SortedVertices.push_back({ (u32)id, {position, scale, rotation, color, (float)tSlot, textureScale, texture, nullptr, {}, vert,alphaChannel } });


				ReCalculateVertices();
				
				m_VertexCount += m_Shape->GetVerticesSize();
				m_IndexCount += m_Shape->GetIndicesSize();

				m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));

				//GE_CORE_ASSERT(id != 0, "ID IS 0");

				return id + (m_id* m_MaxShapes);
			}
			else {
				int id = m_VertexCount/m_Shape->GetVerticesSize();
				
				std::vector<float> vert = m_Shape->GetVertices(position, rotation, scale, color, tSlot, textureScale, nullptr, alphaChannel);
				m_SortedVertices.push_back({ (u32)id, { position, scale, rotation, color, (float)tSlot, textureScale, texture, nullptr, {}, vert, alphaChannel } });

				ReCalculateVertices();

				m_VertexCount += m_Shape->GetVerticesSize();
				m_IndexCount += m_Shape->GetIndicesSize();

				m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));

				
					

				return id + (m_id * m_MaxShapes);
			}
		} 

		return -1;
	}

	long BatchData::AddShape(Vector3 position, float rotation /*= 0*/, Vector3 scale /*= Vector3(1, 1, 1)*/,
		Vector4 color /*= Vector4(1, 1, 1, 1)*/, Ref<SubTexture2D> texture /*= nullptr*/, float textureScale /*= 1*/, float alphaChannel)
	{
		int tSlot = texture != nullptr ? AddTexture(texture->GetTexture()) : 0;

		if (tSlot < 0) {
			return -1;
		}

		if (m_Vertices.size() + 1 <= m_MaxVertices || m_VertexPositions.size() > 0) {

			if (m_VertexPositions.size() > 0) {
				int id = m_VertexPositions.front();
				m_VertexPositions.pop();
				std::vector<float> vert = m_Shape->GetVertices(position, rotation, scale, color, tSlot, textureScale, texture->GetTexCoords(), alphaChannel);

                m_SortedVertices.push_back({ (u32)id, {position, scale, rotation, color, (float)tSlot, textureScale, texture->GetTexture(), texture,
					std::vector<Vector2>(texture->GetTexCoords(), texture->GetTexCoords()+ m_Shape->GetVerticesRows()), vert,alphaChannel  } });

				ReCalculateVertices();

				m_VertexCount += m_Shape->GetVerticesSize();
				m_IndexCount += m_Shape->GetIndicesSize();

				m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
				return id + (m_id * m_MaxShapes);
			}
			else {
				int id = m_VertexCount / m_Shape->GetVerticesSize();
				
				std::vector<float> vert = m_Shape->GetVertices(position, rotation, scale, color, tSlot, textureScale, texture->GetTexCoords(), alphaChannel);
				m_SortedVertices.push_back({ (u32) id, { position, scale, rotation, color, (float)tSlot, textureScale, texture->GetTexture(), texture,
					std::vector<Vector2>(texture->GetTexCoords(), texture->GetTexCoords() + m_Shape->GetVerticesRows()), vert,alphaChannel } });

				ReCalculateVertices();

				m_VertexCount += m_Shape->GetVerticesSize();
				m_IndexCount += m_Shape->GetIndicesSize();

				m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
				return id + (m_id * m_MaxShapes);
			}
		}

		return -1;
	}



	bool BatchData::RemoveShape(int id)
	{
		if (id >= 0) {
			if (id == m_VertexCount - m_Shape->GetVerticesSize()) {
				
				std::vector<std::pair<u32, VertexData>>::iterator it =
					std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
					return e.first == id;
						});

				if (it == m_SortedVertices.end()) {
					return false;
				}

				int slotOffset = m_Shape->GetTextureSlotOffset();
				if (slotOffset != -1) {
					int textureSlot = it->second.vertices[slotOffset];
					RemoveTextureSlot(textureSlot);
				}

				it->second.vertices.clear();
				m_SortedVertices.erase(it);


				m_VertexCount -= m_Shape->GetVerticesSize();
				m_IndexCount -= m_Shape->GetIndicesSize();

			}
			else {

				std::vector<std::pair<u32, VertexData>>::iterator it =
					std::find_if(m_SortedVertices.begin(), m_SortedVertices.end(), [&id](const std::pair<u32, VertexData>& e) {
					return e.first == id;
				});

				int slotOffset = m_Shape->GetTextureSlotOffset();
                if (slotOffset != -1 && it->second.vertices.size() > 0) {
					int textureSlot = it->second.vertices[slotOffset];
					RemoveTextureSlot(textureSlot);
				}

				it->second.vertices.clear();
				m_SortedVertices.erase(it);

				m_VertexPositions.push(id);
				

				m_VertexCount -= m_Shape->GetVerticesSize();
				m_IndexCount -= m_Shape->GetIndicesSize();
			}
			if (m_SortedVertices.size() <= 0)
				std::queue<uint32_t>().swap(m_VertexPositions);
			ReCalculateVertices();
			m_VertexBuffer->SetData(m_Vertices.data(), m_VertexCount * sizeof(float));
		
			if (m_VertexCount <= 0 || m_IndexCount <= 0) {
				return true;
			}

		}
		return false;
	}

	void BatchData::Clear()
	{
		m_Vertices.clear();
		m_Indices.clear();
		m_VertexBuffer->SetData(m_Vertices.data(), 0);
		for (int i = m_Textures.size()-1; i >=0 ; i--) {
			RemoveTextureSlot(i);
		}
		m_Textures[0].texture = nullptr;
		m_SortedVertices.clear();
		m_VertexPositions = std::queue<uint32_t>();
		m_TexturePositions = std::queue<uint32_t>();
	}

	void BatchData::Render()
	{
		if (m_IndexCount > 0) {
			m_Shader->Bind();
			m_Shader->UploadUniformMat4("u_ViewProjection", SceneManager::GetCurrentViewProjectionMatrix());

			for (std::pair < u32, TextureData> pair : m_Textures) {
				if (pair.second.texture == nullptr) continue;
				pair.second.texture->Bind(pair.first);
			}

			m_VertexArray->Bind();

			RenderCommand::DrawIndexed(m_VertexArray, m_IndexCount);
		}
	}


	int BatchData::FindTextureSlot(Ref<Texture2D> texture)
	{
		for (std::pair < u32, TextureData >&& p : m_Textures) {
			if (p.second.texture == texture) {
				return p.first;
			}
		}
		
		return -1;
	}



	bool ShapeBatch2D::RemoveShape(int id)
	{
		int batchShapeId = GetBatchIdFromShapeId(id);
		Ref<BatchData> batch = GetBatchFromId(id);
		RemoveShapeID(id);
		if (batch->RemoveShape(batchShapeId)) {
			if (m_Batches.size() > 1)
				RemoveBatch(batch);
			return true;
		}

		return false;
	}

	GEngine::Ref<GEngine::BatchData> ShapeBatch2D::GetBatchFromId(u32 id)
	{
		u32 _id = m_ShapeIds[id];
		int rv = _id / (m_MaxShapes);
		return m_Batches[rv];
	}

	u32 ShapeBatch2D::GetBatchShapeId(u32 id)
	{
		return m_ShapeIds[id];
	}

	GEngine::u32 ShapeBatch2D::GetBatchIdFromShapeId(u32 id)
	{
		u32 _id = m_ShapeIds[id];
		GE_CORE_ASSERT(m_ShapeIds.find(id) != m_ShapeIds.end(), "NOT FOUND!");	
		int rv = _id / (m_MaxShapes);
		int sId = _id - ((rv)*m_MaxShapes);
		return sId;
	}

	void ShapeBatch2D::Clear()
	{
		for (std::pair<u32, Ref<BatchData>> pair : m_Batches) {
			Renderer::GetPipeline(m_PipelineId.c_str())->Remove(m_Batches[pair.first]);
			m_Batches[pair.first]->Clear();
			m_Batches[pair.first] = nullptr;
		}
		m_Batches.clear();
		m_Shape = nullptr;
	}

	int ShapeBatch2D::UpdateCount(int i)
	{
		refCount += i;
		return refCount;
	}

	u32 ShapeBatch2D::AddShape(Vector3 position, float rotation /*= Vector3(0, 0, 0)*/, Vector3 scale /*= Vector3(1, 1, 1)*/, Vector4 color /*= Vector4(1, 1, 1, 1)*/, Ref<Texture2D> texture /*= 0*/, float textureScale, float alphaChannel)
	{
		int zOrder = floor(position.z);
		Vector3 pos = { position.x, position.y,  position.z};
		if (m_Batches.size() != 0) {
			for (std::pair<u32, Ref<BatchData>> key : m_Batches) {
				if (key.second->GetPriority() != zOrder) {
					
					continue;
				}
				int id = key.second->AddShape(pos, rotation, scale, color, texture, textureScale,alphaChannel);
				if (id >= 0) {
					return AssignShapeID(id);
				}
			}
		}

		Ref<BatchData> batch = CreateBatch(zOrder);
		return AssignShapeID(batch->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel));

	}


	u32 ShapeBatch2D::AddShape(Vector3 position, float rotation /*= 0*/, Vector3 scale /*= Vector3(1, 1, 1)*/, Vector4 color /*= Vector4(1, 1, 1, 1)*/, Ref<SubTexture2D> texture /*= nullptr*/, float textureScale /*= 1*/, float alphaChannel)
	{
		int zOrder = floor(position.z);
		Vector3 pos = { position.x, position.y,  position.z };
		if (m_Batches.size() != 0) {
			for (std::pair<u32, Ref<BatchData>> key : m_Batches) {
				if (key.second->GetPriority() != zOrder) {
					continue;
				}
				int id = key.second->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel);
				if (id >= 0) {
					return AssignShapeID(id);
				}
			}
		}

		Ref<BatchData> batch = CreateBatch(zOrder);
		return AssignShapeID(batch->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel));
	}

	std::pair<u32, u32> ShapeBatch2D::ResetAddShape(Vector3 position, float rotation /*= Vector3(0, 0, 0)*/, Vector3 scale /*= Vector3(1, 1, 1)*/,
		Vector4 color /*= Vector4(1, 1, 1, 1)*/, Ref<Texture2D> texture /*= 0*/, float textureScale, float alphaChannel)
	{
		int zOrder = floor(position.z);
		Vector3 pos = { position.x, position.y,  position.z };
		if (m_Batches.size() != 0) {
			for (std::pair<u32, Ref<BatchData>> key : m_Batches) {
				if (key.second->GetPriority() != zOrder) {

					continue;
				}
				int id = key.second->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel);
				if (id >= 0) {
					return { key.first, id };
				}
			}
		}

		Ref<BatchData> batch = CreateBatch(zOrder);
		return { batch->GetBatchId(),batch->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel) };

	}


	std::pair<u32, u32> ShapeBatch2D::ResetAddShape(Vector3 position, float rotation /*= 0*/, Vector3 scale /*= Vector3(1, 1, 1)*/, 
		Vector4 color /*= Vector4(1, 1, 1, 1)*/, Ref<SubTexture2D> texture /*= nullptr*/, float textureScale /*= 1*/, float alphaChannel)
	{
		int zOrder = floor(position.z);
		Vector3 pos = { position.x, position.y,  position.z };
		if (m_Batches.size() != 0) {
			for (std::pair<u32, Ref<BatchData>> key : m_Batches) {
				if (key.second->GetPriority() != zOrder) {
					continue;
				}
				int id = key.second->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel);
				if (id >= 0) {
					return { key.first, id };
				}
			}
		}

		Ref<BatchData> batch = CreateBatch(zOrder);
		return {batch->GetBatchId(), batch->AddShape(pos, rotation, scale, color, texture, textureScale, alphaChannel)};
	}


	void ShapeBatch2D::SetShapeRotation(u32 id, float rotation)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		GetBatchFromId(id)->SetShapeRotation(shapeBatchId, rotation);
	}

	void ShapeBatch2D::SetShapeScale(u32 id, Vector3 scale)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		GetBatchFromId(id)->SetShapeScale(shapeBatchId, scale);
	}

	void ShapeBatch2D::SetShapeColor(u32 id, Vector4 color)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		GetBatchFromId(id)->SetShapeColor(shapeBatchId, color);
	}

	void ShapeBatch2D::SetShapeTextureScale(u32 id, float scale)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		GetBatchFromId(id)->SetShapeTextureScale(shapeBatchId, scale);
	}

	void ShapeBatch2D::EditShape(long id, Vector3 position, float rotation /*= 0*/, Vector3 scale /*= Vector3(1, 1, 1)*/, Vector4 color /*= Vector4(1, 1, 1, 1)*/)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		GetBatchFromId(id)->EditShape(shapeBatchId, position, rotation, scale, color);
	}

	void ShapeBatch2D::SetShapePosition(u32 id, Vector3 position)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		int zOrder = floor(position.z);
		Ref<BatchData> batch = GetBatchFromId(id);
		if (batch->GetPriority() == zOrder) {
			batch->SetShapePosition(shapeBatchId, position);
			return;
		}
		VertexData& data = batch->GetVertexData(shapeBatchId);
		
		std::pair<u32, u32> _id;
		if (data._texture == nullptr) {
			_id = ResetAddShape(position, data.rot, data.scale, data.color);
		}
		else if (data._subTexture == nullptr) {
			_id = ResetAddShape(position, data.rot, data.scale, data.color, data._texture, data.textureScale, data.alphaChannel);
		}
		else if (data._subTexture != nullptr){
			_id = ResetAddShape(position, data.rot, data.scale, data.color, data._subTexture, data.textureScale, data.alphaChannel);
		}

		m_ShapeIds[id] = _id.second;
		batch->RemoveShape(shapeBatchId);
	}

	void ShapeBatch2D::SetShapeTexture(u32 id, Ref<Texture2D> texture)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		Ref<BatchData> batch = GetBatchFromId(id);
		if (batch->SetShapeTexture(shapeBatchId, texture)) {
			return;
		}

		VertexData& data = batch->GetVertexData(shapeBatchId);

		std::pair<u32, u32> _id;
		if (texture == nullptr) {
			_id = ResetAddShape(data.pos, data.rot, data.scale, data.color);
		}
		else {
			_id = ResetAddShape(data.pos, data.rot, data.scale, data.color, texture, data.textureScale, data.alphaChannel);
		}

        m_ShapeIds[id] = _id.second;
        batch->RemoveShape(shapeBatchId);
	}

	void ShapeBatch2D::SetShapeSubtexture(u32 id, Ref<SubTexture2D> texture)
	{
		long shapeBatchId = GetBatchIdFromShapeId(id);
		Ref<BatchData> batch = GetBatchFromId(id);
		if (batch->SetShapeSubtexture(shapeBatchId, texture)) {
			return;
		}

		VertexData& data = batch->GetVertexData(shapeBatchId);

		std::pair<u32, u32> _id;
		if (texture->GetTexture() == nullptr) {
			_id = ResetAddShape(data.pos, data.rot, data.scale, data.color);
		}
		else {
			_id = ResetAddShape(data.pos, data.rot, data.scale, data.color, texture, data.textureScale, data.alphaChannel);
		}

		m_ShapeIds[id] = _id.second;
        batch->RemoveShape(shapeBatchId);
	}

	void ShapeBatch2D::RemoveBatch(Ref<BatchData> batch)
	{
		int id = batch->GetBatchId();
		Renderer::GetPipeline(m_PipelineId.c_str())->Remove(batch);
		batch->Clear();
		m_Batches.erase(batch->GetBatchId());
		batch = nullptr;
		if (m_Batches.size() > 0)
			m_BatchIds.push(id);
	}

	u32 ShapeBatch2D::AssignShapeID(u32 batchShapeId)
	{
		u32 id = 0;
		if (!m_ShapeIdPos.empty()) {
			id = m_ShapeIdPos.front();
			m_ShapeIdPos.pop();
		}
		else {
			id = m_ShapeIds.size();
		}

		m_ShapeIds[id] = batchShapeId;
		return id;
	}


	void ShapeBatch2D::RemoveShapeID(u32 id)
	{
		if (id != m_ShapeIds.size() - 1) {
			m_ShapeIdPos.push(id);
		}
		m_ShapeIds.erase(id);
		if (m_ShapeIds.size() <= 0) {
			std::queue<uint32_t>().swap(m_ShapeIdPos);
		}
	}

	Ref<BatchData> ShapeBatch2D::CreateBatch(int priority)
	{
		if (m_MaxTextures == -1) {
			m_MaxTextures = RenderCommand::GetMaxTextureSlots();
			//GE_CORE_DEBUG("MAX TEXTURES: {0}", m_MaxTextures);
		}

		if (m_BlankTexture == nullptr) {
			uint32_t whiteTextureData = 0xffffffff;
			m_BlankTexture = Texture2D::Create("shapeFactoryTexture", 1, 1);
			m_BlankTexture->SetData(&whiteTextureData, sizeof(u32));
		}

		Ref<BatchData> batch = Ref<BatchData>(new BatchData(m_MaxShapes, m_MaxTextures, m_Shape, m_Shader));
		batch->SetBatchId(m_Batches.size());
		u32 id = 0;
		if (!m_BatchIds.empty()) {
			id = m_BatchIds.front();
			m_BatchIds.pop();
		}
		else {
			id = m_Batches.size();
		}
		
		m_Batches[id] = batch;
		batch->SetPriority(priority);
		batch->SetBatchId(id);
		Renderer::GetPipeline(m_PipelineId.c_str())->Add(batch);
		return batch;
	}


	ShapeBatch2D::ShapeBatch2D(std::string pipelineId, Ref<Shape> shape, Ref<Shader> shader, u32 maxShapes) : m_MaxShapes(maxShapes),
		m_Shape(shape), m_PipelineId(pipelineId), m_Shader(shader)
	{
		
	}


	ShapeBatch2D::~ShapeBatch2D()
	{
		for (std::pair<u32, Ref<BatchData>> pair : m_Batches) {
			Renderer::GetPipeline(m_PipelineId.c_str())->Remove(m_Batches[pair.first]);
			m_Batches[pair.first]->Clear();
			m_Batches[pair.first] = nullptr;
		}
		m_Batches.clear();
		m_Shape = nullptr;
		m_BlankTexture = nullptr;
		m_Shader = nullptr;
	}



}
