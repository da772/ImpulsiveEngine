#include "gepch.h"

#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Util/ThreadPool.h"

namespace GEngine {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const Vector2f& min, const Vector2f& max) :
		m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	SubTexture2D::~SubTexture2D()
	{
		//GE_CORE_DEBUG("SUB TEXTURE DESTROYERD");
	}

	void SubTexture2D::SetCoords(const Vector2f& coords, const Vector2f& cellSize, const Vector2f& spriteSize /*= { 1,1 }*/)
	{
		m_coords = coords;
		m_cellSize = cellSize;
		m_spriteSize = spriteSize;
		float width = m_Texture->GetWidth(), height = m_Texture->GetHeight();
		const Vector2f textureCords[] = {
			{ (coords.x * cellSize.x) / width,  (coords.y * cellSize.y) / height },
			{ ((coords.x + spriteSize.x) * cellSize.x) / width, ((coords.y + spriteSize.y) * cellSize.y) / height}
		};

		Vector2f min = textureCords[0];
		Vector2f max = textureCords[1];

		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const Vector2f& coords, const Vector2f& cellSize, const Vector2f& spriteSize)
	{
		
		float width = texture->GetWidth(), height = texture->GetHeight();

		if (!ThreadPool::OnMainThread()) {
			std::unique_lock<std::mutex> lck(texture->m_mutex);
			m_condVar.wait(lck, [texture] {return texture->GetHeight() != 0 || texture->GetWidth() != 0; });
			width = texture->GetWidth(), height = texture->GetHeight();
		}
		

		const Vector2f textureCords[] = {
			{ (coords.x * cellSize.x) / width,  (coords.y * cellSize.y) / height },
			{ ((coords.x+ spriteSize.x) * cellSize.x)/width, ((coords.y + spriteSize.y)* cellSize.y)/height}
		};

		Ref<SubTexture2D> t = Ref<SubTexture2D>(new SubTexture2D(texture, textureCords[0], textureCords[1]));
		t->m_cellSize = cellSize;
		t->m_spriteSize = spriteSize;
		t->m_coords = coords;

		return t;

	}

	std::condition_variable SubTexture2D::m_condVar;

}