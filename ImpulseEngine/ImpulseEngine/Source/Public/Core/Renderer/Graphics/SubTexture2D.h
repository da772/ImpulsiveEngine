#pragma once


namespace GEngine {

	class Texture2D;

	class SubTexture2D {

	public:
		SubTexture2D(const Ref<Texture2D>& texture, const Vector2& min, const Vector2& max);
		~SubTexture2D();
		inline Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const Vector2* GetTexCoords() const { return m_TexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texure, const Vector2& coords, const Vector2& cellSize, const Vector2& spriteSize = { 1,1 });

		Vector2 m_TexCoords[4];
		static std::condition_variable m_condVar;
	private:
		Ref<Texture2D> m_Texture;

		
	};



}