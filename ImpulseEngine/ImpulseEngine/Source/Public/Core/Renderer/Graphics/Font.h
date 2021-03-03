#pragma once


namespace GEngine {

	class Texture2D;
	class SubTexture2D;

	struct CharacterData {
		uint32_t character;
		Vector2f position;
		Vector2f scale;
		Ref<SubTexture2D> texture;
	};

	struct StringData {
		Vector2f pen = { 0,0 };
		float maxHeight = 0.f;
		float lastAdvance = 0.f;
		int lastSpace = 0;
	};

	struct StringInfo {
		StringData data;
		std::vector<CharacterData> charData;
	};

	class Font {

	public:
		static Ref<Font> Create(std::string path, int size);
		virtual void LoadCharacters(const char* string) = 0;
		virtual void LoadCharacters_u32(const uint32_t* codepoint, const uint32_t size) = 0;
		virtual void LoadCharacter_u32(const uint32_t codepoint) = 0;
		virtual Ref<StringInfo> DrawString(const std::string& s, float maxWidth, int viewWidth, int viewHeight) = 0;
		virtual Ref<StringInfo> AppendString(Ref<StringInfo> info, const std::string& text, float maxWidth, int viewWidth, int viewHeight) = 0;
		inline Ref<Texture2D> GetTexture() { return m_Texture; };
		virtual Ref<SubTexture2D> GetCharacterCoords(const char c) = 0;
		virtual Ref<SubTexture2D> GetCharacterCoords_u32(const uint32_t c) = 0;
		virtual Ref<StringInfo> DrawString_u32(uint32_t* s, int len, float maxWidth, int viewWidth, int viewHeight) = 0;
		virtual Ref<StringInfo> AppendString_u32(Ref<StringInfo> info, uint32_t* text, int len, float maxWidth, int viewWidth, int viewHeight) = 0;

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
