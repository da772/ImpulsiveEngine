#include "gepch.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"
#include "Public/Core/Util/Pooling.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_Texture.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Core/Platform/Graphics/Vulkan/Vulkan_Texture.h"
#endif

namespace GEngine {

	Texture::~Texture()
	{
		
	}

	std::unordered_map<std::string, Weak<Texture>> Texture::s_TexturePool;

	Texture2D::~Texture2D()
	{
		
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, const u32 flags)
	{

		Ref<Texture2D> t = nullptr;

		if (Texture::s_TexturePool.count(path)) {
			Ref<Texture> _t = Texture::s_TexturePool[path].lock();
			if (_t != nullptr)
				return std::static_pointer_cast<Texture2D>(_t);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			t = Ref<Texture2D>(new OpenGL_Texture2D(path, flags));
			break;
#endif


#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			t = Ref<Texture2D>(new Vulkan_Texture2D(path, flags));
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Invalid Texture 2D for current graphics api: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
		t->name = path;
		t->self = t;
		Texture::s_TexturePool[path] = t;
		return t;

	}

	GEngine::Ref<GEngine::Texture2D> Texture2D::Create(const std::string& name, uint32_t width, uint32_t height)
	{

		Ref<Texture2D> t = nullptr;

		if (Texture::s_TexturePool.count(name)) {
			Ref<Texture> _t = Texture::s_TexturePool[name].lock();
			if (_t != nullptr)
				return std::static_pointer_cast<Texture2D>(_t);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			t = Ref<OpenGL_Texture2D>(new OpenGL_Texture2D(width, height));
			break;
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			t = Ref<Vulkan_Texture2D>(new Vulkan_Texture2D(width, height));
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Invalid Texture 2D for current graphics api: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}

		t->name = name;
		t->self = t;
		Texture::s_TexturePool[name] = t;
		return t;
	}

	void Texture2D::Destroy(Weak<Texture2D> texture)
	{
		if (texture.lock() == nullptr) return;
		int count = Texture::s_TexturePool[texture.lock()->name].use_count();
		if (count <= 3) {
			if (Texture::s_TexturePool.erase(texture.lock()->name)) {
				
			}
		}
	}

}
