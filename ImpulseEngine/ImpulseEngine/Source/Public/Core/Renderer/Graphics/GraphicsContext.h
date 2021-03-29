#pragma once
#include <glm/glm.hpp>

#ifdef GE_SERVER_APP
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::NONE
#endif
#elif defined(GE_PLATFORM_WINDOWS)
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#elif defined(GE_PLATFORM_LINUX)
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#elif defined(GE_PLATFORM_MACOSX)
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#elifs defined(GE_PLATFORM_IOS)
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#elif defined(GE_PLATFORM_ANDROID)
#ifndef GE_GRAPHICS_API_DEFAULT
#define GE_GRAPHICS_API_DEFAULT FGraphicsApi::OPENGL
#endif
#endif

namespace GEngine {


	namespace GraphicsApi { enum class FGraphicsApi : uint8_t { NONE = 0, OPENGL, DIRECTX12, DIRECTX11, VULKAN }; }

	using namespace GraphicsApi;


	class GE_API GraphicsContext {

	public:
		GraphicsContext();
		virtual ~GraphicsContext();

		virtual void Init() {};
		virtual void SwapBuffers() = 0;

		virtual void SetVSync(bool vsync) = 0;

		static GraphicsContext* Create(const void* window);

		static inline FGraphicsApi GetGraphicsApi() { return s_GraphicsApi; };
		static inline void SetGraphicsApi(FGraphicsApi api) { s_GraphicsApi = api; };


	private:
		template<typename T>
		static GraphicsContext* _CreateContext(const void* window);

		static FGraphicsApi s_GraphicsApi;

	};


}
