#pragma once

#include "Public/Core/Renderer/Graphics/Font.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include <freetype-gl.h>
#endif

namespace GEngine {

	class FileData;

	class GE_API OpenGL_Font : public Font {
	public:
		OpenGL_Font(std::string path, float size);
		virtual ~OpenGL_Font();
		virtual void LoadCharacters(const char*) override;
		virtual Ref<SubTexture2D> GetCharacterCoords(const char c) override;
		virtual Ref<SubTexture2D> GetCharacterCoords_u32(const uint32_t c) override;
		virtual void LoadCharacter_u32(const uint32_t codepoint) override;
		virtual void LoadCharacters_u32(const uint32_t* codepoint, const uint32_t size) override;

		virtual Ref<StringInfo> DrawString(const std::string& s, float maxWidth, int viewWidth, int viewHeight) override;
		virtual Ref<StringInfo> AppendString(Ref<StringInfo> info, const std::string& text, float maxWidth, int viewWidth, int viewHeight) override;
		virtual Ref<StringInfo> DrawString_u32(uint32_t* s, int len, float maxWidth, int viewWidth, int viewHeight) override;
		virtual Ref<StringInfo> AppendString_u32(Ref<StringInfo> info, uint32_t* text, int len, float maxWidth, int viewWidth, int viewHeight) override;
		virtual void Unload() override;
		virtual void Reload() override;


	private:
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		texture_atlas_t* m_textureAtlas;
		texture_font_t* m_textureFont;
#endif
		Ref<FileData> m_atlasData;
		std::unordered_set<uint32_t> m_characters;
		std::string m_path;
		std::string m_loaded;

	};



}