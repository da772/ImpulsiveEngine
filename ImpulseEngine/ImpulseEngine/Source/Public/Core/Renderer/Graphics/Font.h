#pragma once


namespace GEngine {

	class Texture2D;
	class SubTexture2D;

	struct CharacterData {
		char character;
		glm::vec2 position;
		glm::vec2 scale;
		Ref<SubTexture2D> texture;
	};

	class Font {

	public:
		static Ref<Font> Create(std::string path, int size);
		virtual void LoadCharacters(const char* string) = 0;
		virtual std::vector<CharacterData> DrawString(std::string s, float maxWidth, int viewWidth, int viewHeight) = 0;
		inline Ref<Texture2D> GetTexture() { return m_Texture; };
		virtual Ref<SubTexture2D> GetCharacterCoords(uint64_t c) = 0;

		static void UnloadGraphics();
		static void ReloadGraphics();
        static void RemoveFont(const std::string& name);

		virtual void Unload() = 0;
		virtual void Reload() = 0;

		virtual inline void LoadCharactersEN() { LoadCharacters("a quick brown fox jumps over the lazy dog 0123456789=-+_)(*&^%$#@!AQUICKBROWNFOXJUMPSOVERTHELAZYDOG[]{}\|/?.,<>\"\\`~"); }

	protected:
		void* m_nativeFont;
		Weak<Font> self;
		Font() {};
		virtual ~Font() {};
		uint32_t m_width = 2048, m_height = 2048;
		Ref<Texture2D> m_Texture;
		


	private:
		
		static std::unordered_map<std::string, Weak<Font>> m_FontList;

	};






}
