#pragma once

#include "Public/Core/Renderer/Graphics/Font.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include <freetype-gl.h>
#endif

namespace GEngine {

	class FileData;

	class OpenGL_Font : public Font {
	public:
		OpenGL_Font(std::string path, int size);
		virtual ~OpenGL_Font();
		virtual void LoadCharacters(const char*) override;
		virtual Ref<SubTexture2D> GetCharacterCoords(uint64_t c) override;
		std::vector<CharacterData> DrawString(std::string s, float maxWidth, int viewWidth, int viewHeight) override;


	private:
		int m_size = 0;
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		texture_atlas_t* m_textureAtlas;
		texture_font_t* m_textureFont;
#endif
		Ref<FileData> m_atlasData;
		std::unordered_set<uint32_t> m_characters;
		std::string m_path;
	};



}