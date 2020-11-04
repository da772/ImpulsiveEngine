#include "gepch.h"

#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_Font.h"
#include "Public/Core/FileSystem/FileSystem.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/SubTexture2D.h"
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)


namespace GEngine {


	OpenGL_Font::OpenGL_Font(std::string path, int size) : m_path(path)
	{
		m_atlasData = FileSystem::FileDataFromPath(path);
		m_textureAtlas = ftgl::texture_atlas_new(m_width, m_height, 1);
		m_textureFont = ftgl::texture_font_new_from_memory(m_textureAtlas, size, m_atlasData->GetData(), m_atlasData->GetDataSize());
		m_Texture = Texture2D::Create(path, m_width, m_height);
		m_textureAtlas->id = m_Texture->GetRendererID();
		m_size = size;
	}

	OpenGL_Font::~OpenGL_Font()
	{
		ftgl::texture_font_delete(m_textureFont);
		ftgl::texture_atlas_delete(m_textureAtlas);
		m_atlasData = nullptr;
		m_Texture = nullptr;
        Font::RemoveFont(m_path);
	}

	void OpenGL_Font::LoadCharacters(const char* string)
	{
		uint64_t size = ftgl::texture_font_load_glyphs(m_textureFont, string);
		m_Texture->SetData(m_textureAtlas->data, 0, TEXTUREFLAGS_RED_CHANNEL | 
			TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Linear | TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap);
		m_loaded += std::string(string);
	}

	GEngine::Ref<GEngine::SubTexture2D> OpenGL_Font::GetCharacterCoords(uint64_t c)
	{
		Ref<SubTexture2D> tex = SubTexture2D::CreateFromCoords(m_Texture, { 0, 0 }, { 0, 0 });
		texture_glyph_t* glyph = texture_font_get_glyph(m_textureFont, (const char*)&c);

		float s0 = glyph->s0;
		float t0 = glyph->t0;
		float s1 = glyph->s1;
		float t1 = glyph->t1;

		tex->m_TexCoords[3] = { s0, t0 };
		tex->m_TexCoords[0] = { s0, t1 };
		tex->m_TexCoords[1] = { s1, t1 };
		tex->m_TexCoords[2] = { s1, t0 };

		return tex;
	}

	std::vector<GEngine::CharacterData> OpenGL_Font::DrawString(std::string s, float maxWidth, int viewWidth, int viewHeight)
	{
		std::vector<CharacterData> charData;
		glm::vec2 pen = { 0,0 };
		float maxHeight = 0.f;
		float lastAdvance = 0.f;
		int lastSpace = 0;
		for (int i = 0; i < s.size(); i++) {
			Ref<SubTexture2D> texture = SubTexture2D::CreateFromCoords(m_Texture, { 0, 0 }, { 0, 0 });
			char character = s[i];

			if (character == ' ') {
				pen.x += (float)m_size/2.f/viewWidth;
				lastSpace = i;
				continue;
			}
			else if (character == '\n') {
				pen.y -= maxHeight > 0 ? maxHeight : (float)m_size / viewHeight;
				maxHeight = 0;
				pen.x = 0;
				continue;
			}

			texture_glyph_t* glyph = texture_font_get_glyph(m_textureFont, &character);

			if (glyph != NULL)
			{

				float s0 = glyph->s0;
				float t0 = glyph->t0;
				float s1 = glyph->s1;
				float t1 = glyph->t1;

				texture->m_TexCoords[3] = { s0, t0 };
				texture->m_TexCoords[0] = { s0, t1 };
				texture->m_TexCoords[1] = { s1, t1 };
				texture->m_TexCoords[2] = { s1, t0 };

				float kerning = 0.f;

				float h = ((float)glyph->height / viewHeight) + (m_size/2.f/viewHeight);

				if (h > maxHeight)
					maxHeight = h;

				if (i > 0) {
					char _s = s[i - 1];
					kerning = texture_glyph_get_kerning(glyph, &_s);
				}

				pen.x += kerning/ (float)viewWidth;

				float x0 = pen.x + ((float)glyph->offset_x / viewWidth);
				float y0 = pen.y + ((float)glyph->offset_y / viewHeight);
				float x1 = x0 + ((float)glyph->width / viewWidth);
				float y1 = y0 + ((float)glyph->height / viewHeight);


				Vector2 pos = { (x1+x0)/2.f , y0-(y1-y0)/2.f };
				Vector2 scale = { x1-x0, y1-y0};

				if (pos.x > maxWidth) {
					pen.x = 0;
					pen.y -= maxHeight;
					maxHeight = h;
					for (int j = 0; j < i-lastSpace-1; j++)
						charData.erase(charData.end() - 1);
					i = lastSpace;
					continue;
				}

				pen.x += (float)glyph->advance_x/viewWidth;//(float)glyph->advance_x / viewWidth;
				
				lastAdvance = (float)glyph->advance_x / viewWidth;

				charData.push_back({ character, pos,scale, texture });
			}
			else {
				GE_CORE_ERROR("INVALID CHARACTER: {0}", s[i]);
			}
		}

		return charData;


	}

	void OpenGL_Font::Unload()
	{
        
	}

	void OpenGL_Font::Reload()
	{
		m_textureAtlas->id = m_Texture->GetRendererID();
	}

}


#endif
