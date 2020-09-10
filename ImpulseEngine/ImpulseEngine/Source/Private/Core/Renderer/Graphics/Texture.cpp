#include "gepch.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"
#include "Public/Core/Util/Pooling.h"
#include "Public/Core/Util/Utility.h"

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
		if (s_TexturePool.find(name) != s_TexturePool.end()) 
			s_TexturePool.erase(this->name);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path, const u32 flags)
	{

		Ref<Texture2D> t = nullptr;

		if (Texture::s_TexturePool.count(path)) {
			Ref<Texture> _t = Texture::s_TexturePool[path].lock();
			if (_t != nullptr)
				return std::static_pointer_cast<Texture2D>(_t);
			else
				Texture::s_TexturePool.erase(path);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			t = Ref<Texture2D>(new OpenGL_Texture2D(path, flags));
			break;
#endif


#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			t = Ref<Texture2D>(new Vulkan_Texture2D(path, flags));
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Invalid Texture 2D for current graphics api: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
		t->name = path;
		t->self = t;
		Texture::s_TexturePool[path] = t;
		return t;

	}

	GEngine::Ref<GEngine::Texture2D> Texture2D::Create(std::string name, uint32_t width, uint32_t height)
	{

		Ref<Texture2D> t = nullptr;

		if (name == "") {
			name = Utility::GenerateHash(16);
			while (s_TexturePool.find(name) != s_TexturePool.end()) {
				name = Utility::GenerateHash(16);
			}
		}

		if (Texture::s_TexturePool.count(name)) {
			Ref<Texture> _t = Texture::s_TexturePool[name].lock();
			if (_t != nullptr)
				return std::static_pointer_cast<Texture2D>(_t);
			else
				s_TexturePool.erase(name);
		}

		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			t = Ref<OpenGL_Texture2D>(new OpenGL_Texture2D(width, height));
			break;
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			t = Ref<Vulkan_Texture2D>(new Vulkan_Texture2D(width, height));
			break;
#endif
		default:
			GE_CORE_ASSERT(false, "Invalid Texture 2D for current graphics api: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}

		t->name = name;
		t->self = t;
		Texture::s_TexturePool[name] = t;
		return t;
	}

	void Texture2D::UnloadTextures() {
		for (std::pair<std::string, Weak<Texture>> p : Texture2D::s_TexturePool ) {
			Ref<Texture> t = p.second.lock();
			if (t)
				t->Unload();
		}
	}
	void Texture2D::ReloadTextures() {
		for (std::pair<std::string, Weak<Texture>> p : Texture2D::s_TexturePool ) {
			Ref<Texture> t = p.second.lock();
			if (t)
				t->Reload();
		}
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
