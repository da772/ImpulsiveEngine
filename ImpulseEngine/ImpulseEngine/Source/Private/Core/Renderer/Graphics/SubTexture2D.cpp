#include "gepch.h"

#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Util/ThreadPool.h"

namespace GEngine {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const Vector2& min, const Vector2& max) :
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

	void SubTexture2D::SetCoords(const Vector2& coords, const Vector2& cellSize, const Vector2& spriteSize /*= { 1,1 }*/)
	{
		float width = m_Texture->GetWidth(), height = m_Texture->GetHeight();
		const Vector2 textureCords[] = {
			{ (coords.x * cellSize.x) / width,  (coords.y * cellSize.y) / height },
			{ ((coords.x + spriteSize.x) * cellSize.x) / width, ((coords.y + spriteSize.y) * cellSize.y) / height}
		};

		Vector2 min = textureCords[0];
		Vector2 max = textureCords[1];

		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const Vector2& coords, const Vector2& cellSize, const Vector2& spriteSize)
	{

		float width = texture->GetWidth(), height = texture->GetHeight();

		if (!ThreadPool::OnMainThread()) {
			std::unique_lock<std::mutex> lck(texture->m_mutex);
			m_condVar.wait(lck, [texture] {return texture->GetHeight() != 0 || texture->GetWidth() != 0; });
			width = texture->GetWidth(), height = texture->GetHeight();
		}
		

		const Vector2 textureCords[] = {
			{ (coords.x * cellSize.x) / width,  (coords.y * cellSize.y) / height },
			{ ((coords.x+ spriteSize.x) * cellSize.x)/width, ((coords.y + spriteSize.y)* cellSize.y)/height}
		};

		return Ref<SubTexture2D>(new SubTexture2D(texture, textureCords[0], textureCords[1]));

	}

	std::condition_variable SubTexture2D::m_condVar;

}