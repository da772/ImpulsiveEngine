#pragma once


namespace GEngine {

	class Texture2D;

	class SubTexture2D {

	public:
		SubTexture2D(const Ref<Texture2D>& texture, const Vector2f& min, const Vector2f& max);
		~SubTexture2D();
		inline Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const Vector2f* GetTexCoords() const { return m_TexCoords; }
		void SetCoords(const Vector2f& coords, const Vector2f& cellSize, const Vector2f& spriteSize = { 1,1 });
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texure, const Vector2f& coords, const Vector2f& cellSize, const Vector2f& spriteSize = { 1,1 });

		Vector2f m_TexCoords[4];
		static std::condition_variable m_condVar;
	private:
		Ref<Texture2D> m_Texture;

		
	};



}