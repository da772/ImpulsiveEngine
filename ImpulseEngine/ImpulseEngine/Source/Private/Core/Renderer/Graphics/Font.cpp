#include "gepch.h"
#include "Public/Core/Renderer/Graphics/Font.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Public/Platform/Graphics/OpenGL/OpenGL_Font.h"
#endif
#ifdef GE_GRAPHICS_API_NONE
#include "Public/Platform/Graphics/Server/Empty_Font.h"
#endif

namespace GEngine {

	std::unordered_map<std::string, Weak<Font>> Font::m_FontList;

	GEngine::Ref<GEngine::Font> Font::Create(std::string path, int size)
	{
		Ref<Font> f;
		if (Font::m_FontList.find(path) != Font::m_FontList.end()) {
			f = Font::m_FontList[path].lock();
			if (f != nullptr)
				return f;
			Font::m_FontList.erase(path);
		}

		switch (GraphicsContext::GetGraphicsApi()) {

		case GraphicsApi::FGraphicsApi::OPENGL:
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
			f = Ref<Font>(new OpenGL_Font(path, size));
			f->self = f;
#endif
			break;
			case GraphicsApi::FGraphicsApi::NONE:
#ifdef GE_GRAPHICS_API_NONE
				f = Ref<Font>(new Empty_Font(path, size));
#endif
				break;
            case GraphicsApi::FGraphicsApi::DIRECTX11:
            case GraphicsApi::FGraphicsApi::DIRECTX12:
            default:
            GE_CORE_ERROR("GRAPHICS API NOT DEFINED");
            break;

		}


		Font::m_FontList[path] = f;

		return f;

	}

    void Font::RemoveFont(const std::string &name) {
        if (Font::m_FontList.find(name) != Font::m_FontList.end())
            Font::m_FontList.erase(name);
    }

	void Font::UnloadGraphics()
	{
		for (std::pair<std::string, Weak<Font>> p : Font::m_FontList) {
            if (p.second.lock()) {
                p.second.lock()->Unload();
            }
		}
	}

	void Font::ReloadGraphics()
	{
		for (std::pair<std::string, Weak<Font>> p : Font::m_FontList) {
			
            if (p.second.lock()) p.second.lock()->Reload();
		}
	}

}
