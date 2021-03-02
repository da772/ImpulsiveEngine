#include "gepch.h"

#include "Public/Platform/Graphics/OpenGL/OpenGL_Font.h"
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

	static Vector3f GetTextureAspectScale(const Vector3f& scale, const float& textureWidth, const float& textureHeight, const float& appWidth, const float& appHeight, const bool aspectRatio = true) {

		Vector3f _scale = scale;
		const Vector3f __scale = scale;
		float tWidth = textureWidth;
		float tHeight = textureHeight;

		float twMax = tWidth > tHeight ? tWidth : tHeight;
		float wMax = appWidth > appHeight ? appWidth : appHeight;
		float buttonX = ((float)tWidth / twMax) / (wMax != (float)appWidth ? (appWidth / wMax) : 1.f);
		float buttonY = ((float)tHeight / twMax) / (wMax != (float)appHeight ? (appHeight / wMax) : 1.f);


		float _s = __scale.x > __scale.y ? __scale.x : __scale.y;
		float _b = __scale.x > __scale.y ? buttonX : buttonY;
		if (aspectRatio) {
			_scale.x = buttonX * (_s / _b);
			_scale.y = buttonY * (_s / _b);
		}

		return _scale;
	}

	Ref<StringInfo> OpenGL_Font::DrawString(const std::string& s, float maxWidth, int viewWidth, int viewHeight)
	{
		return AppendString(make_shared<StringInfo>(), s, maxWidth, viewWidth, viewHeight);
	}

	GEngine::Ref<GEngine::StringInfo> OpenGL_Font::AppendString(Ref<StringInfo> info, const std::string& s, float maxWidth, int viewWidth, int viewHeight)
	{
		for (int i = 0; i < s.size(); i++) {
			Ref<SubTexture2D> texture = SubTexture2D::CreateFromCoords(m_Texture, { 0, 0 }, { 0, 0 });
			char character = s[i];

			if (character == ' ') {
				info->data.pen.x += (float)m_size / 2.f / viewWidth;
				info->data.lastSpace = i;
				continue;
			}
			else if (character == '\n') {
				info->data.pen.y -= info->data.maxHeight > 0 ? info->data.maxHeight : (float)m_size / viewHeight;
				info->data.maxHeight = 0;
				info->data.pen.x = 0;
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

				float h = ((float)glyph->height / viewHeight) + (m_size / 2.f / viewHeight);

				if (h > info->data.maxHeight)
					info->data.maxHeight = h;

				if (i > 0) {
					char _s = s[i - 1];
					kerning = texture_glyph_get_kerning(glyph, &_s);
				}
				else if (info->charData.size() > 0) {
					char _s = info->charData[info->charData.size() - 1].character;
					kerning = texture_glyph_get_kerning(glyph, &_s);
				}

				info->data.pen.x += kerning / (float)viewWidth;

				float x0 = info->data.pen.x + ((float)glyph->offset_x / viewWidth);
				float y0 = info->data.pen.y + ((float)glyph->offset_y / viewHeight);
				float x1 = x0 + ((float)glyph->width / viewWidth);
				float y1 = y0 + ((float)glyph->height / viewHeight);


				Vector2f pos = { (x1 + x0) / 2.f , y0 - (y1 - y0) / 2.f };
				Vector2f scale = { x1 - x0, y1 - y0 };

				if (pos.x > maxWidth) {
					info->data.pen.x = 0;
					info->data.pen.y -= info->data.maxHeight;
					info->data.maxHeight = h;
					for (int j = 0; j < i - info->data.lastSpace - 1; j++)
						info->charData.erase(info->charData.end() - 1);
					i = info->data.lastSpace;
					continue;
				}

				info->data.pen.x += (float)glyph->advance_x / viewWidth;//(float)glyph->advance_x / viewWidth;

				info->data.lastAdvance = (float)glyph->advance_x / viewWidth;

				info->charData.push_back({ character, pos,scale, texture });
			}
			else {
				GE_CORE_ERROR("INVALID CHARACTER: {0}", s[i]);
			}
		}
		info->data.lastSpace = 0;
		return info;
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
